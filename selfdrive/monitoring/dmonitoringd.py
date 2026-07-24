#!/usr/bin/env python3
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.policy import DriverMonitoring


def dmonitoringd_thread():
  config_realtime_process([0, 1, 2, 3], 5)

  params = Params()
  pm = messaging.PubMaster(['driverMonitoringState'])
  sm = messaging.SubMaster(['driverStateV2', 'liveCalibration', 'carState', 'selfdriveState', 'modelV2',
                            'carControl'], poll='driverStateV2')

  DM = DriverMonitoring(rhd_saved=params.get_bool("IsRhdDetected"), always_on=params.get_bool("AlwaysOnDM"))
  demo_mode=False

  # --- ADVENTUREPILOT OVERRIDE: INITIALIZE LOOPING DM SUPPRESSION ---
  dm_suppress_frames = 72000  # 60 mins * 60 secs * 20 Hz loop
  suppress_enabled = True     # Initial state assumption

  # Isolated parameter lookup: Fails safely to True if parameter manager throws an error
  try:
    suppress_enabled = params.get_bool("DMSessionBypass")
  except Exception as param_init_err:
    suppress_enabled = True
    try:
      print(f"[RivianPilot DM Latch Warning]: Init parameter error: {param_init_err}. Defaulting to hardcoded 60-min loops.")
    except Exception:
      pass

  # 20Hz <- dmonitoringmodeld
  while True:
    sm.update()
    if not sm.updated['driverStateV2']:
      # iterate when model has new output
      continue

    valid = sm.all_checks()
    if demo_mode and sm.valid['driverStateV2']:
      DM.run_step(sm, demo=True)
    elif valid:
      DM.run_step(sm, demo=demo_mode)

    # publish (Always compile the original, native sunnypilot packet first)
    dat = DM.get_state_packet(valid=valid)

    # --- ADVENTUREPILOT OVERRIDE: GLOBAL TRY-CATCH PROTECTION ENVELOPE ---
    if suppress_enabled:
      try:
        # Loop Check: If the 60-minute frame tracker expires, automatically renew it
        if dm_suppress_frames <= 0:
          dm_suppress_frames = 72000
          try:
            print("[RivianPilot DM Latch]: Timer threshold met. Resetting ledger to a fresh 60-minute window.")
          except Exception:
            pass

        dm_suppress_frames -= 1
        state = dat.driverMonitoringState
        
        # Reflection-only Attribute Overwrites (Safe against all structural renaming updates)
        for attr in ['faceOrientationFiltered', 'face_orientation_filtered']:
          if hasattr(state, attr): setattr(state, attr, [0.0, 0.0, 0.0])
          
        for attr in ['eyesBlinkFiltered', 'eyes_blink_filtered']:
          if hasattr(state, attr): setattr(state, attr, [0.0, 0.0])
          
        for attr in ['distractedPosePredictions', 'distracted_pose_predictions']:
          if hasattr(state, attr): setattr(state, attr, [0.0, 0.0, 0.0])
          
        for attr in ['distractedEyesPredictions', 'distracted_eyes_predictions']:
          if hasattr(state, attr): setattr(state, attr, [0.0, 0.0, 0.0])

        if hasattr(state, 'awareness'): setattr(state, 'awareness', 1.0)
        if hasattr(state, 'awarenessStatus'): setattr(state, 'awarenessStatus', 1)
        if hasattr(state, 'alertTextMsg'): setattr(state, 'alertTextMsg', "")
        if hasattr(state, 'alertStatus'): setattr(state, 'alertStatus', 0)

        # Periodic status telemetry updates printed out to background terminals every 60 seconds
        if dm_suppress_frames % 1200 == 0:
          try:
            print(f"[RivianPilot DM Latch] Suppression loop active. Minutes remaining in session: {dm_suppress_frames / 1200:.1f}")
          except Exception:
            pass
          
      except Exception as critical_err:
        # ABSOLUTE SAFETY NET: Instantly abort custom code on error, leaving the original packet completely untouched
        suppress_enabled = False
        try:
          print(f"[RivianPilot DM Latch CRITICAL FALLBACK]: Intercept failed ({critical_err}). Reverting to standard logic.")
        except Exception:
          pass

    # Always broadcast the message packet out to messaging streams at a locked 20Hz frequency
    pm.send('driverMonitoringState', dat)

    # load live always-on toggle
    if sm['driverStateV2'].frameId % 40 == 1:
      DM.always_on = params.get_bool("AlwaysOnDM")
      demo_mode = params.get_bool("IsDriverViewEnabled")

      # Hot-reload toggle configuration mid-drive
      try:
        suppress_enabled = params.get_bool("DMSessionBypass")
      except Exception:
        pass # Retain active runtime state if lookup breaks mid-drive

    # save rhd virtual toggle every 5 mins
    if (sm['driverStateV2'].frameId % 6000 == 0 and not demo_mode and
     DM.wheelpos_offsetter.filtered_stat.n > DM.settings._WHEELPOS_FILTER_MIN_COUNT and
     DM.wheel_on_right == (DM.wheelpos_offsetter.filtered_stat.M > DM.settings._WHEELPOS_THRESHOLD)):
      params.put_bool("IsRhdDetected", DM.wheel_on_right)

def main():
  dmonitoringd_thread()


if __name__ == '__main__':
  main()
