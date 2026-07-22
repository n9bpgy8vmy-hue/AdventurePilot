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
void err_fun(double *nom_x, double *delta_x, double *out_2896449614613514894) {
   out_2896449614613514894[0] = delta_x[0] + nom_x[0];
   out_2896449614613514894[1] = delta_x[1] + nom_x[1];
   out_2896449614613514894[2] = delta_x[2] + nom_x[2];
   out_2896449614613514894[3] = delta_x[3] + nom_x[3];
   out_2896449614613514894[4] = delta_x[4] + nom_x[4];
   out_2896449614613514894[5] = delta_x[5] + nom_x[5];
   out_2896449614613514894[6] = delta_x[6] + nom_x[6];
   out_2896449614613514894[7] = delta_x[7] + nom_x[7];
   out_2896449614613514894[8] = delta_x[8] + nom_x[8];
   out_2896449614613514894[9] = delta_x[9] + nom_x[9];
   out_2896449614613514894[10] = delta_x[10] + nom_x[10];
   out_2896449614613514894[11] = delta_x[11] + nom_x[11];
   out_2896449614613514894[12] = delta_x[12] + nom_x[12];
   out_2896449614613514894[13] = delta_x[13] + nom_x[13];
   out_2896449614613514894[14] = delta_x[14] + nom_x[14];
   out_2896449614613514894[15] = delta_x[15] + nom_x[15];
   out_2896449614613514894[16] = delta_x[16] + nom_x[16];
   out_2896449614613514894[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7848261021210579995) {
   out_7848261021210579995[0] = -nom_x[0] + true_x[0];
   out_7848261021210579995[1] = -nom_x[1] + true_x[1];
   out_7848261021210579995[2] = -nom_x[2] + true_x[2];
   out_7848261021210579995[3] = -nom_x[3] + true_x[3];
   out_7848261021210579995[4] = -nom_x[4] + true_x[4];
   out_7848261021210579995[5] = -nom_x[5] + true_x[5];
   out_7848261021210579995[6] = -nom_x[6] + true_x[6];
   out_7848261021210579995[7] = -nom_x[7] + true_x[7];
   out_7848261021210579995[8] = -nom_x[8] + true_x[8];
   out_7848261021210579995[9] = -nom_x[9] + true_x[9];
   out_7848261021210579995[10] = -nom_x[10] + true_x[10];
   out_7848261021210579995[11] = -nom_x[11] + true_x[11];
   out_7848261021210579995[12] = -nom_x[12] + true_x[12];
   out_7848261021210579995[13] = -nom_x[13] + true_x[13];
   out_7848261021210579995[14] = -nom_x[14] + true_x[14];
   out_7848261021210579995[15] = -nom_x[15] + true_x[15];
   out_7848261021210579995[16] = -nom_x[16] + true_x[16];
   out_7848261021210579995[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_1001504416551130467) {
   out_1001504416551130467[0] = 1.0;
   out_1001504416551130467[1] = 0.0;
   out_1001504416551130467[2] = 0.0;
   out_1001504416551130467[3] = 0.0;
   out_1001504416551130467[4] = 0.0;
   out_1001504416551130467[5] = 0.0;
   out_1001504416551130467[6] = 0.0;
   out_1001504416551130467[7] = 0.0;
   out_1001504416551130467[8] = 0.0;
   out_1001504416551130467[9] = 0.0;
   out_1001504416551130467[10] = 0.0;
   out_1001504416551130467[11] = 0.0;
   out_1001504416551130467[12] = 0.0;
   out_1001504416551130467[13] = 0.0;
   out_1001504416551130467[14] = 0.0;
   out_1001504416551130467[15] = 0.0;
   out_1001504416551130467[16] = 0.0;
   out_1001504416551130467[17] = 0.0;
   out_1001504416551130467[18] = 0.0;
   out_1001504416551130467[19] = 1.0;
   out_1001504416551130467[20] = 0.0;
   out_1001504416551130467[21] = 0.0;
   out_1001504416551130467[22] = 0.0;
   out_1001504416551130467[23] = 0.0;
   out_1001504416551130467[24] = 0.0;
   out_1001504416551130467[25] = 0.0;
   out_1001504416551130467[26] = 0.0;
   out_1001504416551130467[27] = 0.0;
   out_1001504416551130467[28] = 0.0;
   out_1001504416551130467[29] = 0.0;
   out_1001504416551130467[30] = 0.0;
   out_1001504416551130467[31] = 0.0;
   out_1001504416551130467[32] = 0.0;
   out_1001504416551130467[33] = 0.0;
   out_1001504416551130467[34] = 0.0;
   out_1001504416551130467[35] = 0.0;
   out_1001504416551130467[36] = 0.0;
   out_1001504416551130467[37] = 0.0;
   out_1001504416551130467[38] = 1.0;
   out_1001504416551130467[39] = 0.0;
   out_1001504416551130467[40] = 0.0;
   out_1001504416551130467[41] = 0.0;
   out_1001504416551130467[42] = 0.0;
   out_1001504416551130467[43] = 0.0;
   out_1001504416551130467[44] = 0.0;
   out_1001504416551130467[45] = 0.0;
   out_1001504416551130467[46] = 0.0;
   out_1001504416551130467[47] = 0.0;
   out_1001504416551130467[48] = 0.0;
   out_1001504416551130467[49] = 0.0;
   out_1001504416551130467[50] = 0.0;
   out_1001504416551130467[51] = 0.0;
   out_1001504416551130467[52] = 0.0;
   out_1001504416551130467[53] = 0.0;
   out_1001504416551130467[54] = 0.0;
   out_1001504416551130467[55] = 0.0;
   out_1001504416551130467[56] = 0.0;
   out_1001504416551130467[57] = 1.0;
   out_1001504416551130467[58] = 0.0;
   out_1001504416551130467[59] = 0.0;
   out_1001504416551130467[60] = 0.0;
   out_1001504416551130467[61] = 0.0;
   out_1001504416551130467[62] = 0.0;
   out_1001504416551130467[63] = 0.0;
   out_1001504416551130467[64] = 0.0;
   out_1001504416551130467[65] = 0.0;
   out_1001504416551130467[66] = 0.0;
   out_1001504416551130467[67] = 0.0;
   out_1001504416551130467[68] = 0.0;
   out_1001504416551130467[69] = 0.0;
   out_1001504416551130467[70] = 0.0;
   out_1001504416551130467[71] = 0.0;
   out_1001504416551130467[72] = 0.0;
   out_1001504416551130467[73] = 0.0;
   out_1001504416551130467[74] = 0.0;
   out_1001504416551130467[75] = 0.0;
   out_1001504416551130467[76] = 1.0;
   out_1001504416551130467[77] = 0.0;
   out_1001504416551130467[78] = 0.0;
   out_1001504416551130467[79] = 0.0;
   out_1001504416551130467[80] = 0.0;
   out_1001504416551130467[81] = 0.0;
   out_1001504416551130467[82] = 0.0;
   out_1001504416551130467[83] = 0.0;
   out_1001504416551130467[84] = 0.0;
   out_1001504416551130467[85] = 0.0;
   out_1001504416551130467[86] = 0.0;
   out_1001504416551130467[87] = 0.0;
   out_1001504416551130467[88] = 0.0;
   out_1001504416551130467[89] = 0.0;
   out_1001504416551130467[90] = 0.0;
   out_1001504416551130467[91] = 0.0;
   out_1001504416551130467[92] = 0.0;
   out_1001504416551130467[93] = 0.0;
   out_1001504416551130467[94] = 0.0;
   out_1001504416551130467[95] = 1.0;
   out_1001504416551130467[96] = 0.0;
   out_1001504416551130467[97] = 0.0;
   out_1001504416551130467[98] = 0.0;
   out_1001504416551130467[99] = 0.0;
   out_1001504416551130467[100] = 0.0;
   out_1001504416551130467[101] = 0.0;
   out_1001504416551130467[102] = 0.0;
   out_1001504416551130467[103] = 0.0;
   out_1001504416551130467[104] = 0.0;
   out_1001504416551130467[105] = 0.0;
   out_1001504416551130467[106] = 0.0;
   out_1001504416551130467[107] = 0.0;
   out_1001504416551130467[108] = 0.0;
   out_1001504416551130467[109] = 0.0;
   out_1001504416551130467[110] = 0.0;
   out_1001504416551130467[111] = 0.0;
   out_1001504416551130467[112] = 0.0;
   out_1001504416551130467[113] = 0.0;
   out_1001504416551130467[114] = 1.0;
   out_1001504416551130467[115] = 0.0;
   out_1001504416551130467[116] = 0.0;
   out_1001504416551130467[117] = 0.0;
   out_1001504416551130467[118] = 0.0;
   out_1001504416551130467[119] = 0.0;
   out_1001504416551130467[120] = 0.0;
   out_1001504416551130467[121] = 0.0;
   out_1001504416551130467[122] = 0.0;
   out_1001504416551130467[123] = 0.0;
   out_1001504416551130467[124] = 0.0;
   out_1001504416551130467[125] = 0.0;
   out_1001504416551130467[126] = 0.0;
   out_1001504416551130467[127] = 0.0;
   out_1001504416551130467[128] = 0.0;
   out_1001504416551130467[129] = 0.0;
   out_1001504416551130467[130] = 0.0;
   out_1001504416551130467[131] = 0.0;
   out_1001504416551130467[132] = 0.0;
   out_1001504416551130467[133] = 1.0;
   out_1001504416551130467[134] = 0.0;
   out_1001504416551130467[135] = 0.0;
   out_1001504416551130467[136] = 0.0;
   out_1001504416551130467[137] = 0.0;
   out_1001504416551130467[138] = 0.0;
   out_1001504416551130467[139] = 0.0;
   out_1001504416551130467[140] = 0.0;
   out_1001504416551130467[141] = 0.0;
   out_1001504416551130467[142] = 0.0;
   out_1001504416551130467[143] = 0.0;
   out_1001504416551130467[144] = 0.0;
   out_1001504416551130467[145] = 0.0;
   out_1001504416551130467[146] = 0.0;
   out_1001504416551130467[147] = 0.0;
   out_1001504416551130467[148] = 0.0;
   out_1001504416551130467[149] = 0.0;
   out_1001504416551130467[150] = 0.0;
   out_1001504416551130467[151] = 0.0;
   out_1001504416551130467[152] = 1.0;
   out_1001504416551130467[153] = 0.0;
   out_1001504416551130467[154] = 0.0;
   out_1001504416551130467[155] = 0.0;
   out_1001504416551130467[156] = 0.0;
   out_1001504416551130467[157] = 0.0;
   out_1001504416551130467[158] = 0.0;
   out_1001504416551130467[159] = 0.0;
   out_1001504416551130467[160] = 0.0;
   out_1001504416551130467[161] = 0.0;
   out_1001504416551130467[162] = 0.0;
   out_1001504416551130467[163] = 0.0;
   out_1001504416551130467[164] = 0.0;
   out_1001504416551130467[165] = 0.0;
   out_1001504416551130467[166] = 0.0;
   out_1001504416551130467[167] = 0.0;
   out_1001504416551130467[168] = 0.0;
   out_1001504416551130467[169] = 0.0;
   out_1001504416551130467[170] = 0.0;
   out_1001504416551130467[171] = 1.0;
   out_1001504416551130467[172] = 0.0;
   out_1001504416551130467[173] = 0.0;
   out_1001504416551130467[174] = 0.0;
   out_1001504416551130467[175] = 0.0;
   out_1001504416551130467[176] = 0.0;
   out_1001504416551130467[177] = 0.0;
   out_1001504416551130467[178] = 0.0;
   out_1001504416551130467[179] = 0.0;
   out_1001504416551130467[180] = 0.0;
   out_1001504416551130467[181] = 0.0;
   out_1001504416551130467[182] = 0.0;
   out_1001504416551130467[183] = 0.0;
   out_1001504416551130467[184] = 0.0;
   out_1001504416551130467[185] = 0.0;
   out_1001504416551130467[186] = 0.0;
   out_1001504416551130467[187] = 0.0;
   out_1001504416551130467[188] = 0.0;
   out_1001504416551130467[189] = 0.0;
   out_1001504416551130467[190] = 1.0;
   out_1001504416551130467[191] = 0.0;
   out_1001504416551130467[192] = 0.0;
   out_1001504416551130467[193] = 0.0;
   out_1001504416551130467[194] = 0.0;
   out_1001504416551130467[195] = 0.0;
   out_1001504416551130467[196] = 0.0;
   out_1001504416551130467[197] = 0.0;
   out_1001504416551130467[198] = 0.0;
   out_1001504416551130467[199] = 0.0;
   out_1001504416551130467[200] = 0.0;
   out_1001504416551130467[201] = 0.0;
   out_1001504416551130467[202] = 0.0;
   out_1001504416551130467[203] = 0.0;
   out_1001504416551130467[204] = 0.0;
   out_1001504416551130467[205] = 0.0;
   out_1001504416551130467[206] = 0.0;
   out_1001504416551130467[207] = 0.0;
   out_1001504416551130467[208] = 0.0;
   out_1001504416551130467[209] = 1.0;
   out_1001504416551130467[210] = 0.0;
   out_1001504416551130467[211] = 0.0;
   out_1001504416551130467[212] = 0.0;
   out_1001504416551130467[213] = 0.0;
   out_1001504416551130467[214] = 0.0;
   out_1001504416551130467[215] = 0.0;
   out_1001504416551130467[216] = 0.0;
   out_1001504416551130467[217] = 0.0;
   out_1001504416551130467[218] = 0.0;
   out_1001504416551130467[219] = 0.0;
   out_1001504416551130467[220] = 0.0;
   out_1001504416551130467[221] = 0.0;
   out_1001504416551130467[222] = 0.0;
   out_1001504416551130467[223] = 0.0;
   out_1001504416551130467[224] = 0.0;
   out_1001504416551130467[225] = 0.0;
   out_1001504416551130467[226] = 0.0;
   out_1001504416551130467[227] = 0.0;
   out_1001504416551130467[228] = 1.0;
   out_1001504416551130467[229] = 0.0;
   out_1001504416551130467[230] = 0.0;
   out_1001504416551130467[231] = 0.0;
   out_1001504416551130467[232] = 0.0;
   out_1001504416551130467[233] = 0.0;
   out_1001504416551130467[234] = 0.0;
   out_1001504416551130467[235] = 0.0;
   out_1001504416551130467[236] = 0.0;
   out_1001504416551130467[237] = 0.0;
   out_1001504416551130467[238] = 0.0;
   out_1001504416551130467[239] = 0.0;
   out_1001504416551130467[240] = 0.0;
   out_1001504416551130467[241] = 0.0;
   out_1001504416551130467[242] = 0.0;
   out_1001504416551130467[243] = 0.0;
   out_1001504416551130467[244] = 0.0;
   out_1001504416551130467[245] = 0.0;
   out_1001504416551130467[246] = 0.0;
   out_1001504416551130467[247] = 1.0;
   out_1001504416551130467[248] = 0.0;
   out_1001504416551130467[249] = 0.0;
   out_1001504416551130467[250] = 0.0;
   out_1001504416551130467[251] = 0.0;
   out_1001504416551130467[252] = 0.0;
   out_1001504416551130467[253] = 0.0;
   out_1001504416551130467[254] = 0.0;
   out_1001504416551130467[255] = 0.0;
   out_1001504416551130467[256] = 0.0;
   out_1001504416551130467[257] = 0.0;
   out_1001504416551130467[258] = 0.0;
   out_1001504416551130467[259] = 0.0;
   out_1001504416551130467[260] = 0.0;
   out_1001504416551130467[261] = 0.0;
   out_1001504416551130467[262] = 0.0;
   out_1001504416551130467[263] = 0.0;
   out_1001504416551130467[264] = 0.0;
   out_1001504416551130467[265] = 0.0;
   out_1001504416551130467[266] = 1.0;
   out_1001504416551130467[267] = 0.0;
   out_1001504416551130467[268] = 0.0;
   out_1001504416551130467[269] = 0.0;
   out_1001504416551130467[270] = 0.0;
   out_1001504416551130467[271] = 0.0;
   out_1001504416551130467[272] = 0.0;
   out_1001504416551130467[273] = 0.0;
   out_1001504416551130467[274] = 0.0;
   out_1001504416551130467[275] = 0.0;
   out_1001504416551130467[276] = 0.0;
   out_1001504416551130467[277] = 0.0;
   out_1001504416551130467[278] = 0.0;
   out_1001504416551130467[279] = 0.0;
   out_1001504416551130467[280] = 0.0;
   out_1001504416551130467[281] = 0.0;
   out_1001504416551130467[282] = 0.0;
   out_1001504416551130467[283] = 0.0;
   out_1001504416551130467[284] = 0.0;
   out_1001504416551130467[285] = 1.0;
   out_1001504416551130467[286] = 0.0;
   out_1001504416551130467[287] = 0.0;
   out_1001504416551130467[288] = 0.0;
   out_1001504416551130467[289] = 0.0;
   out_1001504416551130467[290] = 0.0;
   out_1001504416551130467[291] = 0.0;
   out_1001504416551130467[292] = 0.0;
   out_1001504416551130467[293] = 0.0;
   out_1001504416551130467[294] = 0.0;
   out_1001504416551130467[295] = 0.0;
   out_1001504416551130467[296] = 0.0;
   out_1001504416551130467[297] = 0.0;
   out_1001504416551130467[298] = 0.0;
   out_1001504416551130467[299] = 0.0;
   out_1001504416551130467[300] = 0.0;
   out_1001504416551130467[301] = 0.0;
   out_1001504416551130467[302] = 0.0;
   out_1001504416551130467[303] = 0.0;
   out_1001504416551130467[304] = 1.0;
   out_1001504416551130467[305] = 0.0;
   out_1001504416551130467[306] = 0.0;
   out_1001504416551130467[307] = 0.0;
   out_1001504416551130467[308] = 0.0;
   out_1001504416551130467[309] = 0.0;
   out_1001504416551130467[310] = 0.0;
   out_1001504416551130467[311] = 0.0;
   out_1001504416551130467[312] = 0.0;
   out_1001504416551130467[313] = 0.0;
   out_1001504416551130467[314] = 0.0;
   out_1001504416551130467[315] = 0.0;
   out_1001504416551130467[316] = 0.0;
   out_1001504416551130467[317] = 0.0;
   out_1001504416551130467[318] = 0.0;
   out_1001504416551130467[319] = 0.0;
   out_1001504416551130467[320] = 0.0;
   out_1001504416551130467[321] = 0.0;
   out_1001504416551130467[322] = 0.0;
   out_1001504416551130467[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_3536772200585022865) {
   out_3536772200585022865[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_3536772200585022865[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_3536772200585022865[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_3536772200585022865[3] = dt*state[12] + state[3];
   out_3536772200585022865[4] = dt*state[13] + state[4];
   out_3536772200585022865[5] = dt*state[14] + state[5];
   out_3536772200585022865[6] = state[6];
   out_3536772200585022865[7] = state[7];
   out_3536772200585022865[8] = state[8];
   out_3536772200585022865[9] = state[9];
   out_3536772200585022865[10] = state[10];
   out_3536772200585022865[11] = state[11];
   out_3536772200585022865[12] = state[12];
   out_3536772200585022865[13] = state[13];
   out_3536772200585022865[14] = state[14];
   out_3536772200585022865[15] = state[15];
   out_3536772200585022865[16] = state[16];
   out_3536772200585022865[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2853128647338935157) {
   out_2853128647338935157[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2853128647338935157[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2853128647338935157[2] = 0;
   out_2853128647338935157[3] = 0;
   out_2853128647338935157[4] = 0;
   out_2853128647338935157[5] = 0;
   out_2853128647338935157[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2853128647338935157[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2853128647338935157[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2853128647338935157[9] = 0;
   out_2853128647338935157[10] = 0;
   out_2853128647338935157[11] = 0;
   out_2853128647338935157[12] = 0;
   out_2853128647338935157[13] = 0;
   out_2853128647338935157[14] = 0;
   out_2853128647338935157[15] = 0;
   out_2853128647338935157[16] = 0;
   out_2853128647338935157[17] = 0;
   out_2853128647338935157[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2853128647338935157[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2853128647338935157[20] = 0;
   out_2853128647338935157[21] = 0;
   out_2853128647338935157[22] = 0;
   out_2853128647338935157[23] = 0;
   out_2853128647338935157[24] = 0;
   out_2853128647338935157[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2853128647338935157[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2853128647338935157[27] = 0;
   out_2853128647338935157[28] = 0;
   out_2853128647338935157[29] = 0;
   out_2853128647338935157[30] = 0;
   out_2853128647338935157[31] = 0;
   out_2853128647338935157[32] = 0;
   out_2853128647338935157[33] = 0;
   out_2853128647338935157[34] = 0;
   out_2853128647338935157[35] = 0;
   out_2853128647338935157[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2853128647338935157[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2853128647338935157[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2853128647338935157[39] = 0;
   out_2853128647338935157[40] = 0;
   out_2853128647338935157[41] = 0;
   out_2853128647338935157[42] = 0;
   out_2853128647338935157[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2853128647338935157[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2853128647338935157[45] = 0;
   out_2853128647338935157[46] = 0;
   out_2853128647338935157[47] = 0;
   out_2853128647338935157[48] = 0;
   out_2853128647338935157[49] = 0;
   out_2853128647338935157[50] = 0;
   out_2853128647338935157[51] = 0;
   out_2853128647338935157[52] = 0;
   out_2853128647338935157[53] = 0;
   out_2853128647338935157[54] = 0;
   out_2853128647338935157[55] = 0;
   out_2853128647338935157[56] = 0;
   out_2853128647338935157[57] = 1;
   out_2853128647338935157[58] = 0;
   out_2853128647338935157[59] = 0;
   out_2853128647338935157[60] = 0;
   out_2853128647338935157[61] = 0;
   out_2853128647338935157[62] = 0;
   out_2853128647338935157[63] = 0;
   out_2853128647338935157[64] = 0;
   out_2853128647338935157[65] = 0;
   out_2853128647338935157[66] = dt;
   out_2853128647338935157[67] = 0;
   out_2853128647338935157[68] = 0;
   out_2853128647338935157[69] = 0;
   out_2853128647338935157[70] = 0;
   out_2853128647338935157[71] = 0;
   out_2853128647338935157[72] = 0;
   out_2853128647338935157[73] = 0;
   out_2853128647338935157[74] = 0;
   out_2853128647338935157[75] = 0;
   out_2853128647338935157[76] = 1;
   out_2853128647338935157[77] = 0;
   out_2853128647338935157[78] = 0;
   out_2853128647338935157[79] = 0;
   out_2853128647338935157[80] = 0;
   out_2853128647338935157[81] = 0;
   out_2853128647338935157[82] = 0;
   out_2853128647338935157[83] = 0;
   out_2853128647338935157[84] = 0;
   out_2853128647338935157[85] = dt;
   out_2853128647338935157[86] = 0;
   out_2853128647338935157[87] = 0;
   out_2853128647338935157[88] = 0;
   out_2853128647338935157[89] = 0;
   out_2853128647338935157[90] = 0;
   out_2853128647338935157[91] = 0;
   out_2853128647338935157[92] = 0;
   out_2853128647338935157[93] = 0;
   out_2853128647338935157[94] = 0;
   out_2853128647338935157[95] = 1;
   out_2853128647338935157[96] = 0;
   out_2853128647338935157[97] = 0;
   out_2853128647338935157[98] = 0;
   out_2853128647338935157[99] = 0;
   out_2853128647338935157[100] = 0;
   out_2853128647338935157[101] = 0;
   out_2853128647338935157[102] = 0;
   out_2853128647338935157[103] = 0;
   out_2853128647338935157[104] = dt;
   out_2853128647338935157[105] = 0;
   out_2853128647338935157[106] = 0;
   out_2853128647338935157[107] = 0;
   out_2853128647338935157[108] = 0;
   out_2853128647338935157[109] = 0;
   out_2853128647338935157[110] = 0;
   out_2853128647338935157[111] = 0;
   out_2853128647338935157[112] = 0;
   out_2853128647338935157[113] = 0;
   out_2853128647338935157[114] = 1;
   out_2853128647338935157[115] = 0;
   out_2853128647338935157[116] = 0;
   out_2853128647338935157[117] = 0;
   out_2853128647338935157[118] = 0;
   out_2853128647338935157[119] = 0;
   out_2853128647338935157[120] = 0;
   out_2853128647338935157[121] = 0;
   out_2853128647338935157[122] = 0;
   out_2853128647338935157[123] = 0;
   out_2853128647338935157[124] = 0;
   out_2853128647338935157[125] = 0;
   out_2853128647338935157[126] = 0;
   out_2853128647338935157[127] = 0;
   out_2853128647338935157[128] = 0;
   out_2853128647338935157[129] = 0;
   out_2853128647338935157[130] = 0;
   out_2853128647338935157[131] = 0;
   out_2853128647338935157[132] = 0;
   out_2853128647338935157[133] = 1;
   out_2853128647338935157[134] = 0;
   out_2853128647338935157[135] = 0;
   out_2853128647338935157[136] = 0;
   out_2853128647338935157[137] = 0;
   out_2853128647338935157[138] = 0;
   out_2853128647338935157[139] = 0;
   out_2853128647338935157[140] = 0;
   out_2853128647338935157[141] = 0;
   out_2853128647338935157[142] = 0;
   out_2853128647338935157[143] = 0;
   out_2853128647338935157[144] = 0;
   out_2853128647338935157[145] = 0;
   out_2853128647338935157[146] = 0;
   out_2853128647338935157[147] = 0;
   out_2853128647338935157[148] = 0;
   out_2853128647338935157[149] = 0;
   out_2853128647338935157[150] = 0;
   out_2853128647338935157[151] = 0;
   out_2853128647338935157[152] = 1;
   out_2853128647338935157[153] = 0;
   out_2853128647338935157[154] = 0;
   out_2853128647338935157[155] = 0;
   out_2853128647338935157[156] = 0;
   out_2853128647338935157[157] = 0;
   out_2853128647338935157[158] = 0;
   out_2853128647338935157[159] = 0;
   out_2853128647338935157[160] = 0;
   out_2853128647338935157[161] = 0;
   out_2853128647338935157[162] = 0;
   out_2853128647338935157[163] = 0;
   out_2853128647338935157[164] = 0;
   out_2853128647338935157[165] = 0;
   out_2853128647338935157[166] = 0;
   out_2853128647338935157[167] = 0;
   out_2853128647338935157[168] = 0;
   out_2853128647338935157[169] = 0;
   out_2853128647338935157[170] = 0;
   out_2853128647338935157[171] = 1;
   out_2853128647338935157[172] = 0;
   out_2853128647338935157[173] = 0;
   out_2853128647338935157[174] = 0;
   out_2853128647338935157[175] = 0;
   out_2853128647338935157[176] = 0;
   out_2853128647338935157[177] = 0;
   out_2853128647338935157[178] = 0;
   out_2853128647338935157[179] = 0;
   out_2853128647338935157[180] = 0;
   out_2853128647338935157[181] = 0;
   out_2853128647338935157[182] = 0;
   out_2853128647338935157[183] = 0;
   out_2853128647338935157[184] = 0;
   out_2853128647338935157[185] = 0;
   out_2853128647338935157[186] = 0;
   out_2853128647338935157[187] = 0;
   out_2853128647338935157[188] = 0;
   out_2853128647338935157[189] = 0;
   out_2853128647338935157[190] = 1;
   out_2853128647338935157[191] = 0;
   out_2853128647338935157[192] = 0;
   out_2853128647338935157[193] = 0;
   out_2853128647338935157[194] = 0;
   out_2853128647338935157[195] = 0;
   out_2853128647338935157[196] = 0;
   out_2853128647338935157[197] = 0;
   out_2853128647338935157[198] = 0;
   out_2853128647338935157[199] = 0;
   out_2853128647338935157[200] = 0;
   out_2853128647338935157[201] = 0;
   out_2853128647338935157[202] = 0;
   out_2853128647338935157[203] = 0;
   out_2853128647338935157[204] = 0;
   out_2853128647338935157[205] = 0;
   out_2853128647338935157[206] = 0;
   out_2853128647338935157[207] = 0;
   out_2853128647338935157[208] = 0;
   out_2853128647338935157[209] = 1;
   out_2853128647338935157[210] = 0;
   out_2853128647338935157[211] = 0;
   out_2853128647338935157[212] = 0;
   out_2853128647338935157[213] = 0;
   out_2853128647338935157[214] = 0;
   out_2853128647338935157[215] = 0;
   out_2853128647338935157[216] = 0;
   out_2853128647338935157[217] = 0;
   out_2853128647338935157[218] = 0;
   out_2853128647338935157[219] = 0;
   out_2853128647338935157[220] = 0;
   out_2853128647338935157[221] = 0;
   out_2853128647338935157[222] = 0;
   out_2853128647338935157[223] = 0;
   out_2853128647338935157[224] = 0;
   out_2853128647338935157[225] = 0;
   out_2853128647338935157[226] = 0;
   out_2853128647338935157[227] = 0;
   out_2853128647338935157[228] = 1;
   out_2853128647338935157[229] = 0;
   out_2853128647338935157[230] = 0;
   out_2853128647338935157[231] = 0;
   out_2853128647338935157[232] = 0;
   out_2853128647338935157[233] = 0;
   out_2853128647338935157[234] = 0;
   out_2853128647338935157[235] = 0;
   out_2853128647338935157[236] = 0;
   out_2853128647338935157[237] = 0;
   out_2853128647338935157[238] = 0;
   out_2853128647338935157[239] = 0;
   out_2853128647338935157[240] = 0;
   out_2853128647338935157[241] = 0;
   out_2853128647338935157[242] = 0;
   out_2853128647338935157[243] = 0;
   out_2853128647338935157[244] = 0;
   out_2853128647338935157[245] = 0;
   out_2853128647338935157[246] = 0;
   out_2853128647338935157[247] = 1;
   out_2853128647338935157[248] = 0;
   out_2853128647338935157[249] = 0;
   out_2853128647338935157[250] = 0;
   out_2853128647338935157[251] = 0;
   out_2853128647338935157[252] = 0;
   out_2853128647338935157[253] = 0;
   out_2853128647338935157[254] = 0;
   out_2853128647338935157[255] = 0;
   out_2853128647338935157[256] = 0;
   out_2853128647338935157[257] = 0;
   out_2853128647338935157[258] = 0;
   out_2853128647338935157[259] = 0;
   out_2853128647338935157[260] = 0;
   out_2853128647338935157[261] = 0;
   out_2853128647338935157[262] = 0;
   out_2853128647338935157[263] = 0;
   out_2853128647338935157[264] = 0;
   out_2853128647338935157[265] = 0;
   out_2853128647338935157[266] = 1;
   out_2853128647338935157[267] = 0;
   out_2853128647338935157[268] = 0;
   out_2853128647338935157[269] = 0;
   out_2853128647338935157[270] = 0;
   out_2853128647338935157[271] = 0;
   out_2853128647338935157[272] = 0;
   out_2853128647338935157[273] = 0;
   out_2853128647338935157[274] = 0;
   out_2853128647338935157[275] = 0;
   out_2853128647338935157[276] = 0;
   out_2853128647338935157[277] = 0;
   out_2853128647338935157[278] = 0;
   out_2853128647338935157[279] = 0;
   out_2853128647338935157[280] = 0;
   out_2853128647338935157[281] = 0;
   out_2853128647338935157[282] = 0;
   out_2853128647338935157[283] = 0;
   out_2853128647338935157[284] = 0;
   out_2853128647338935157[285] = 1;
   out_2853128647338935157[286] = 0;
   out_2853128647338935157[287] = 0;
   out_2853128647338935157[288] = 0;
   out_2853128647338935157[289] = 0;
   out_2853128647338935157[290] = 0;
   out_2853128647338935157[291] = 0;
   out_2853128647338935157[292] = 0;
   out_2853128647338935157[293] = 0;
   out_2853128647338935157[294] = 0;
   out_2853128647338935157[295] = 0;
   out_2853128647338935157[296] = 0;
   out_2853128647338935157[297] = 0;
   out_2853128647338935157[298] = 0;
   out_2853128647338935157[299] = 0;
   out_2853128647338935157[300] = 0;
   out_2853128647338935157[301] = 0;
   out_2853128647338935157[302] = 0;
   out_2853128647338935157[303] = 0;
   out_2853128647338935157[304] = 1;
   out_2853128647338935157[305] = 0;
   out_2853128647338935157[306] = 0;
   out_2853128647338935157[307] = 0;
   out_2853128647338935157[308] = 0;
   out_2853128647338935157[309] = 0;
   out_2853128647338935157[310] = 0;
   out_2853128647338935157[311] = 0;
   out_2853128647338935157[312] = 0;
   out_2853128647338935157[313] = 0;
   out_2853128647338935157[314] = 0;
   out_2853128647338935157[315] = 0;
   out_2853128647338935157[316] = 0;
   out_2853128647338935157[317] = 0;
   out_2853128647338935157[318] = 0;
   out_2853128647338935157[319] = 0;
   out_2853128647338935157[320] = 0;
   out_2853128647338935157[321] = 0;
   out_2853128647338935157[322] = 0;
   out_2853128647338935157[323] = 1;
}
void h_4(double *state, double *unused, double *out_2776811696301497625) {
   out_2776811696301497625[0] = state[6] + state[9];
   out_2776811696301497625[1] = state[7] + state[10];
   out_2776811696301497625[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_4846577390200828254) {
   out_4846577390200828254[0] = 0;
   out_4846577390200828254[1] = 0;
   out_4846577390200828254[2] = 0;
   out_4846577390200828254[3] = 0;
   out_4846577390200828254[4] = 0;
   out_4846577390200828254[5] = 0;
   out_4846577390200828254[6] = 1;
   out_4846577390200828254[7] = 0;
   out_4846577390200828254[8] = 0;
   out_4846577390200828254[9] = 1;
   out_4846577390200828254[10] = 0;
   out_4846577390200828254[11] = 0;
   out_4846577390200828254[12] = 0;
   out_4846577390200828254[13] = 0;
   out_4846577390200828254[14] = 0;
   out_4846577390200828254[15] = 0;
   out_4846577390200828254[16] = 0;
   out_4846577390200828254[17] = 0;
   out_4846577390200828254[18] = 0;
   out_4846577390200828254[19] = 0;
   out_4846577390200828254[20] = 0;
   out_4846577390200828254[21] = 0;
   out_4846577390200828254[22] = 0;
   out_4846577390200828254[23] = 0;
   out_4846577390200828254[24] = 0;
   out_4846577390200828254[25] = 1;
   out_4846577390200828254[26] = 0;
   out_4846577390200828254[27] = 0;
   out_4846577390200828254[28] = 1;
   out_4846577390200828254[29] = 0;
   out_4846577390200828254[30] = 0;
   out_4846577390200828254[31] = 0;
   out_4846577390200828254[32] = 0;
   out_4846577390200828254[33] = 0;
   out_4846577390200828254[34] = 0;
   out_4846577390200828254[35] = 0;
   out_4846577390200828254[36] = 0;
   out_4846577390200828254[37] = 0;
   out_4846577390200828254[38] = 0;
   out_4846577390200828254[39] = 0;
   out_4846577390200828254[40] = 0;
   out_4846577390200828254[41] = 0;
   out_4846577390200828254[42] = 0;
   out_4846577390200828254[43] = 0;
   out_4846577390200828254[44] = 1;
   out_4846577390200828254[45] = 0;
   out_4846577390200828254[46] = 0;
   out_4846577390200828254[47] = 1;
   out_4846577390200828254[48] = 0;
   out_4846577390200828254[49] = 0;
   out_4846577390200828254[50] = 0;
   out_4846577390200828254[51] = 0;
   out_4846577390200828254[52] = 0;
   out_4846577390200828254[53] = 0;
}
void h_10(double *state, double *unused, double *out_8680472392571157658) {
   out_8680472392571157658[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8680472392571157658[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8680472392571157658[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_2994919368328266305) {
   out_2994919368328266305[0] = 0;
   out_2994919368328266305[1] = 9.8100000000000005*cos(state[1]);
   out_2994919368328266305[2] = 0;
   out_2994919368328266305[3] = 0;
   out_2994919368328266305[4] = -state[8];
   out_2994919368328266305[5] = state[7];
   out_2994919368328266305[6] = 0;
   out_2994919368328266305[7] = state[5];
   out_2994919368328266305[8] = -state[4];
   out_2994919368328266305[9] = 0;
   out_2994919368328266305[10] = 0;
   out_2994919368328266305[11] = 0;
   out_2994919368328266305[12] = 1;
   out_2994919368328266305[13] = 0;
   out_2994919368328266305[14] = 0;
   out_2994919368328266305[15] = 1;
   out_2994919368328266305[16] = 0;
   out_2994919368328266305[17] = 0;
   out_2994919368328266305[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_2994919368328266305[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_2994919368328266305[20] = 0;
   out_2994919368328266305[21] = state[8];
   out_2994919368328266305[22] = 0;
   out_2994919368328266305[23] = -state[6];
   out_2994919368328266305[24] = -state[5];
   out_2994919368328266305[25] = 0;
   out_2994919368328266305[26] = state[3];
   out_2994919368328266305[27] = 0;
   out_2994919368328266305[28] = 0;
   out_2994919368328266305[29] = 0;
   out_2994919368328266305[30] = 0;
   out_2994919368328266305[31] = 1;
   out_2994919368328266305[32] = 0;
   out_2994919368328266305[33] = 0;
   out_2994919368328266305[34] = 1;
   out_2994919368328266305[35] = 0;
   out_2994919368328266305[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_2994919368328266305[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_2994919368328266305[38] = 0;
   out_2994919368328266305[39] = -state[7];
   out_2994919368328266305[40] = state[6];
   out_2994919368328266305[41] = 0;
   out_2994919368328266305[42] = state[4];
   out_2994919368328266305[43] = -state[3];
   out_2994919368328266305[44] = 0;
   out_2994919368328266305[45] = 0;
   out_2994919368328266305[46] = 0;
   out_2994919368328266305[47] = 0;
   out_2994919368328266305[48] = 0;
   out_2994919368328266305[49] = 0;
   out_2994919368328266305[50] = 1;
   out_2994919368328266305[51] = 0;
   out_2994919368328266305[52] = 0;
   out_2994919368328266305[53] = 1;
}
void h_13(double *state, double *unused, double *out_8479283368027075574) {
   out_8479283368027075574[0] = state[3];
   out_8479283368027075574[1] = state[4];
   out_8479283368027075574[2] = state[5];
}
void H_13(double *state, double *unused, double *out_1634303564868495453) {
   out_1634303564868495453[0] = 0;
   out_1634303564868495453[1] = 0;
   out_1634303564868495453[2] = 0;
   out_1634303564868495453[3] = 1;
   out_1634303564868495453[4] = 0;
   out_1634303564868495453[5] = 0;
   out_1634303564868495453[6] = 0;
   out_1634303564868495453[7] = 0;
   out_1634303564868495453[8] = 0;
   out_1634303564868495453[9] = 0;
   out_1634303564868495453[10] = 0;
   out_1634303564868495453[11] = 0;
   out_1634303564868495453[12] = 0;
   out_1634303564868495453[13] = 0;
   out_1634303564868495453[14] = 0;
   out_1634303564868495453[15] = 0;
   out_1634303564868495453[16] = 0;
   out_1634303564868495453[17] = 0;
   out_1634303564868495453[18] = 0;
   out_1634303564868495453[19] = 0;
   out_1634303564868495453[20] = 0;
   out_1634303564868495453[21] = 0;
   out_1634303564868495453[22] = 1;
   out_1634303564868495453[23] = 0;
   out_1634303564868495453[24] = 0;
   out_1634303564868495453[25] = 0;
   out_1634303564868495453[26] = 0;
   out_1634303564868495453[27] = 0;
   out_1634303564868495453[28] = 0;
   out_1634303564868495453[29] = 0;
   out_1634303564868495453[30] = 0;
   out_1634303564868495453[31] = 0;
   out_1634303564868495453[32] = 0;
   out_1634303564868495453[33] = 0;
   out_1634303564868495453[34] = 0;
   out_1634303564868495453[35] = 0;
   out_1634303564868495453[36] = 0;
   out_1634303564868495453[37] = 0;
   out_1634303564868495453[38] = 0;
   out_1634303564868495453[39] = 0;
   out_1634303564868495453[40] = 0;
   out_1634303564868495453[41] = 1;
   out_1634303564868495453[42] = 0;
   out_1634303564868495453[43] = 0;
   out_1634303564868495453[44] = 0;
   out_1634303564868495453[45] = 0;
   out_1634303564868495453[46] = 0;
   out_1634303564868495453[47] = 0;
   out_1634303564868495453[48] = 0;
   out_1634303564868495453[49] = 0;
   out_1634303564868495453[50] = 0;
   out_1634303564868495453[51] = 0;
   out_1634303564868495453[52] = 0;
   out_1634303564868495453[53] = 0;
}
void h_14(double *state, double *unused, double *out_6479428408185621571) {
   out_6479428408185621571[0] = state[6];
   out_6479428408185621571[1] = state[7];
   out_6479428408185621571[2] = state[8];
}
void H_14(double *state, double *unused, double *out_883336533861343725) {
   out_883336533861343725[0] = 0;
   out_883336533861343725[1] = 0;
   out_883336533861343725[2] = 0;
   out_883336533861343725[3] = 0;
   out_883336533861343725[4] = 0;
   out_883336533861343725[5] = 0;
   out_883336533861343725[6] = 1;
   out_883336533861343725[7] = 0;
   out_883336533861343725[8] = 0;
   out_883336533861343725[9] = 0;
   out_883336533861343725[10] = 0;
   out_883336533861343725[11] = 0;
   out_883336533861343725[12] = 0;
   out_883336533861343725[13] = 0;
   out_883336533861343725[14] = 0;
   out_883336533861343725[15] = 0;
   out_883336533861343725[16] = 0;
   out_883336533861343725[17] = 0;
   out_883336533861343725[18] = 0;
   out_883336533861343725[19] = 0;
   out_883336533861343725[20] = 0;
   out_883336533861343725[21] = 0;
   out_883336533861343725[22] = 0;
   out_883336533861343725[23] = 0;
   out_883336533861343725[24] = 0;
   out_883336533861343725[25] = 1;
   out_883336533861343725[26] = 0;
   out_883336533861343725[27] = 0;
   out_883336533861343725[28] = 0;
   out_883336533861343725[29] = 0;
   out_883336533861343725[30] = 0;
   out_883336533861343725[31] = 0;
   out_883336533861343725[32] = 0;
   out_883336533861343725[33] = 0;
   out_883336533861343725[34] = 0;
   out_883336533861343725[35] = 0;
   out_883336533861343725[36] = 0;
   out_883336533861343725[37] = 0;
   out_883336533861343725[38] = 0;
   out_883336533861343725[39] = 0;
   out_883336533861343725[40] = 0;
   out_883336533861343725[41] = 0;
   out_883336533861343725[42] = 0;
   out_883336533861343725[43] = 0;
   out_883336533861343725[44] = 1;
   out_883336533861343725[45] = 0;
   out_883336533861343725[46] = 0;
   out_883336533861343725[47] = 0;
   out_883336533861343725[48] = 0;
   out_883336533861343725[49] = 0;
   out_883336533861343725[50] = 0;
   out_883336533861343725[51] = 0;
   out_883336533861343725[52] = 0;
   out_883336533861343725[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_2896449614613514894) {
  err_fun(nom_x, delta_x, out_2896449614613514894);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7848261021210579995) {
  inv_err_fun(nom_x, true_x, out_7848261021210579995);
}
void pose_H_mod_fun(double *state, double *out_1001504416551130467) {
  H_mod_fun(state, out_1001504416551130467);
}
void pose_f_fun(double *state, double dt, double *out_3536772200585022865) {
  f_fun(state,  dt, out_3536772200585022865);
}
void pose_F_fun(double *state, double dt, double *out_2853128647338935157) {
  F_fun(state,  dt, out_2853128647338935157);
}
void pose_h_4(double *state, double *unused, double *out_2776811696301497625) {
  h_4(state, unused, out_2776811696301497625);
}
void pose_H_4(double *state, double *unused, double *out_4846577390200828254) {
  H_4(state, unused, out_4846577390200828254);
}
void pose_h_10(double *state, double *unused, double *out_8680472392571157658) {
  h_10(state, unused, out_8680472392571157658);
}
void pose_H_10(double *state, double *unused, double *out_2994919368328266305) {
  H_10(state, unused, out_2994919368328266305);
}
void pose_h_13(double *state, double *unused, double *out_8479283368027075574) {
  h_13(state, unused, out_8479283368027075574);
}
void pose_H_13(double *state, double *unused, double *out_1634303564868495453) {
  H_13(state, unused, out_1634303564868495453);
}
void pose_h_14(double *state, double *unused, double *out_6479428408185621571) {
  h_14(state, unused, out_6479428408185621571);
}
void pose_H_14(double *state, double *unused, double *out_883336533861343725) {
  H_14(state, unused, out_883336533861343725);
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
