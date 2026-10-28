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
void err_fun(double *nom_x, double *delta_x, double *out_1694318774695559648) {
   out_1694318774695559648[0] = delta_x[0] + nom_x[0];
   out_1694318774695559648[1] = delta_x[1] + nom_x[1];
   out_1694318774695559648[2] = delta_x[2] + nom_x[2];
   out_1694318774695559648[3] = delta_x[3] + nom_x[3];
   out_1694318774695559648[4] = delta_x[4] + nom_x[4];
   out_1694318774695559648[5] = delta_x[5] + nom_x[5];
   out_1694318774695559648[6] = delta_x[6] + nom_x[6];
   out_1694318774695559648[7] = delta_x[7] + nom_x[7];
   out_1694318774695559648[8] = delta_x[8] + nom_x[8];
   out_1694318774695559648[9] = delta_x[9] + nom_x[9];
   out_1694318774695559648[10] = delta_x[10] + nom_x[10];
   out_1694318774695559648[11] = delta_x[11] + nom_x[11];
   out_1694318774695559648[12] = delta_x[12] + nom_x[12];
   out_1694318774695559648[13] = delta_x[13] + nom_x[13];
   out_1694318774695559648[14] = delta_x[14] + nom_x[14];
   out_1694318774695559648[15] = delta_x[15] + nom_x[15];
   out_1694318774695559648[16] = delta_x[16] + nom_x[16];
   out_1694318774695559648[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1527863431850837134) {
   out_1527863431850837134[0] = -nom_x[0] + true_x[0];
   out_1527863431850837134[1] = -nom_x[1] + true_x[1];
   out_1527863431850837134[2] = -nom_x[2] + true_x[2];
   out_1527863431850837134[3] = -nom_x[3] + true_x[3];
   out_1527863431850837134[4] = -nom_x[4] + true_x[4];
   out_1527863431850837134[5] = -nom_x[5] + true_x[5];
   out_1527863431850837134[6] = -nom_x[6] + true_x[6];
   out_1527863431850837134[7] = -nom_x[7] + true_x[7];
   out_1527863431850837134[8] = -nom_x[8] + true_x[8];
   out_1527863431850837134[9] = -nom_x[9] + true_x[9];
   out_1527863431850837134[10] = -nom_x[10] + true_x[10];
   out_1527863431850837134[11] = -nom_x[11] + true_x[11];
   out_1527863431850837134[12] = -nom_x[12] + true_x[12];
   out_1527863431850837134[13] = -nom_x[13] + true_x[13];
   out_1527863431850837134[14] = -nom_x[14] + true_x[14];
   out_1527863431850837134[15] = -nom_x[15] + true_x[15];
   out_1527863431850837134[16] = -nom_x[16] + true_x[16];
   out_1527863431850837134[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_2459924378456607327) {
   out_2459924378456607327[0] = 1.0;
   out_2459924378456607327[1] = 0.0;
   out_2459924378456607327[2] = 0.0;
   out_2459924378456607327[3] = 0.0;
   out_2459924378456607327[4] = 0.0;
   out_2459924378456607327[5] = 0.0;
   out_2459924378456607327[6] = 0.0;
   out_2459924378456607327[7] = 0.0;
   out_2459924378456607327[8] = 0.0;
   out_2459924378456607327[9] = 0.0;
   out_2459924378456607327[10] = 0.0;
   out_2459924378456607327[11] = 0.0;
   out_2459924378456607327[12] = 0.0;
   out_2459924378456607327[13] = 0.0;
   out_2459924378456607327[14] = 0.0;
   out_2459924378456607327[15] = 0.0;
   out_2459924378456607327[16] = 0.0;
   out_2459924378456607327[17] = 0.0;
   out_2459924378456607327[18] = 0.0;
   out_2459924378456607327[19] = 1.0;
   out_2459924378456607327[20] = 0.0;
   out_2459924378456607327[21] = 0.0;
   out_2459924378456607327[22] = 0.0;
   out_2459924378456607327[23] = 0.0;
   out_2459924378456607327[24] = 0.0;
   out_2459924378456607327[25] = 0.0;
   out_2459924378456607327[26] = 0.0;
   out_2459924378456607327[27] = 0.0;
   out_2459924378456607327[28] = 0.0;
   out_2459924378456607327[29] = 0.0;
   out_2459924378456607327[30] = 0.0;
   out_2459924378456607327[31] = 0.0;
   out_2459924378456607327[32] = 0.0;
   out_2459924378456607327[33] = 0.0;
   out_2459924378456607327[34] = 0.0;
   out_2459924378456607327[35] = 0.0;
   out_2459924378456607327[36] = 0.0;
   out_2459924378456607327[37] = 0.0;
   out_2459924378456607327[38] = 1.0;
   out_2459924378456607327[39] = 0.0;
   out_2459924378456607327[40] = 0.0;
   out_2459924378456607327[41] = 0.0;
   out_2459924378456607327[42] = 0.0;
   out_2459924378456607327[43] = 0.0;
   out_2459924378456607327[44] = 0.0;
   out_2459924378456607327[45] = 0.0;
   out_2459924378456607327[46] = 0.0;
   out_2459924378456607327[47] = 0.0;
   out_2459924378456607327[48] = 0.0;
   out_2459924378456607327[49] = 0.0;
   out_2459924378456607327[50] = 0.0;
   out_2459924378456607327[51] = 0.0;
   out_2459924378456607327[52] = 0.0;
   out_2459924378456607327[53] = 0.0;
   out_2459924378456607327[54] = 0.0;
   out_2459924378456607327[55] = 0.0;
   out_2459924378456607327[56] = 0.0;
   out_2459924378456607327[57] = 1.0;
   out_2459924378456607327[58] = 0.0;
   out_2459924378456607327[59] = 0.0;
   out_2459924378456607327[60] = 0.0;
   out_2459924378456607327[61] = 0.0;
   out_2459924378456607327[62] = 0.0;
   out_2459924378456607327[63] = 0.0;
   out_2459924378456607327[64] = 0.0;
   out_2459924378456607327[65] = 0.0;
   out_2459924378456607327[66] = 0.0;
   out_2459924378456607327[67] = 0.0;
   out_2459924378456607327[68] = 0.0;
   out_2459924378456607327[69] = 0.0;
   out_2459924378456607327[70] = 0.0;
   out_2459924378456607327[71] = 0.0;
   out_2459924378456607327[72] = 0.0;
   out_2459924378456607327[73] = 0.0;
   out_2459924378456607327[74] = 0.0;
   out_2459924378456607327[75] = 0.0;
   out_2459924378456607327[76] = 1.0;
   out_2459924378456607327[77] = 0.0;
   out_2459924378456607327[78] = 0.0;
   out_2459924378456607327[79] = 0.0;
   out_2459924378456607327[80] = 0.0;
   out_2459924378456607327[81] = 0.0;
   out_2459924378456607327[82] = 0.0;
   out_2459924378456607327[83] = 0.0;
   out_2459924378456607327[84] = 0.0;
   out_2459924378456607327[85] = 0.0;
   out_2459924378456607327[86] = 0.0;
   out_2459924378456607327[87] = 0.0;
   out_2459924378456607327[88] = 0.0;
   out_2459924378456607327[89] = 0.0;
   out_2459924378456607327[90] = 0.0;
   out_2459924378456607327[91] = 0.0;
   out_2459924378456607327[92] = 0.0;
   out_2459924378456607327[93] = 0.0;
   out_2459924378456607327[94] = 0.0;
   out_2459924378456607327[95] = 1.0;
   out_2459924378456607327[96] = 0.0;
   out_2459924378456607327[97] = 0.0;
   out_2459924378456607327[98] = 0.0;
   out_2459924378456607327[99] = 0.0;
   out_2459924378456607327[100] = 0.0;
   out_2459924378456607327[101] = 0.0;
   out_2459924378456607327[102] = 0.0;
   out_2459924378456607327[103] = 0.0;
   out_2459924378456607327[104] = 0.0;
   out_2459924378456607327[105] = 0.0;
   out_2459924378456607327[106] = 0.0;
   out_2459924378456607327[107] = 0.0;
   out_2459924378456607327[108] = 0.0;
   out_2459924378456607327[109] = 0.0;
   out_2459924378456607327[110] = 0.0;
   out_2459924378456607327[111] = 0.0;
   out_2459924378456607327[112] = 0.0;
   out_2459924378456607327[113] = 0.0;
   out_2459924378456607327[114] = 1.0;
   out_2459924378456607327[115] = 0.0;
   out_2459924378456607327[116] = 0.0;
   out_2459924378456607327[117] = 0.0;
   out_2459924378456607327[118] = 0.0;
   out_2459924378456607327[119] = 0.0;
   out_2459924378456607327[120] = 0.0;
   out_2459924378456607327[121] = 0.0;
   out_2459924378456607327[122] = 0.0;
   out_2459924378456607327[123] = 0.0;
   out_2459924378456607327[124] = 0.0;
   out_2459924378456607327[125] = 0.0;
   out_2459924378456607327[126] = 0.0;
   out_2459924378456607327[127] = 0.0;
   out_2459924378456607327[128] = 0.0;
   out_2459924378456607327[129] = 0.0;
   out_2459924378456607327[130] = 0.0;
   out_2459924378456607327[131] = 0.0;
   out_2459924378456607327[132] = 0.0;
   out_2459924378456607327[133] = 1.0;
   out_2459924378456607327[134] = 0.0;
   out_2459924378456607327[135] = 0.0;
   out_2459924378456607327[136] = 0.0;
   out_2459924378456607327[137] = 0.0;
   out_2459924378456607327[138] = 0.0;
   out_2459924378456607327[139] = 0.0;
   out_2459924378456607327[140] = 0.0;
   out_2459924378456607327[141] = 0.0;
   out_2459924378456607327[142] = 0.0;
   out_2459924378456607327[143] = 0.0;
   out_2459924378456607327[144] = 0.0;
   out_2459924378456607327[145] = 0.0;
   out_2459924378456607327[146] = 0.0;
   out_2459924378456607327[147] = 0.0;
   out_2459924378456607327[148] = 0.0;
   out_2459924378456607327[149] = 0.0;
   out_2459924378456607327[150] = 0.0;
   out_2459924378456607327[151] = 0.0;
   out_2459924378456607327[152] = 1.0;
   out_2459924378456607327[153] = 0.0;
   out_2459924378456607327[154] = 0.0;
   out_2459924378456607327[155] = 0.0;
   out_2459924378456607327[156] = 0.0;
   out_2459924378456607327[157] = 0.0;
   out_2459924378456607327[158] = 0.0;
   out_2459924378456607327[159] = 0.0;
   out_2459924378456607327[160] = 0.0;
   out_2459924378456607327[161] = 0.0;
   out_2459924378456607327[162] = 0.0;
   out_2459924378456607327[163] = 0.0;
   out_2459924378456607327[164] = 0.0;
   out_2459924378456607327[165] = 0.0;
   out_2459924378456607327[166] = 0.0;
   out_2459924378456607327[167] = 0.0;
   out_2459924378456607327[168] = 0.0;
   out_2459924378456607327[169] = 0.0;
   out_2459924378456607327[170] = 0.0;
   out_2459924378456607327[171] = 1.0;
   out_2459924378456607327[172] = 0.0;
   out_2459924378456607327[173] = 0.0;
   out_2459924378456607327[174] = 0.0;
   out_2459924378456607327[175] = 0.0;
   out_2459924378456607327[176] = 0.0;
   out_2459924378456607327[177] = 0.0;
   out_2459924378456607327[178] = 0.0;
   out_2459924378456607327[179] = 0.0;
   out_2459924378456607327[180] = 0.0;
   out_2459924378456607327[181] = 0.0;
   out_2459924378456607327[182] = 0.0;
   out_2459924378456607327[183] = 0.0;
   out_2459924378456607327[184] = 0.0;
   out_2459924378456607327[185] = 0.0;
   out_2459924378456607327[186] = 0.0;
   out_2459924378456607327[187] = 0.0;
   out_2459924378456607327[188] = 0.0;
   out_2459924378456607327[189] = 0.0;
   out_2459924378456607327[190] = 1.0;
   out_2459924378456607327[191] = 0.0;
   out_2459924378456607327[192] = 0.0;
   out_2459924378456607327[193] = 0.0;
   out_2459924378456607327[194] = 0.0;
   out_2459924378456607327[195] = 0.0;
   out_2459924378456607327[196] = 0.0;
   out_2459924378456607327[197] = 0.0;
   out_2459924378456607327[198] = 0.0;
   out_2459924378456607327[199] = 0.0;
   out_2459924378456607327[200] = 0.0;
   out_2459924378456607327[201] = 0.0;
   out_2459924378456607327[202] = 0.0;
   out_2459924378456607327[203] = 0.0;
   out_2459924378456607327[204] = 0.0;
   out_2459924378456607327[205] = 0.0;
   out_2459924378456607327[206] = 0.0;
   out_2459924378456607327[207] = 0.0;
   out_2459924378456607327[208] = 0.0;
   out_2459924378456607327[209] = 1.0;
   out_2459924378456607327[210] = 0.0;
   out_2459924378456607327[211] = 0.0;
   out_2459924378456607327[212] = 0.0;
   out_2459924378456607327[213] = 0.0;
   out_2459924378456607327[214] = 0.0;
   out_2459924378456607327[215] = 0.0;
   out_2459924378456607327[216] = 0.0;
   out_2459924378456607327[217] = 0.0;
   out_2459924378456607327[218] = 0.0;
   out_2459924378456607327[219] = 0.0;
   out_2459924378456607327[220] = 0.0;
   out_2459924378456607327[221] = 0.0;
   out_2459924378456607327[222] = 0.0;
   out_2459924378456607327[223] = 0.0;
   out_2459924378456607327[224] = 0.0;
   out_2459924378456607327[225] = 0.0;
   out_2459924378456607327[226] = 0.0;
   out_2459924378456607327[227] = 0.0;
   out_2459924378456607327[228] = 1.0;
   out_2459924378456607327[229] = 0.0;
   out_2459924378456607327[230] = 0.0;
   out_2459924378456607327[231] = 0.0;
   out_2459924378456607327[232] = 0.0;
   out_2459924378456607327[233] = 0.0;
   out_2459924378456607327[234] = 0.0;
   out_2459924378456607327[235] = 0.0;
   out_2459924378456607327[236] = 0.0;
   out_2459924378456607327[237] = 0.0;
   out_2459924378456607327[238] = 0.0;
   out_2459924378456607327[239] = 0.0;
   out_2459924378456607327[240] = 0.0;
   out_2459924378456607327[241] = 0.0;
   out_2459924378456607327[242] = 0.0;
   out_2459924378456607327[243] = 0.0;
   out_2459924378456607327[244] = 0.0;
   out_2459924378456607327[245] = 0.0;
   out_2459924378456607327[246] = 0.0;
   out_2459924378456607327[247] = 1.0;
   out_2459924378456607327[248] = 0.0;
   out_2459924378456607327[249] = 0.0;
   out_2459924378456607327[250] = 0.0;
   out_2459924378456607327[251] = 0.0;
   out_2459924378456607327[252] = 0.0;
   out_2459924378456607327[253] = 0.0;
   out_2459924378456607327[254] = 0.0;
   out_2459924378456607327[255] = 0.0;
   out_2459924378456607327[256] = 0.0;
   out_2459924378456607327[257] = 0.0;
   out_2459924378456607327[258] = 0.0;
   out_2459924378456607327[259] = 0.0;
   out_2459924378456607327[260] = 0.0;
   out_2459924378456607327[261] = 0.0;
   out_2459924378456607327[262] = 0.0;
   out_2459924378456607327[263] = 0.0;
   out_2459924378456607327[264] = 0.0;
   out_2459924378456607327[265] = 0.0;
   out_2459924378456607327[266] = 1.0;
   out_2459924378456607327[267] = 0.0;
   out_2459924378456607327[268] = 0.0;
   out_2459924378456607327[269] = 0.0;
   out_2459924378456607327[270] = 0.0;
   out_2459924378456607327[271] = 0.0;
   out_2459924378456607327[272] = 0.0;
   out_2459924378456607327[273] = 0.0;
   out_2459924378456607327[274] = 0.0;
   out_2459924378456607327[275] = 0.0;
   out_2459924378456607327[276] = 0.0;
   out_2459924378456607327[277] = 0.0;
   out_2459924378456607327[278] = 0.0;
   out_2459924378456607327[279] = 0.0;
   out_2459924378456607327[280] = 0.0;
   out_2459924378456607327[281] = 0.0;
   out_2459924378456607327[282] = 0.0;
   out_2459924378456607327[283] = 0.0;
   out_2459924378456607327[284] = 0.0;
   out_2459924378456607327[285] = 1.0;
   out_2459924378456607327[286] = 0.0;
   out_2459924378456607327[287] = 0.0;
   out_2459924378456607327[288] = 0.0;
   out_2459924378456607327[289] = 0.0;
   out_2459924378456607327[290] = 0.0;
   out_2459924378456607327[291] = 0.0;
   out_2459924378456607327[292] = 0.0;
   out_2459924378456607327[293] = 0.0;
   out_2459924378456607327[294] = 0.0;
   out_2459924378456607327[295] = 0.0;
   out_2459924378456607327[296] = 0.0;
   out_2459924378456607327[297] = 0.0;
   out_2459924378456607327[298] = 0.0;
   out_2459924378456607327[299] = 0.0;
   out_2459924378456607327[300] = 0.0;
   out_2459924378456607327[301] = 0.0;
   out_2459924378456607327[302] = 0.0;
   out_2459924378456607327[303] = 0.0;
   out_2459924378456607327[304] = 1.0;
   out_2459924378456607327[305] = 0.0;
   out_2459924378456607327[306] = 0.0;
   out_2459924378456607327[307] = 0.0;
   out_2459924378456607327[308] = 0.0;
   out_2459924378456607327[309] = 0.0;
   out_2459924378456607327[310] = 0.0;
   out_2459924378456607327[311] = 0.0;
   out_2459924378456607327[312] = 0.0;
   out_2459924378456607327[313] = 0.0;
   out_2459924378456607327[314] = 0.0;
   out_2459924378456607327[315] = 0.0;
   out_2459924378456607327[316] = 0.0;
   out_2459924378456607327[317] = 0.0;
   out_2459924378456607327[318] = 0.0;
   out_2459924378456607327[319] = 0.0;
   out_2459924378456607327[320] = 0.0;
   out_2459924378456607327[321] = 0.0;
   out_2459924378456607327[322] = 0.0;
   out_2459924378456607327[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_862851979799617352) {
   out_862851979799617352[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_862851979799617352[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_862851979799617352[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_862851979799617352[3] = dt*state[12] + state[3];
   out_862851979799617352[4] = dt*state[13] + state[4];
   out_862851979799617352[5] = dt*state[14] + state[5];
   out_862851979799617352[6] = state[6];
   out_862851979799617352[7] = state[7];
   out_862851979799617352[8] = state[8];
   out_862851979799617352[9] = state[9];
   out_862851979799617352[10] = state[10];
   out_862851979799617352[11] = state[11];
   out_862851979799617352[12] = state[12];
   out_862851979799617352[13] = state[13];
   out_862851979799617352[14] = state[14];
   out_862851979799617352[15] = state[15];
   out_862851979799617352[16] = state[16];
   out_862851979799617352[17] = state[17];
}
void F_fun(double *state, double dt, double *out_6264058293643605080) {
   out_6264058293643605080[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6264058293643605080[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6264058293643605080[2] = 0;
   out_6264058293643605080[3] = 0;
   out_6264058293643605080[4] = 0;
   out_6264058293643605080[5] = 0;
   out_6264058293643605080[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6264058293643605080[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6264058293643605080[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6264058293643605080[9] = 0;
   out_6264058293643605080[10] = 0;
   out_6264058293643605080[11] = 0;
   out_6264058293643605080[12] = 0;
   out_6264058293643605080[13] = 0;
   out_6264058293643605080[14] = 0;
   out_6264058293643605080[15] = 0;
   out_6264058293643605080[16] = 0;
   out_6264058293643605080[17] = 0;
   out_6264058293643605080[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6264058293643605080[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6264058293643605080[20] = 0;
   out_6264058293643605080[21] = 0;
   out_6264058293643605080[22] = 0;
   out_6264058293643605080[23] = 0;
   out_6264058293643605080[24] = 0;
   out_6264058293643605080[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6264058293643605080[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6264058293643605080[27] = 0;
   out_6264058293643605080[28] = 0;
   out_6264058293643605080[29] = 0;
   out_6264058293643605080[30] = 0;
   out_6264058293643605080[31] = 0;
   out_6264058293643605080[32] = 0;
   out_6264058293643605080[33] = 0;
   out_6264058293643605080[34] = 0;
   out_6264058293643605080[35] = 0;
   out_6264058293643605080[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6264058293643605080[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6264058293643605080[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6264058293643605080[39] = 0;
   out_6264058293643605080[40] = 0;
   out_6264058293643605080[41] = 0;
   out_6264058293643605080[42] = 0;
   out_6264058293643605080[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6264058293643605080[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6264058293643605080[45] = 0;
   out_6264058293643605080[46] = 0;
   out_6264058293643605080[47] = 0;
   out_6264058293643605080[48] = 0;
   out_6264058293643605080[49] = 0;
   out_6264058293643605080[50] = 0;
   out_6264058293643605080[51] = 0;
   out_6264058293643605080[52] = 0;
   out_6264058293643605080[53] = 0;
   out_6264058293643605080[54] = 0;
   out_6264058293643605080[55] = 0;
   out_6264058293643605080[56] = 0;
   out_6264058293643605080[57] = 1;
   out_6264058293643605080[58] = 0;
   out_6264058293643605080[59] = 0;
   out_6264058293643605080[60] = 0;
   out_6264058293643605080[61] = 0;
   out_6264058293643605080[62] = 0;
   out_6264058293643605080[63] = 0;
   out_6264058293643605080[64] = 0;
   out_6264058293643605080[65] = 0;
   out_6264058293643605080[66] = dt;
   out_6264058293643605080[67] = 0;
   out_6264058293643605080[68] = 0;
   out_6264058293643605080[69] = 0;
   out_6264058293643605080[70] = 0;
   out_6264058293643605080[71] = 0;
   out_6264058293643605080[72] = 0;
   out_6264058293643605080[73] = 0;
   out_6264058293643605080[74] = 0;
   out_6264058293643605080[75] = 0;
   out_6264058293643605080[76] = 1;
   out_6264058293643605080[77] = 0;
   out_6264058293643605080[78] = 0;
   out_6264058293643605080[79] = 0;
   out_6264058293643605080[80] = 0;
   out_6264058293643605080[81] = 0;
   out_6264058293643605080[82] = 0;
   out_6264058293643605080[83] = 0;
   out_6264058293643605080[84] = 0;
   out_6264058293643605080[85] = dt;
   out_6264058293643605080[86] = 0;
   out_6264058293643605080[87] = 0;
   out_6264058293643605080[88] = 0;
   out_6264058293643605080[89] = 0;
   out_6264058293643605080[90] = 0;
   out_6264058293643605080[91] = 0;
   out_6264058293643605080[92] = 0;
   out_6264058293643605080[93] = 0;
   out_6264058293643605080[94] = 0;
   out_6264058293643605080[95] = 1;
   out_6264058293643605080[96] = 0;
   out_6264058293643605080[97] = 0;
   out_6264058293643605080[98] = 0;
   out_6264058293643605080[99] = 0;
   out_6264058293643605080[100] = 0;
   out_6264058293643605080[101] = 0;
   out_6264058293643605080[102] = 0;
   out_6264058293643605080[103] = 0;
   out_6264058293643605080[104] = dt;
   out_6264058293643605080[105] = 0;
   out_6264058293643605080[106] = 0;
   out_6264058293643605080[107] = 0;
   out_6264058293643605080[108] = 0;
   out_6264058293643605080[109] = 0;
   out_6264058293643605080[110] = 0;
   out_6264058293643605080[111] = 0;
   out_6264058293643605080[112] = 0;
   out_6264058293643605080[113] = 0;
   out_6264058293643605080[114] = 1;
   out_6264058293643605080[115] = 0;
   out_6264058293643605080[116] = 0;
   out_6264058293643605080[117] = 0;
   out_6264058293643605080[118] = 0;
   out_6264058293643605080[119] = 0;
   out_6264058293643605080[120] = 0;
   out_6264058293643605080[121] = 0;
   out_6264058293643605080[122] = 0;
   out_6264058293643605080[123] = 0;
   out_6264058293643605080[124] = 0;
   out_6264058293643605080[125] = 0;
   out_6264058293643605080[126] = 0;
   out_6264058293643605080[127] = 0;
   out_6264058293643605080[128] = 0;
   out_6264058293643605080[129] = 0;
   out_6264058293643605080[130] = 0;
   out_6264058293643605080[131] = 0;
   out_6264058293643605080[132] = 0;
   out_6264058293643605080[133] = 1;
   out_6264058293643605080[134] = 0;
   out_6264058293643605080[135] = 0;
   out_6264058293643605080[136] = 0;
   out_6264058293643605080[137] = 0;
   out_6264058293643605080[138] = 0;
   out_6264058293643605080[139] = 0;
   out_6264058293643605080[140] = 0;
   out_6264058293643605080[141] = 0;
   out_6264058293643605080[142] = 0;
   out_6264058293643605080[143] = 0;
   out_6264058293643605080[144] = 0;
   out_6264058293643605080[145] = 0;
   out_6264058293643605080[146] = 0;
   out_6264058293643605080[147] = 0;
   out_6264058293643605080[148] = 0;
   out_6264058293643605080[149] = 0;
   out_6264058293643605080[150] = 0;
   out_6264058293643605080[151] = 0;
   out_6264058293643605080[152] = 1;
   out_6264058293643605080[153] = 0;
   out_6264058293643605080[154] = 0;
   out_6264058293643605080[155] = 0;
   out_6264058293643605080[156] = 0;
   out_6264058293643605080[157] = 0;
   out_6264058293643605080[158] = 0;
   out_6264058293643605080[159] = 0;
   out_6264058293643605080[160] = 0;
   out_6264058293643605080[161] = 0;
   out_6264058293643605080[162] = 0;
   out_6264058293643605080[163] = 0;
   out_6264058293643605080[164] = 0;
   out_6264058293643605080[165] = 0;
   out_6264058293643605080[166] = 0;
   out_6264058293643605080[167] = 0;
   out_6264058293643605080[168] = 0;
   out_6264058293643605080[169] = 0;
   out_6264058293643605080[170] = 0;
   out_6264058293643605080[171] = 1;
   out_6264058293643605080[172] = 0;
   out_6264058293643605080[173] = 0;
   out_6264058293643605080[174] = 0;
   out_6264058293643605080[175] = 0;
   out_6264058293643605080[176] = 0;
   out_6264058293643605080[177] = 0;
   out_6264058293643605080[178] = 0;
   out_6264058293643605080[179] = 0;
   out_6264058293643605080[180] = 0;
   out_6264058293643605080[181] = 0;
   out_6264058293643605080[182] = 0;
   out_6264058293643605080[183] = 0;
   out_6264058293643605080[184] = 0;
   out_6264058293643605080[185] = 0;
   out_6264058293643605080[186] = 0;
   out_6264058293643605080[187] = 0;
   out_6264058293643605080[188] = 0;
   out_6264058293643605080[189] = 0;
   out_6264058293643605080[190] = 1;
   out_6264058293643605080[191] = 0;
   out_6264058293643605080[192] = 0;
   out_6264058293643605080[193] = 0;
   out_6264058293643605080[194] = 0;
   out_6264058293643605080[195] = 0;
   out_6264058293643605080[196] = 0;
   out_6264058293643605080[197] = 0;
   out_6264058293643605080[198] = 0;
   out_6264058293643605080[199] = 0;
   out_6264058293643605080[200] = 0;
   out_6264058293643605080[201] = 0;
   out_6264058293643605080[202] = 0;
   out_6264058293643605080[203] = 0;
   out_6264058293643605080[204] = 0;
   out_6264058293643605080[205] = 0;
   out_6264058293643605080[206] = 0;
   out_6264058293643605080[207] = 0;
   out_6264058293643605080[208] = 0;
   out_6264058293643605080[209] = 1;
   out_6264058293643605080[210] = 0;
   out_6264058293643605080[211] = 0;
   out_6264058293643605080[212] = 0;
   out_6264058293643605080[213] = 0;
   out_6264058293643605080[214] = 0;
   out_6264058293643605080[215] = 0;
   out_6264058293643605080[216] = 0;
   out_6264058293643605080[217] = 0;
   out_6264058293643605080[218] = 0;
   out_6264058293643605080[219] = 0;
   out_6264058293643605080[220] = 0;
   out_6264058293643605080[221] = 0;
   out_6264058293643605080[222] = 0;
   out_6264058293643605080[223] = 0;
   out_6264058293643605080[224] = 0;
   out_6264058293643605080[225] = 0;
   out_6264058293643605080[226] = 0;
   out_6264058293643605080[227] = 0;
   out_6264058293643605080[228] = 1;
   out_6264058293643605080[229] = 0;
   out_6264058293643605080[230] = 0;
   out_6264058293643605080[231] = 0;
   out_6264058293643605080[232] = 0;
   out_6264058293643605080[233] = 0;
   out_6264058293643605080[234] = 0;
   out_6264058293643605080[235] = 0;
   out_6264058293643605080[236] = 0;
   out_6264058293643605080[237] = 0;
   out_6264058293643605080[238] = 0;
   out_6264058293643605080[239] = 0;
   out_6264058293643605080[240] = 0;
   out_6264058293643605080[241] = 0;
   out_6264058293643605080[242] = 0;
   out_6264058293643605080[243] = 0;
   out_6264058293643605080[244] = 0;
   out_6264058293643605080[245] = 0;
   out_6264058293643605080[246] = 0;
   out_6264058293643605080[247] = 1;
   out_6264058293643605080[248] = 0;
   out_6264058293643605080[249] = 0;
   out_6264058293643605080[250] = 0;
   out_6264058293643605080[251] = 0;
   out_6264058293643605080[252] = 0;
   out_6264058293643605080[253] = 0;
   out_6264058293643605080[254] = 0;
   out_6264058293643605080[255] = 0;
   out_6264058293643605080[256] = 0;
   out_6264058293643605080[257] = 0;
   out_6264058293643605080[258] = 0;
   out_6264058293643605080[259] = 0;
   out_6264058293643605080[260] = 0;
   out_6264058293643605080[261] = 0;
   out_6264058293643605080[262] = 0;
   out_6264058293643605080[263] = 0;
   out_6264058293643605080[264] = 0;
   out_6264058293643605080[265] = 0;
   out_6264058293643605080[266] = 1;
   out_6264058293643605080[267] = 0;
   out_6264058293643605080[268] = 0;
   out_6264058293643605080[269] = 0;
   out_6264058293643605080[270] = 0;
   out_6264058293643605080[271] = 0;
   out_6264058293643605080[272] = 0;
   out_6264058293643605080[273] = 0;
   out_6264058293643605080[274] = 0;
   out_6264058293643605080[275] = 0;
   out_6264058293643605080[276] = 0;
   out_6264058293643605080[277] = 0;
   out_6264058293643605080[278] = 0;
   out_6264058293643605080[279] = 0;
   out_6264058293643605080[280] = 0;
   out_6264058293643605080[281] = 0;
   out_6264058293643605080[282] = 0;
   out_6264058293643605080[283] = 0;
   out_6264058293643605080[284] = 0;
   out_6264058293643605080[285] = 1;
   out_6264058293643605080[286] = 0;
   out_6264058293643605080[287] = 0;
   out_6264058293643605080[288] = 0;
   out_6264058293643605080[289] = 0;
   out_6264058293643605080[290] = 0;
   out_6264058293643605080[291] = 0;
   out_6264058293643605080[292] = 0;
   out_6264058293643605080[293] = 0;
   out_6264058293643605080[294] = 0;
   out_6264058293643605080[295] = 0;
   out_6264058293643605080[296] = 0;
   out_6264058293643605080[297] = 0;
   out_6264058293643605080[298] = 0;
   out_6264058293643605080[299] = 0;
   out_6264058293643605080[300] = 0;
   out_6264058293643605080[301] = 0;
   out_6264058293643605080[302] = 0;
   out_6264058293643605080[303] = 0;
   out_6264058293643605080[304] = 1;
   out_6264058293643605080[305] = 0;
   out_6264058293643605080[306] = 0;
   out_6264058293643605080[307] = 0;
   out_6264058293643605080[308] = 0;
   out_6264058293643605080[309] = 0;
   out_6264058293643605080[310] = 0;
   out_6264058293643605080[311] = 0;
   out_6264058293643605080[312] = 0;
   out_6264058293643605080[313] = 0;
   out_6264058293643605080[314] = 0;
   out_6264058293643605080[315] = 0;
   out_6264058293643605080[316] = 0;
   out_6264058293643605080[317] = 0;
   out_6264058293643605080[318] = 0;
   out_6264058293643605080[319] = 0;
   out_6264058293643605080[320] = 0;
   out_6264058293643605080[321] = 0;
   out_6264058293643605080[322] = 0;
   out_6264058293643605080[323] = 1;
}
void h_4(double *state, double *unused, double *out_6982350416274880013) {
   out_6982350416274880013[0] = state[6] + state[9];
   out_6982350416274880013[1] = state[7] + state[10];
   out_6982350416274880013[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_5740380505516617440) {
   out_5740380505516617440[0] = 0;
   out_5740380505516617440[1] = 0;
   out_5740380505516617440[2] = 0;
   out_5740380505516617440[3] = 0;
   out_5740380505516617440[4] = 0;
   out_5740380505516617440[5] = 0;
   out_5740380505516617440[6] = 1;
   out_5740380505516617440[7] = 0;
   out_5740380505516617440[8] = 0;
   out_5740380505516617440[9] = 1;
   out_5740380505516617440[10] = 0;
   out_5740380505516617440[11] = 0;
   out_5740380505516617440[12] = 0;
   out_5740380505516617440[13] = 0;
   out_5740380505516617440[14] = 0;
   out_5740380505516617440[15] = 0;
   out_5740380505516617440[16] = 0;
   out_5740380505516617440[17] = 0;
   out_5740380505516617440[18] = 0;
   out_5740380505516617440[19] = 0;
   out_5740380505516617440[20] = 0;
   out_5740380505516617440[21] = 0;
   out_5740380505516617440[22] = 0;
   out_5740380505516617440[23] = 0;
   out_5740380505516617440[24] = 0;
   out_5740380505516617440[25] = 1;
   out_5740380505516617440[26] = 0;
   out_5740380505516617440[27] = 0;
   out_5740380505516617440[28] = 1;
   out_5740380505516617440[29] = 0;
   out_5740380505516617440[30] = 0;
   out_5740380505516617440[31] = 0;
   out_5740380505516617440[32] = 0;
   out_5740380505516617440[33] = 0;
   out_5740380505516617440[34] = 0;
   out_5740380505516617440[35] = 0;
   out_5740380505516617440[36] = 0;
   out_5740380505516617440[37] = 0;
   out_5740380505516617440[38] = 0;
   out_5740380505516617440[39] = 0;
   out_5740380505516617440[40] = 0;
   out_5740380505516617440[41] = 0;
   out_5740380505516617440[42] = 0;
   out_5740380505516617440[43] = 0;
   out_5740380505516617440[44] = 1;
   out_5740380505516617440[45] = 0;
   out_5740380505516617440[46] = 0;
   out_5740380505516617440[47] = 1;
   out_5740380505516617440[48] = 0;
   out_5740380505516617440[49] = 0;
   out_5740380505516617440[50] = 0;
   out_5740380505516617440[51] = 0;
   out_5740380505516617440[52] = 0;
   out_5740380505516617440[53] = 0;
}
void h_10(double *state, double *unused, double *out_8881427016800317352) {
   out_8881427016800317352[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8881427016800317352[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8881427016800317352[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_6831836666791918057) {
   out_6831836666791918057[0] = 0;
   out_6831836666791918057[1] = 9.8100000000000005*cos(state[1]);
   out_6831836666791918057[2] = 0;
   out_6831836666791918057[3] = 0;
   out_6831836666791918057[4] = -state[8];
   out_6831836666791918057[5] = state[7];
   out_6831836666791918057[6] = 0;
   out_6831836666791918057[7] = state[5];
   out_6831836666791918057[8] = -state[4];
   out_6831836666791918057[9] = 0;
   out_6831836666791918057[10] = 0;
   out_6831836666791918057[11] = 0;
   out_6831836666791918057[12] = 1;
   out_6831836666791918057[13] = 0;
   out_6831836666791918057[14] = 0;
   out_6831836666791918057[15] = 1;
   out_6831836666791918057[16] = 0;
   out_6831836666791918057[17] = 0;
   out_6831836666791918057[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_6831836666791918057[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_6831836666791918057[20] = 0;
   out_6831836666791918057[21] = state[8];
   out_6831836666791918057[22] = 0;
   out_6831836666791918057[23] = -state[6];
   out_6831836666791918057[24] = -state[5];
   out_6831836666791918057[25] = 0;
   out_6831836666791918057[26] = state[3];
   out_6831836666791918057[27] = 0;
   out_6831836666791918057[28] = 0;
   out_6831836666791918057[29] = 0;
   out_6831836666791918057[30] = 0;
   out_6831836666791918057[31] = 1;
   out_6831836666791918057[32] = 0;
   out_6831836666791918057[33] = 0;
   out_6831836666791918057[34] = 1;
   out_6831836666791918057[35] = 0;
   out_6831836666791918057[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_6831836666791918057[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_6831836666791918057[38] = 0;
   out_6831836666791918057[39] = -state[7];
   out_6831836666791918057[40] = state[6];
   out_6831836666791918057[41] = 0;
   out_6831836666791918057[42] = state[4];
   out_6831836666791918057[43] = -state[3];
   out_6831836666791918057[44] = 0;
   out_6831836666791918057[45] = 0;
   out_6831836666791918057[46] = 0;
   out_6831836666791918057[47] = 0;
   out_6831836666791918057[48] = 0;
   out_6831836666791918057[49] = 0;
   out_6831836666791918057[50] = 1;
   out_6831836666791918057[51] = 0;
   out_6831836666791918057[52] = 0;
   out_6831836666791918057[53] = 1;
}
void h_13(double *state, double *unused, double *out_7903236857747805930) {
   out_7903236857747805930[0] = state[3];
   out_7903236857747805930[1] = state[4];
   out_7903236857747805930[2] = state[5];
}
void H_13(double *state, double *unused, double *out_5095732359876233247) {
   out_5095732359876233247[0] = 0;
   out_5095732359876233247[1] = 0;
   out_5095732359876233247[2] = 0;
   out_5095732359876233247[3] = 1;
   out_5095732359876233247[4] = 0;
   out_5095732359876233247[5] = 0;
   out_5095732359876233247[6] = 0;
   out_5095732359876233247[7] = 0;
   out_5095732359876233247[8] = 0;
   out_5095732359876233247[9] = 0;
   out_5095732359876233247[10] = 0;
   out_5095732359876233247[11] = 0;
   out_5095732359876233247[12] = 0;
   out_5095732359876233247[13] = 0;
   out_5095732359876233247[14] = 0;
   out_5095732359876233247[15] = 0;
   out_5095732359876233247[16] = 0;
   out_5095732359876233247[17] = 0;
   out_5095732359876233247[18] = 0;
   out_5095732359876233247[19] = 0;
   out_5095732359876233247[20] = 0;
   out_5095732359876233247[21] = 0;
   out_5095732359876233247[22] = 1;
   out_5095732359876233247[23] = 0;
   out_5095732359876233247[24] = 0;
   out_5095732359876233247[25] = 0;
   out_5095732359876233247[26] = 0;
   out_5095732359876233247[27] = 0;
   out_5095732359876233247[28] = 0;
   out_5095732359876233247[29] = 0;
   out_5095732359876233247[30] = 0;
   out_5095732359876233247[31] = 0;
   out_5095732359876233247[32] = 0;
   out_5095732359876233247[33] = 0;
   out_5095732359876233247[34] = 0;
   out_5095732359876233247[35] = 0;
   out_5095732359876233247[36] = 0;
   out_5095732359876233247[37] = 0;
   out_5095732359876233247[38] = 0;
   out_5095732359876233247[39] = 0;
   out_5095732359876233247[40] = 0;
   out_5095732359876233247[41] = 1;
   out_5095732359876233247[42] = 0;
   out_5095732359876233247[43] = 0;
   out_5095732359876233247[44] = 0;
   out_5095732359876233247[45] = 0;
   out_5095732359876233247[46] = 0;
   out_5095732359876233247[47] = 0;
   out_5095732359876233247[48] = 0;
   out_5095732359876233247[49] = 0;
   out_5095732359876233247[50] = 0;
   out_5095732359876233247[51] = 0;
   out_5095732359876233247[52] = 0;
   out_5095732359876233247[53] = 0;
}
void h_14(double *state, double *unused, double *out_6008800958186249240) {
   out_6008800958186249240[0] = state[6];
   out_6008800958186249240[1] = state[7];
   out_6008800958186249240[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8743122711853449647) {
   out_8743122711853449647[0] = 0;
   out_8743122711853449647[1] = 0;
   out_8743122711853449647[2] = 0;
   out_8743122711853449647[3] = 0;
   out_8743122711853449647[4] = 0;
   out_8743122711853449647[5] = 0;
   out_8743122711853449647[6] = 1;
   out_8743122711853449647[7] = 0;
   out_8743122711853449647[8] = 0;
   out_8743122711853449647[9] = 0;
   out_8743122711853449647[10] = 0;
   out_8743122711853449647[11] = 0;
   out_8743122711853449647[12] = 0;
   out_8743122711853449647[13] = 0;
   out_8743122711853449647[14] = 0;
   out_8743122711853449647[15] = 0;
   out_8743122711853449647[16] = 0;
   out_8743122711853449647[17] = 0;
   out_8743122711853449647[18] = 0;
   out_8743122711853449647[19] = 0;
   out_8743122711853449647[20] = 0;
   out_8743122711853449647[21] = 0;
   out_8743122711853449647[22] = 0;
   out_8743122711853449647[23] = 0;
   out_8743122711853449647[24] = 0;
   out_8743122711853449647[25] = 1;
   out_8743122711853449647[26] = 0;
   out_8743122711853449647[27] = 0;
   out_8743122711853449647[28] = 0;
   out_8743122711853449647[29] = 0;
   out_8743122711853449647[30] = 0;
   out_8743122711853449647[31] = 0;
   out_8743122711853449647[32] = 0;
   out_8743122711853449647[33] = 0;
   out_8743122711853449647[34] = 0;
   out_8743122711853449647[35] = 0;
   out_8743122711853449647[36] = 0;
   out_8743122711853449647[37] = 0;
   out_8743122711853449647[38] = 0;
   out_8743122711853449647[39] = 0;
   out_8743122711853449647[40] = 0;
   out_8743122711853449647[41] = 0;
   out_8743122711853449647[42] = 0;
   out_8743122711853449647[43] = 0;
   out_8743122711853449647[44] = 1;
   out_8743122711853449647[45] = 0;
   out_8743122711853449647[46] = 0;
   out_8743122711853449647[47] = 0;
   out_8743122711853449647[48] = 0;
   out_8743122711853449647[49] = 0;
   out_8743122711853449647[50] = 0;
   out_8743122711853449647[51] = 0;
   out_8743122711853449647[52] = 0;
   out_8743122711853449647[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_1694318774695559648) {
  err_fun(nom_x, delta_x, out_1694318774695559648);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1527863431850837134) {
  inv_err_fun(nom_x, true_x, out_1527863431850837134);
}
void pose_H_mod_fun(double *state, double *out_2459924378456607327) {
  H_mod_fun(state, out_2459924378456607327);
}
void pose_f_fun(double *state, double dt, double *out_862851979799617352) {
  f_fun(state,  dt, out_862851979799617352);
}
void pose_F_fun(double *state, double dt, double *out_6264058293643605080) {
  F_fun(state,  dt, out_6264058293643605080);
}
void pose_h_4(double *state, double *unused, double *out_6982350416274880013) {
  h_4(state, unused, out_6982350416274880013);
}
void pose_H_4(double *state, double *unused, double *out_5740380505516617440) {
  H_4(state, unused, out_5740380505516617440);
}
void pose_h_10(double *state, double *unused, double *out_8881427016800317352) {
  h_10(state, unused, out_8881427016800317352);
}
void pose_H_10(double *state, double *unused, double *out_6831836666791918057) {
  H_10(state, unused, out_6831836666791918057);
}
void pose_h_13(double *state, double *unused, double *out_7903236857747805930) {
  h_13(state, unused, out_7903236857747805930);
}
void pose_H_13(double *state, double *unused, double *out_5095732359876233247) {
  H_13(state, unused, out_5095732359876233247);
}
void pose_h_14(double *state, double *unused, double *out_6008800958186249240) {
  h_14(state, unused, out_6008800958186249240);
}
void pose_H_14(double *state, double *unused, double *out_8743122711853449647) {
  H_14(state, unused, out_8743122711853449647);
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
