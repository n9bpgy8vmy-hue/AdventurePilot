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
void err_fun(double *nom_x, double *delta_x, double *out_7475085428031653655) {
   out_7475085428031653655[0] = delta_x[0] + nom_x[0];
   out_7475085428031653655[1] = delta_x[1] + nom_x[1];
   out_7475085428031653655[2] = delta_x[2] + nom_x[2];
   out_7475085428031653655[3] = delta_x[3] + nom_x[3];
   out_7475085428031653655[4] = delta_x[4] + nom_x[4];
   out_7475085428031653655[5] = delta_x[5] + nom_x[5];
   out_7475085428031653655[6] = delta_x[6] + nom_x[6];
   out_7475085428031653655[7] = delta_x[7] + nom_x[7];
   out_7475085428031653655[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6740238556142338987) {
   out_6740238556142338987[0] = -nom_x[0] + true_x[0];
   out_6740238556142338987[1] = -nom_x[1] + true_x[1];
   out_6740238556142338987[2] = -nom_x[2] + true_x[2];
   out_6740238556142338987[3] = -nom_x[3] + true_x[3];
   out_6740238556142338987[4] = -nom_x[4] + true_x[4];
   out_6740238556142338987[5] = -nom_x[5] + true_x[5];
   out_6740238556142338987[6] = -nom_x[6] + true_x[6];
   out_6740238556142338987[7] = -nom_x[7] + true_x[7];
   out_6740238556142338987[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_7914256537279797197) {
   out_7914256537279797197[0] = 1.0;
   out_7914256537279797197[1] = 0.0;
   out_7914256537279797197[2] = 0.0;
   out_7914256537279797197[3] = 0.0;
   out_7914256537279797197[4] = 0.0;
   out_7914256537279797197[5] = 0.0;
   out_7914256537279797197[6] = 0.0;
   out_7914256537279797197[7] = 0.0;
   out_7914256537279797197[8] = 0.0;
   out_7914256537279797197[9] = 0.0;
   out_7914256537279797197[10] = 1.0;
   out_7914256537279797197[11] = 0.0;
   out_7914256537279797197[12] = 0.0;
   out_7914256537279797197[13] = 0.0;
   out_7914256537279797197[14] = 0.0;
   out_7914256537279797197[15] = 0.0;
   out_7914256537279797197[16] = 0.0;
   out_7914256537279797197[17] = 0.0;
   out_7914256537279797197[18] = 0.0;
   out_7914256537279797197[19] = 0.0;
   out_7914256537279797197[20] = 1.0;
   out_7914256537279797197[21] = 0.0;
   out_7914256537279797197[22] = 0.0;
   out_7914256537279797197[23] = 0.0;
   out_7914256537279797197[24] = 0.0;
   out_7914256537279797197[25] = 0.0;
   out_7914256537279797197[26] = 0.0;
   out_7914256537279797197[27] = 0.0;
   out_7914256537279797197[28] = 0.0;
   out_7914256537279797197[29] = 0.0;
   out_7914256537279797197[30] = 1.0;
   out_7914256537279797197[31] = 0.0;
   out_7914256537279797197[32] = 0.0;
   out_7914256537279797197[33] = 0.0;
   out_7914256537279797197[34] = 0.0;
   out_7914256537279797197[35] = 0.0;
   out_7914256537279797197[36] = 0.0;
   out_7914256537279797197[37] = 0.0;
   out_7914256537279797197[38] = 0.0;
   out_7914256537279797197[39] = 0.0;
   out_7914256537279797197[40] = 1.0;
   out_7914256537279797197[41] = 0.0;
   out_7914256537279797197[42] = 0.0;
   out_7914256537279797197[43] = 0.0;
   out_7914256537279797197[44] = 0.0;
   out_7914256537279797197[45] = 0.0;
   out_7914256537279797197[46] = 0.0;
   out_7914256537279797197[47] = 0.0;
   out_7914256537279797197[48] = 0.0;
   out_7914256537279797197[49] = 0.0;
   out_7914256537279797197[50] = 1.0;
   out_7914256537279797197[51] = 0.0;
   out_7914256537279797197[52] = 0.0;
   out_7914256537279797197[53] = 0.0;
   out_7914256537279797197[54] = 0.0;
   out_7914256537279797197[55] = 0.0;
   out_7914256537279797197[56] = 0.0;
   out_7914256537279797197[57] = 0.0;
   out_7914256537279797197[58] = 0.0;
   out_7914256537279797197[59] = 0.0;
   out_7914256537279797197[60] = 1.0;
   out_7914256537279797197[61] = 0.0;
   out_7914256537279797197[62] = 0.0;
   out_7914256537279797197[63] = 0.0;
   out_7914256537279797197[64] = 0.0;
   out_7914256537279797197[65] = 0.0;
   out_7914256537279797197[66] = 0.0;
   out_7914256537279797197[67] = 0.0;
   out_7914256537279797197[68] = 0.0;
   out_7914256537279797197[69] = 0.0;
   out_7914256537279797197[70] = 1.0;
   out_7914256537279797197[71] = 0.0;
   out_7914256537279797197[72] = 0.0;
   out_7914256537279797197[73] = 0.0;
   out_7914256537279797197[74] = 0.0;
   out_7914256537279797197[75] = 0.0;
   out_7914256537279797197[76] = 0.0;
   out_7914256537279797197[77] = 0.0;
   out_7914256537279797197[78] = 0.0;
   out_7914256537279797197[79] = 0.0;
   out_7914256537279797197[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_7922073110339915414) {
   out_7922073110339915414[0] = state[0];
   out_7922073110339915414[1] = state[1];
   out_7922073110339915414[2] = state[2];
   out_7922073110339915414[3] = state[3];
   out_7922073110339915414[4] = state[4];
   out_7922073110339915414[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_7922073110339915414[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_7922073110339915414[7] = state[7];
   out_7922073110339915414[8] = state[8];
}
void F_fun(double *state, double dt, double *out_71818873545892622) {
   out_71818873545892622[0] = 1;
   out_71818873545892622[1] = 0;
   out_71818873545892622[2] = 0;
   out_71818873545892622[3] = 0;
   out_71818873545892622[4] = 0;
   out_71818873545892622[5] = 0;
   out_71818873545892622[6] = 0;
   out_71818873545892622[7] = 0;
   out_71818873545892622[8] = 0;
   out_71818873545892622[9] = 0;
   out_71818873545892622[10] = 1;
   out_71818873545892622[11] = 0;
   out_71818873545892622[12] = 0;
   out_71818873545892622[13] = 0;
   out_71818873545892622[14] = 0;
   out_71818873545892622[15] = 0;
   out_71818873545892622[16] = 0;
   out_71818873545892622[17] = 0;
   out_71818873545892622[18] = 0;
   out_71818873545892622[19] = 0;
   out_71818873545892622[20] = 1;
   out_71818873545892622[21] = 0;
   out_71818873545892622[22] = 0;
   out_71818873545892622[23] = 0;
   out_71818873545892622[24] = 0;
   out_71818873545892622[25] = 0;
   out_71818873545892622[26] = 0;
   out_71818873545892622[27] = 0;
   out_71818873545892622[28] = 0;
   out_71818873545892622[29] = 0;
   out_71818873545892622[30] = 1;
   out_71818873545892622[31] = 0;
   out_71818873545892622[32] = 0;
   out_71818873545892622[33] = 0;
   out_71818873545892622[34] = 0;
   out_71818873545892622[35] = 0;
   out_71818873545892622[36] = 0;
   out_71818873545892622[37] = 0;
   out_71818873545892622[38] = 0;
   out_71818873545892622[39] = 0;
   out_71818873545892622[40] = 1;
   out_71818873545892622[41] = 0;
   out_71818873545892622[42] = 0;
   out_71818873545892622[43] = 0;
   out_71818873545892622[44] = 0;
   out_71818873545892622[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_71818873545892622[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_71818873545892622[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_71818873545892622[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_71818873545892622[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_71818873545892622[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_71818873545892622[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_71818873545892622[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_71818873545892622[53] = -9.8100000000000005*dt;
   out_71818873545892622[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_71818873545892622[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_71818873545892622[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_71818873545892622[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_71818873545892622[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_71818873545892622[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_71818873545892622[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_71818873545892622[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_71818873545892622[62] = 0;
   out_71818873545892622[63] = 0;
   out_71818873545892622[64] = 0;
   out_71818873545892622[65] = 0;
   out_71818873545892622[66] = 0;
   out_71818873545892622[67] = 0;
   out_71818873545892622[68] = 0;
   out_71818873545892622[69] = 0;
   out_71818873545892622[70] = 1;
   out_71818873545892622[71] = 0;
   out_71818873545892622[72] = 0;
   out_71818873545892622[73] = 0;
   out_71818873545892622[74] = 0;
   out_71818873545892622[75] = 0;
   out_71818873545892622[76] = 0;
   out_71818873545892622[77] = 0;
   out_71818873545892622[78] = 0;
   out_71818873545892622[79] = 0;
   out_71818873545892622[80] = 1;
}
void h_25(double *state, double *unused, double *out_6786654345537079850) {
   out_6786654345537079850[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1417233577389767246) {
   out_1417233577389767246[0] = 0;
   out_1417233577389767246[1] = 0;
   out_1417233577389767246[2] = 0;
   out_1417233577389767246[3] = 0;
   out_1417233577389767246[4] = 0;
   out_1417233577389767246[5] = 0;
   out_1417233577389767246[6] = 1;
   out_1417233577389767246[7] = 0;
   out_1417233577389767246[8] = 0;
}
void h_24(double *state, double *unused, double *out_2658964353809124865) {
   out_2658964353809124865[0] = state[4];
   out_2658964353809124865[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3451581287637939606) {
   out_3451581287637939606[0] = 0;
   out_3451581287637939606[1] = 0;
   out_3451581287637939606[2] = 0;
   out_3451581287637939606[3] = 0;
   out_3451581287637939606[4] = 1;
   out_3451581287637939606[5] = 0;
   out_3451581287637939606[6] = 0;
   out_3451581287637939606[7] = 0;
   out_3451581287637939606[8] = 0;
   out_3451581287637939606[9] = 0;
   out_3451581287637939606[10] = 0;
   out_3451581287637939606[11] = 0;
   out_3451581287637939606[12] = 0;
   out_3451581287637939606[13] = 0;
   out_3451581287637939606[14] = 1;
   out_3451581287637939606[15] = 0;
   out_3451581287637939606[16] = 0;
   out_3451581287637939606[17] = 0;
}
void h_30(double *state, double *unused, double *out_3149204817283057500) {
   out_3149204817283057500[0] = state[4];
}
void H_30(double *state, double *unused, double *out_3110462752737840952) {
   out_3110462752737840952[0] = 0;
   out_3110462752737840952[1] = 0;
   out_3110462752737840952[2] = 0;
   out_3110462752737840952[3] = 0;
   out_3110462752737840952[4] = 1;
   out_3110462752737840952[5] = 0;
   out_3110462752737840952[6] = 0;
   out_3110462752737840952[7] = 0;
   out_3110462752737840952[8] = 0;
}
void h_26(double *state, double *unused, double *out_4131288910618355612) {
   out_4131288910618355612[0] = state[7];
}
void H_26(double *state, double *unused, double *out_2324269741484288978) {
   out_2324269741484288978[0] = 0;
   out_2324269741484288978[1] = 0;
   out_2324269741484288978[2] = 0;
   out_2324269741484288978[3] = 0;
   out_2324269741484288978[4] = 0;
   out_2324269741484288978[5] = 0;
   out_2324269741484288978[6] = 0;
   out_2324269741484288978[7] = 1;
   out_2324269741484288978[8] = 0;
}
void h_27(double *state, double *unused, double *out_6183338460860602333) {
   out_6183338460860602333[0] = state[3];
}
void H_27(double *state, double *unused, double *out_5285226064538265863) {
   out_5285226064538265863[0] = 0;
   out_5285226064538265863[1] = 0;
   out_5285226064538265863[2] = 0;
   out_5285226064538265863[3] = 1;
   out_5285226064538265863[4] = 0;
   out_5285226064538265863[5] = 0;
   out_5285226064538265863[6] = 0;
   out_5285226064538265863[7] = 0;
   out_5285226064538265863[8] = 0;
}
void h_29(double *state, double *unused, double *out_2545888932606579983) {
   out_2545888932606579983[0] = state[1];
}
void H_29(double *state, double *unused, double *out_2600231408423448768) {
   out_2600231408423448768[0] = 0;
   out_2600231408423448768[1] = 1;
   out_2600231408423448768[2] = 0;
   out_2600231408423448768[3] = 0;
   out_2600231408423448768[4] = 0;
   out_2600231408423448768[5] = 0;
   out_2600231408423448768[6] = 0;
   out_2600231408423448768[7] = 0;
   out_2600231408423448768[8] = 0;
}
void h_28(double *state, double *unused, double *out_3281436440842108518) {
   out_3281436440842108518[0] = state[0];
}
void H_28(double *state, double *unused, double *out_7682630425492979342) {
   out_7682630425492979342[0] = 1;
   out_7682630425492979342[1] = 0;
   out_7682630425492979342[2] = 0;
   out_7682630425492979342[3] = 0;
   out_7682630425492979342[4] = 0;
   out_7682630425492979342[5] = 0;
   out_7682630425492979342[6] = 0;
   out_7682630425492979342[7] = 0;
   out_7682630425492979342[8] = 0;
}
void h_31(double *state, double *unused, double *out_673904418711808475) {
   out_673904418711808475[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2950477843717640454) {
   out_2950477843717640454[0] = 0;
   out_2950477843717640454[1] = 0;
   out_2950477843717640454[2] = 0;
   out_2950477843717640454[3] = 0;
   out_2950477843717640454[4] = 0;
   out_2950477843717640454[5] = 0;
   out_2950477843717640454[6] = 0;
   out_2950477843717640454[7] = 0;
   out_2950477843717640454[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7475085428031653655) {
  err_fun(nom_x, delta_x, out_7475085428031653655);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6740238556142338987) {
  inv_err_fun(nom_x, true_x, out_6740238556142338987);
}
void car_H_mod_fun(double *state, double *out_7914256537279797197) {
  H_mod_fun(state, out_7914256537279797197);
}
void car_f_fun(double *state, double dt, double *out_7922073110339915414) {
  f_fun(state,  dt, out_7922073110339915414);
}
void car_F_fun(double *state, double dt, double *out_71818873545892622) {
  F_fun(state,  dt, out_71818873545892622);
}
void car_h_25(double *state, double *unused, double *out_6786654345537079850) {
  h_25(state, unused, out_6786654345537079850);
}
void car_H_25(double *state, double *unused, double *out_1417233577389767246) {
  H_25(state, unused, out_1417233577389767246);
}
void car_h_24(double *state, double *unused, double *out_2658964353809124865) {
  h_24(state, unused, out_2658964353809124865);
}
void car_H_24(double *state, double *unused, double *out_3451581287637939606) {
  H_24(state, unused, out_3451581287637939606);
}
void car_h_30(double *state, double *unused, double *out_3149204817283057500) {
  h_30(state, unused, out_3149204817283057500);
}
void car_H_30(double *state, double *unused, double *out_3110462752737840952) {
  H_30(state, unused, out_3110462752737840952);
}
void car_h_26(double *state, double *unused, double *out_4131288910618355612) {
  h_26(state, unused, out_4131288910618355612);
}
void car_H_26(double *state, double *unused, double *out_2324269741484288978) {
  H_26(state, unused, out_2324269741484288978);
}
void car_h_27(double *state, double *unused, double *out_6183338460860602333) {
  h_27(state, unused, out_6183338460860602333);
}
void car_H_27(double *state, double *unused, double *out_5285226064538265863) {
  H_27(state, unused, out_5285226064538265863);
}
void car_h_29(double *state, double *unused, double *out_2545888932606579983) {
  h_29(state, unused, out_2545888932606579983);
}
void car_H_29(double *state, double *unused, double *out_2600231408423448768) {
  H_29(state, unused, out_2600231408423448768);
}
void car_h_28(double *state, double *unused, double *out_3281436440842108518) {
  h_28(state, unused, out_3281436440842108518);
}
void car_H_28(double *state, double *unused, double *out_7682630425492979342) {
  H_28(state, unused, out_7682630425492979342);
}
void car_h_31(double *state, double *unused, double *out_673904418711808475) {
  h_31(state, unused, out_673904418711808475);
}
void car_H_31(double *state, double *unused, double *out_2950477843717640454) {
  H_31(state, unused, out_2950477843717640454);
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
