#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_6009606406978622996);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7018273336111784530);
void pose_H_mod_fun(double *state, double *out_2069747755750682261);
void pose_f_fun(double *state, double dt, double *out_5602288806834867512);
void pose_F_fun(double *state, double dt, double *out_7448356295547946507);
void pose_h_4(double *state, double *unused, double *out_5722605381439183411);
void pose_H_4(double *state, double *unused, double *out_7364011898045184863);
void pose_h_10(double *state, double *unused, double *out_8524506374835914602);
void pose_H_10(double *state, double *unused, double *out_5210940050657868130);
void pose_h_13(double *state, double *unused, double *out_7279317158969388905);
void pose_H_13(double *state, double *unused, double *out_4151738072712852062);
void pose_h_14(double *state, double *unused, double *out_4647165386070377155);
void pose_H_14(double *state, double *unused, double *out_3400771041705700334);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}