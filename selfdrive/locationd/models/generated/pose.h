#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_1694318774695559648);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1527863431850837134);
void pose_H_mod_fun(double *state, double *out_2459924378456607327);
void pose_f_fun(double *state, double dt, double *out_862851979799617352);
void pose_F_fun(double *state, double dt, double *out_6264058293643605080);
void pose_h_4(double *state, double *unused, double *out_6982350416274880013);
void pose_H_4(double *state, double *unused, double *out_5740380505516617440);
void pose_h_10(double *state, double *unused, double *out_8881427016800317352);
void pose_H_10(double *state, double *unused, double *out_6831836666791918057);
void pose_h_13(double *state, double *unused, double *out_7903236857747805930);
void pose_H_13(double *state, double *unused, double *out_5095732359876233247);
void pose_h_14(double *state, double *unused, double *out_6008800958186249240);
void pose_H_14(double *state, double *unused, double *out_8743122711853449647);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}