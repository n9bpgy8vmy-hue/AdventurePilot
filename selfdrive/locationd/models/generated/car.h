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
void car_err_fun(double *nom_x, double *delta_x, double *out_224298435625695922);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_246092340363984604);
void car_H_mod_fun(double *state, double *out_4279458356555016822);
void car_f_fun(double *state, double dt, double *out_1545223943974543840);
void car_F_fun(double *state, double dt, double *out_1739281589418524436);
void car_h_25(double *state, double *unused, double *out_6496775841314482133);
void car_H_25(double *state, double *unused, double *out_624233468629648018);
void car_h_24(double *state, double *unused, double *out_8130981640671652032);
void car_H_24(double *state, double *unused, double *out_8042625102306574911);
void car_h_30(double *state, double *unused, double *out_5895357683701118579);
void car_H_30(double *state, double *unused, double *out_3903462861497960180);
void car_h_26(double *state, double *unused, double *out_893684416040272934);
void car_H_26(double *state, double *unused, double *out_3117269850244408206);
void car_h_27(double *state, double *unused, double *out_7327782202938961169);
void car_H_27(double *state, double *unused, double *out_6078226173298385091);
void car_h_29(double *state, double *unused, double *out_553490447815088016);
void car_H_29(double *state, double *unused, double *out_3393231517183567996);
void car_h_28(double *state, double *unused, double *out_2326108935278115524);
void car_H_28(double *state, double *unused, double *out_1429601245618241745);
void car_h_31(double *state, double *unused, double *out_4186702975574397042);
void car_H_31(double *state, double *unused, double *out_3743477952477759682);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}