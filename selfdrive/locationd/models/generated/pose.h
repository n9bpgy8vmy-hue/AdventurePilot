#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2896449614613514894);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7848261021210579995);
void pose_H_mod_fun(double *state, double *out_1001504416551130467);
void pose_f_fun(double *state, double dt, double *out_3536772200585022865);
void pose_F_fun(double *state, double dt, double *out_2853128647338935157);
void pose_h_4(double *state, double *unused, double *out_2776811696301497625);
void pose_H_4(double *state, double *unused, double *out_4846577390200828254);
void pose_h_10(double *state, double *unused, double *out_8680472392571157658);
void pose_H_10(double *state, double *unused, double *out_2994919368328266305);
void pose_h_13(double *state, double *unused, double *out_8479283368027075574);
void pose_H_13(double *state, double *unused, double *out_1634303564868495453);
void pose_h_14(double *state, double *unused, double *out_6479428408185621571);
void pose_H_14(double *state, double *unused, double *out_883336533861343725);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}