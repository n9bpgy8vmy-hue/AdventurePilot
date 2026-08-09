#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_7437321502825855060);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2999325586563752225);
void pose_H_mod_fun(double *state, double *out_38184059748906263);
void pose_f_fun(double *state, double dt, double *out_3123441002119390513);
void pose_F_fun(double *state, double dt, double *out_7604329213029297715);
void pose_h_4(double *state, double *unused, double *out_3079979003165382555);
void pose_H_4(double *state, double *unused, double *out_9051168479662590755);
void pose_h_10(double *state, double *unused, double *out_207658145770227555);
void pose_H_10(double *state, double *unused, double *out_400170142804588747);
void pose_h_13(double *state, double *unused, double *out_5067820152380537214);
void pose_H_13(double *state, double *unused, double *out_6183301768714628060);
void pose_h_14(double *state, double *unused, double *out_7916219184308929140);
void pose_H_14(double *state, double *unused, double *out_8616051953017707156);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}