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
void car_err_fun(double *nom_x, double *delta_x, double *out_3924106122848769848);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_295877716488110901);
void car_H_mod_fun(double *state, double *out_2075088556811994884);
void car_f_fun(double *state, double dt, double *out_5212472755668323988);
void car_F_fun(double *state, double dt, double *out_332930578810202466);
void car_h_25(double *state, double *unused, double *out_8627932228849007647);
void car_H_25(double *state, double *unused, double *out_7178467290985319495);
void car_h_24(double *state, double *unused, double *out_6158507038393289631);
void car_H_24(double *state, double *unused, double *out_9042568998745363559);
void car_h_30(double *state, double *unused, double *out_8416847386710087001);
void car_H_30(double *state, double *unused, double *out_4351586441232615366);
void car_h_26(double *state, double *unused, double *out_3991486471411855468);
void car_H_26(double *state, double *unused, double *out_3436963972111263271);
void car_h_27(double *state, double *unused, double *out_3626258961188162002);
void car_H_27(double *state, double *unused, double *out_6526349753033040277);
void car_h_29(double *state, double *unused, double *out_11190567065860348);
void car_H_29(double *state, double *unused, double *out_3841355096918223182);
void car_h_28(double *state, double *unused, double *out_7385341678092688801);
void car_H_28(double *state, double *unused, double *out_5124632576737429732);
void car_h_31(double *state, double *unused, double *out_1808430068020230512);
void car_H_31(double *state, double *unused, double *out_7209113252862279923);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}