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
void live_H(double *in_vec, double *out_4123666139337923227);
void live_err_fun(double *nom_x, double *delta_x, double *out_6257871288334760387);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_750662778335564654);
void live_H_mod_fun(double *state, double *out_7048165149536674149);
void live_f_fun(double *state, double dt, double *out_8526888296506604367);
void live_F_fun(double *state, double dt, double *out_7278181980142455985);
void live_h_4(double *state, double *unused, double *out_5151655482057911327);
void live_H_4(double *state, double *unused, double *out_8348806789302824252);
void live_h_9(double *state, double *unused, double *out_3442168079240756246);
void live_H_9(double *state, double *unused, double *out_1061587854038376782);
void live_h_10(double *state, double *unused, double *out_2490857774610267509);
void live_H_10(double *state, double *unused, double *out_138513917014987603);
void live_h_12(double *state, double *unused, double *out_5824443406507322553);
void live_H_12(double *state, double *unused, double *out_7727707764255230585);
void live_h_35(double *state, double *unused, double *out_4274593111408923978);
void live_H_35(double *state, double *unused, double *out_4982144731930216876);
void live_h_32(double *state, double *unused, double *out_5756799683362710561);
void live_H_32(double *state, double *unused, double *out_2964056991041309981);
void live_h_13(double *state, double *unused, double *out_35401514536736591);
void live_H_13(double *state, double *unused, double *out_4969432717040657377);
void live_h_14(double *state, double *unused, double *out_3442168079240756246);
void live_H_14(double *state, double *unused, double *out_1061587854038376782);
void live_h_33(double *state, double *unused, double *out_8350735230499231646);
void live_H_33(double *state, double *unused, double *out_1831587727291359272);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}