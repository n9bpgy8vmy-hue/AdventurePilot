#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_6129668094290001530);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_8035171941691199446);
void pose_H_mod_fun(double *state, double *out_1839861344935556007);
void pose_f_fun(double *state, double dt, double *out_5572599306340567392);
void pose_F_fun(double *state, double dt, double *out_7997319965218444246);
void pose_h_4(double *state, double *unused, double *out_1088883195134300204);
void pose_H_4(double *state, double *unused, double *out_7104500321122265335);
void pose_h_10(double *state, double *unused, double *out_308386678055815256);
void pose_H_10(double *state, double *unused, double *out_105212834104445393);
void pose_h_13(double *state, double *unused, double *out_2800280990455093987);
void pose_H_13(double *state, double *unused, double *out_506130887194435594);
void pose_h_14(double *state, double *unused, double *out_1589465505812499279);
void pose_H_14(double *state, double *unused, double *out_3141259464782780806);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}