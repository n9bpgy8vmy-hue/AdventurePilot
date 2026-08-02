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
void err_fun(double *nom_x, double *delta_x, double *out_224298435625695922) {
   out_224298435625695922[0] = delta_x[0] + nom_x[0];
   out_224298435625695922[1] = delta_x[1] + nom_x[1];
   out_224298435625695922[2] = delta_x[2] + nom_x[2];
   out_224298435625695922[3] = delta_x[3] + nom_x[3];
   out_224298435625695922[4] = delta_x[4] + nom_x[4];
   out_224298435625695922[5] = delta_x[5] + nom_x[5];
   out_224298435625695922[6] = delta_x[6] + nom_x[6];
   out_224298435625695922[7] = delta_x[7] + nom_x[7];
   out_224298435625695922[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_246092340363984604) {
   out_246092340363984604[0] = -nom_x[0] + true_x[0];
   out_246092340363984604[1] = -nom_x[1] + true_x[1];
   out_246092340363984604[2] = -nom_x[2] + true_x[2];
   out_246092340363984604[3] = -nom_x[3] + true_x[3];
   out_246092340363984604[4] = -nom_x[4] + true_x[4];
   out_246092340363984604[5] = -nom_x[5] + true_x[5];
   out_246092340363984604[6] = -nom_x[6] + true_x[6];
   out_246092340363984604[7] = -nom_x[7] + true_x[7];
   out_246092340363984604[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_4279458356555016822) {
   out_4279458356555016822[0] = 1.0;
   out_4279458356555016822[1] = 0.0;
   out_4279458356555016822[2] = 0.0;
   out_4279458356555016822[3] = 0.0;
   out_4279458356555016822[4] = 0.0;
   out_4279458356555016822[5] = 0.0;
   out_4279458356555016822[6] = 0.0;
   out_4279458356555016822[7] = 0.0;
   out_4279458356555016822[8] = 0.0;
   out_4279458356555016822[9] = 0.0;
   out_4279458356555016822[10] = 1.0;
   out_4279458356555016822[11] = 0.0;
   out_4279458356555016822[12] = 0.0;
   out_4279458356555016822[13] = 0.0;
   out_4279458356555016822[14] = 0.0;
   out_4279458356555016822[15] = 0.0;
   out_4279458356555016822[16] = 0.0;
   out_4279458356555016822[17] = 0.0;
   out_4279458356555016822[18] = 0.0;
   out_4279458356555016822[19] = 0.0;
   out_4279458356555016822[20] = 1.0;
   out_4279458356555016822[21] = 0.0;
   out_4279458356555016822[22] = 0.0;
   out_4279458356555016822[23] = 0.0;
   out_4279458356555016822[24] = 0.0;
   out_4279458356555016822[25] = 0.0;
   out_4279458356555016822[26] = 0.0;
   out_4279458356555016822[27] = 0.0;
   out_4279458356555016822[28] = 0.0;
   out_4279458356555016822[29] = 0.0;
   out_4279458356555016822[30] = 1.0;
   out_4279458356555016822[31] = 0.0;
   out_4279458356555016822[32] = 0.0;
   out_4279458356555016822[33] = 0.0;
   out_4279458356555016822[34] = 0.0;
   out_4279458356555016822[35] = 0.0;
   out_4279458356555016822[36] = 0.0;
   out_4279458356555016822[37] = 0.0;
   out_4279458356555016822[38] = 0.0;
   out_4279458356555016822[39] = 0.0;
   out_4279458356555016822[40] = 1.0;
   out_4279458356555016822[41] = 0.0;
   out_4279458356555016822[42] = 0.0;
   out_4279458356555016822[43] = 0.0;
   out_4279458356555016822[44] = 0.0;
   out_4279458356555016822[45] = 0.0;
   out_4279458356555016822[46] = 0.0;
   out_4279458356555016822[47] = 0.0;
   out_4279458356555016822[48] = 0.0;
   out_4279458356555016822[49] = 0.0;
   out_4279458356555016822[50] = 1.0;
   out_4279458356555016822[51] = 0.0;
   out_4279458356555016822[52] = 0.0;
   out_4279458356555016822[53] = 0.0;
   out_4279458356555016822[54] = 0.0;
   out_4279458356555016822[55] = 0.0;
   out_4279458356555016822[56] = 0.0;
   out_4279458356555016822[57] = 0.0;
   out_4279458356555016822[58] = 0.0;
   out_4279458356555016822[59] = 0.0;
   out_4279458356555016822[60] = 1.0;
   out_4279458356555016822[61] = 0.0;
   out_4279458356555016822[62] = 0.0;
   out_4279458356555016822[63] = 0.0;
   out_4279458356555016822[64] = 0.0;
   out_4279458356555016822[65] = 0.0;
   out_4279458356555016822[66] = 0.0;
   out_4279458356555016822[67] = 0.0;
   out_4279458356555016822[68] = 0.0;
   out_4279458356555016822[69] = 0.0;
   out_4279458356555016822[70] = 1.0;
   out_4279458356555016822[71] = 0.0;
   out_4279458356555016822[72] = 0.0;
   out_4279458356555016822[73] = 0.0;
   out_4279458356555016822[74] = 0.0;
   out_4279458356555016822[75] = 0.0;
   out_4279458356555016822[76] = 0.0;
   out_4279458356555016822[77] = 0.0;
   out_4279458356555016822[78] = 0.0;
   out_4279458356555016822[79] = 0.0;
   out_4279458356555016822[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1545223943974543840) {
   out_1545223943974543840[0] = state[0];
   out_1545223943974543840[1] = state[1];
   out_1545223943974543840[2] = state[2];
   out_1545223943974543840[3] = state[3];
   out_1545223943974543840[4] = state[4];
   out_1545223943974543840[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1545223943974543840[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1545223943974543840[7] = state[7];
   out_1545223943974543840[8] = state[8];
}
void F_fun(double *state, double dt, double *out_1739281589418524436) {
   out_1739281589418524436[0] = 1;
   out_1739281589418524436[1] = 0;
   out_1739281589418524436[2] = 0;
   out_1739281589418524436[3] = 0;
   out_1739281589418524436[4] = 0;
   out_1739281589418524436[5] = 0;
   out_1739281589418524436[6] = 0;
   out_1739281589418524436[7] = 0;
   out_1739281589418524436[8] = 0;
   out_1739281589418524436[9] = 0;
   out_1739281589418524436[10] = 1;
   out_1739281589418524436[11] = 0;
   out_1739281589418524436[12] = 0;
   out_1739281589418524436[13] = 0;
   out_1739281589418524436[14] = 0;
   out_1739281589418524436[15] = 0;
   out_1739281589418524436[16] = 0;
   out_1739281589418524436[17] = 0;
   out_1739281589418524436[18] = 0;
   out_1739281589418524436[19] = 0;
   out_1739281589418524436[20] = 1;
   out_1739281589418524436[21] = 0;
   out_1739281589418524436[22] = 0;
   out_1739281589418524436[23] = 0;
   out_1739281589418524436[24] = 0;
   out_1739281589418524436[25] = 0;
   out_1739281589418524436[26] = 0;
   out_1739281589418524436[27] = 0;
   out_1739281589418524436[28] = 0;
   out_1739281589418524436[29] = 0;
   out_1739281589418524436[30] = 1;
   out_1739281589418524436[31] = 0;
   out_1739281589418524436[32] = 0;
   out_1739281589418524436[33] = 0;
   out_1739281589418524436[34] = 0;
   out_1739281589418524436[35] = 0;
   out_1739281589418524436[36] = 0;
   out_1739281589418524436[37] = 0;
   out_1739281589418524436[38] = 0;
   out_1739281589418524436[39] = 0;
   out_1739281589418524436[40] = 1;
   out_1739281589418524436[41] = 0;
   out_1739281589418524436[42] = 0;
   out_1739281589418524436[43] = 0;
   out_1739281589418524436[44] = 0;
   out_1739281589418524436[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1739281589418524436[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1739281589418524436[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1739281589418524436[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1739281589418524436[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1739281589418524436[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1739281589418524436[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1739281589418524436[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1739281589418524436[53] = -9.8100000000000005*dt;
   out_1739281589418524436[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1739281589418524436[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1739281589418524436[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1739281589418524436[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1739281589418524436[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1739281589418524436[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1739281589418524436[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1739281589418524436[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1739281589418524436[62] = 0;
   out_1739281589418524436[63] = 0;
   out_1739281589418524436[64] = 0;
   out_1739281589418524436[65] = 0;
   out_1739281589418524436[66] = 0;
   out_1739281589418524436[67] = 0;
   out_1739281589418524436[68] = 0;
   out_1739281589418524436[69] = 0;
   out_1739281589418524436[70] = 1;
   out_1739281589418524436[71] = 0;
   out_1739281589418524436[72] = 0;
   out_1739281589418524436[73] = 0;
   out_1739281589418524436[74] = 0;
   out_1739281589418524436[75] = 0;
   out_1739281589418524436[76] = 0;
   out_1739281589418524436[77] = 0;
   out_1739281589418524436[78] = 0;
   out_1739281589418524436[79] = 0;
   out_1739281589418524436[80] = 1;
}
void h_25(double *state, double *unused, double *out_6496775841314482133) {
   out_6496775841314482133[0] = state[6];
}
void H_25(double *state, double *unused, double *out_624233468629648018) {
   out_624233468629648018[0] = 0;
   out_624233468629648018[1] = 0;
   out_624233468629648018[2] = 0;
   out_624233468629648018[3] = 0;
   out_624233468629648018[4] = 0;
   out_624233468629648018[5] = 0;
   out_624233468629648018[6] = 1;
   out_624233468629648018[7] = 0;
   out_624233468629648018[8] = 0;
}
void h_24(double *state, double *unused, double *out_8130981640671652032) {
   out_8130981640671652032[0] = state[4];
   out_8130981640671652032[1] = state[5];
}
void H_24(double *state, double *unused, double *out_8042625102306574911) {
   out_8042625102306574911[0] = 0;
   out_8042625102306574911[1] = 0;
   out_8042625102306574911[2] = 0;
   out_8042625102306574911[3] = 0;
   out_8042625102306574911[4] = 1;
   out_8042625102306574911[5] = 0;
   out_8042625102306574911[6] = 0;
   out_8042625102306574911[7] = 0;
   out_8042625102306574911[8] = 0;
   out_8042625102306574911[9] = 0;
   out_8042625102306574911[10] = 0;
   out_8042625102306574911[11] = 0;
   out_8042625102306574911[12] = 0;
   out_8042625102306574911[13] = 0;
   out_8042625102306574911[14] = 1;
   out_8042625102306574911[15] = 0;
   out_8042625102306574911[16] = 0;
   out_8042625102306574911[17] = 0;
}
void h_30(double *state, double *unused, double *out_5895357683701118579) {
   out_5895357683701118579[0] = state[4];
}
void H_30(double *state, double *unused, double *out_3903462861497960180) {
   out_3903462861497960180[0] = 0;
   out_3903462861497960180[1] = 0;
   out_3903462861497960180[2] = 0;
   out_3903462861497960180[3] = 0;
   out_3903462861497960180[4] = 1;
   out_3903462861497960180[5] = 0;
   out_3903462861497960180[6] = 0;
   out_3903462861497960180[7] = 0;
   out_3903462861497960180[8] = 0;
}
void h_26(double *state, double *unused, double *out_893684416040272934) {
   out_893684416040272934[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3117269850244408206) {
   out_3117269850244408206[0] = 0;
   out_3117269850244408206[1] = 0;
   out_3117269850244408206[2] = 0;
   out_3117269850244408206[3] = 0;
   out_3117269850244408206[4] = 0;
   out_3117269850244408206[5] = 0;
   out_3117269850244408206[6] = 0;
   out_3117269850244408206[7] = 1;
   out_3117269850244408206[8] = 0;
}
void h_27(double *state, double *unused, double *out_7327782202938961169) {
   out_7327782202938961169[0] = state[3];
}
void H_27(double *state, double *unused, double *out_6078226173298385091) {
   out_6078226173298385091[0] = 0;
   out_6078226173298385091[1] = 0;
   out_6078226173298385091[2] = 0;
   out_6078226173298385091[3] = 1;
   out_6078226173298385091[4] = 0;
   out_6078226173298385091[5] = 0;
   out_6078226173298385091[6] = 0;
   out_6078226173298385091[7] = 0;
   out_6078226173298385091[8] = 0;
}
void h_29(double *state, double *unused, double *out_553490447815088016) {
   out_553490447815088016[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3393231517183567996) {
   out_3393231517183567996[0] = 0;
   out_3393231517183567996[1] = 1;
   out_3393231517183567996[2] = 0;
   out_3393231517183567996[3] = 0;
   out_3393231517183567996[4] = 0;
   out_3393231517183567996[5] = 0;
   out_3393231517183567996[6] = 0;
   out_3393231517183567996[7] = 0;
   out_3393231517183567996[8] = 0;
}
void h_28(double *state, double *unused, double *out_2326108935278115524) {
   out_2326108935278115524[0] = state[0];
}
void H_28(double *state, double *unused, double *out_1429601245618241745) {
   out_1429601245618241745[0] = 1;
   out_1429601245618241745[1] = 0;
   out_1429601245618241745[2] = 0;
   out_1429601245618241745[3] = 0;
   out_1429601245618241745[4] = 0;
   out_1429601245618241745[5] = 0;
   out_1429601245618241745[6] = 0;
   out_1429601245618241745[7] = 0;
   out_1429601245618241745[8] = 0;
}
void h_31(double *state, double *unused, double *out_4186702975574397042) {
   out_4186702975574397042[0] = state[8];
}
void H_31(double *state, double *unused, double *out_3743477952477759682) {
   out_3743477952477759682[0] = 0;
   out_3743477952477759682[1] = 0;
   out_3743477952477759682[2] = 0;
   out_3743477952477759682[3] = 0;
   out_3743477952477759682[4] = 0;
   out_3743477952477759682[5] = 0;
   out_3743477952477759682[6] = 0;
   out_3743477952477759682[7] = 0;
   out_3743477952477759682[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_224298435625695922) {
  err_fun(nom_x, delta_x, out_224298435625695922);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_246092340363984604) {
  inv_err_fun(nom_x, true_x, out_246092340363984604);
}
void car_H_mod_fun(double *state, double *out_4279458356555016822) {
  H_mod_fun(state, out_4279458356555016822);
}
void car_f_fun(double *state, double dt, double *out_1545223943974543840) {
  f_fun(state,  dt, out_1545223943974543840);
}
void car_F_fun(double *state, double dt, double *out_1739281589418524436) {
  F_fun(state,  dt, out_1739281589418524436);
}
void car_h_25(double *state, double *unused, double *out_6496775841314482133) {
  h_25(state, unused, out_6496775841314482133);
}
void car_H_25(double *state, double *unused, double *out_624233468629648018) {
  H_25(state, unused, out_624233468629648018);
}
void car_h_24(double *state, double *unused, double *out_8130981640671652032) {
  h_24(state, unused, out_8130981640671652032);
}
void car_H_24(double *state, double *unused, double *out_8042625102306574911) {
  H_24(state, unused, out_8042625102306574911);
}
void car_h_30(double *state, double *unused, double *out_5895357683701118579) {
  h_30(state, unused, out_5895357683701118579);
}
void car_H_30(double *state, double *unused, double *out_3903462861497960180) {
  H_30(state, unused, out_3903462861497960180);
}
void car_h_26(double *state, double *unused, double *out_893684416040272934) {
  h_26(state, unused, out_893684416040272934);
}
void car_H_26(double *state, double *unused, double *out_3117269850244408206) {
  H_26(state, unused, out_3117269850244408206);
}
void car_h_27(double *state, double *unused, double *out_7327782202938961169) {
  h_27(state, unused, out_7327782202938961169);
}
void car_H_27(double *state, double *unused, double *out_6078226173298385091) {
  H_27(state, unused, out_6078226173298385091);
}
void car_h_29(double *state, double *unused, double *out_553490447815088016) {
  h_29(state, unused, out_553490447815088016);
}
void car_H_29(double *state, double *unused, double *out_3393231517183567996) {
  H_29(state, unused, out_3393231517183567996);
}
void car_h_28(double *state, double *unused, double *out_2326108935278115524) {
  h_28(state, unused, out_2326108935278115524);
}
void car_H_28(double *state, double *unused, double *out_1429601245618241745) {
  H_28(state, unused, out_1429601245618241745);
}
void car_h_31(double *state, double *unused, double *out_4186702975574397042) {
  h_31(state, unused, out_4186702975574397042);
}
void car_H_31(double *state, double *unused, double *out_3743477952477759682) {
  H_31(state, unused, out_3743477952477759682);
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
