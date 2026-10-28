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
void live_H(double *in_vec, double *out_914486509044661731);
void live_err_fun(double *nom_x, double *delta_x, double *out_4192608500703087650);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_6408029548143719895);
void live_H_mod_fun(double *state, double *out_9157722347086796174);
void live_f_fun(double *state, double dt, double *out_3913903153448683206);
void live_F_fun(double *state, double dt, double *out_6600325781017639204);
void live_h_4(double *state, double *unused, double *out_6252844089262010874);
void live_H_4(double *state, double *unused, double *out_3107067527245144939);
void live_h_9(double *state, double *unused, double *out_1350981492391499072);
void live_H_9(double *state, double *unused, double *out_2865877880615554294);
void live_h_10(double *state, double *unused, double *out_2642301617111915071);
void live_H_10(double *state, double *unused, double *out_3488056870982252636);
void live_h_12(double *state, double *unused, double *out_512510834702633023);
void live_H_12(double *state, double *unused, double *out_1912388880786816856);
void live_h_35(double *state, double *unused, double *out_3829578536231626836);
void live_H_35(double *state, double *unused, double *out_259594530127462437);
void live_h_32(double *state, double *unused, double *out_5017118348894392132);
void live_H_32(double *state, double *unused, double *out_7441610077530248158);
void live_h_13(double *state, double *unused, double *out_4340798269744014671);
void live_H_13(double *state, double *unused, double *out_5105550115273818223);
void live_h_14(double *state, double *unused, double *out_1350981492391499072);
void live_H_14(double *state, double *unused, double *out_2865877880615554294);
void live_h_33(double *state, double *unused, double *out_5261964068145175709);
void live_H_33(double *state, double *unused, double *out_3410151534766320041);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}