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
void car_err_fun(double *nom_x, double *delta_x, double *out_353556826058708159);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8600824748059787407);
void car_H_mod_fun(double *state, double *out_5193379997354933514);
void car_f_fun(double *state, double dt, double *out_5326070812951508673);
void car_F_fun(double *state, double dt, double *out_6549260589681859698);
void car_h_25(double *state, double *unused, double *out_1817901079504340394);
void car_H_25(double *state, double *unused, double *out_2727958035618926291);
void car_h_24(double *state, double *unused, double *out_5841318999192697280);
void car_H_24(double *state, double *unused, double *out_555308436613426725);
void car_h_30(double *state, double *unused, double *out_9168284229956322897);
void car_H_30(double *state, double *unused, double *out_5246290994126174918);
void car_h_26(double *state, double *unused, double *out_371974765690325757);
void car_H_26(double *state, double *unused, double *out_1013545283255129933);
void car_h_27(double *state, double *unused, double *out_8216942596918189540);
void car_H_27(double *state, double *unused, double *out_3071527682325750007);
void car_h_29(double *state, double *unused, double *out_8675143959076751202);
void car_H_29(double *state, double *unused, double *out_5756522338440567102);
void car_h_28(double *state, double *unused, double *out_9160452449819851975);
void car_H_28(double *state, double *unused, double *out_674123321371036528);
void car_h_31(double *state, double *unused, double *out_5455350607758362744);
void car_H_31(double *state, double *unused, double *out_1639753385488481409);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}