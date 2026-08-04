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
void err_fun(double *nom_x, double *delta_x, double *out_9000377994212201456) {
   out_9000377994212201456[0] = delta_x[0] + nom_x[0];
   out_9000377994212201456[1] = delta_x[1] + nom_x[1];
   out_9000377994212201456[2] = delta_x[2] + nom_x[2];
   out_9000377994212201456[3] = delta_x[3] + nom_x[3];
   out_9000377994212201456[4] = delta_x[4] + nom_x[4];
   out_9000377994212201456[5] = delta_x[5] + nom_x[5];
   out_9000377994212201456[6] = delta_x[6] + nom_x[6];
   out_9000377994212201456[7] = delta_x[7] + nom_x[7];
   out_9000377994212201456[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8968830433719023777) {
   out_8968830433719023777[0] = -nom_x[0] + true_x[0];
   out_8968830433719023777[1] = -nom_x[1] + true_x[1];
   out_8968830433719023777[2] = -nom_x[2] + true_x[2];
   out_8968830433719023777[3] = -nom_x[3] + true_x[3];
   out_8968830433719023777[4] = -nom_x[4] + true_x[4];
   out_8968830433719023777[5] = -nom_x[5] + true_x[5];
   out_8968830433719023777[6] = -nom_x[6] + true_x[6];
   out_8968830433719023777[7] = -nom_x[7] + true_x[7];
   out_8968830433719023777[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_9201143408597581971) {
   out_9201143408597581971[0] = 1.0;
   out_9201143408597581971[1] = 0.0;
   out_9201143408597581971[2] = 0.0;
   out_9201143408597581971[3] = 0.0;
   out_9201143408597581971[4] = 0.0;
   out_9201143408597581971[5] = 0.0;
   out_9201143408597581971[6] = 0.0;
   out_9201143408597581971[7] = 0.0;
   out_9201143408597581971[8] = 0.0;
   out_9201143408597581971[9] = 0.0;
   out_9201143408597581971[10] = 1.0;
   out_9201143408597581971[11] = 0.0;
   out_9201143408597581971[12] = 0.0;
   out_9201143408597581971[13] = 0.0;
   out_9201143408597581971[14] = 0.0;
   out_9201143408597581971[15] = 0.0;
   out_9201143408597581971[16] = 0.0;
   out_9201143408597581971[17] = 0.0;
   out_9201143408597581971[18] = 0.0;
   out_9201143408597581971[19] = 0.0;
   out_9201143408597581971[20] = 1.0;
   out_9201143408597581971[21] = 0.0;
   out_9201143408597581971[22] = 0.0;
   out_9201143408597581971[23] = 0.0;
   out_9201143408597581971[24] = 0.0;
   out_9201143408597581971[25] = 0.0;
   out_9201143408597581971[26] = 0.0;
   out_9201143408597581971[27] = 0.0;
   out_9201143408597581971[28] = 0.0;
   out_9201143408597581971[29] = 0.0;
   out_9201143408597581971[30] = 1.0;
   out_9201143408597581971[31] = 0.0;
   out_9201143408597581971[32] = 0.0;
   out_9201143408597581971[33] = 0.0;
   out_9201143408597581971[34] = 0.0;
   out_9201143408597581971[35] = 0.0;
   out_9201143408597581971[36] = 0.0;
   out_9201143408597581971[37] = 0.0;
   out_9201143408597581971[38] = 0.0;
   out_9201143408597581971[39] = 0.0;
   out_9201143408597581971[40] = 1.0;
   out_9201143408597581971[41] = 0.0;
   out_9201143408597581971[42] = 0.0;
   out_9201143408597581971[43] = 0.0;
   out_9201143408597581971[44] = 0.0;
   out_9201143408597581971[45] = 0.0;
   out_9201143408597581971[46] = 0.0;
   out_9201143408597581971[47] = 0.0;
   out_9201143408597581971[48] = 0.0;
   out_9201143408597581971[49] = 0.0;
   out_9201143408597581971[50] = 1.0;
   out_9201143408597581971[51] = 0.0;
   out_9201143408597581971[52] = 0.0;
   out_9201143408597581971[53] = 0.0;
   out_9201143408597581971[54] = 0.0;
   out_9201143408597581971[55] = 0.0;
   out_9201143408597581971[56] = 0.0;
   out_9201143408597581971[57] = 0.0;
   out_9201143408597581971[58] = 0.0;
   out_9201143408597581971[59] = 0.0;
   out_9201143408597581971[60] = 1.0;
   out_9201143408597581971[61] = 0.0;
   out_9201143408597581971[62] = 0.0;
   out_9201143408597581971[63] = 0.0;
   out_9201143408597581971[64] = 0.0;
   out_9201143408597581971[65] = 0.0;
   out_9201143408597581971[66] = 0.0;
   out_9201143408597581971[67] = 0.0;
   out_9201143408597581971[68] = 0.0;
   out_9201143408597581971[69] = 0.0;
   out_9201143408597581971[70] = 1.0;
   out_9201143408597581971[71] = 0.0;
   out_9201143408597581971[72] = 0.0;
   out_9201143408597581971[73] = 0.0;
   out_9201143408597581971[74] = 0.0;
   out_9201143408597581971[75] = 0.0;
   out_9201143408597581971[76] = 0.0;
   out_9201143408597581971[77] = 0.0;
   out_9201143408597581971[78] = 0.0;
   out_9201143408597581971[79] = 0.0;
   out_9201143408597581971[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_4422444681951896033) {
   out_4422444681951896033[0] = state[0];
   out_4422444681951896033[1] = state[1];
   out_4422444681951896033[2] = state[2];
   out_4422444681951896033[3] = state[3];
   out_4422444681951896033[4] = state[4];
   out_4422444681951896033[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_4422444681951896033[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_4422444681951896033[7] = state[7];
   out_4422444681951896033[8] = state[8];
}
void F_fun(double *state, double dt, double *out_3632050189681908649) {
   out_3632050189681908649[0] = 1;
   out_3632050189681908649[1] = 0;
   out_3632050189681908649[2] = 0;
   out_3632050189681908649[3] = 0;
   out_3632050189681908649[4] = 0;
   out_3632050189681908649[5] = 0;
   out_3632050189681908649[6] = 0;
   out_3632050189681908649[7] = 0;
   out_3632050189681908649[8] = 0;
   out_3632050189681908649[9] = 0;
   out_3632050189681908649[10] = 1;
   out_3632050189681908649[11] = 0;
   out_3632050189681908649[12] = 0;
   out_3632050189681908649[13] = 0;
   out_3632050189681908649[14] = 0;
   out_3632050189681908649[15] = 0;
   out_3632050189681908649[16] = 0;
   out_3632050189681908649[17] = 0;
   out_3632050189681908649[18] = 0;
   out_3632050189681908649[19] = 0;
   out_3632050189681908649[20] = 1;
   out_3632050189681908649[21] = 0;
   out_3632050189681908649[22] = 0;
   out_3632050189681908649[23] = 0;
   out_3632050189681908649[24] = 0;
   out_3632050189681908649[25] = 0;
   out_3632050189681908649[26] = 0;
   out_3632050189681908649[27] = 0;
   out_3632050189681908649[28] = 0;
   out_3632050189681908649[29] = 0;
   out_3632050189681908649[30] = 1;
   out_3632050189681908649[31] = 0;
   out_3632050189681908649[32] = 0;
   out_3632050189681908649[33] = 0;
   out_3632050189681908649[34] = 0;
   out_3632050189681908649[35] = 0;
   out_3632050189681908649[36] = 0;
   out_3632050189681908649[37] = 0;
   out_3632050189681908649[38] = 0;
   out_3632050189681908649[39] = 0;
   out_3632050189681908649[40] = 1;
   out_3632050189681908649[41] = 0;
   out_3632050189681908649[42] = 0;
   out_3632050189681908649[43] = 0;
   out_3632050189681908649[44] = 0;
   out_3632050189681908649[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_3632050189681908649[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_3632050189681908649[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3632050189681908649[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3632050189681908649[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_3632050189681908649[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_3632050189681908649[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_3632050189681908649[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_3632050189681908649[53] = -9.8100000000000005*dt;
   out_3632050189681908649[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_3632050189681908649[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_3632050189681908649[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3632050189681908649[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3632050189681908649[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_3632050189681908649[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_3632050189681908649[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_3632050189681908649[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3632050189681908649[62] = 0;
   out_3632050189681908649[63] = 0;
   out_3632050189681908649[64] = 0;
   out_3632050189681908649[65] = 0;
   out_3632050189681908649[66] = 0;
   out_3632050189681908649[67] = 0;
   out_3632050189681908649[68] = 0;
   out_3632050189681908649[69] = 0;
   out_3632050189681908649[70] = 1;
   out_3632050189681908649[71] = 0;
   out_3632050189681908649[72] = 0;
   out_3632050189681908649[73] = 0;
   out_3632050189681908649[74] = 0;
   out_3632050189681908649[75] = 0;
   out_3632050189681908649[76] = 0;
   out_3632050189681908649[77] = 0;
   out_3632050189681908649[78] = 0;
   out_3632050189681908649[79] = 0;
   out_3632050189681908649[80] = 1;
}
void h_25(double *state, double *unused, double *out_8556756409565152671) {
   out_8556756409565152671[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1324262632138109840) {
   out_1324262632138109840[0] = 0;
   out_1324262632138109840[1] = 0;
   out_1324262632138109840[2] = 0;
   out_1324262632138109840[3] = 0;
   out_1324262632138109840[4] = 0;
   out_1324262632138109840[5] = 0;
   out_1324262632138109840[6] = 1;
   out_1324262632138109840[7] = 0;
   out_1324262632138109840[8] = 0;
}
void h_24(double *state, double *unused, double *out_2600025209261031362) {
   out_2600025209261031362[0] = state[4];
   out_2600025209261031362[1] = state[5];
}
void H_24(double *state, double *unused, double *out_8766937225126826160) {
   out_8766937225126826160[0] = 0;
   out_8766937225126826160[1] = 0;
   out_8766937225126826160[2] = 0;
   out_8766937225126826160[3] = 0;
   out_8766937225126826160[4] = 1;
   out_8766937225126826160[5] = 0;
   out_8766937225126826160[6] = 0;
   out_8766937225126826160[7] = 0;
   out_8766937225126826160[8] = 0;
   out_8766937225126826160[9] = 0;
   out_8766937225126826160[10] = 0;
   out_8766937225126826160[11] = 0;
   out_8766937225126826160[12] = 0;
   out_8766937225126826160[13] = 0;
   out_8766937225126826160[14] = 1;
   out_8766937225126826160[15] = 0;
   out_8766937225126826160[16] = 0;
   out_8766937225126826160[17] = 0;
}
void h_30(double *state, double *unused, double *out_1968928483088857508) {
   out_1968928483088857508[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5851958962265718038) {
   out_5851958962265718038[0] = 0;
   out_5851958962265718038[1] = 0;
   out_5851958962265718038[2] = 0;
   out_5851958962265718038[3] = 0;
   out_5851958962265718038[4] = 1;
   out_5851958962265718038[5] = 0;
   out_5851958962265718038[6] = 0;
   out_5851958962265718038[7] = 0;
   out_5851958962265718038[8] = 0;
}
void h_26(double *state, double *unused, double *out_7835892302344293645) {
   out_7835892302344293645[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5065765951012166064) {
   out_5065765951012166064[0] = 0;
   out_5065765951012166064[1] = 0;
   out_5065765951012166064[2] = 0;
   out_5065765951012166064[3] = 0;
   out_5065765951012166064[4] = 0;
   out_5065765951012166064[5] = 0;
   out_5065765951012166064[6] = 0;
   out_5065765951012166064[7] = 1;
   out_5065765951012166064[8] = 0;
}
void h_27(double *state, double *unused, double *out_3555083141904307026) {
   out_3555083141904307026[0] = state[3];
}
void H_27(double *state, double *unused, double *out_8026722274066142949) {
   out_8026722274066142949[0] = 0;
   out_8026722274066142949[1] = 0;
   out_8026722274066142949[2] = 0;
   out_8026722274066142949[3] = 1;
   out_8026722274066142949[4] = 0;
   out_8026722274066142949[5] = 0;
   out_8026722274066142949[6] = 0;
   out_8026722274066142949[7] = 0;
   out_8026722274066142949[8] = 0;
}
void h_29(double *state, double *unused, double *out_3032744784571988137) {
   out_3032744784571988137[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5341727617951325854) {
   out_5341727617951325854[0] = 0;
   out_5341727617951325854[1] = 1;
   out_5341727617951325854[2] = 0;
   out_5341727617951325854[3] = 0;
   out_5341727617951325854[4] = 0;
   out_5341727617951325854[5] = 0;
   out_5341727617951325854[6] = 0;
   out_5341727617951325854[7] = 0;
   out_5341727617951325854[8] = 0;
}
void h_28(double *state, double *unused, double *out_8635836209846197336) {
   out_8635836209846197336[0] = state[0];
}
void H_28(double *state, double *unused, double *out_8022617438688695188) {
   out_8022617438688695188[0] = 1;
   out_8022617438688695188[1] = 0;
   out_8022617438688695188[2] = 0;
   out_8022617438688695188[3] = 0;
   out_8022617438688695188[4] = 0;
   out_8022617438688695188[5] = 0;
   out_8022617438688695188[6] = 0;
   out_8022617438688695188[7] = 0;
   out_8022617438688695188[8] = 0;
}
void h_31(double *state, double *unused, double *out_6252538135890376595) {
   out_6252538135890376595[0] = state[8];
}
void H_31(double *state, double *unused, double *out_5691974053245517540) {
   out_5691974053245517540[0] = 0;
   out_5691974053245517540[1] = 0;
   out_5691974053245517540[2] = 0;
   out_5691974053245517540[3] = 0;
   out_5691974053245517540[4] = 0;
   out_5691974053245517540[5] = 0;
   out_5691974053245517540[6] = 0;
   out_5691974053245517540[7] = 0;
   out_5691974053245517540[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_9000377994212201456) {
  err_fun(nom_x, delta_x, out_9000377994212201456);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8968830433719023777) {
  inv_err_fun(nom_x, true_x, out_8968830433719023777);
}
void car_H_mod_fun(double *state, double *out_9201143408597581971) {
  H_mod_fun(state, out_9201143408597581971);
}
void car_f_fun(double *state, double dt, double *out_4422444681951896033) {
  f_fun(state,  dt, out_4422444681951896033);
}
void car_F_fun(double *state, double dt, double *out_3632050189681908649) {
  F_fun(state,  dt, out_3632050189681908649);
}
void car_h_25(double *state, double *unused, double *out_8556756409565152671) {
  h_25(state, unused, out_8556756409565152671);
}
void car_H_25(double *state, double *unused, double *out_1324262632138109840) {
  H_25(state, unused, out_1324262632138109840);
}
void car_h_24(double *state, double *unused, double *out_2600025209261031362) {
  h_24(state, unused, out_2600025209261031362);
}
void car_H_24(double *state, double *unused, double *out_8766937225126826160) {
  H_24(state, unused, out_8766937225126826160);
}
void car_h_30(double *state, double *unused, double *out_1968928483088857508) {
  h_30(state, unused, out_1968928483088857508);
}
void car_H_30(double *state, double *unused, double *out_5851958962265718038) {
  H_30(state, unused, out_5851958962265718038);
}
void car_h_26(double *state, double *unused, double *out_7835892302344293645) {
  h_26(state, unused, out_7835892302344293645);
}
void car_H_26(double *state, double *unused, double *out_5065765951012166064) {
  H_26(state, unused, out_5065765951012166064);
}
void car_h_27(double *state, double *unused, double *out_3555083141904307026) {
  h_27(state, unused, out_3555083141904307026);
}
void car_H_27(double *state, double *unused, double *out_8026722274066142949) {
  H_27(state, unused, out_8026722274066142949);
}
void car_h_29(double *state, double *unused, double *out_3032744784571988137) {
  h_29(state, unused, out_3032744784571988137);
}
void car_H_29(double *state, double *unused, double *out_5341727617951325854) {
  H_29(state, unused, out_5341727617951325854);
}
void car_h_28(double *state, double *unused, double *out_8635836209846197336) {
  h_28(state, unused, out_8635836209846197336);
}
void car_H_28(double *state, double *unused, double *out_8022617438688695188) {
  H_28(state, unused, out_8022617438688695188);
}
void car_h_31(double *state, double *unused, double *out_6252538135890376595) {
  h_31(state, unused, out_6252538135890376595);
}
void car_H_31(double *state, double *unused, double *out_5691974053245517540) {
  H_31(state, unused, out_5691974053245517540);
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
