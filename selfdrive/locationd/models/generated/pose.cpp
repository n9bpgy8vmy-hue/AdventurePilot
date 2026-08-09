#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7437321502825855060) {
   out_7437321502825855060[0] = delta_x[0] + nom_x[0];
   out_7437321502825855060[1] = delta_x[1] + nom_x[1];
   out_7437321502825855060[2] = delta_x[2] + nom_x[2];
   out_7437321502825855060[3] = delta_x[3] + nom_x[3];
   out_7437321502825855060[4] = delta_x[4] + nom_x[4];
   out_7437321502825855060[5] = delta_x[5] + nom_x[5];
   out_7437321502825855060[6] = delta_x[6] + nom_x[6];
   out_7437321502825855060[7] = delta_x[7] + nom_x[7];
   out_7437321502825855060[8] = delta_x[8] + nom_x[8];
   out_7437321502825855060[9] = delta_x[9] + nom_x[9];
   out_7437321502825855060[10] = delta_x[10] + nom_x[10];
   out_7437321502825855060[11] = delta_x[11] + nom_x[11];
   out_7437321502825855060[12] = delta_x[12] + nom_x[12];
   out_7437321502825855060[13] = delta_x[13] + nom_x[13];
   out_7437321502825855060[14] = delta_x[14] + nom_x[14];
   out_7437321502825855060[15] = delta_x[15] + nom_x[15];
   out_7437321502825855060[16] = delta_x[16] + nom_x[16];
   out_7437321502825855060[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2999325586563752225) {
   out_2999325586563752225[0] = -nom_x[0] + true_x[0];
   out_2999325586563752225[1] = -nom_x[1] + true_x[1];
   out_2999325586563752225[2] = -nom_x[2] + true_x[2];
   out_2999325586563752225[3] = -nom_x[3] + true_x[3];
   out_2999325586563752225[4] = -nom_x[4] + true_x[4];
   out_2999325586563752225[5] = -nom_x[5] + true_x[5];
   out_2999325586563752225[6] = -nom_x[6] + true_x[6];
   out_2999325586563752225[7] = -nom_x[7] + true_x[7];
   out_2999325586563752225[8] = -nom_x[8] + true_x[8];
   out_2999325586563752225[9] = -nom_x[9] + true_x[9];
   out_2999325586563752225[10] = -nom_x[10] + true_x[10];
   out_2999325586563752225[11] = -nom_x[11] + true_x[11];
   out_2999325586563752225[12] = -nom_x[12] + true_x[12];
   out_2999325586563752225[13] = -nom_x[13] + true_x[13];
   out_2999325586563752225[14] = -nom_x[14] + true_x[14];
   out_2999325586563752225[15] = -nom_x[15] + true_x[15];
   out_2999325586563752225[16] = -nom_x[16] + true_x[16];
   out_2999325586563752225[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_38184059748906263) {
   out_38184059748906263[0] = 1.0;
   out_38184059748906263[1] = 0.0;
   out_38184059748906263[2] = 0.0;
   out_38184059748906263[3] = 0.0;
   out_38184059748906263[4] = 0.0;
   out_38184059748906263[5] = 0.0;
   out_38184059748906263[6] = 0.0;
   out_38184059748906263[7] = 0.0;
   out_38184059748906263[8] = 0.0;
   out_38184059748906263[9] = 0.0;
   out_38184059748906263[10] = 0.0;
   out_38184059748906263[11] = 0.0;
   out_38184059748906263[12] = 0.0;
   out_38184059748906263[13] = 0.0;
   out_38184059748906263[14] = 0.0;
   out_38184059748906263[15] = 0.0;
   out_38184059748906263[16] = 0.0;
   out_38184059748906263[17] = 0.0;
   out_38184059748906263[18] = 0.0;
   out_38184059748906263[19] = 1.0;
   out_38184059748906263[20] = 0.0;
   out_38184059748906263[21] = 0.0;
   out_38184059748906263[22] = 0.0;
   out_38184059748906263[23] = 0.0;
   out_38184059748906263[24] = 0.0;
   out_38184059748906263[25] = 0.0;
   out_38184059748906263[26] = 0.0;
   out_38184059748906263[27] = 0.0;
   out_38184059748906263[28] = 0.0;
   out_38184059748906263[29] = 0.0;
   out_38184059748906263[30] = 0.0;
   out_38184059748906263[31] = 0.0;
   out_38184059748906263[32] = 0.0;
   out_38184059748906263[33] = 0.0;
   out_38184059748906263[34] = 0.0;
   out_38184059748906263[35] = 0.0;
   out_38184059748906263[36] = 0.0;
   out_38184059748906263[37] = 0.0;
   out_38184059748906263[38] = 1.0;
   out_38184059748906263[39] = 0.0;
   out_38184059748906263[40] = 0.0;
   out_38184059748906263[41] = 0.0;
   out_38184059748906263[42] = 0.0;
   out_38184059748906263[43] = 0.0;
   out_38184059748906263[44] = 0.0;
   out_38184059748906263[45] = 0.0;
   out_38184059748906263[46] = 0.0;
   out_38184059748906263[47] = 0.0;
   out_38184059748906263[48] = 0.0;
   out_38184059748906263[49] = 0.0;
   out_38184059748906263[50] = 0.0;
   out_38184059748906263[51] = 0.0;
   out_38184059748906263[52] = 0.0;
   out_38184059748906263[53] = 0.0;
   out_38184059748906263[54] = 0.0;
   out_38184059748906263[55] = 0.0;
   out_38184059748906263[56] = 0.0;
   out_38184059748906263[57] = 1.0;
   out_38184059748906263[58] = 0.0;
   out_38184059748906263[59] = 0.0;
   out_38184059748906263[60] = 0.0;
   out_38184059748906263[61] = 0.0;
   out_38184059748906263[62] = 0.0;
   out_38184059748906263[63] = 0.0;
   out_38184059748906263[64] = 0.0;
   out_38184059748906263[65] = 0.0;
   out_38184059748906263[66] = 0.0;
   out_38184059748906263[67] = 0.0;
   out_38184059748906263[68] = 0.0;
   out_38184059748906263[69] = 0.0;
   out_38184059748906263[70] = 0.0;
   out_38184059748906263[71] = 0.0;
   out_38184059748906263[72] = 0.0;
   out_38184059748906263[73] = 0.0;
   out_38184059748906263[74] = 0.0;
   out_38184059748906263[75] = 0.0;
   out_38184059748906263[76] = 1.0;
   out_38184059748906263[77] = 0.0;
   out_38184059748906263[78] = 0.0;
   out_38184059748906263[79] = 0.0;
   out_38184059748906263[80] = 0.0;
   out_38184059748906263[81] = 0.0;
   out_38184059748906263[82] = 0.0;
   out_38184059748906263[83] = 0.0;
   out_38184059748906263[84] = 0.0;
   out_38184059748906263[85] = 0.0;
   out_38184059748906263[86] = 0.0;
   out_38184059748906263[87] = 0.0;
   out_38184059748906263[88] = 0.0;
   out_38184059748906263[89] = 0.0;
   out_38184059748906263[90] = 0.0;
   out_38184059748906263[91] = 0.0;
   out_38184059748906263[92] = 0.0;
   out_38184059748906263[93] = 0.0;
   out_38184059748906263[94] = 0.0;
   out_38184059748906263[95] = 1.0;
   out_38184059748906263[96] = 0.0;
   out_38184059748906263[97] = 0.0;
   out_38184059748906263[98] = 0.0;
   out_38184059748906263[99] = 0.0;
   out_38184059748906263[100] = 0.0;
   out_38184059748906263[101] = 0.0;
   out_38184059748906263[102] = 0.0;
   out_38184059748906263[103] = 0.0;
   out_38184059748906263[104] = 0.0;
   out_38184059748906263[105] = 0.0;
   out_38184059748906263[106] = 0.0;
   out_38184059748906263[107] = 0.0;
   out_38184059748906263[108] = 0.0;
   out_38184059748906263[109] = 0.0;
   out_38184059748906263[110] = 0.0;
   out_38184059748906263[111] = 0.0;
   out_38184059748906263[112] = 0.0;
   out_38184059748906263[113] = 0.0;
   out_38184059748906263[114] = 1.0;
   out_38184059748906263[115] = 0.0;
   out_38184059748906263[116] = 0.0;
   out_38184059748906263[117] = 0.0;
   out_38184059748906263[118] = 0.0;
   out_38184059748906263[119] = 0.0;
   out_38184059748906263[120] = 0.0;
   out_38184059748906263[121] = 0.0;
   out_38184059748906263[122] = 0.0;
   out_38184059748906263[123] = 0.0;
   out_38184059748906263[124] = 0.0;
   out_38184059748906263[125] = 0.0;
   out_38184059748906263[126] = 0.0;
   out_38184059748906263[127] = 0.0;
   out_38184059748906263[128] = 0.0;
   out_38184059748906263[129] = 0.0;
   out_38184059748906263[130] = 0.0;
   out_38184059748906263[131] = 0.0;
   out_38184059748906263[132] = 0.0;
   out_38184059748906263[133] = 1.0;
   out_38184059748906263[134] = 0.0;
   out_38184059748906263[135] = 0.0;
   out_38184059748906263[136] = 0.0;
   out_38184059748906263[137] = 0.0;
   out_38184059748906263[138] = 0.0;
   out_38184059748906263[139] = 0.0;
   out_38184059748906263[140] = 0.0;
   out_38184059748906263[141] = 0.0;
   out_38184059748906263[142] = 0.0;
   out_38184059748906263[143] = 0.0;
   out_38184059748906263[144] = 0.0;
   out_38184059748906263[145] = 0.0;
   out_38184059748906263[146] = 0.0;
   out_38184059748906263[147] = 0.0;
   out_38184059748906263[148] = 0.0;
   out_38184059748906263[149] = 0.0;
   out_38184059748906263[150] = 0.0;
   out_38184059748906263[151] = 0.0;
   out_38184059748906263[152] = 1.0;
   out_38184059748906263[153] = 0.0;
   out_38184059748906263[154] = 0.0;
   out_38184059748906263[155] = 0.0;
   out_38184059748906263[156] = 0.0;
   out_38184059748906263[157] = 0.0;
   out_38184059748906263[158] = 0.0;
   out_38184059748906263[159] = 0.0;
   out_38184059748906263[160] = 0.0;
   out_38184059748906263[161] = 0.0;
   out_38184059748906263[162] = 0.0;
   out_38184059748906263[163] = 0.0;
   out_38184059748906263[164] = 0.0;
   out_38184059748906263[165] = 0.0;
   out_38184059748906263[166] = 0.0;
   out_38184059748906263[167] = 0.0;
   out_38184059748906263[168] = 0.0;
   out_38184059748906263[169] = 0.0;
   out_38184059748906263[170] = 0.0;
   out_38184059748906263[171] = 1.0;
   out_38184059748906263[172] = 0.0;
   out_38184059748906263[173] = 0.0;
   out_38184059748906263[174] = 0.0;
   out_38184059748906263[175] = 0.0;
   out_38184059748906263[176] = 0.0;
   out_38184059748906263[177] = 0.0;
   out_38184059748906263[178] = 0.0;
   out_38184059748906263[179] = 0.0;
   out_38184059748906263[180] = 0.0;
   out_38184059748906263[181] = 0.0;
   out_38184059748906263[182] = 0.0;
   out_38184059748906263[183] = 0.0;
   out_38184059748906263[184] = 0.0;
   out_38184059748906263[185] = 0.0;
   out_38184059748906263[186] = 0.0;
   out_38184059748906263[187] = 0.0;
   out_38184059748906263[188] = 0.0;
   out_38184059748906263[189] = 0.0;
   out_38184059748906263[190] = 1.0;
   out_38184059748906263[191] = 0.0;
   out_38184059748906263[192] = 0.0;
   out_38184059748906263[193] = 0.0;
   out_38184059748906263[194] = 0.0;
   out_38184059748906263[195] = 0.0;
   out_38184059748906263[196] = 0.0;
   out_38184059748906263[197] = 0.0;
   out_38184059748906263[198] = 0.0;
   out_38184059748906263[199] = 0.0;
   out_38184059748906263[200] = 0.0;
   out_38184059748906263[201] = 0.0;
   out_38184059748906263[202] = 0.0;
   out_38184059748906263[203] = 0.0;
   out_38184059748906263[204] = 0.0;
   out_38184059748906263[205] = 0.0;
   out_38184059748906263[206] = 0.0;
   out_38184059748906263[207] = 0.0;
   out_38184059748906263[208] = 0.0;
   out_38184059748906263[209] = 1.0;
   out_38184059748906263[210] = 0.0;
   out_38184059748906263[211] = 0.0;
   out_38184059748906263[212] = 0.0;
   out_38184059748906263[213] = 0.0;
   out_38184059748906263[214] = 0.0;
   out_38184059748906263[215] = 0.0;
   out_38184059748906263[216] = 0.0;
   out_38184059748906263[217] = 0.0;
   out_38184059748906263[218] = 0.0;
   out_38184059748906263[219] = 0.0;
   out_38184059748906263[220] = 0.0;
   out_38184059748906263[221] = 0.0;
   out_38184059748906263[222] = 0.0;
   out_38184059748906263[223] = 0.0;
   out_38184059748906263[224] = 0.0;
   out_38184059748906263[225] = 0.0;
   out_38184059748906263[226] = 0.0;
   out_38184059748906263[227] = 0.0;
   out_38184059748906263[228] = 1.0;
   out_38184059748906263[229] = 0.0;
   out_38184059748906263[230] = 0.0;
   out_38184059748906263[231] = 0.0;
   out_38184059748906263[232] = 0.0;
   out_38184059748906263[233] = 0.0;
   out_38184059748906263[234] = 0.0;
   out_38184059748906263[235] = 0.0;
   out_38184059748906263[236] = 0.0;
   out_38184059748906263[237] = 0.0;
   out_38184059748906263[238] = 0.0;
   out_38184059748906263[239] = 0.0;
   out_38184059748906263[240] = 0.0;
   out_38184059748906263[241] = 0.0;
   out_38184059748906263[242] = 0.0;
   out_38184059748906263[243] = 0.0;
   out_38184059748906263[244] = 0.0;
   out_38184059748906263[245] = 0.0;
   out_38184059748906263[246] = 0.0;
   out_38184059748906263[247] = 1.0;
   out_38184059748906263[248] = 0.0;
   out_38184059748906263[249] = 0.0;
   out_38184059748906263[250] = 0.0;
   out_38184059748906263[251] = 0.0;
   out_38184059748906263[252] = 0.0;
   out_38184059748906263[253] = 0.0;
   out_38184059748906263[254] = 0.0;
   out_38184059748906263[255] = 0.0;
   out_38184059748906263[256] = 0.0;
   out_38184059748906263[257] = 0.0;
   out_38184059748906263[258] = 0.0;
   out_38184059748906263[259] = 0.0;
   out_38184059748906263[260] = 0.0;
   out_38184059748906263[261] = 0.0;
   out_38184059748906263[262] = 0.0;
   out_38184059748906263[263] = 0.0;
   out_38184059748906263[264] = 0.0;
   out_38184059748906263[265] = 0.0;
   out_38184059748906263[266] = 1.0;
   out_38184059748906263[267] = 0.0;
   out_38184059748906263[268] = 0.0;
   out_38184059748906263[269] = 0.0;
   out_38184059748906263[270] = 0.0;
   out_38184059748906263[271] = 0.0;
   out_38184059748906263[272] = 0.0;
   out_38184059748906263[273] = 0.0;
   out_38184059748906263[274] = 0.0;
   out_38184059748906263[275] = 0.0;
   out_38184059748906263[276] = 0.0;
   out_38184059748906263[277] = 0.0;
   out_38184059748906263[278] = 0.0;
   out_38184059748906263[279] = 0.0;
   out_38184059748906263[280] = 0.0;
   out_38184059748906263[281] = 0.0;
   out_38184059748906263[282] = 0.0;
   out_38184059748906263[283] = 0.0;
   out_38184059748906263[284] = 0.0;
   out_38184059748906263[285] = 1.0;
   out_38184059748906263[286] = 0.0;
   out_38184059748906263[287] = 0.0;
   out_38184059748906263[288] = 0.0;
   out_38184059748906263[289] = 0.0;
   out_38184059748906263[290] = 0.0;
   out_38184059748906263[291] = 0.0;
   out_38184059748906263[292] = 0.0;
   out_38184059748906263[293] = 0.0;
   out_38184059748906263[294] = 0.0;
   out_38184059748906263[295] = 0.0;
   out_38184059748906263[296] = 0.0;
   out_38184059748906263[297] = 0.0;
   out_38184059748906263[298] = 0.0;
   out_38184059748906263[299] = 0.0;
   out_38184059748906263[300] = 0.0;
   out_38184059748906263[301] = 0.0;
   out_38184059748906263[302] = 0.0;
   out_38184059748906263[303] = 0.0;
   out_38184059748906263[304] = 1.0;
   out_38184059748906263[305] = 0.0;
   out_38184059748906263[306] = 0.0;
   out_38184059748906263[307] = 0.0;
   out_38184059748906263[308] = 0.0;
   out_38184059748906263[309] = 0.0;
   out_38184059748906263[310] = 0.0;
   out_38184059748906263[311] = 0.0;
   out_38184059748906263[312] = 0.0;
   out_38184059748906263[313] = 0.0;
   out_38184059748906263[314] = 0.0;
   out_38184059748906263[315] = 0.0;
   out_38184059748906263[316] = 0.0;
   out_38184059748906263[317] = 0.0;
   out_38184059748906263[318] = 0.0;
   out_38184059748906263[319] = 0.0;
   out_38184059748906263[320] = 0.0;
   out_38184059748906263[321] = 0.0;
   out_38184059748906263[322] = 0.0;
   out_38184059748906263[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_3123441002119390513) {
   out_3123441002119390513[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_3123441002119390513[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_3123441002119390513[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_3123441002119390513[3] = dt*state[12] + state[3];
   out_3123441002119390513[4] = dt*state[13] + state[4];
   out_3123441002119390513[5] = dt*state[14] + state[5];
   out_3123441002119390513[6] = state[6];
   out_3123441002119390513[7] = state[7];
   out_3123441002119390513[8] = state[8];
   out_3123441002119390513[9] = state[9];
   out_3123441002119390513[10] = state[10];
   out_3123441002119390513[11] = state[11];
   out_3123441002119390513[12] = state[12];
   out_3123441002119390513[13] = state[13];
   out_3123441002119390513[14] = state[14];
   out_3123441002119390513[15] = state[15];
   out_3123441002119390513[16] = state[16];
   out_3123441002119390513[17] = state[17];
}
void F_fun(double *state, double dt, double *out_7604329213029297715) {
   out_7604329213029297715[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7604329213029297715[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7604329213029297715[2] = 0;
   out_7604329213029297715[3] = 0;
   out_7604329213029297715[4] = 0;
   out_7604329213029297715[5] = 0;
   out_7604329213029297715[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7604329213029297715[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7604329213029297715[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7604329213029297715[9] = 0;
   out_7604329213029297715[10] = 0;
   out_7604329213029297715[11] = 0;
   out_7604329213029297715[12] = 0;
   out_7604329213029297715[13] = 0;
   out_7604329213029297715[14] = 0;
   out_7604329213029297715[15] = 0;
   out_7604329213029297715[16] = 0;
   out_7604329213029297715[17] = 0;
   out_7604329213029297715[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7604329213029297715[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7604329213029297715[20] = 0;
   out_7604329213029297715[21] = 0;
   out_7604329213029297715[22] = 0;
   out_7604329213029297715[23] = 0;
   out_7604329213029297715[24] = 0;
   out_7604329213029297715[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7604329213029297715[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7604329213029297715[27] = 0;
   out_7604329213029297715[28] = 0;
   out_7604329213029297715[29] = 0;
   out_7604329213029297715[30] = 0;
   out_7604329213029297715[31] = 0;
   out_7604329213029297715[32] = 0;
   out_7604329213029297715[33] = 0;
   out_7604329213029297715[34] = 0;
   out_7604329213029297715[35] = 0;
   out_7604329213029297715[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7604329213029297715[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7604329213029297715[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7604329213029297715[39] = 0;
   out_7604329213029297715[40] = 0;
   out_7604329213029297715[41] = 0;
   out_7604329213029297715[42] = 0;
   out_7604329213029297715[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7604329213029297715[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7604329213029297715[45] = 0;
   out_7604329213029297715[46] = 0;
   out_7604329213029297715[47] = 0;
   out_7604329213029297715[48] = 0;
   out_7604329213029297715[49] = 0;
   out_7604329213029297715[50] = 0;
   out_7604329213029297715[51] = 0;
   out_7604329213029297715[52] = 0;
   out_7604329213029297715[53] = 0;
   out_7604329213029297715[54] = 0;
   out_7604329213029297715[55] = 0;
   out_7604329213029297715[56] = 0;
   out_7604329213029297715[57] = 1;
   out_7604329213029297715[58] = 0;
   out_7604329213029297715[59] = 0;
   out_7604329213029297715[60] = 0;
   out_7604329213029297715[61] = 0;
   out_7604329213029297715[62] = 0;
   out_7604329213029297715[63] = 0;
   out_7604329213029297715[64] = 0;
   out_7604329213029297715[65] = 0;
   out_7604329213029297715[66] = dt;
   out_7604329213029297715[67] = 0;
   out_7604329213029297715[68] = 0;
   out_7604329213029297715[69] = 0;
   out_7604329213029297715[70] = 0;
   out_7604329213029297715[71] = 0;
   out_7604329213029297715[72] = 0;
   out_7604329213029297715[73] = 0;
   out_7604329213029297715[74] = 0;
   out_7604329213029297715[75] = 0;
   out_7604329213029297715[76] = 1;
   out_7604329213029297715[77] = 0;
   out_7604329213029297715[78] = 0;
   out_7604329213029297715[79] = 0;
   out_7604329213029297715[80] = 0;
   out_7604329213029297715[81] = 0;
   out_7604329213029297715[82] = 0;
   out_7604329213029297715[83] = 0;
   out_7604329213029297715[84] = 0;
   out_7604329213029297715[85] = dt;
   out_7604329213029297715[86] = 0;
   out_7604329213029297715[87] = 0;
   out_7604329213029297715[88] = 0;
   out_7604329213029297715[89] = 0;
   out_7604329213029297715[90] = 0;
   out_7604329213029297715[91] = 0;
   out_7604329213029297715[92] = 0;
   out_7604329213029297715[93] = 0;
   out_7604329213029297715[94] = 0;
   out_7604329213029297715[95] = 1;
   out_7604329213029297715[96] = 0;
   out_7604329213029297715[97] = 0;
   out_7604329213029297715[98] = 0;
   out_7604329213029297715[99] = 0;
   out_7604329213029297715[100] = 0;
   out_7604329213029297715[101] = 0;
   out_7604329213029297715[102] = 0;
   out_7604329213029297715[103] = 0;
   out_7604329213029297715[104] = dt;
   out_7604329213029297715[105] = 0;
   out_7604329213029297715[106] = 0;
   out_7604329213029297715[107] = 0;
   out_7604329213029297715[108] = 0;
   out_7604329213029297715[109] = 0;
   out_7604329213029297715[110] = 0;
   out_7604329213029297715[111] = 0;
   out_7604329213029297715[112] = 0;
   out_7604329213029297715[113] = 0;
   out_7604329213029297715[114] = 1;
   out_7604329213029297715[115] = 0;
   out_7604329213029297715[116] = 0;
   out_7604329213029297715[117] = 0;
   out_7604329213029297715[118] = 0;
   out_7604329213029297715[119] = 0;
   out_7604329213029297715[120] = 0;
   out_7604329213029297715[121] = 0;
   out_7604329213029297715[122] = 0;
   out_7604329213029297715[123] = 0;
   out_7604329213029297715[124] = 0;
   out_7604329213029297715[125] = 0;
   out_7604329213029297715[126] = 0;
   out_7604329213029297715[127] = 0;
   out_7604329213029297715[128] = 0;
   out_7604329213029297715[129] = 0;
   out_7604329213029297715[130] = 0;
   out_7604329213029297715[131] = 0;
   out_7604329213029297715[132] = 0;
   out_7604329213029297715[133] = 1;
   out_7604329213029297715[134] = 0;
   out_7604329213029297715[135] = 0;
   out_7604329213029297715[136] = 0;
   out_7604329213029297715[137] = 0;
   out_7604329213029297715[138] = 0;
   out_7604329213029297715[139] = 0;
   out_7604329213029297715[140] = 0;
   out_7604329213029297715[141] = 0;
   out_7604329213029297715[142] = 0;
   out_7604329213029297715[143] = 0;
   out_7604329213029297715[144] = 0;
   out_7604329213029297715[145] = 0;
   out_7604329213029297715[146] = 0;
   out_7604329213029297715[147] = 0;
   out_7604329213029297715[148] = 0;
   out_7604329213029297715[149] = 0;
   out_7604329213029297715[150] = 0;
   out_7604329213029297715[151] = 0;
   out_7604329213029297715[152] = 1;
   out_7604329213029297715[153] = 0;
   out_7604329213029297715[154] = 0;
   out_7604329213029297715[155] = 0;
   out_7604329213029297715[156] = 0;
   out_7604329213029297715[157] = 0;
   out_7604329213029297715[158] = 0;
   out_7604329213029297715[159] = 0;
   out_7604329213029297715[160] = 0;
   out_7604329213029297715[161] = 0;
   out_7604329213029297715[162] = 0;
   out_7604329213029297715[163] = 0;
   out_7604329213029297715[164] = 0;
   out_7604329213029297715[165] = 0;
   out_7604329213029297715[166] = 0;
   out_7604329213029297715[167] = 0;
   out_7604329213029297715[168] = 0;
   out_7604329213029297715[169] = 0;
   out_7604329213029297715[170] = 0;
   out_7604329213029297715[171] = 1;
   out_7604329213029297715[172] = 0;
   out_7604329213029297715[173] = 0;
   out_7604329213029297715[174] = 0;
   out_7604329213029297715[175] = 0;
   out_7604329213029297715[176] = 0;
   out_7604329213029297715[177] = 0;
   out_7604329213029297715[178] = 0;
   out_7604329213029297715[179] = 0;
   out_7604329213029297715[180] = 0;
   out_7604329213029297715[181] = 0;
   out_7604329213029297715[182] = 0;
   out_7604329213029297715[183] = 0;
   out_7604329213029297715[184] = 0;
   out_7604329213029297715[185] = 0;
   out_7604329213029297715[186] = 0;
   out_7604329213029297715[187] = 0;
   out_7604329213029297715[188] = 0;
   out_7604329213029297715[189] = 0;
   out_7604329213029297715[190] = 1;
   out_7604329213029297715[191] = 0;
   out_7604329213029297715[192] = 0;
   out_7604329213029297715[193] = 0;
   out_7604329213029297715[194] = 0;
   out_7604329213029297715[195] = 0;
   out_7604329213029297715[196] = 0;
   out_7604329213029297715[197] = 0;
   out_7604329213029297715[198] = 0;
   out_7604329213029297715[199] = 0;
   out_7604329213029297715[200] = 0;
   out_7604329213029297715[201] = 0;
   out_7604329213029297715[202] = 0;
   out_7604329213029297715[203] = 0;
   out_7604329213029297715[204] = 0;
   out_7604329213029297715[205] = 0;
   out_7604329213029297715[206] = 0;
   out_7604329213029297715[207] = 0;
   out_7604329213029297715[208] = 0;
   out_7604329213029297715[209] = 1;
   out_7604329213029297715[210] = 0;
   out_7604329213029297715[211] = 0;
   out_7604329213029297715[212] = 0;
   out_7604329213029297715[213] = 0;
   out_7604329213029297715[214] = 0;
   out_7604329213029297715[215] = 0;
   out_7604329213029297715[216] = 0;
   out_7604329213029297715[217] = 0;
   out_7604329213029297715[218] = 0;
   out_7604329213029297715[219] = 0;
   out_7604329213029297715[220] = 0;
   out_7604329213029297715[221] = 0;
   out_7604329213029297715[222] = 0;
   out_7604329213029297715[223] = 0;
   out_7604329213029297715[224] = 0;
   out_7604329213029297715[225] = 0;
   out_7604329213029297715[226] = 0;
   out_7604329213029297715[227] = 0;
   out_7604329213029297715[228] = 1;
   out_7604329213029297715[229] = 0;
   out_7604329213029297715[230] = 0;
   out_7604329213029297715[231] = 0;
   out_7604329213029297715[232] = 0;
   out_7604329213029297715[233] = 0;
   out_7604329213029297715[234] = 0;
   out_7604329213029297715[235] = 0;
   out_7604329213029297715[236] = 0;
   out_7604329213029297715[237] = 0;
   out_7604329213029297715[238] = 0;
   out_7604329213029297715[239] = 0;
   out_7604329213029297715[240] = 0;
   out_7604329213029297715[241] = 0;
   out_7604329213029297715[242] = 0;
   out_7604329213029297715[243] = 0;
   out_7604329213029297715[244] = 0;
   out_7604329213029297715[245] = 0;
   out_7604329213029297715[246] = 0;
   out_7604329213029297715[247] = 1;
   out_7604329213029297715[248] = 0;
   out_7604329213029297715[249] = 0;
   out_7604329213029297715[250] = 0;
   out_7604329213029297715[251] = 0;
   out_7604329213029297715[252] = 0;
   out_7604329213029297715[253] = 0;
   out_7604329213029297715[254] = 0;
   out_7604329213029297715[255] = 0;
   out_7604329213029297715[256] = 0;
   out_7604329213029297715[257] = 0;
   out_7604329213029297715[258] = 0;
   out_7604329213029297715[259] = 0;
   out_7604329213029297715[260] = 0;
   out_7604329213029297715[261] = 0;
   out_7604329213029297715[262] = 0;
   out_7604329213029297715[263] = 0;
   out_7604329213029297715[264] = 0;
   out_7604329213029297715[265] = 0;
   out_7604329213029297715[266] = 1;
   out_7604329213029297715[267] = 0;
   out_7604329213029297715[268] = 0;
   out_7604329213029297715[269] = 0;
   out_7604329213029297715[270] = 0;
   out_7604329213029297715[271] = 0;
   out_7604329213029297715[272] = 0;
   out_7604329213029297715[273] = 0;
   out_7604329213029297715[274] = 0;
   out_7604329213029297715[275] = 0;
   out_7604329213029297715[276] = 0;
   out_7604329213029297715[277] = 0;
   out_7604329213029297715[278] = 0;
   out_7604329213029297715[279] = 0;
   out_7604329213029297715[280] = 0;
   out_7604329213029297715[281] = 0;
   out_7604329213029297715[282] = 0;
   out_7604329213029297715[283] = 0;
   out_7604329213029297715[284] = 0;
   out_7604329213029297715[285] = 1;
   out_7604329213029297715[286] = 0;
   out_7604329213029297715[287] = 0;
   out_7604329213029297715[288] = 0;
   out_7604329213029297715[289] = 0;
   out_7604329213029297715[290] = 0;
   out_7604329213029297715[291] = 0;
   out_7604329213029297715[292] = 0;
   out_7604329213029297715[293] = 0;
   out_7604329213029297715[294] = 0;
   out_7604329213029297715[295] = 0;
   out_7604329213029297715[296] = 0;
   out_7604329213029297715[297] = 0;
   out_7604329213029297715[298] = 0;
   out_7604329213029297715[299] = 0;
   out_7604329213029297715[300] = 0;
   out_7604329213029297715[301] = 0;
   out_7604329213029297715[302] = 0;
   out_7604329213029297715[303] = 0;
   out_7604329213029297715[304] = 1;
   out_7604329213029297715[305] = 0;
   out_7604329213029297715[306] = 0;
   out_7604329213029297715[307] = 0;
   out_7604329213029297715[308] = 0;
   out_7604329213029297715[309] = 0;
   out_7604329213029297715[310] = 0;
   out_7604329213029297715[311] = 0;
   out_7604329213029297715[312] = 0;
   out_7604329213029297715[313] = 0;
   out_7604329213029297715[314] = 0;
   out_7604329213029297715[315] = 0;
   out_7604329213029297715[316] = 0;
   out_7604329213029297715[317] = 0;
   out_7604329213029297715[318] = 0;
   out_7604329213029297715[319] = 0;
   out_7604329213029297715[320] = 0;
   out_7604329213029297715[321] = 0;
   out_7604329213029297715[322] = 0;
   out_7604329213029297715[323] = 1;
}
void h_4(double *state, double *unused, double *out_3079979003165382555) {
   out_3079979003165382555[0] = state[6] + state[9];
   out_3079979003165382555[1] = state[7] + state[10];
   out_3079979003165382555[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_9051168479662590755) {
   out_9051168479662590755[0] = 0;
   out_9051168479662590755[1] = 0;
   out_9051168479662590755[2] = 0;
   out_9051168479662590755[3] = 0;
   out_9051168479662590755[4] = 0;
   out_9051168479662590755[5] = 0;
   out_9051168479662590755[6] = 1;
   out_9051168479662590755[7] = 0;
   out_9051168479662590755[8] = 0;
   out_9051168479662590755[9] = 1;
   out_9051168479662590755[10] = 0;
   out_9051168479662590755[11] = 0;
   out_9051168479662590755[12] = 0;
   out_9051168479662590755[13] = 0;
   out_9051168479662590755[14] = 0;
   out_9051168479662590755[15] = 0;
   out_9051168479662590755[16] = 0;
   out_9051168479662590755[17] = 0;
   out_9051168479662590755[18] = 0;
   out_9051168479662590755[19] = 0;
   out_9051168479662590755[20] = 0;
   out_9051168479662590755[21] = 0;
   out_9051168479662590755[22] = 0;
   out_9051168479662590755[23] = 0;
   out_9051168479662590755[24] = 0;
   out_9051168479662590755[25] = 1;
   out_9051168479662590755[26] = 0;
   out_9051168479662590755[27] = 0;
   out_9051168479662590755[28] = 1;
   out_9051168479662590755[29] = 0;
   out_9051168479662590755[30] = 0;
   out_9051168479662590755[31] = 0;
   out_9051168479662590755[32] = 0;
   out_9051168479662590755[33] = 0;
   out_9051168479662590755[34] = 0;
   out_9051168479662590755[35] = 0;
   out_9051168479662590755[36] = 0;
   out_9051168479662590755[37] = 0;
   out_9051168479662590755[38] = 0;
   out_9051168479662590755[39] = 0;
   out_9051168479662590755[40] = 0;
   out_9051168479662590755[41] = 0;
   out_9051168479662590755[42] = 0;
   out_9051168479662590755[43] = 0;
   out_9051168479662590755[44] = 1;
   out_9051168479662590755[45] = 0;
   out_9051168479662590755[46] = 0;
   out_9051168479662590755[47] = 1;
   out_9051168479662590755[48] = 0;
   out_9051168479662590755[49] = 0;
   out_9051168479662590755[50] = 0;
   out_9051168479662590755[51] = 0;
   out_9051168479662590755[52] = 0;
   out_9051168479662590755[53] = 0;
}
void h_10(double *state, double *unused, double *out_207658145770227555) {
   out_207658145770227555[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_207658145770227555[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_207658145770227555[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_400170142804588747) {
   out_400170142804588747[0] = 0;
   out_400170142804588747[1] = 9.8100000000000005*cos(state[1]);
   out_400170142804588747[2] = 0;
   out_400170142804588747[3] = 0;
   out_400170142804588747[4] = -state[8];
   out_400170142804588747[5] = state[7];
   out_400170142804588747[6] = 0;
   out_400170142804588747[7] = state[5];
   out_400170142804588747[8] = -state[4];
   out_400170142804588747[9] = 0;
   out_400170142804588747[10] = 0;
   out_400170142804588747[11] = 0;
   out_400170142804588747[12] = 1;
   out_400170142804588747[13] = 0;
   out_400170142804588747[14] = 0;
   out_400170142804588747[15] = 1;
   out_400170142804588747[16] = 0;
   out_400170142804588747[17] = 0;
   out_400170142804588747[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_400170142804588747[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_400170142804588747[20] = 0;
   out_400170142804588747[21] = state[8];
   out_400170142804588747[22] = 0;
   out_400170142804588747[23] = -state[6];
   out_400170142804588747[24] = -state[5];
   out_400170142804588747[25] = 0;
   out_400170142804588747[26] = state[3];
   out_400170142804588747[27] = 0;
   out_400170142804588747[28] = 0;
   out_400170142804588747[29] = 0;
   out_400170142804588747[30] = 0;
   out_400170142804588747[31] = 1;
   out_400170142804588747[32] = 0;
   out_400170142804588747[33] = 0;
   out_400170142804588747[34] = 1;
   out_400170142804588747[35] = 0;
   out_400170142804588747[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_400170142804588747[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_400170142804588747[38] = 0;
   out_400170142804588747[39] = -state[7];
   out_400170142804588747[40] = state[6];
   out_400170142804588747[41] = 0;
   out_400170142804588747[42] = state[4];
   out_400170142804588747[43] = -state[3];
   out_400170142804588747[44] = 0;
   out_400170142804588747[45] = 0;
   out_400170142804588747[46] = 0;
   out_400170142804588747[47] = 0;
   out_400170142804588747[48] = 0;
   out_400170142804588747[49] = 0;
   out_400170142804588747[50] = 1;
   out_400170142804588747[51] = 0;
   out_400170142804588747[52] = 0;
   out_400170142804588747[53] = 1;
}
void h_13(double *state, double *unused, double *out_5067820152380537214) {
   out_5067820152380537214[0] = state[3];
   out_5067820152380537214[1] = state[4];
   out_5067820152380537214[2] = state[5];
}
void H_13(double *state, double *unused, double *out_6183301768714628060) {
   out_6183301768714628060[0] = 0;
   out_6183301768714628060[1] = 0;
   out_6183301768714628060[2] = 0;
   out_6183301768714628060[3] = 1;
   out_6183301768714628060[4] = 0;
   out_6183301768714628060[5] = 0;
   out_6183301768714628060[6] = 0;
   out_6183301768714628060[7] = 0;
   out_6183301768714628060[8] = 0;
   out_6183301768714628060[9] = 0;
   out_6183301768714628060[10] = 0;
   out_6183301768714628060[11] = 0;
   out_6183301768714628060[12] = 0;
   out_6183301768714628060[13] = 0;
   out_6183301768714628060[14] = 0;
   out_6183301768714628060[15] = 0;
   out_6183301768714628060[16] = 0;
   out_6183301768714628060[17] = 0;
   out_6183301768714628060[18] = 0;
   out_6183301768714628060[19] = 0;
   out_6183301768714628060[20] = 0;
   out_6183301768714628060[21] = 0;
   out_6183301768714628060[22] = 1;
   out_6183301768714628060[23] = 0;
   out_6183301768714628060[24] = 0;
   out_6183301768714628060[25] = 0;
   out_6183301768714628060[26] = 0;
   out_6183301768714628060[27] = 0;
   out_6183301768714628060[28] = 0;
   out_6183301768714628060[29] = 0;
   out_6183301768714628060[30] = 0;
   out_6183301768714628060[31] = 0;
   out_6183301768714628060[32] = 0;
   out_6183301768714628060[33] = 0;
   out_6183301768714628060[34] = 0;
   out_6183301768714628060[35] = 0;
   out_6183301768714628060[36] = 0;
   out_6183301768714628060[37] = 0;
   out_6183301768714628060[38] = 0;
   out_6183301768714628060[39] = 0;
   out_6183301768714628060[40] = 0;
   out_6183301768714628060[41] = 1;
   out_6183301768714628060[42] = 0;
   out_6183301768714628060[43] = 0;
   out_6183301768714628060[44] = 0;
   out_6183301768714628060[45] = 0;
   out_6183301768714628060[46] = 0;
   out_6183301768714628060[47] = 0;
   out_6183301768714628060[48] = 0;
   out_6183301768714628060[49] = 0;
   out_6183301768714628060[50] = 0;
   out_6183301768714628060[51] = 0;
   out_6183301768714628060[52] = 0;
   out_6183301768714628060[53] = 0;
}
void h_14(double *state, double *unused, double *out_7916219184308929140) {
   out_7916219184308929140[0] = state[6];
   out_7916219184308929140[1] = state[7];
   out_7916219184308929140[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8616051953017707156) {
   out_8616051953017707156[0] = 0;
   out_8616051953017707156[1] = 0;
   out_8616051953017707156[2] = 0;
   out_8616051953017707156[3] = 0;
   out_8616051953017707156[4] = 0;
   out_8616051953017707156[5] = 0;
   out_8616051953017707156[6] = 1;
   out_8616051953017707156[7] = 0;
   out_8616051953017707156[8] = 0;
   out_8616051953017707156[9] = 0;
   out_8616051953017707156[10] = 0;
   out_8616051953017707156[11] = 0;
   out_8616051953017707156[12] = 0;
   out_8616051953017707156[13] = 0;
   out_8616051953017707156[14] = 0;
   out_8616051953017707156[15] = 0;
   out_8616051953017707156[16] = 0;
   out_8616051953017707156[17] = 0;
   out_8616051953017707156[18] = 0;
   out_8616051953017707156[19] = 0;
   out_8616051953017707156[20] = 0;
   out_8616051953017707156[21] = 0;
   out_8616051953017707156[22] = 0;
   out_8616051953017707156[23] = 0;
   out_8616051953017707156[24] = 0;
   out_8616051953017707156[25] = 1;
   out_8616051953017707156[26] = 0;
   out_8616051953017707156[27] = 0;
   out_8616051953017707156[28] = 0;
   out_8616051953017707156[29] = 0;
   out_8616051953017707156[30] = 0;
   out_8616051953017707156[31] = 0;
   out_8616051953017707156[32] = 0;
   out_8616051953017707156[33] = 0;
   out_8616051953017707156[34] = 0;
   out_8616051953017707156[35] = 0;
   out_8616051953017707156[36] = 0;
   out_8616051953017707156[37] = 0;
   out_8616051953017707156[38] = 0;
   out_8616051953017707156[39] = 0;
   out_8616051953017707156[40] = 0;
   out_8616051953017707156[41] = 0;
   out_8616051953017707156[42] = 0;
   out_8616051953017707156[43] = 0;
   out_8616051953017707156[44] = 1;
   out_8616051953017707156[45] = 0;
   out_8616051953017707156[46] = 0;
   out_8616051953017707156[47] = 0;
   out_8616051953017707156[48] = 0;
   out_8616051953017707156[49] = 0;
   out_8616051953017707156[50] = 0;
   out_8616051953017707156[51] = 0;
   out_8616051953017707156[52] = 0;
   out_8616051953017707156[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_7437321502825855060) {
  err_fun(nom_x, delta_x, out_7437321502825855060);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2999325586563752225) {
  inv_err_fun(nom_x, true_x, out_2999325586563752225);
}
void pose_H_mod_fun(double *state, double *out_38184059748906263) {
  H_mod_fun(state, out_38184059748906263);
}
void pose_f_fun(double *state, double dt, double *out_3123441002119390513) {
  f_fun(state,  dt, out_3123441002119390513);
}
void pose_F_fun(double *state, double dt, double *out_7604329213029297715) {
  F_fun(state,  dt, out_7604329213029297715);
}
void pose_h_4(double *state, double *unused, double *out_3079979003165382555) {
  h_4(state, unused, out_3079979003165382555);
}
void pose_H_4(double *state, double *unused, double *out_9051168479662590755) {
  H_4(state, unused, out_9051168479662590755);
}
void pose_h_10(double *state, double *unused, double *out_207658145770227555) {
  h_10(state, unused, out_207658145770227555);
}
void pose_H_10(double *state, double *unused, double *out_400170142804588747) {
  H_10(state, unused, out_400170142804588747);
}
void pose_h_13(double *state, double *unused, double *out_5067820152380537214) {
  h_13(state, unused, out_5067820152380537214);
}
void pose_H_13(double *state, double *unused, double *out_6183301768714628060) {
  H_13(state, unused, out_6183301768714628060);
}
void pose_h_14(double *state, double *unused, double *out_7916219184308929140) {
  h_14(state, unused, out_7916219184308929140);
}
void pose_H_14(double *state, double *unused, double *out_8616051953017707156) {
  H_14(state, unused, out_8616051953017707156);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
