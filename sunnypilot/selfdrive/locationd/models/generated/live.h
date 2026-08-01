#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_5659636377840311484);
void live_err_fun(double *nom_x, double *delta_x, double *out_6747310299157848703);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_3611737236074382282);
void live_H_mod_fun(double *state, double *out_6452301196121584385);
void live_f_fun(double *state, double dt, double *out_1993270872132566873);
void live_F_fun(double *state, double dt, double *out_1150322535863346023);
void live_h_4(double *state, double *unused, double *out_7813843799838027882);
void live_H_4(double *state, double *unused, double *out_1298137527970650782);
void live_h_9(double *state, double *unused, double *out_1853463940561154341);
void live_H_9(double *state, double *unused, double *out_8585356463235098252);
void live_h_10(double *state, double *unused, double *out_73087431827676527);
void live_H_10(double *state, double *unused, double *out_4137933468278839214);
void live_h_12(double *state, double *unused, double *out_2840791334162750862);
void live_H_12(double *state, double *unused, double *out_5083120849072082214);
void live_h_35(double *state, double *unused, double *out_7839703626951315814);
void live_H_35(double *state, double *unused, double *out_9063156968327626286);
void live_h_32(double *state, double *unused, double *out_1445970095623956661);
void live_H_32(double *state, double *unused, double *out_4795964370283998627);
void live_h_13(double *state, double *unused, double *out_704300614945100093);
void live_H_13(double *state, double *unused, double *out_5657847277767723281);
void live_h_14(double *state, double *unused, double *out_1853463940561154341);
void live_H_14(double *state, double *unused, double *out_8585356463235098252);
void live_h_33(double *state, double *unused, double *out_2566494479596735255);
void live_H_33(double *state, double *unused, double *out_7815356589982115762);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}