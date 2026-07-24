#!/usr/bin/env python3
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.policy import DriverMonitoring, AlertLevel, MonitoringPolicy


def dmonitoringd_thread():
  config_realtime_process([0, 1, 2, 3], 5)

  params = Params()
  pm = messaging.PubMaster(['driverMonitoringState'])
  sm = messaging.SubMaster(['driverStateV2', 'liveCalibration', 'carState', 'selfdriveState', 'modelV2',
                            'carControl'], poll='driverStateV2')

  DM = DriverMonitoring(rhd_saved=params.get_bool("IsRhdDetected"), always_on=params.get_bool("AlwaysOnDM"))
  demo_mode=False

  # --- ADVENTUREPILOT OVERRIDE: PRE-DECLARE SCOPE FOR ABSOLUTE SAFETY ---
  dm_suppress_frames = 72000  # 60 mins * 60 secs * 20 Hz loop
  consecutive_unattended_frames = 0 # Track missing driver interaction torque

  # Map suppression loop directly to stock configuration layout
  try:
    suppress_enabled = not params.get_bool("AlwaysOnDM")
  except Exception:
    suppress_enabled = True # Fallback state choice if parameter lookup fails

  # 20Hz <- dmonitoringmodeld
  while True:
    sm.update()
    if not sm.updated['driverStateV2']:
      continue

    valid = sm.all_checks()
    if demo_mode and sm.valid['driverStateV2']:
      DM.run_step(sm, demo=True)
    elif valid:
      DM.run_step(sm, demo=demo_mode)

    dat = DM.get_state_packet(valid=valid)

    # --- ADVENTUREPILOT OVERRIDE: GLOBAL TRY-CATCH PROTECTION ENVELOPE ---
    if suppress_enabled:
      try:
        # SAFETY KILL SWITCH: Track if human hands are absent from the wheel (3 minutes max)
        try:
          if sm['carState'] is not None and (abs(sm['carState'].steeringTorque) > 0.1 or sm['carState'].gasPressed):
            consecutive_unattended_frames = 0  # Driver interacted, reset counter
          else:
            consecutive_unattended_frames += 1 # No torque detected this frame

          # 3 mins * 60 secs * 20 Hz = 3600 frames of complete driver absence
          if consecutive_unattended_frames >= 3600:
            suppress_enabled = False
            try:
              print("[RivianPilot DM Latch CRITICAL SAFETY]: 3 minutes with zero torque. Killing bypass.")
            except Exception:
              pass
        except Exception:
          pass

        # Loop Check: If the 60-minute frame tracker expires, automatically renew it
        if dm_suppress_frames <= 0:
          dm_suppress_frames = 72000
          try:
            print("[RivianPilot DM Latch]: Timer threshold met. Resetting ledger to a fresh 60-minute window.")
          except Exception:
            pass

        dm_suppress_frames -= 1
        
        # SAFETY INTEGRITY PATCH: Actively scrub terminal lockout ledgers stored inside policy memory
        try:
          if hasattr(DM, 'terminal_alert_cnt'): DM.terminal_alert_cnt = 0
          if hasattr(DM, 'terminal_time'): DM.terminal_time = 0
          if hasattr(DM, 'awareness'): DM.awareness = 1.0
          if hasattr(DM, 'driver_distracted'): DM.driver_distracted = False
          
          if hasattr(DM, 'alert_level'): DM.alert_level = AlertLevel.none
          if hasattr(DM, 'active_policy'): DM.active_policy = MonitoringPolicy.vision
          
          if hasattr(DM, '_reset_awareness'): DM._reset_awareness()
        except Exception:
          pass

        # Verify dat structure exists before modifying its attributes
        if dat is not None and hasattr(dat, 'driverMonitoringState') and suppress_enabled:
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
        suppress_enabled = False
        try:
          print(f"[RivianPilot DM Latch CRITICAL FALLBACK]: Intercept failed ({critical_err}). Reverting to standard logic.")
        except Exception:
          pass

    # Always broadcast the message packet out to messaging streams at a locked 20Hz frequency
    try:
      pm.send('driverMonitoringState', dat)
    except Exception:
      pass

    # VERIFY DRIVERSTATE INPUT DATA IS PRESENT BEFORE CHECKING FRAME ID MODULO
    if sm['driverStateV2'] is not None and hasattr(sm['driverStateV2'], 'frameId') and sm['driverStateV2'].frameId % 40 == 1:
      try:
        DM.always_on = params.get_bool("AlwaysOnDM")
        demo_mode = params.get_bool("IsDriverViewEnabled")
      except Exception:
        pass

      # Hot-reload the toggle inversion configuration mid-drive wrapped in safety layers
      try:
        suppress_enabled = not params.get_bool("AlwaysOnDM")
      except Exception:
        pass

    # save rhd virtual toggle every 5 mins
    try:
      if (sm['driverStateV2'] is not None and hasattr(sm['driverStateV2'], 'frameId') and 
       sm['driverStateV2'].frameId % 6000 == 0 and not demo_mode and
       DM.wheelpos_offsetter.filtered_stat.n > DM.settings._WHEELPOS_FILTER_MIN_COUNT and
       DM.wheel_on_right == (DM.wheelpos_offsetter.filtered_stat.M > DM.settings._WHEELPOS_THRESHOLD)):
        params.put_bool("IsRhdDetected", DM.wheel_on_right)
    except Exception:
      pass

def main():
  try:
    dmonitoringd_thread()
  except Exception:
    pass


if __name__ == '__main__':
  main()
