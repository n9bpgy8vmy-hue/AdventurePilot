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
void car_err_fun(double *nom_x, double *delta_x, double *out_1113594304406611873);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1615000100367747659);
void car_H_mod_fun(double *state, double *out_3739319813539620931);
void car_f_fun(double *state, double dt, double *out_1691855582217777164);
void car_F_fun(double *state, double dt, double *out_6948739413312517271);
void car_h_25(double *state, double *unused, double *out_6659975241378880537);
void car_H_25(double *state, double *unused, double *out_7262300463529112608);
void car_h_24(double *state, double *unused, double *out_6745238207722972952);
void car_H_24(double *state, double *unused, double *out_2515791136562603574);
void car_h_30(double *state, double *unused, double *out_7688465534051299215);
void car_H_30(double *state, double *unused, double *out_4267753268688822253);
void car_h_26(double *state, double *unused, double *out_8631975386952961650);
void car_H_26(double *state, double *unused, double *out_3520797144655056384);
void car_h_27(double *state, double *unused, double *out_8749982780886338394);
void car_H_27(double *state, double *unused, double *out_4958198204585447627);
void car_h_29(double *state, double *unused, double *out_5756605271997406756);
void car_H_29(double *state, double *unused, double *out_3244835477715896594);
void car_h_28(double *state, double *unused, double *out_900451828557216082);
void car_H_28(double *state, double *unused, double *out_5208465749281222845);
void car_h_31(double *state, double *unused, double *out_6935169303663386426);
void car_H_31(double *state, double *unused, double *out_7292946425406073036);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}