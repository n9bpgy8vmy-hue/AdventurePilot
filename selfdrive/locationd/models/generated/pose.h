#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2806378377347710691);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6769519321017128267);
void pose_H_mod_fun(double *state, double *out_7023130148617119261);
void pose_f_fun(double *state, double dt, double *out_5185471552589919441);
void pose_F_fun(double *state, double dt, double *out_7795547570733027780);
void pose_h_4(double *state, double *unused, double *out_4129260781455939877);
void pose_H_4(double *state, double *unused, double *out_1078411690355293289);
void pose_h_10(double *state, double *unused, double *out_275654426612283760);
void pose_H_10(double *state, double *unused, double *out_3325379419715504236);
void pose_h_13(double *state, double *unused, double *out_8251649658839689529);
void pose_H_13(double *state, double *unused, double *out_2133862134977039512);
void pose_h_14(double *state, double *unused, double *out_349945032698230729);
void pose_H_14(double *state, double *unused, double *out_2884829165984191240);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}