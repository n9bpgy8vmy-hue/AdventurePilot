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
void car_err_fun(double *nom_x, double *delta_x, double *out_9000377994212201456);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8968830433719023777);
void car_H_mod_fun(double *state, double *out_9201143408597581971);
void car_f_fun(double *state, double dt, double *out_4422444681951896033);
void car_F_fun(double *state, double dt, double *out_3632050189681908649);
void car_h_25(double *state, double *unused, double *out_8556756409565152671);
void car_H_25(double *state, double *unused, double *out_1324262632138109840);
void car_h_24(double *state, double *unused, double *out_2600025209261031362);
void car_H_24(double *state, double *unused, double *out_8766937225126826160);
void car_h_30(double *state, double *unused, double *out_1968928483088857508);
void car_H_30(double *state, double *unused, double *out_5851958962265718038);
void car_h_26(double *state, double *unused, double *out_7835892302344293645);
void car_H_26(double *state, double *unused, double *out_5065765951012166064);
void car_h_27(double *state, double *unused, double *out_3555083141904307026);
void car_H_27(double *state, double *unused, double *out_8026722274066142949);
void car_h_29(double *state, double *unused, double *out_3032744784571988137);
void car_H_29(double *state, double *unused, double *out_5341727617951325854);
void car_h_28(double *state, double *unused, double *out_8635836209846197336);
void car_H_28(double *state, double *unused, double *out_8022617438688695188);
void car_h_31(double *state, double *unused, double *out_6252538135890376595);
void car_H_31(double *state, double *unused, double *out_5691974053245517540);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}