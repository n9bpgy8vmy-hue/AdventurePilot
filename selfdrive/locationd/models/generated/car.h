#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_601419191659664397);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8444406230307903594);
void car_H_mod_fun(double *state, double *out_2868789824472056264);
void car_f_fun(double *state, double dt, double *out_6067676848218784765);
void car_F_fun(double *state, double dt, double *out_2663403797678380193);
void car_h_25(double *state, double *unused, double *out_2501551368784051534);
void car_H_25(double *state, double *unused, double *out_1993481080133053284);
void car_h_24(double *state, double *unused, double *out_3098465434219434725);
void car_H_24(double *state, double *unused, double *out_6739990407099562318);
void car_h_30(double *state, double *unused, double *out_6139000897038073884);
void car_H_30(double *state, double *unused, double *out_524851878374195343);
void car_h_26(double *state, double *unused, double *out_1137327629377228239);
void car_H_26(double *state, double *unused, double *out_5734984399007109508);
void car_h_27(double *state, double *unused, double *out_6480417803222789231);
void car_H_27(double *state, double *unused, double *out_1649911433426229568);
void car_h_29(double *state, double *unused, double *out_6205223740938283342);
void car_H_29(double *state, double *unused, double *out_1035083222688587527);
void car_h_28(double *state, double *unused, double *out_1478744535561943586);
void car_H_28(double *state, double *unused, double *out_4047315794380943047);
void car_h_31(double *state, double *unused, double *out_8415284395454799907);
void car_H_31(double *state, double *unused, double *out_6361192501240460984);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}