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
void err_fun(double *nom_x, double *delta_x, double *out_353556826058708159) {
   out_353556826058708159[0] = delta_x[0] + nom_x[0];
   out_353556826058708159[1] = delta_x[1] + nom_x[1];
   out_353556826058708159[2] = delta_x[2] + nom_x[2];
   out_353556826058708159[3] = delta_x[3] + nom_x[3];
   out_353556826058708159[4] = delta_x[4] + nom_x[4];
   out_353556826058708159[5] = delta_x[5] + nom_x[5];
   out_353556826058708159[6] = delta_x[6] + nom_x[6];
   out_353556826058708159[7] = delta_x[7] + nom_x[7];
   out_353556826058708159[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8600824748059787407) {
   out_8600824748059787407[0] = -nom_x[0] + true_x[0];
   out_8600824748059787407[1] = -nom_x[1] + true_x[1];
   out_8600824748059787407[2] = -nom_x[2] + true_x[2];
   out_8600824748059787407[3] = -nom_x[3] + true_x[3];
   out_8600824748059787407[4] = -nom_x[4] + true_x[4];
   out_8600824748059787407[5] = -nom_x[5] + true_x[5];
   out_8600824748059787407[6] = -nom_x[6] + true_x[6];
   out_8600824748059787407[7] = -nom_x[7] + true_x[7];
   out_8600824748059787407[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_5193379997354933514) {
   out_5193379997354933514[0] = 1.0;
   out_5193379997354933514[1] = 0.0;
   out_5193379997354933514[2] = 0.0;
   out_5193379997354933514[3] = 0.0;
   out_5193379997354933514[4] = 0.0;
   out_5193379997354933514[5] = 0.0;
   out_5193379997354933514[6] = 0.0;
   out_5193379997354933514[7] = 0.0;
   out_5193379997354933514[8] = 0.0;
   out_5193379997354933514[9] = 0.0;
   out_5193379997354933514[10] = 1.0;
   out_5193379997354933514[11] = 0.0;
   out_5193379997354933514[12] = 0.0;
   out_5193379997354933514[13] = 0.0;
   out_5193379997354933514[14] = 0.0;
   out_5193379997354933514[15] = 0.0;
   out_5193379997354933514[16] = 0.0;
   out_5193379997354933514[17] = 0.0;
   out_5193379997354933514[18] = 0.0;
   out_5193379997354933514[19] = 0.0;
   out_5193379997354933514[20] = 1.0;
   out_5193379997354933514[21] = 0.0;
   out_5193379997354933514[22] = 0.0;
   out_5193379997354933514[23] = 0.0;
   out_5193379997354933514[24] = 0.0;
   out_5193379997354933514[25] = 0.0;
   out_5193379997354933514[26] = 0.0;
   out_5193379997354933514[27] = 0.0;
   out_5193379997354933514[28] = 0.0;
   out_5193379997354933514[29] = 0.0;
   out_5193379997354933514[30] = 1.0;
   out_5193379997354933514[31] = 0.0;
   out_5193379997354933514[32] = 0.0;
   out_5193379997354933514[33] = 0.0;
   out_5193379997354933514[34] = 0.0;
   out_5193379997354933514[35] = 0.0;
   out_5193379997354933514[36] = 0.0;
   out_5193379997354933514[37] = 0.0;
   out_5193379997354933514[38] = 0.0;
   out_5193379997354933514[39] = 0.0;
   out_5193379997354933514[40] = 1.0;
   out_5193379997354933514[41] = 0.0;
   out_5193379997354933514[42] = 0.0;
   out_5193379997354933514[43] = 0.0;
   out_5193379997354933514[44] = 0.0;
   out_5193379997354933514[45] = 0.0;
   out_5193379997354933514[46] = 0.0;
   out_5193379997354933514[47] = 0.0;
   out_5193379997354933514[48] = 0.0;
   out_5193379997354933514[49] = 0.0;
   out_5193379997354933514[50] = 1.0;
   out_5193379997354933514[51] = 0.0;
   out_5193379997354933514[52] = 0.0;
   out_5193379997354933514[53] = 0.0;
   out_5193379997354933514[54] = 0.0;
   out_5193379997354933514[55] = 0.0;
   out_5193379997354933514[56] = 0.0;
   out_5193379997354933514[57] = 0.0;
   out_5193379997354933514[58] = 0.0;
   out_5193379997354933514[59] = 0.0;
   out_5193379997354933514[60] = 1.0;
   out_5193379997354933514[61] = 0.0;
   out_5193379997354933514[62] = 0.0;
   out_5193379997354933514[63] = 0.0;
   out_5193379997354933514[64] = 0.0;
   out_5193379997354933514[65] = 0.0;
   out_5193379997354933514[66] = 0.0;
   out_5193379997354933514[67] = 0.0;
   out_5193379997354933514[68] = 0.0;
   out_5193379997354933514[69] = 0.0;
   out_5193379997354933514[70] = 1.0;
   out_5193379997354933514[71] = 0.0;
   out_5193379997354933514[72] = 0.0;
   out_5193379997354933514[73] = 0.0;
   out_5193379997354933514[74] = 0.0;
   out_5193379997354933514[75] = 0.0;
   out_5193379997354933514[76] = 0.0;
   out_5193379997354933514[77] = 0.0;
   out_5193379997354933514[78] = 0.0;
   out_5193379997354933514[79] = 0.0;
   out_5193379997354933514[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_5326070812951508673) {
   out_5326070812951508673[0] = state[0];
   out_5326070812951508673[1] = state[1];
   out_5326070812951508673[2] = state[2];
   out_5326070812951508673[3] = state[3];
   out_5326070812951508673[4] = state[4];
   out_5326070812951508673[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5326070812951508673[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5326070812951508673[7] = state[7];
   out_5326070812951508673[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6549260589681859698) {
   out_6549260589681859698[0] = 1;
   out_6549260589681859698[1] = 0;
   out_6549260589681859698[2] = 0;
   out_6549260589681859698[3] = 0;
   out_6549260589681859698[4] = 0;
   out_6549260589681859698[5] = 0;
   out_6549260589681859698[6] = 0;
   out_6549260589681859698[7] = 0;
   out_6549260589681859698[8] = 0;
   out_6549260589681859698[9] = 0;
   out_6549260589681859698[10] = 1;
   out_6549260589681859698[11] = 0;
   out_6549260589681859698[12] = 0;
   out_6549260589681859698[13] = 0;
   out_6549260589681859698[14] = 0;
   out_6549260589681859698[15] = 0;
   out_6549260589681859698[16] = 0;
   out_6549260589681859698[17] = 0;
   out_6549260589681859698[18] = 0;
   out_6549260589681859698[19] = 0;
   out_6549260589681859698[20] = 1;
   out_6549260589681859698[21] = 0;
   out_6549260589681859698[22] = 0;
   out_6549260589681859698[23] = 0;
   out_6549260589681859698[24] = 0;
   out_6549260589681859698[25] = 0;
   out_6549260589681859698[26] = 0;
   out_6549260589681859698[27] = 0;
   out_6549260589681859698[28] = 0;
   out_6549260589681859698[29] = 0;
   out_6549260589681859698[30] = 1;
   out_6549260589681859698[31] = 0;
   out_6549260589681859698[32] = 0;
   out_6549260589681859698[33] = 0;
   out_6549260589681859698[34] = 0;
   out_6549260589681859698[35] = 0;
   out_6549260589681859698[36] = 0;
   out_6549260589681859698[37] = 0;
   out_6549260589681859698[38] = 0;
   out_6549260589681859698[39] = 0;
   out_6549260589681859698[40] = 1;
   out_6549260589681859698[41] = 0;
   out_6549260589681859698[42] = 0;
   out_6549260589681859698[43] = 0;
   out_6549260589681859698[44] = 0;
   out_6549260589681859698[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6549260589681859698[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6549260589681859698[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6549260589681859698[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6549260589681859698[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6549260589681859698[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6549260589681859698[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6549260589681859698[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6549260589681859698[53] = -9.8100000000000005*dt;
   out_6549260589681859698[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6549260589681859698[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6549260589681859698[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6549260589681859698[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6549260589681859698[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6549260589681859698[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6549260589681859698[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6549260589681859698[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6549260589681859698[62] = 0;
   out_6549260589681859698[63] = 0;
   out_6549260589681859698[64] = 0;
   out_6549260589681859698[65] = 0;
   out_6549260589681859698[66] = 0;
   out_6549260589681859698[67] = 0;
   out_6549260589681859698[68] = 0;
   out_6549260589681859698[69] = 0;
   out_6549260589681859698[70] = 1;
   out_6549260589681859698[71] = 0;
   out_6549260589681859698[72] = 0;
   out_6549260589681859698[73] = 0;
   out_6549260589681859698[74] = 0;
   out_6549260589681859698[75] = 0;
   out_6549260589681859698[76] = 0;
   out_6549260589681859698[77] = 0;
   out_6549260589681859698[78] = 0;
   out_6549260589681859698[79] = 0;
   out_6549260589681859698[80] = 1;
}
void h_25(double *state, double *unused, double *out_1817901079504340394) {
   out_1817901079504340394[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2727958035618926291) {
   out_2727958035618926291[0] = 0;
   out_2727958035618926291[1] = 0;
   out_2727958035618926291[2] = 0;
   out_2727958035618926291[3] = 0;
   out_2727958035618926291[4] = 0;
   out_2727958035618926291[5] = 0;
   out_2727958035618926291[6] = 1;
   out_2727958035618926291[7] = 0;
   out_2727958035618926291[8] = 0;
}
void h_24(double *state, double *unused, double *out_5841318999192697280) {
   out_5841318999192697280[0] = state[4];
   out_5841318999192697280[1] = state[5];
}
void H_24(double *state, double *unused, double *out_555308436613426725) {
   out_555308436613426725[0] = 0;
   out_555308436613426725[1] = 0;
   out_555308436613426725[2] = 0;
   out_555308436613426725[3] = 0;
   out_555308436613426725[4] = 1;
   out_555308436613426725[5] = 0;
   out_555308436613426725[6] = 0;
   out_555308436613426725[7] = 0;
   out_555308436613426725[8] = 0;
   out_555308436613426725[9] = 0;
   out_555308436613426725[10] = 0;
   out_555308436613426725[11] = 0;
   out_555308436613426725[12] = 0;
   out_555308436613426725[13] = 0;
   out_555308436613426725[14] = 1;
   out_555308436613426725[15] = 0;
   out_555308436613426725[16] = 0;
   out_555308436613426725[17] = 0;
}
void h_30(double *state, double *unused, double *out_9168284229956322897) {
   out_9168284229956322897[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5246290994126174918) {
   out_5246290994126174918[0] = 0;
   out_5246290994126174918[1] = 0;
   out_5246290994126174918[2] = 0;
   out_5246290994126174918[3] = 0;
   out_5246290994126174918[4] = 1;
   out_5246290994126174918[5] = 0;
   out_5246290994126174918[6] = 0;
   out_5246290994126174918[7] = 0;
   out_5246290994126174918[8] = 0;
}
void h_26(double *state, double *unused, double *out_371974765690325757) {
   out_371974765690325757[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1013545283255129933) {
   out_1013545283255129933[0] = 0;
   out_1013545283255129933[1] = 0;
   out_1013545283255129933[2] = 0;
   out_1013545283255129933[3] = 0;
   out_1013545283255129933[4] = 0;
   out_1013545283255129933[5] = 0;
   out_1013545283255129933[6] = 0;
   out_1013545283255129933[7] = 1;
   out_1013545283255129933[8] = 0;
}
void h_27(double *state, double *unused, double *out_8216942596918189540) {
   out_8216942596918189540[0] = state[3];
}
void H_27(double *state, double *unused, double *out_3071527682325750007) {
   out_3071527682325750007[0] = 0;
   out_3071527682325750007[1] = 0;
   out_3071527682325750007[2] = 0;
   out_3071527682325750007[3] = 1;
   out_3071527682325750007[4] = 0;
   out_3071527682325750007[5] = 0;
   out_3071527682325750007[6] = 0;
   out_3071527682325750007[7] = 0;
   out_3071527682325750007[8] = 0;
}
void h_29(double *state, double *unused, double *out_8675143959076751202) {
   out_8675143959076751202[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5756522338440567102) {
   out_5756522338440567102[0] = 0;
   out_5756522338440567102[1] = 1;
   out_5756522338440567102[2] = 0;
   out_5756522338440567102[3] = 0;
   out_5756522338440567102[4] = 0;
   out_5756522338440567102[5] = 0;
   out_5756522338440567102[6] = 0;
   out_5756522338440567102[7] = 0;
   out_5756522338440567102[8] = 0;
}
void h_28(double *state, double *unused, double *out_9160452449819851975) {
   out_9160452449819851975[0] = state[0];
}
void H_28(double *state, double *unused, double *out_674123321371036528) {
   out_674123321371036528[0] = 1;
   out_674123321371036528[1] = 0;
   out_674123321371036528[2] = 0;
   out_674123321371036528[3] = 0;
   out_674123321371036528[4] = 0;
   out_674123321371036528[5] = 0;
   out_674123321371036528[6] = 0;
   out_674123321371036528[7] = 0;
   out_674123321371036528[8] = 0;
}
void h_31(double *state, double *unused, double *out_5455350607758362744) {
   out_5455350607758362744[0] = state[8];
}
void H_31(double *state, double *unused, double *out_1639753385488481409) {
   out_1639753385488481409[0] = 0;
   out_1639753385488481409[1] = 0;
   out_1639753385488481409[2] = 0;
   out_1639753385488481409[3] = 0;
   out_1639753385488481409[4] = 0;
   out_1639753385488481409[5] = 0;
   out_1639753385488481409[6] = 0;
   out_1639753385488481409[7] = 0;
   out_1639753385488481409[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_353556826058708159) {
  err_fun(nom_x, delta_x, out_353556826058708159);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8600824748059787407) {
  inv_err_fun(nom_x, true_x, out_8600824748059787407);
}
void car_H_mod_fun(double *state, double *out_5193379997354933514) {
  H_mod_fun(state, out_5193379997354933514);
}
void car_f_fun(double *state, double dt, double *out_5326070812951508673) {
  f_fun(state,  dt, out_5326070812951508673);
}
void car_F_fun(double *state, double dt, double *out_6549260589681859698) {
  F_fun(state,  dt, out_6549260589681859698);
}
void car_h_25(double *state, double *unused, double *out_1817901079504340394) {
  h_25(state, unused, out_1817901079504340394);
}
void car_H_25(double *state, double *unused, double *out_2727958035618926291) {
  H_25(state, unused, out_2727958035618926291);
}
void car_h_24(double *state, double *unused, double *out_5841318999192697280) {
  h_24(state, unused, out_5841318999192697280);
}
void car_H_24(double *state, double *unused, double *out_555308436613426725) {
  H_24(state, unused, out_555308436613426725);
}
void car_h_30(double *state, double *unused, double *out_9168284229956322897) {
  h_30(state, unused, out_9168284229956322897);
}
void car_H_30(double *state, double *unused, double *out_5246290994126174918) {
  H_30(state, unused, out_5246290994126174918);
}
void car_h_26(double *state, double *unused, double *out_371974765690325757) {
  h_26(state, unused, out_371974765690325757);
}
void car_H_26(double *state, double *unused, double *out_1013545283255129933) {
  H_26(state, unused, out_1013545283255129933);
}
void car_h_27(double *state, double *unused, double *out_8216942596918189540) {
  h_27(state, unused, out_8216942596918189540);
}
void car_H_27(double *state, double *unused, double *out_3071527682325750007) {
  H_27(state, unused, out_3071527682325750007);
}
void car_h_29(double *state, double *unused, double *out_8675143959076751202) {
  h_29(state, unused, out_8675143959076751202);
}
void car_H_29(double *state, double *unused, double *out_5756522338440567102) {
  H_29(state, unused, out_5756522338440567102);
}
void car_h_28(double *state, double *unused, double *out_9160452449819851975) {
  h_28(state, unused, out_9160452449819851975);
}
void car_H_28(double *state, double *unused, double *out_674123321371036528) {
  H_28(state, unused, out_674123321371036528);
}
void car_h_31(double *state, double *unused, double *out_5455350607758362744) {
  h_31(state, unused, out_5455350607758362744);
}
void car_H_31(double *state, double *unused, double *out_1639753385488481409) {
  H_31(state, unused, out_1639753385488481409);
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
