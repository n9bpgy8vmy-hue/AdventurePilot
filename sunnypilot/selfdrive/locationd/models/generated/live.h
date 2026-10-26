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
void live_H(double *in_vec, double *out_6526915109055246386);
void live_err_fun(double *nom_x, double *delta_x, double *out_5590691638403910847);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5178236399156366824);
void live_H_mod_fun(double *state, double *out_481025187510325863);
void live_f_fun(double *state, double dt, double *out_7993885765710005037);
void live_F_fun(double *state, double dt, double *out_5739006166566628385);
void live_h_4(double *state, double *unused, double *out_191530746733227921);
void live_H_4(double *state, double *unused, double *out_631391594853702196);
void live_h_9(double *state, double *unused, double *out_7360553224280239975);
void live_H_9(double *state, double *unused, double *out_3525776141501075287);
void live_h_10(double *state, double *unused, double *out_5763456777166484282);
void live_H_10(double *state, double *unused, double *out_8970628791985655579);
void live_h_12(double *state, double *unused, double *out_7942094291557644221);
void live_H_12(double *state, double *unused, double *out_1252490619901295863);
void live_h_35(double *state, double *unused, double *out_2268453483933334346);
void live_H_35(double *state, double *unused, double *out_3998053652226309572);
void live_h_32(double *state, double *unused, double *out_8337604899153432200);
void live_H_32(double *state, double *unused, double *out_342951123506797316);
void live_h_13(double *state, double *unused, double *out_229487251379413592);
void live_H_13(double *state, double *unused, double *out_7071681902550636598);
void live_h_14(double *state, double *unused, double *out_7360553224280239975);
void live_H_14(double *state, double *unused, double *out_3525776141501075287);
void live_h_33(double *state, double *unused, double *out_7349526668324464107);
void live_H_33(double *state, double *unused, double *out_7148610656865167176);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}