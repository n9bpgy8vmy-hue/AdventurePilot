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
void err_fun(double *nom_x, double *delta_x, double *out_3924106122848769848) {
   out_3924106122848769848[0] = delta_x[0] + nom_x[0];
   out_3924106122848769848[1] = delta_x[1] + nom_x[1];
   out_3924106122848769848[2] = delta_x[2] + nom_x[2];
   out_3924106122848769848[3] = delta_x[3] + nom_x[3];
   out_3924106122848769848[4] = delta_x[4] + nom_x[4];
   out_3924106122848769848[5] = delta_x[5] + nom_x[5];
   out_3924106122848769848[6] = delta_x[6] + nom_x[6];
   out_3924106122848769848[7] = delta_x[7] + nom_x[7];
   out_3924106122848769848[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_295877716488110901) {
   out_295877716488110901[0] = -nom_x[0] + true_x[0];
   out_295877716488110901[1] = -nom_x[1] + true_x[1];
   out_295877716488110901[2] = -nom_x[2] + true_x[2];
   out_295877716488110901[3] = -nom_x[3] + true_x[3];
   out_295877716488110901[4] = -nom_x[4] + true_x[4];
   out_295877716488110901[5] = -nom_x[5] + true_x[5];
   out_295877716488110901[6] = -nom_x[6] + true_x[6];
   out_295877716488110901[7] = -nom_x[7] + true_x[7];
   out_295877716488110901[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_2075088556811994884) {
   out_2075088556811994884[0] = 1.0;
   out_2075088556811994884[1] = 0.0;
   out_2075088556811994884[2] = 0.0;
   out_2075088556811994884[3] = 0.0;
   out_2075088556811994884[4] = 0.0;
   out_2075088556811994884[5] = 0.0;
   out_2075088556811994884[6] = 0.0;
   out_2075088556811994884[7] = 0.0;
   out_2075088556811994884[8] = 0.0;
   out_2075088556811994884[9] = 0.0;
   out_2075088556811994884[10] = 1.0;
   out_2075088556811994884[11] = 0.0;
   out_2075088556811994884[12] = 0.0;
   out_2075088556811994884[13] = 0.0;
   out_2075088556811994884[14] = 0.0;
   out_2075088556811994884[15] = 0.0;
   out_2075088556811994884[16] = 0.0;
   out_2075088556811994884[17] = 0.0;
   out_2075088556811994884[18] = 0.0;
   out_2075088556811994884[19] = 0.0;
   out_2075088556811994884[20] = 1.0;
   out_2075088556811994884[21] = 0.0;
   out_2075088556811994884[22] = 0.0;
   out_2075088556811994884[23] = 0.0;
   out_2075088556811994884[24] = 0.0;
   out_2075088556811994884[25] = 0.0;
   out_2075088556811994884[26] = 0.0;
   out_2075088556811994884[27] = 0.0;
   out_2075088556811994884[28] = 0.0;
   out_2075088556811994884[29] = 0.0;
   out_2075088556811994884[30] = 1.0;
   out_2075088556811994884[31] = 0.0;
   out_2075088556811994884[32] = 0.0;
   out_2075088556811994884[33] = 0.0;
   out_2075088556811994884[34] = 0.0;
   out_2075088556811994884[35] = 0.0;
   out_2075088556811994884[36] = 0.0;
   out_2075088556811994884[37] = 0.0;
   out_2075088556811994884[38] = 0.0;
   out_2075088556811994884[39] = 0.0;
   out_2075088556811994884[40] = 1.0;
   out_2075088556811994884[41] = 0.0;
   out_2075088556811994884[42] = 0.0;
   out_2075088556811994884[43] = 0.0;
   out_2075088556811994884[44] = 0.0;
   out_2075088556811994884[45] = 0.0;
   out_2075088556811994884[46] = 0.0;
   out_2075088556811994884[47] = 0.0;
   out_2075088556811994884[48] = 0.0;
   out_2075088556811994884[49] = 0.0;
   out_2075088556811994884[50] = 1.0;
   out_2075088556811994884[51] = 0.0;
   out_2075088556811994884[52] = 0.0;
   out_2075088556811994884[53] = 0.0;
   out_2075088556811994884[54] = 0.0;
   out_2075088556811994884[55] = 0.0;
   out_2075088556811994884[56] = 0.0;
   out_2075088556811994884[57] = 0.0;
   out_2075088556811994884[58] = 0.0;
   out_2075088556811994884[59] = 0.0;
   out_2075088556811994884[60] = 1.0;
   out_2075088556811994884[61] = 0.0;
   out_2075088556811994884[62] = 0.0;
   out_2075088556811994884[63] = 0.0;
   out_2075088556811994884[64] = 0.0;
   out_2075088556811994884[65] = 0.0;
   out_2075088556811994884[66] = 0.0;
   out_2075088556811994884[67] = 0.0;
   out_2075088556811994884[68] = 0.0;
   out_2075088556811994884[69] = 0.0;
   out_2075088556811994884[70] = 1.0;
   out_2075088556811994884[71] = 0.0;
   out_2075088556811994884[72] = 0.0;
   out_2075088556811994884[73] = 0.0;
   out_2075088556811994884[74] = 0.0;
   out_2075088556811994884[75] = 0.0;
   out_2075088556811994884[76] = 0.0;
   out_2075088556811994884[77] = 0.0;
   out_2075088556811994884[78] = 0.0;
   out_2075088556811994884[79] = 0.0;
   out_2075088556811994884[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_5212472755668323988) {
   out_5212472755668323988[0] = state[0];
   out_5212472755668323988[1] = state[1];
   out_5212472755668323988[2] = state[2];
   out_5212472755668323988[3] = state[3];
   out_5212472755668323988[4] = state[4];
   out_5212472755668323988[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5212472755668323988[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5212472755668323988[7] = state[7];
   out_5212472755668323988[8] = state[8];
}
void F_fun(double *state, double dt, double *out_332930578810202466) {
   out_332930578810202466[0] = 1;
   out_332930578810202466[1] = 0;
   out_332930578810202466[2] = 0;
   out_332930578810202466[3] = 0;
   out_332930578810202466[4] = 0;
   out_332930578810202466[5] = 0;
   out_332930578810202466[6] = 0;
   out_332930578810202466[7] = 0;
   out_332930578810202466[8] = 0;
   out_332930578810202466[9] = 0;
   out_332930578810202466[10] = 1;
   out_332930578810202466[11] = 0;
   out_332930578810202466[12] = 0;
   out_332930578810202466[13] = 0;
   out_332930578810202466[14] = 0;
   out_332930578810202466[15] = 0;
   out_332930578810202466[16] = 0;
   out_332930578810202466[17] = 0;
   out_332930578810202466[18] = 0;
   out_332930578810202466[19] = 0;
   out_332930578810202466[20] = 1;
   out_332930578810202466[21] = 0;
   out_332930578810202466[22] = 0;
   out_332930578810202466[23] = 0;
   out_332930578810202466[24] = 0;
   out_332930578810202466[25] = 0;
   out_332930578810202466[26] = 0;
   out_332930578810202466[27] = 0;
   out_332930578810202466[28] = 0;
   out_332930578810202466[29] = 0;
   out_332930578810202466[30] = 1;
   out_332930578810202466[31] = 0;
   out_332930578810202466[32] = 0;
   out_332930578810202466[33] = 0;
   out_332930578810202466[34] = 0;
   out_332930578810202466[35] = 0;
   out_332930578810202466[36] = 0;
   out_332930578810202466[37] = 0;
   out_332930578810202466[38] = 0;
   out_332930578810202466[39] = 0;
   out_332930578810202466[40] = 1;
   out_332930578810202466[41] = 0;
   out_332930578810202466[42] = 0;
   out_332930578810202466[43] = 0;
   out_332930578810202466[44] = 0;
   out_332930578810202466[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_332930578810202466[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_332930578810202466[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_332930578810202466[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_332930578810202466[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_332930578810202466[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_332930578810202466[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_332930578810202466[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_332930578810202466[53] = -9.8100000000000005*dt;
   out_332930578810202466[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_332930578810202466[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_332930578810202466[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_332930578810202466[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_332930578810202466[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_332930578810202466[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_332930578810202466[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_332930578810202466[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_332930578810202466[62] = 0;
   out_332930578810202466[63] = 0;
   out_332930578810202466[64] = 0;
   out_332930578810202466[65] = 0;
   out_332930578810202466[66] = 0;
   out_332930578810202466[67] = 0;
   out_332930578810202466[68] = 0;
   out_332930578810202466[69] = 0;
   out_332930578810202466[70] = 1;
   out_332930578810202466[71] = 0;
   out_332930578810202466[72] = 0;
   out_332930578810202466[73] = 0;
   out_332930578810202466[74] = 0;
   out_332930578810202466[75] = 0;
   out_332930578810202466[76] = 0;
   out_332930578810202466[77] = 0;
   out_332930578810202466[78] = 0;
   out_332930578810202466[79] = 0;
   out_332930578810202466[80] = 1;
}
void h_25(double *state, double *unused, double *out_8627932228849007647) {
   out_8627932228849007647[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7178467290985319495) {
   out_7178467290985319495[0] = 0;
   out_7178467290985319495[1] = 0;
   out_7178467290985319495[2] = 0;
   out_7178467290985319495[3] = 0;
   out_7178467290985319495[4] = 0;
   out_7178467290985319495[5] = 0;
   out_7178467290985319495[6] = 1;
   out_7178467290985319495[7] = 0;
   out_7178467290985319495[8] = 0;
}
void h_24(double *state, double *unused, double *out_6158507038393289631) {
   out_6158507038393289631[0] = state[4];
   out_6158507038393289631[1] = state[5];
}
void H_24(double *state, double *unused, double *out_9042568998745363559) {
   out_9042568998745363559[0] = 0;
   out_9042568998745363559[1] = 0;
   out_9042568998745363559[2] = 0;
   out_9042568998745363559[3] = 0;
   out_9042568998745363559[4] = 1;
   out_9042568998745363559[5] = 0;
   out_9042568998745363559[6] = 0;
   out_9042568998745363559[7] = 0;
   out_9042568998745363559[8] = 0;
   out_9042568998745363559[9] = 0;
   out_9042568998745363559[10] = 0;
   out_9042568998745363559[11] = 0;
   out_9042568998745363559[12] = 0;
   out_9042568998745363559[13] = 0;
   out_9042568998745363559[14] = 1;
   out_9042568998745363559[15] = 0;
   out_9042568998745363559[16] = 0;
   out_9042568998745363559[17] = 0;
}
void h_30(double *state, double *unused, double *out_8416847386710087001) {
   out_8416847386710087001[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4351586441232615366) {
   out_4351586441232615366[0] = 0;
   out_4351586441232615366[1] = 0;
   out_4351586441232615366[2] = 0;
   out_4351586441232615366[3] = 0;
   out_4351586441232615366[4] = 1;
   out_4351586441232615366[5] = 0;
   out_4351586441232615366[6] = 0;
   out_4351586441232615366[7] = 0;
   out_4351586441232615366[8] = 0;
}
void h_26(double *state, double *unused, double *out_3991486471411855468) {
   out_3991486471411855468[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3436963972111263271) {
   out_3436963972111263271[0] = 0;
   out_3436963972111263271[1] = 0;
   out_3436963972111263271[2] = 0;
   out_3436963972111263271[3] = 0;
   out_3436963972111263271[4] = 0;
   out_3436963972111263271[5] = 0;
   out_3436963972111263271[6] = 0;
   out_3436963972111263271[7] = 1;
   out_3436963972111263271[8] = 0;
}
void h_27(double *state, double *unused, double *out_3626258961188162002) {
   out_3626258961188162002[0] = state[3];
}
void H_27(double *state, double *unused, double *out_6526349753033040277) {
   out_6526349753033040277[0] = 0;
   out_6526349753033040277[1] = 0;
   out_6526349753033040277[2] = 0;
   out_6526349753033040277[3] = 1;
   out_6526349753033040277[4] = 0;
   out_6526349753033040277[5] = 0;
   out_6526349753033040277[6] = 0;
   out_6526349753033040277[7] = 0;
   out_6526349753033040277[8] = 0;
}
void h_29(double *state, double *unused, double *out_11190567065860348) {
   out_11190567065860348[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3841355096918223182) {
   out_3841355096918223182[0] = 0;
   out_3841355096918223182[1] = 1;
   out_3841355096918223182[2] = 0;
   out_3841355096918223182[3] = 0;
   out_3841355096918223182[4] = 0;
   out_3841355096918223182[5] = 0;
   out_3841355096918223182[6] = 0;
   out_3841355096918223182[7] = 0;
   out_3841355096918223182[8] = 0;
}
void h_28(double *state, double *unused, double *out_7385341678092688801) {
   out_7385341678092688801[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5124632576737429732) {
   out_5124632576737429732[0] = 1;
   out_5124632576737429732[1] = 0;
   out_5124632576737429732[2] = 0;
   out_5124632576737429732[3] = 0;
   out_5124632576737429732[4] = 0;
   out_5124632576737429732[5] = 0;
   out_5124632576737429732[6] = 0;
   out_5124632576737429732[7] = 0;
   out_5124632576737429732[8] = 0;
}
void h_31(double *state, double *unused, double *out_1808430068020230512) {
   out_1808430068020230512[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7209113252862279923) {
   out_7209113252862279923[0] = 0;
   out_7209113252862279923[1] = 0;
   out_7209113252862279923[2] = 0;
   out_7209113252862279923[3] = 0;
   out_7209113252862279923[4] = 0;
   out_7209113252862279923[5] = 0;
   out_7209113252862279923[6] = 0;
   out_7209113252862279923[7] = 0;
   out_7209113252862279923[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_3924106122848769848) {
  err_fun(nom_x, delta_x, out_3924106122848769848);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_295877716488110901) {
  inv_err_fun(nom_x, true_x, out_295877716488110901);
}
void car_H_mod_fun(double *state, double *out_2075088556811994884) {
  H_mod_fun(state, out_2075088556811994884);
}
void car_f_fun(double *state, double dt, double *out_5212472755668323988) {
  f_fun(state,  dt, out_5212472755668323988);
}
void car_F_fun(double *state, double dt, double *out_332930578810202466) {
  F_fun(state,  dt, out_332930578810202466);
}
void car_h_25(double *state, double *unused, double *out_8627932228849007647) {
  h_25(state, unused, out_8627932228849007647);
}
void car_H_25(double *state, double *unused, double *out_7178467290985319495) {
  H_25(state, unused, out_7178467290985319495);
}
void car_h_24(double *state, double *unused, double *out_6158507038393289631) {
  h_24(state, unused, out_6158507038393289631);
}
void car_H_24(double *state, double *unused, double *out_9042568998745363559) {
  H_24(state, unused, out_9042568998745363559);
}
void car_h_30(double *state, double *unused, double *out_8416847386710087001) {
  h_30(state, unused, out_8416847386710087001);
}
void car_H_30(double *state, double *unused, double *out_4351586441232615366) {
  H_30(state, unused, out_4351586441232615366);
}
void car_h_26(double *state, double *unused, double *out_3991486471411855468) {
  h_26(state, unused, out_3991486471411855468);
}
void car_H_26(double *state, double *unused, double *out_3436963972111263271) {
  H_26(state, unused, out_3436963972111263271);
}
void car_h_27(double *state, double *unused, double *out_3626258961188162002) {
  h_27(state, unused, out_3626258961188162002);
}
void car_H_27(double *state, double *unused, double *out_6526349753033040277) {
  H_27(state, unused, out_6526349753033040277);
}
void car_h_29(double *state, double *unused, double *out_11190567065860348) {
  h_29(state, unused, out_11190567065860348);
}
void car_H_29(double *state, double *unused, double *out_3841355096918223182) {
  H_29(state, unused, out_3841355096918223182);
}
void car_h_28(double *state, double *unused, double *out_7385341678092688801) {
  h_28(state, unused, out_7385341678092688801);
}
void car_H_28(double *state, double *unused, double *out_5124632576737429732) {
  H_28(state, unused, out_5124632576737429732);
}
void car_h_31(double *state, double *unused, double *out_1808430068020230512) {
  h_31(state, unused, out_1808430068020230512);
}
void car_H_31(double *state, double *unused, double *out_7209113252862279923) {
  H_31(state, unused, out_7209113252862279923);
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
