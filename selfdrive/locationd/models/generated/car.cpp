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
void err_fun(double *nom_x, double *delta_x, double *out_1113594304406611873) {
   out_1113594304406611873[0] = delta_x[0] + nom_x[0];
   out_1113594304406611873[1] = delta_x[1] + nom_x[1];
   out_1113594304406611873[2] = delta_x[2] + nom_x[2];
   out_1113594304406611873[3] = delta_x[3] + nom_x[3];
   out_1113594304406611873[4] = delta_x[4] + nom_x[4];
   out_1113594304406611873[5] = delta_x[5] + nom_x[5];
   out_1113594304406611873[6] = delta_x[6] + nom_x[6];
   out_1113594304406611873[7] = delta_x[7] + nom_x[7];
   out_1113594304406611873[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1615000100367747659) {
   out_1615000100367747659[0] = -nom_x[0] + true_x[0];
   out_1615000100367747659[1] = -nom_x[1] + true_x[1];
   out_1615000100367747659[2] = -nom_x[2] + true_x[2];
   out_1615000100367747659[3] = -nom_x[3] + true_x[3];
   out_1615000100367747659[4] = -nom_x[4] + true_x[4];
   out_1615000100367747659[5] = -nom_x[5] + true_x[5];
   out_1615000100367747659[6] = -nom_x[6] + true_x[6];
   out_1615000100367747659[7] = -nom_x[7] + true_x[7];
   out_1615000100367747659[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_3739319813539620931) {
   out_3739319813539620931[0] = 1.0;
   out_3739319813539620931[1] = 0.0;
   out_3739319813539620931[2] = 0.0;
   out_3739319813539620931[3] = 0.0;
   out_3739319813539620931[4] = 0.0;
   out_3739319813539620931[5] = 0.0;
   out_3739319813539620931[6] = 0.0;
   out_3739319813539620931[7] = 0.0;
   out_3739319813539620931[8] = 0.0;
   out_3739319813539620931[9] = 0.0;
   out_3739319813539620931[10] = 1.0;
   out_3739319813539620931[11] = 0.0;
   out_3739319813539620931[12] = 0.0;
   out_3739319813539620931[13] = 0.0;
   out_3739319813539620931[14] = 0.0;
   out_3739319813539620931[15] = 0.0;
   out_3739319813539620931[16] = 0.0;
   out_3739319813539620931[17] = 0.0;
   out_3739319813539620931[18] = 0.0;
   out_3739319813539620931[19] = 0.0;
   out_3739319813539620931[20] = 1.0;
   out_3739319813539620931[21] = 0.0;
   out_3739319813539620931[22] = 0.0;
   out_3739319813539620931[23] = 0.0;
   out_3739319813539620931[24] = 0.0;
   out_3739319813539620931[25] = 0.0;
   out_3739319813539620931[26] = 0.0;
   out_3739319813539620931[27] = 0.0;
   out_3739319813539620931[28] = 0.0;
   out_3739319813539620931[29] = 0.0;
   out_3739319813539620931[30] = 1.0;
   out_3739319813539620931[31] = 0.0;
   out_3739319813539620931[32] = 0.0;
   out_3739319813539620931[33] = 0.0;
   out_3739319813539620931[34] = 0.0;
   out_3739319813539620931[35] = 0.0;
   out_3739319813539620931[36] = 0.0;
   out_3739319813539620931[37] = 0.0;
   out_3739319813539620931[38] = 0.0;
   out_3739319813539620931[39] = 0.0;
   out_3739319813539620931[40] = 1.0;
   out_3739319813539620931[41] = 0.0;
   out_3739319813539620931[42] = 0.0;
   out_3739319813539620931[43] = 0.0;
   out_3739319813539620931[44] = 0.0;
   out_3739319813539620931[45] = 0.0;
   out_3739319813539620931[46] = 0.0;
   out_3739319813539620931[47] = 0.0;
   out_3739319813539620931[48] = 0.0;
   out_3739319813539620931[49] = 0.0;
   out_3739319813539620931[50] = 1.0;
   out_3739319813539620931[51] = 0.0;
   out_3739319813539620931[52] = 0.0;
   out_3739319813539620931[53] = 0.0;
   out_3739319813539620931[54] = 0.0;
   out_3739319813539620931[55] = 0.0;
   out_3739319813539620931[56] = 0.0;
   out_3739319813539620931[57] = 0.0;
   out_3739319813539620931[58] = 0.0;
   out_3739319813539620931[59] = 0.0;
   out_3739319813539620931[60] = 1.0;
   out_3739319813539620931[61] = 0.0;
   out_3739319813539620931[62] = 0.0;
   out_3739319813539620931[63] = 0.0;
   out_3739319813539620931[64] = 0.0;
   out_3739319813539620931[65] = 0.0;
   out_3739319813539620931[66] = 0.0;
   out_3739319813539620931[67] = 0.0;
   out_3739319813539620931[68] = 0.0;
   out_3739319813539620931[69] = 0.0;
   out_3739319813539620931[70] = 1.0;
   out_3739319813539620931[71] = 0.0;
   out_3739319813539620931[72] = 0.0;
   out_3739319813539620931[73] = 0.0;
   out_3739319813539620931[74] = 0.0;
   out_3739319813539620931[75] = 0.0;
   out_3739319813539620931[76] = 0.0;
   out_3739319813539620931[77] = 0.0;
   out_3739319813539620931[78] = 0.0;
   out_3739319813539620931[79] = 0.0;
   out_3739319813539620931[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1691855582217777164) {
   out_1691855582217777164[0] = state[0];
   out_1691855582217777164[1] = state[1];
   out_1691855582217777164[2] = state[2];
   out_1691855582217777164[3] = state[3];
   out_1691855582217777164[4] = state[4];
   out_1691855582217777164[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1691855582217777164[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1691855582217777164[7] = state[7];
   out_1691855582217777164[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6948739413312517271) {
   out_6948739413312517271[0] = 1;
   out_6948739413312517271[1] = 0;
   out_6948739413312517271[2] = 0;
   out_6948739413312517271[3] = 0;
   out_6948739413312517271[4] = 0;
   out_6948739413312517271[5] = 0;
   out_6948739413312517271[6] = 0;
   out_6948739413312517271[7] = 0;
   out_6948739413312517271[8] = 0;
   out_6948739413312517271[9] = 0;
   out_6948739413312517271[10] = 1;
   out_6948739413312517271[11] = 0;
   out_6948739413312517271[12] = 0;
   out_6948739413312517271[13] = 0;
   out_6948739413312517271[14] = 0;
   out_6948739413312517271[15] = 0;
   out_6948739413312517271[16] = 0;
   out_6948739413312517271[17] = 0;
   out_6948739413312517271[18] = 0;
   out_6948739413312517271[19] = 0;
   out_6948739413312517271[20] = 1;
   out_6948739413312517271[21] = 0;
   out_6948739413312517271[22] = 0;
   out_6948739413312517271[23] = 0;
   out_6948739413312517271[24] = 0;
   out_6948739413312517271[25] = 0;
   out_6948739413312517271[26] = 0;
   out_6948739413312517271[27] = 0;
   out_6948739413312517271[28] = 0;
   out_6948739413312517271[29] = 0;
   out_6948739413312517271[30] = 1;
   out_6948739413312517271[31] = 0;
   out_6948739413312517271[32] = 0;
   out_6948739413312517271[33] = 0;
   out_6948739413312517271[34] = 0;
   out_6948739413312517271[35] = 0;
   out_6948739413312517271[36] = 0;
   out_6948739413312517271[37] = 0;
   out_6948739413312517271[38] = 0;
   out_6948739413312517271[39] = 0;
   out_6948739413312517271[40] = 1;
   out_6948739413312517271[41] = 0;
   out_6948739413312517271[42] = 0;
   out_6948739413312517271[43] = 0;
   out_6948739413312517271[44] = 0;
   out_6948739413312517271[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6948739413312517271[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6948739413312517271[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6948739413312517271[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6948739413312517271[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6948739413312517271[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6948739413312517271[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6948739413312517271[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6948739413312517271[53] = -9.8100000000000005*dt;
   out_6948739413312517271[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6948739413312517271[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6948739413312517271[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6948739413312517271[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6948739413312517271[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6948739413312517271[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6948739413312517271[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6948739413312517271[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6948739413312517271[62] = 0;
   out_6948739413312517271[63] = 0;
   out_6948739413312517271[64] = 0;
   out_6948739413312517271[65] = 0;
   out_6948739413312517271[66] = 0;
   out_6948739413312517271[67] = 0;
   out_6948739413312517271[68] = 0;
   out_6948739413312517271[69] = 0;
   out_6948739413312517271[70] = 1;
   out_6948739413312517271[71] = 0;
   out_6948739413312517271[72] = 0;
   out_6948739413312517271[73] = 0;
   out_6948739413312517271[74] = 0;
   out_6948739413312517271[75] = 0;
   out_6948739413312517271[76] = 0;
   out_6948739413312517271[77] = 0;
   out_6948739413312517271[78] = 0;
   out_6948739413312517271[79] = 0;
   out_6948739413312517271[80] = 1;
}
void h_25(double *state, double *unused, double *out_6659975241378880537) {
   out_6659975241378880537[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7262300463529112608) {
   out_7262300463529112608[0] = 0;
   out_7262300463529112608[1] = 0;
   out_7262300463529112608[2] = 0;
   out_7262300463529112608[3] = 0;
   out_7262300463529112608[4] = 0;
   out_7262300463529112608[5] = 0;
   out_7262300463529112608[6] = 1;
   out_7262300463529112608[7] = 0;
   out_7262300463529112608[8] = 0;
}
void h_24(double *state, double *unused, double *out_6745238207722972952) {
   out_6745238207722972952[0] = state[4];
   out_6745238207722972952[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2515791136562603574) {
   out_2515791136562603574[0] = 0;
   out_2515791136562603574[1] = 0;
   out_2515791136562603574[2] = 0;
   out_2515791136562603574[3] = 0;
   out_2515791136562603574[4] = 1;
   out_2515791136562603574[5] = 0;
   out_2515791136562603574[6] = 0;
   out_2515791136562603574[7] = 0;
   out_2515791136562603574[8] = 0;
   out_2515791136562603574[9] = 0;
   out_2515791136562603574[10] = 0;
   out_2515791136562603574[11] = 0;
   out_2515791136562603574[12] = 0;
   out_2515791136562603574[13] = 0;
   out_2515791136562603574[14] = 1;
   out_2515791136562603574[15] = 0;
   out_2515791136562603574[16] = 0;
   out_2515791136562603574[17] = 0;
}
void h_30(double *state, double *unused, double *out_7688465534051299215) {
   out_7688465534051299215[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4267753268688822253) {
   out_4267753268688822253[0] = 0;
   out_4267753268688822253[1] = 0;
   out_4267753268688822253[2] = 0;
   out_4267753268688822253[3] = 0;
   out_4267753268688822253[4] = 1;
   out_4267753268688822253[5] = 0;
   out_4267753268688822253[6] = 0;
   out_4267753268688822253[7] = 0;
   out_4267753268688822253[8] = 0;
}
void h_26(double *state, double *unused, double *out_8631975386952961650) {
   out_8631975386952961650[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3520797144655056384) {
   out_3520797144655056384[0] = 0;
   out_3520797144655056384[1] = 0;
   out_3520797144655056384[2] = 0;
   out_3520797144655056384[3] = 0;
   out_3520797144655056384[4] = 0;
   out_3520797144655056384[5] = 0;
   out_3520797144655056384[6] = 0;
   out_3520797144655056384[7] = 1;
   out_3520797144655056384[8] = 0;
}
void h_27(double *state, double *unused, double *out_8749982780886338394) {
   out_8749982780886338394[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4958198204585447627) {
   out_4958198204585447627[0] = 0;
   out_4958198204585447627[1] = 0;
   out_4958198204585447627[2] = 0;
   out_4958198204585447627[3] = 1;
   out_4958198204585447627[4] = 0;
   out_4958198204585447627[5] = 0;
   out_4958198204585447627[6] = 0;
   out_4958198204585447627[7] = 0;
   out_4958198204585447627[8] = 0;
}
void h_29(double *state, double *unused, double *out_5756605271997406756) {
   out_5756605271997406756[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3244835477715896594) {
   out_3244835477715896594[0] = 0;
   out_3244835477715896594[1] = 1;
   out_3244835477715896594[2] = 0;
   out_3244835477715896594[3] = 0;
   out_3244835477715896594[4] = 0;
   out_3244835477715896594[5] = 0;
   out_3244835477715896594[6] = 0;
   out_3244835477715896594[7] = 0;
   out_3244835477715896594[8] = 0;
}
void h_28(double *state, double *unused, double *out_900451828557216082) {
   out_900451828557216082[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5208465749281222845) {
   out_5208465749281222845[0] = 1;
   out_5208465749281222845[1] = 0;
   out_5208465749281222845[2] = 0;
   out_5208465749281222845[3] = 0;
   out_5208465749281222845[4] = 0;
   out_5208465749281222845[5] = 0;
   out_5208465749281222845[6] = 0;
   out_5208465749281222845[7] = 0;
   out_5208465749281222845[8] = 0;
}
void h_31(double *state, double *unused, double *out_6935169303663386426) {
   out_6935169303663386426[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7292946425406073036) {
   out_7292946425406073036[0] = 0;
   out_7292946425406073036[1] = 0;
   out_7292946425406073036[2] = 0;
   out_7292946425406073036[3] = 0;
   out_7292946425406073036[4] = 0;
   out_7292946425406073036[5] = 0;
   out_7292946425406073036[6] = 0;
   out_7292946425406073036[7] = 0;
   out_7292946425406073036[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_1113594304406611873) {
  err_fun(nom_x, delta_x, out_1113594304406611873);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1615000100367747659) {
  inv_err_fun(nom_x, true_x, out_1615000100367747659);
}
void car_H_mod_fun(double *state, double *out_3739319813539620931) {
  H_mod_fun(state, out_3739319813539620931);
}
void car_f_fun(double *state, double dt, double *out_1691855582217777164) {
  f_fun(state,  dt, out_1691855582217777164);
}
void car_F_fun(double *state, double dt, double *out_6948739413312517271) {
  F_fun(state,  dt, out_6948739413312517271);
}
void car_h_25(double *state, double *unused, double *out_6659975241378880537) {
  h_25(state, unused, out_6659975241378880537);
}
void car_H_25(double *state, double *unused, double *out_7262300463529112608) {
  H_25(state, unused, out_7262300463529112608);
}
void car_h_24(double *state, double *unused, double *out_6745238207722972952) {
  h_24(state, unused, out_6745238207722972952);
}
void car_H_24(double *state, double *unused, double *out_2515791136562603574) {
  H_24(state, unused, out_2515791136562603574);
}
void car_h_30(double *state, double *unused, double *out_7688465534051299215) {
  h_30(state, unused, out_7688465534051299215);
}
void car_H_30(double *state, double *unused, double *out_4267753268688822253) {
  H_30(state, unused, out_4267753268688822253);
}
void car_h_26(double *state, double *unused, double *out_8631975386952961650) {
  h_26(state, unused, out_8631975386952961650);
}
void car_H_26(double *state, double *unused, double *out_3520797144655056384) {
  H_26(state, unused, out_3520797144655056384);
}
void car_h_27(double *state, double *unused, double *out_8749982780886338394) {
  h_27(state, unused, out_8749982780886338394);
}
void car_H_27(double *state, double *unused, double *out_4958198204585447627) {
  H_27(state, unused, out_4958198204585447627);
}
void car_h_29(double *state, double *unused, double *out_5756605271997406756) {
  h_29(state, unused, out_5756605271997406756);
}
void car_H_29(double *state, double *unused, double *out_3244835477715896594) {
  H_29(state, unused, out_3244835477715896594);
}
void car_h_28(double *state, double *unused, double *out_900451828557216082) {
  h_28(state, unused, out_900451828557216082);
}
void car_H_28(double *state, double *unused, double *out_5208465749281222845) {
  H_28(state, unused, out_5208465749281222845);
}
void car_h_31(double *state, double *unused, double *out_6935169303663386426) {
  h_31(state, unused, out_6935169303663386426);
}
void car_H_31(double *state, double *unused, double *out_7292946425406073036) {
  H_31(state, unused, out_7292946425406073036);
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
