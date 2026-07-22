#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_601419191659664397) {
   out_601419191659664397[0] = delta_x[0] + nom_x[0];
   out_601419191659664397[1] = delta_x[1] + nom_x[1];
   out_601419191659664397[2] = delta_x[2] + nom_x[2];
   out_601419191659664397[3] = delta_x[3] + nom_x[3];
   out_601419191659664397[4] = delta_x[4] + nom_x[4];
   out_601419191659664397[5] = delta_x[5] + nom_x[5];
   out_601419191659664397[6] = delta_x[6] + nom_x[6];
   out_601419191659664397[7] = delta_x[7] + nom_x[7];
   out_601419191659664397[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8444406230307903594) {
   out_8444406230307903594[0] = -nom_x[0] + true_x[0];
   out_8444406230307903594[1] = -nom_x[1] + true_x[1];
   out_8444406230307903594[2] = -nom_x[2] + true_x[2];
   out_8444406230307903594[3] = -nom_x[3] + true_x[3];
   out_8444406230307903594[4] = -nom_x[4] + true_x[4];
   out_8444406230307903594[5] = -nom_x[5] + true_x[5];
   out_8444406230307903594[6] = -nom_x[6] + true_x[6];
   out_8444406230307903594[7] = -nom_x[7] + true_x[7];
   out_8444406230307903594[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_2868789824472056264) {
   out_2868789824472056264[0] = 1.0;
   out_2868789824472056264[1] = 0.0;
   out_2868789824472056264[2] = 0.0;
   out_2868789824472056264[3] = 0.0;
   out_2868789824472056264[4] = 0.0;
   out_2868789824472056264[5] = 0.0;
   out_2868789824472056264[6] = 0.0;
   out_2868789824472056264[7] = 0.0;
   out_2868789824472056264[8] = 0.0;
   out_2868789824472056264[9] = 0.0;
   out_2868789824472056264[10] = 1.0;
   out_2868789824472056264[11] = 0.0;
   out_2868789824472056264[12] = 0.0;
   out_2868789824472056264[13] = 0.0;
   out_2868789824472056264[14] = 0.0;
   out_2868789824472056264[15] = 0.0;
   out_2868789824472056264[16] = 0.0;
   out_2868789824472056264[17] = 0.0;
   out_2868789824472056264[18] = 0.0;
   out_2868789824472056264[19] = 0.0;
   out_2868789824472056264[20] = 1.0;
   out_2868789824472056264[21] = 0.0;
   out_2868789824472056264[22] = 0.0;
   out_2868789824472056264[23] = 0.0;
   out_2868789824472056264[24] = 0.0;
   out_2868789824472056264[25] = 0.0;
   out_2868789824472056264[26] = 0.0;
   out_2868789824472056264[27] = 0.0;
   out_2868789824472056264[28] = 0.0;
   out_2868789824472056264[29] = 0.0;
   out_2868789824472056264[30] = 1.0;
   out_2868789824472056264[31] = 0.0;
   out_2868789824472056264[32] = 0.0;
   out_2868789824472056264[33] = 0.0;
   out_2868789824472056264[34] = 0.0;
   out_2868789824472056264[35] = 0.0;
   out_2868789824472056264[36] = 0.0;
   out_2868789824472056264[37] = 0.0;
   out_2868789824472056264[38] = 0.0;
   out_2868789824472056264[39] = 0.0;
   out_2868789824472056264[40] = 1.0;
   out_2868789824472056264[41] = 0.0;
   out_2868789824472056264[42] = 0.0;
   out_2868789824472056264[43] = 0.0;
   out_2868789824472056264[44] = 0.0;
   out_2868789824472056264[45] = 0.0;
   out_2868789824472056264[46] = 0.0;
   out_2868789824472056264[47] = 0.0;
   out_2868789824472056264[48] = 0.0;
   out_2868789824472056264[49] = 0.0;
   out_2868789824472056264[50] = 1.0;
   out_2868789824472056264[51] = 0.0;
   out_2868789824472056264[52] = 0.0;
   out_2868789824472056264[53] = 0.0;
   out_2868789824472056264[54] = 0.0;
   out_2868789824472056264[55] = 0.0;
   out_2868789824472056264[56] = 0.0;
   out_2868789824472056264[57] = 0.0;
   out_2868789824472056264[58] = 0.0;
   out_2868789824472056264[59] = 0.0;
   out_2868789824472056264[60] = 1.0;
   out_2868789824472056264[61] = 0.0;
   out_2868789824472056264[62] = 0.0;
   out_2868789824472056264[63] = 0.0;
   out_2868789824472056264[64] = 0.0;
   out_2868789824472056264[65] = 0.0;
   out_2868789824472056264[66] = 0.0;
   out_2868789824472056264[67] = 0.0;
   out_2868789824472056264[68] = 0.0;
   out_2868789824472056264[69] = 0.0;
   out_2868789824472056264[70] = 1.0;
   out_2868789824472056264[71] = 0.0;
   out_2868789824472056264[72] = 0.0;
   out_2868789824472056264[73] = 0.0;
   out_2868789824472056264[74] = 0.0;
   out_2868789824472056264[75] = 0.0;
   out_2868789824472056264[76] = 0.0;
   out_2868789824472056264[77] = 0.0;
   out_2868789824472056264[78] = 0.0;
   out_2868789824472056264[79] = 0.0;
   out_2868789824472056264[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_6067676848218784765) {
   out_6067676848218784765[0] = state[0];
   out_6067676848218784765[1] = state[1];
   out_6067676848218784765[2] = state[2];
   out_6067676848218784765[3] = state[3];
   out_6067676848218784765[4] = state[4];
   out_6067676848218784765[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_6067676848218784765[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_6067676848218784765[7] = state[7];
   out_6067676848218784765[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2663403797678380193) {
   out_2663403797678380193[0] = 1;
   out_2663403797678380193[1] = 0;
   out_2663403797678380193[2] = 0;
   out_2663403797678380193[3] = 0;
   out_2663403797678380193[4] = 0;
   out_2663403797678380193[5] = 0;
   out_2663403797678380193[6] = 0;
   out_2663403797678380193[7] = 0;
   out_2663403797678380193[8] = 0;
   out_2663403797678380193[9] = 0;
   out_2663403797678380193[10] = 1;
   out_2663403797678380193[11] = 0;
   out_2663403797678380193[12] = 0;
   out_2663403797678380193[13] = 0;
   out_2663403797678380193[14] = 0;
   out_2663403797678380193[15] = 0;
   out_2663403797678380193[16] = 0;
   out_2663403797678380193[17] = 0;
   out_2663403797678380193[18] = 0;
   out_2663403797678380193[19] = 0;
   out_2663403797678380193[20] = 1;
   out_2663403797678380193[21] = 0;
   out_2663403797678380193[22] = 0;
   out_2663403797678380193[23] = 0;
   out_2663403797678380193[24] = 0;
   out_2663403797678380193[25] = 0;
   out_2663403797678380193[26] = 0;
   out_2663403797678380193[27] = 0;
   out_2663403797678380193[28] = 0;
   out_2663403797678380193[29] = 0;
   out_2663403797678380193[30] = 1;
   out_2663403797678380193[31] = 0;
   out_2663403797678380193[32] = 0;
   out_2663403797678380193[33] = 0;
   out_2663403797678380193[34] = 0;
   out_2663403797678380193[35] = 0;
   out_2663403797678380193[36] = 0;
   out_2663403797678380193[37] = 0;
   out_2663403797678380193[38] = 0;
   out_2663403797678380193[39] = 0;
   out_2663403797678380193[40] = 1;
   out_2663403797678380193[41] = 0;
   out_2663403797678380193[42] = 0;
   out_2663403797678380193[43] = 0;
   out_2663403797678380193[44] = 0;
   out_2663403797678380193[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2663403797678380193[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2663403797678380193[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2663403797678380193[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2663403797678380193[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2663403797678380193[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2663403797678380193[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2663403797678380193[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2663403797678380193[53] = -9.8100000000000005*dt;
   out_2663403797678380193[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2663403797678380193[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2663403797678380193[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2663403797678380193[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2663403797678380193[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2663403797678380193[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2663403797678380193[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2663403797678380193[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2663403797678380193[62] = 0;
   out_2663403797678380193[63] = 0;
   out_2663403797678380193[64] = 0;
   out_2663403797678380193[65] = 0;
   out_2663403797678380193[66] = 0;
   out_2663403797678380193[67] = 0;
   out_2663403797678380193[68] = 0;
   out_2663403797678380193[69] = 0;
   out_2663403797678380193[70] = 1;
   out_2663403797678380193[71] = 0;
   out_2663403797678380193[72] = 0;
   out_2663403797678380193[73] = 0;
   out_2663403797678380193[74] = 0;
   out_2663403797678380193[75] = 0;
   out_2663403797678380193[76] = 0;
   out_2663403797678380193[77] = 0;
   out_2663403797678380193[78] = 0;
   out_2663403797678380193[79] = 0;
   out_2663403797678380193[80] = 1;
}
void h_25(double *state, double *unused, double *out_2501551368784051534) {
   out_2501551368784051534[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1993481080133053284) {
   out_1993481080133053284[0] = 0;
   out_1993481080133053284[1] = 0;
   out_1993481080133053284[2] = 0;
   out_1993481080133053284[3] = 0;
   out_1993481080133053284[4] = 0;
   out_1993481080133053284[5] = 0;
   out_1993481080133053284[6] = 1;
   out_1993481080133053284[7] = 0;
   out_1993481080133053284[8] = 0;
}
void h_24(double *state, double *unused, double *out_3098465434219434725) {
   out_3098465434219434725[0] = state[4];
   out_3098465434219434725[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6739990407099562318) {
   out_6739990407099562318[0] = 0;
   out_6739990407099562318[1] = 0;
   out_6739990407099562318[2] = 0;
   out_6739990407099562318[3] = 0;
   out_6739990407099562318[4] = 1;
   out_6739990407099562318[5] = 0;
   out_6739990407099562318[6] = 0;
   out_6739990407099562318[7] = 0;
   out_6739990407099562318[8] = 0;
   out_6739990407099562318[9] = 0;
   out_6739990407099562318[10] = 0;
   out_6739990407099562318[11] = 0;
   out_6739990407099562318[12] = 0;
   out_6739990407099562318[13] = 0;
   out_6739990407099562318[14] = 1;
   out_6739990407099562318[15] = 0;
   out_6739990407099562318[16] = 0;
   out_6739990407099562318[17] = 0;
}
void h_30(double *state, double *unused, double *out_6139000897038073884) {
   out_6139000897038073884[0] = state[4];
}
void H_30(double *state, double *unused, double *out_524851878374195343) {
   out_524851878374195343[0] = 0;
   out_524851878374195343[1] = 0;
   out_524851878374195343[2] = 0;
   out_524851878374195343[3] = 0;
   out_524851878374195343[4] = 1;
   out_524851878374195343[5] = 0;
   out_524851878374195343[6] = 0;
   out_524851878374195343[7] = 0;
   out_524851878374195343[8] = 0;
}
void h_26(double *state, double *unused, double *out_1137327629377228239) {
   out_1137327629377228239[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5734984399007109508) {
   out_5734984399007109508[0] = 0;
   out_5734984399007109508[1] = 0;
   out_5734984399007109508[2] = 0;
   out_5734984399007109508[3] = 0;
   out_5734984399007109508[4] = 0;
   out_5734984399007109508[5] = 0;
   out_5734984399007109508[6] = 0;
   out_5734984399007109508[7] = 1;
   out_5734984399007109508[8] = 0;
}
void h_27(double *state, double *unused, double *out_6480417803222789231) {
   out_6480417803222789231[0] = state[3];
}
void H_27(double *state, double *unused, double *out_1649911433426229568) {
   out_1649911433426229568[0] = 0;
   out_1649911433426229568[1] = 0;
   out_1649911433426229568[2] = 0;
   out_1649911433426229568[3] = 1;
   out_1649911433426229568[4] = 0;
   out_1649911433426229568[5] = 0;
   out_1649911433426229568[6] = 0;
   out_1649911433426229568[7] = 0;
   out_1649911433426229568[8] = 0;
}
void h_29(double *state, double *unused, double *out_6205223740938283342) {
   out_6205223740938283342[0] = state[1];
}
void H_29(double *state, double *unused, double *out_1035083222688587527) {
   out_1035083222688587527[0] = 0;
   out_1035083222688587527[1] = 1;
   out_1035083222688587527[2] = 0;
   out_1035083222688587527[3] = 0;
   out_1035083222688587527[4] = 0;
   out_1035083222688587527[5] = 0;
   out_1035083222688587527[6] = 0;
   out_1035083222688587527[7] = 0;
   out_1035083222688587527[8] = 0;
}
void h_28(double *state, double *unused, double *out_1478744535561943586) {
   out_1478744535561943586[0] = state[0];
}
void H_28(double *state, double *unused, double *out_4047315794380943047) {
   out_4047315794380943047[0] = 1;
   out_4047315794380943047[1] = 0;
   out_4047315794380943047[2] = 0;
   out_4047315794380943047[3] = 0;
   out_4047315794380943047[4] = 0;
   out_4047315794380943047[5] = 0;
   out_4047315794380943047[6] = 0;
   out_4047315794380943047[7] = 0;
   out_4047315794380943047[8] = 0;
}
void h_31(double *state, double *unused, double *out_8415284395454799907) {
   out_8415284395454799907[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6361192501240460984) {
   out_6361192501240460984[0] = 0;
   out_6361192501240460984[1] = 0;
   out_6361192501240460984[2] = 0;
   out_6361192501240460984[3] = 0;
   out_6361192501240460984[4] = 0;
   out_6361192501240460984[5] = 0;
   out_6361192501240460984[6] = 0;
   out_6361192501240460984[7] = 0;
   out_6361192501240460984[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_601419191659664397) {
  err_fun(nom_x, delta_x, out_601419191659664397);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8444406230307903594) {
  inv_err_fun(nom_x, true_x, out_8444406230307903594);
}
void car_H_mod_fun(double *state, double *out_2868789824472056264) {
  H_mod_fun(state, out_2868789824472056264);
}
void car_f_fun(double *state, double dt, double *out_6067676848218784765) {
  f_fun(state,  dt, out_6067676848218784765);
}
void car_F_fun(double *state, double dt, double *out_2663403797678380193) {
  F_fun(state,  dt, out_2663403797678380193);
}
void car_h_25(double *state, double *unused, double *out_2501551368784051534) {
  h_25(state, unused, out_2501551368784051534);
}
void car_H_25(double *state, double *unused, double *out_1993481080133053284) {
  H_25(state, unused, out_1993481080133053284);
}
void car_h_24(double *state, double *unused, double *out_3098465434219434725) {
  h_24(state, unused, out_3098465434219434725);
}
void car_H_24(double *state, double *unused, double *out_6739990407099562318) {
  H_24(state, unused, out_6739990407099562318);
}
void car_h_30(double *state, double *unused, double *out_6139000897038073884) {
  h_30(state, unused, out_6139000897038073884);
}
void car_H_30(double *state, double *unused, double *out_524851878374195343) {
  H_30(state, unused, out_524851878374195343);
}
void car_h_26(double *state, double *unused, double *out_1137327629377228239) {
  h_26(state, unused, out_1137327629377228239);
}
void car_H_26(double *state, double *unused, double *out_5734984399007109508) {
  H_26(state, unused, out_5734984399007109508);
}
void car_h_27(double *state, double *unused, double *out_6480417803222789231) {
  h_27(state, unused, out_6480417803222789231);
}
void car_H_27(double *state, double *unused, double *out_1649911433426229568) {
  H_27(state, unused, out_1649911433426229568);
}
void car_h_29(double *state, double *unused, double *out_6205223740938283342) {
  h_29(state, unused, out_6205223740938283342);
}
void car_H_29(double *state, double *unused, double *out_1035083222688587527) {
  H_29(state, unused, out_1035083222688587527);
}
void car_h_28(double *state, double *unused, double *out_1478744535561943586) {
  h_28(state, unused, out_1478744535561943586);
}
void car_H_28(double *state, double *unused, double *out_4047315794380943047) {
  H_28(state, unused, out_4047315794380943047);
}
void car_h_31(double *state, double *unused, double *out_8415284395454799907) {
  h_31(state, unused, out_8415284395454799907);
}
void car_H_31(double *state, double *unused, double *out_6361192501240460984) {
  H_31(state, unused, out_6361192501240460984);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
