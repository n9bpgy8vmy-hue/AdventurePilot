#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_5709026608963937263);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9100639615517483437);
void pose_H_mod_fun(double *state, double *out_372251806579087438);
void pose_f_fun(double *state, double dt, double *out_1740152809646530052);
void pose_F_fun(double *state, double dt, double *out_1353467089812513678);
void pose_h_4(double *state, double *unused, double *out_1851635960522651698);
void pose_H_4(double *state, double *unused, double *out_3399555403264820134);
void pose_h_10(double *state, double *unused, double *out_3439758590286442773);
void pose_H_10(double *state, double *unused, double *out_3773575779738063167);
void pose_h_13(double *state, double *unused, double *out_6062599988059421211);
void pose_H_13(double *state, double *unused, double *out_6611829228597152935);
void pose_h_14(double *state, double *unused, double *out_5840628258535751267);
void pose_H_14(double *state, double *unused, double *out_316766970969447838);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}