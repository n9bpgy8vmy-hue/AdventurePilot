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
void live_H(double *in_vec, double *out_8568510632117623450);
void live_err_fun(double *nom_x, double *delta_x, double *out_8643634735066293074);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2893014789582749238);
void live_H_mod_fun(double *state, double *out_264367461545582282);
void live_f_fun(double *state, double dt, double *out_2455377741109885261);
void live_F_fun(double *state, double dt, double *out_7419450473869867511);
void live_h_4(double *state, double *unused, double *out_2689657685173416455);
void live_H_4(double *state, double *unused, double *out_1200851063578706643);
void live_h_9(double *state, double *unused, double *out_1310857176558934360);
void live_H_9(double *state, double *unused, double *out_8488069998843154113);
void live_h_10(double *state, double *unused, double *out_3017837665157239100);
void live_H_10(double *state, double *unused, double *out_6821337756598017841);
void live_h_12(double *state, double *unused, double *out_2377300687363926339);
void live_H_12(double *state, double *unused, double *out_6220307471610668438);
void live_h_35(double *state, double *unused, double *out_8048815376530893376);
void live_H_35(double *state, double *unused, double *out_8965870503935682147);
void live_h_32(double *state, double *unused, double *out_3580057496210904748);
void live_H_32(double *state, double *unused, double *out_4698677905892054488);
void live_h_13(double *state, double *unused, double *out_2518949243856380334);
void live_H_13(double *state, double *unused, double *out_986221089313211891);
void live_h_14(double *state, double *unused, double *out_1310857176558934360);
void live_H_14(double *state, double *unused, double *out_8488069998843154113);
void live_h_33(double *state, double *unused, double *out_774240050443405146);
void live_H_33(double *state, double *unused, double *out_6330316565135011865);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}