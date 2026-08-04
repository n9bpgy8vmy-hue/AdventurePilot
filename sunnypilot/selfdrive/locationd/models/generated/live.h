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
void live_H(double *in_vec, double *out_4085420471447912561);
void live_err_fun(double *nom_x, double *delta_x, double *out_1147421228885968089);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_6006074350638807357);
void live_H_mod_fun(double *state, double *out_6565813408332961927);
void live_f_fun(double *state, double dt, double *out_2004525202202356284);
void live_F_fun(double *state, double dt, double *out_2551503688943933409);
void live_h_4(double *state, double *unused, double *out_2470768691104874466);
void live_H_4(double *state, double *unused, double *out_3820506649700183153);
void live_h_9(double *state, double *unused, double *out_2455666447203645998);
void live_H_9(double *state, double *unused, double *out_336661086654594330);
void live_h_10(double *state, double *unused, double *out_889668250851075734);
void live_H_10(double *state, double *unused, double *out_8871930108727704896);
void live_h_12(double *state, double *unused, double *out_3593551603115088559);
void live_H_12(double *state, double *unused, double *out_4441605674747776820);
void live_h_35(double *state, double *unused, double *out_6503196321519188515);
void live_H_35(double *state, double *unused, double *out_7187168707072790529);
void live_h_32(double *state, double *unused, double *out_6696747232997019166);
void live_H_32(double *state, double *unused, double *out_1429887913583486669);
void live_h_13(double *state, double *unused, double *out_3729987384510597130);
void live_H_13(double *state, double *unused, double *out_3071112897394787060);
void live_h_14(double *state, double *unused, double *out_2455666447203645998);
void live_H_14(double *state, double *unused, double *out_336661086654594330);
void live_h_33(double *state, double *unused, double *out_1288891217411474012);
void live_H_33(double *state, double *unused, double *out_8109018361997903483);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}