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
void err_fun(double *nom_x, double *delta_x, double *out_5709026608963937263) {
   out_5709026608963937263[0] = delta_x[0] + nom_x[0];
   out_5709026608963937263[1] = delta_x[1] + nom_x[1];
   out_5709026608963937263[2] = delta_x[2] + nom_x[2];
   out_5709026608963937263[3] = delta_x[3] + nom_x[3];
   out_5709026608963937263[4] = delta_x[4] + nom_x[4];
   out_5709026608963937263[5] = delta_x[5] + nom_x[5];
   out_5709026608963937263[6] = delta_x[6] + nom_x[6];
   out_5709026608963937263[7] = delta_x[7] + nom_x[7];
   out_5709026608963937263[8] = delta_x[8] + nom_x[8];
   out_5709026608963937263[9] = delta_x[9] + nom_x[9];
   out_5709026608963937263[10] = delta_x[10] + nom_x[10];
   out_5709026608963937263[11] = delta_x[11] + nom_x[11];
   out_5709026608963937263[12] = delta_x[12] + nom_x[12];
   out_5709026608963937263[13] = delta_x[13] + nom_x[13];
   out_5709026608963937263[14] = delta_x[14] + nom_x[14];
   out_5709026608963937263[15] = delta_x[15] + nom_x[15];
   out_5709026608963937263[16] = delta_x[16] + nom_x[16];
   out_5709026608963937263[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_9100639615517483437) {
   out_9100639615517483437[0] = -nom_x[0] + true_x[0];
   out_9100639615517483437[1] = -nom_x[1] + true_x[1];
   out_9100639615517483437[2] = -nom_x[2] + true_x[2];
   out_9100639615517483437[3] = -nom_x[3] + true_x[3];
   out_9100639615517483437[4] = -nom_x[4] + true_x[4];
   out_9100639615517483437[5] = -nom_x[5] + true_x[5];
   out_9100639615517483437[6] = -nom_x[6] + true_x[6];
   out_9100639615517483437[7] = -nom_x[7] + true_x[7];
   out_9100639615517483437[8] = -nom_x[8] + true_x[8];
   out_9100639615517483437[9] = -nom_x[9] + true_x[9];
   out_9100639615517483437[10] = -nom_x[10] + true_x[10];
   out_9100639615517483437[11] = -nom_x[11] + true_x[11];
   out_9100639615517483437[12] = -nom_x[12] + true_x[12];
   out_9100639615517483437[13] = -nom_x[13] + true_x[13];
   out_9100639615517483437[14] = -nom_x[14] + true_x[14];
   out_9100639615517483437[15] = -nom_x[15] + true_x[15];
   out_9100639615517483437[16] = -nom_x[16] + true_x[16];
   out_9100639615517483437[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_372251806579087438) {
   out_372251806579087438[0] = 1.0;
   out_372251806579087438[1] = 0.0;
   out_372251806579087438[2] = 0.0;
   out_372251806579087438[3] = 0.0;
   out_372251806579087438[4] = 0.0;
   out_372251806579087438[5] = 0.0;
   out_372251806579087438[6] = 0.0;
   out_372251806579087438[7] = 0.0;
   out_372251806579087438[8] = 0.0;
   out_372251806579087438[9] = 0.0;
   out_372251806579087438[10] = 0.0;
   out_372251806579087438[11] = 0.0;
   out_372251806579087438[12] = 0.0;
   out_372251806579087438[13] = 0.0;
   out_372251806579087438[14] = 0.0;
   out_372251806579087438[15] = 0.0;
   out_372251806579087438[16] = 0.0;
   out_372251806579087438[17] = 0.0;
   out_372251806579087438[18] = 0.0;
   out_372251806579087438[19] = 1.0;
   out_372251806579087438[20] = 0.0;
   out_372251806579087438[21] = 0.0;
   out_372251806579087438[22] = 0.0;
   out_372251806579087438[23] = 0.0;
   out_372251806579087438[24] = 0.0;
   out_372251806579087438[25] = 0.0;
   out_372251806579087438[26] = 0.0;
   out_372251806579087438[27] = 0.0;
   out_372251806579087438[28] = 0.0;
   out_372251806579087438[29] = 0.0;
   out_372251806579087438[30] = 0.0;
   out_372251806579087438[31] = 0.0;
   out_372251806579087438[32] = 0.0;
   out_372251806579087438[33] = 0.0;
   out_372251806579087438[34] = 0.0;
   out_372251806579087438[35] = 0.0;
   out_372251806579087438[36] = 0.0;
   out_372251806579087438[37] = 0.0;
   out_372251806579087438[38] = 1.0;
   out_372251806579087438[39] = 0.0;
   out_372251806579087438[40] = 0.0;
   out_372251806579087438[41] = 0.0;
   out_372251806579087438[42] = 0.0;
   out_372251806579087438[43] = 0.0;
   out_372251806579087438[44] = 0.0;
   out_372251806579087438[45] = 0.0;
   out_372251806579087438[46] = 0.0;
   out_372251806579087438[47] = 0.0;
   out_372251806579087438[48] = 0.0;
   out_372251806579087438[49] = 0.0;
   out_372251806579087438[50] = 0.0;
   out_372251806579087438[51] = 0.0;
   out_372251806579087438[52] = 0.0;
   out_372251806579087438[53] = 0.0;
   out_372251806579087438[54] = 0.0;
   out_372251806579087438[55] = 0.0;
   out_372251806579087438[56] = 0.0;
   out_372251806579087438[57] = 1.0;
   out_372251806579087438[58] = 0.0;
   out_372251806579087438[59] = 0.0;
   out_372251806579087438[60] = 0.0;
   out_372251806579087438[61] = 0.0;
   out_372251806579087438[62] = 0.0;
   out_372251806579087438[63] = 0.0;
   out_372251806579087438[64] = 0.0;
   out_372251806579087438[65] = 0.0;
   out_372251806579087438[66] = 0.0;
   out_372251806579087438[67] = 0.0;
   out_372251806579087438[68] = 0.0;
   out_372251806579087438[69] = 0.0;
   out_372251806579087438[70] = 0.0;
   out_372251806579087438[71] = 0.0;
   out_372251806579087438[72] = 0.0;
   out_372251806579087438[73] = 0.0;
   out_372251806579087438[74] = 0.0;
   out_372251806579087438[75] = 0.0;
   out_372251806579087438[76] = 1.0;
   out_372251806579087438[77] = 0.0;
   out_372251806579087438[78] = 0.0;
   out_372251806579087438[79] = 0.0;
   out_372251806579087438[80] = 0.0;
   out_372251806579087438[81] = 0.0;
   out_372251806579087438[82] = 0.0;
   out_372251806579087438[83] = 0.0;
   out_372251806579087438[84] = 0.0;
   out_372251806579087438[85] = 0.0;
   out_372251806579087438[86] = 0.0;
   out_372251806579087438[87] = 0.0;
   out_372251806579087438[88] = 0.0;
   out_372251806579087438[89] = 0.0;
   out_372251806579087438[90] = 0.0;
   out_372251806579087438[91] = 0.0;
   out_372251806579087438[92] = 0.0;
   out_372251806579087438[93] = 0.0;
   out_372251806579087438[94] = 0.0;
   out_372251806579087438[95] = 1.0;
   out_372251806579087438[96] = 0.0;
   out_372251806579087438[97] = 0.0;
   out_372251806579087438[98] = 0.0;
   out_372251806579087438[99] = 0.0;
   out_372251806579087438[100] = 0.0;
   out_372251806579087438[101] = 0.0;
   out_372251806579087438[102] = 0.0;
   out_372251806579087438[103] = 0.0;
   out_372251806579087438[104] = 0.0;
   out_372251806579087438[105] = 0.0;
   out_372251806579087438[106] = 0.0;
   out_372251806579087438[107] = 0.0;
   out_372251806579087438[108] = 0.0;
   out_372251806579087438[109] = 0.0;
   out_372251806579087438[110] = 0.0;
   out_372251806579087438[111] = 0.0;
   out_372251806579087438[112] = 0.0;
   out_372251806579087438[113] = 0.0;
   out_372251806579087438[114] = 1.0;
   out_372251806579087438[115] = 0.0;
   out_372251806579087438[116] = 0.0;
   out_372251806579087438[117] = 0.0;
   out_372251806579087438[118] = 0.0;
   out_372251806579087438[119] = 0.0;
   out_372251806579087438[120] = 0.0;
   out_372251806579087438[121] = 0.0;
   out_372251806579087438[122] = 0.0;
   out_372251806579087438[123] = 0.0;
   out_372251806579087438[124] = 0.0;
   out_372251806579087438[125] = 0.0;
   out_372251806579087438[126] = 0.0;
   out_372251806579087438[127] = 0.0;
   out_372251806579087438[128] = 0.0;
   out_372251806579087438[129] = 0.0;
   out_372251806579087438[130] = 0.0;
   out_372251806579087438[131] = 0.0;
   out_372251806579087438[132] = 0.0;
   out_372251806579087438[133] = 1.0;
   out_372251806579087438[134] = 0.0;
   out_372251806579087438[135] = 0.0;
   out_372251806579087438[136] = 0.0;
   out_372251806579087438[137] = 0.0;
   out_372251806579087438[138] = 0.0;
   out_372251806579087438[139] = 0.0;
   out_372251806579087438[140] = 0.0;
   out_372251806579087438[141] = 0.0;
   out_372251806579087438[142] = 0.0;
   out_372251806579087438[143] = 0.0;
   out_372251806579087438[144] = 0.0;
   out_372251806579087438[145] = 0.0;
   out_372251806579087438[146] = 0.0;
   out_372251806579087438[147] = 0.0;
   out_372251806579087438[148] = 0.0;
   out_372251806579087438[149] = 0.0;
   out_372251806579087438[150] = 0.0;
   out_372251806579087438[151] = 0.0;
   out_372251806579087438[152] = 1.0;
   out_372251806579087438[153] = 0.0;
   out_372251806579087438[154] = 0.0;
   out_372251806579087438[155] = 0.0;
   out_372251806579087438[156] = 0.0;
   out_372251806579087438[157] = 0.0;
   out_372251806579087438[158] = 0.0;
   out_372251806579087438[159] = 0.0;
   out_372251806579087438[160] = 0.0;
   out_372251806579087438[161] = 0.0;
   out_372251806579087438[162] = 0.0;
   out_372251806579087438[163] = 0.0;
   out_372251806579087438[164] = 0.0;
   out_372251806579087438[165] = 0.0;
   out_372251806579087438[166] = 0.0;
   out_372251806579087438[167] = 0.0;
   out_372251806579087438[168] = 0.0;
   out_372251806579087438[169] = 0.0;
   out_372251806579087438[170] = 0.0;
   out_372251806579087438[171] = 1.0;
   out_372251806579087438[172] = 0.0;
   out_372251806579087438[173] = 0.0;
   out_372251806579087438[174] = 0.0;
   out_372251806579087438[175] = 0.0;
   out_372251806579087438[176] = 0.0;
   out_372251806579087438[177] = 0.0;
   out_372251806579087438[178] = 0.0;
   out_372251806579087438[179] = 0.0;
   out_372251806579087438[180] = 0.0;
   out_372251806579087438[181] = 0.0;
   out_372251806579087438[182] = 0.0;
   out_372251806579087438[183] = 0.0;
   out_372251806579087438[184] = 0.0;
   out_372251806579087438[185] = 0.0;
   out_372251806579087438[186] = 0.0;
   out_372251806579087438[187] = 0.0;
   out_372251806579087438[188] = 0.0;
   out_372251806579087438[189] = 0.0;
   out_372251806579087438[190] = 1.0;
   out_372251806579087438[191] = 0.0;
   out_372251806579087438[192] = 0.0;
   out_372251806579087438[193] = 0.0;
   out_372251806579087438[194] = 0.0;
   out_372251806579087438[195] = 0.0;
   out_372251806579087438[196] = 0.0;
   out_372251806579087438[197] = 0.0;
   out_372251806579087438[198] = 0.0;
   out_372251806579087438[199] = 0.0;
   out_372251806579087438[200] = 0.0;
   out_372251806579087438[201] = 0.0;
   out_372251806579087438[202] = 0.0;
   out_372251806579087438[203] = 0.0;
   out_372251806579087438[204] = 0.0;
   out_372251806579087438[205] = 0.0;
   out_372251806579087438[206] = 0.0;
   out_372251806579087438[207] = 0.0;
   out_372251806579087438[208] = 0.0;
   out_372251806579087438[209] = 1.0;
   out_372251806579087438[210] = 0.0;
   out_372251806579087438[211] = 0.0;
   out_372251806579087438[212] = 0.0;
   out_372251806579087438[213] = 0.0;
   out_372251806579087438[214] = 0.0;
   out_372251806579087438[215] = 0.0;
   out_372251806579087438[216] = 0.0;
   out_372251806579087438[217] = 0.0;
   out_372251806579087438[218] = 0.0;
   out_372251806579087438[219] = 0.0;
   out_372251806579087438[220] = 0.0;
   out_372251806579087438[221] = 0.0;
   out_372251806579087438[222] = 0.0;
   out_372251806579087438[223] = 0.0;
   out_372251806579087438[224] = 0.0;
   out_372251806579087438[225] = 0.0;
   out_372251806579087438[226] = 0.0;
   out_372251806579087438[227] = 0.0;
   out_372251806579087438[228] = 1.0;
   out_372251806579087438[229] = 0.0;
   out_372251806579087438[230] = 0.0;
   out_372251806579087438[231] = 0.0;
   out_372251806579087438[232] = 0.0;
   out_372251806579087438[233] = 0.0;
   out_372251806579087438[234] = 0.0;
   out_372251806579087438[235] = 0.0;
   out_372251806579087438[236] = 0.0;
   out_372251806579087438[237] = 0.0;
   out_372251806579087438[238] = 0.0;
   out_372251806579087438[239] = 0.0;
   out_372251806579087438[240] = 0.0;
   out_372251806579087438[241] = 0.0;
   out_372251806579087438[242] = 0.0;
   out_372251806579087438[243] = 0.0;
   out_372251806579087438[244] = 0.0;
   out_372251806579087438[245] = 0.0;
   out_372251806579087438[246] = 0.0;
   out_372251806579087438[247] = 1.0;
   out_372251806579087438[248] = 0.0;
   out_372251806579087438[249] = 0.0;
   out_372251806579087438[250] = 0.0;
   out_372251806579087438[251] = 0.0;
   out_372251806579087438[252] = 0.0;
   out_372251806579087438[253] = 0.0;
   out_372251806579087438[254] = 0.0;
   out_372251806579087438[255] = 0.0;
   out_372251806579087438[256] = 0.0;
   out_372251806579087438[257] = 0.0;
   out_372251806579087438[258] = 0.0;
   out_372251806579087438[259] = 0.0;
   out_372251806579087438[260] = 0.0;
   out_372251806579087438[261] = 0.0;
   out_372251806579087438[262] = 0.0;
   out_372251806579087438[263] = 0.0;
   out_372251806579087438[264] = 0.0;
   out_372251806579087438[265] = 0.0;
   out_372251806579087438[266] = 1.0;
   out_372251806579087438[267] = 0.0;
   out_372251806579087438[268] = 0.0;
   out_372251806579087438[269] = 0.0;
   out_372251806579087438[270] = 0.0;
   out_372251806579087438[271] = 0.0;
   out_372251806579087438[272] = 0.0;
   out_372251806579087438[273] = 0.0;
   out_372251806579087438[274] = 0.0;
   out_372251806579087438[275] = 0.0;
   out_372251806579087438[276] = 0.0;
   out_372251806579087438[277] = 0.0;
   out_372251806579087438[278] = 0.0;
   out_372251806579087438[279] = 0.0;
   out_372251806579087438[280] = 0.0;
   out_372251806579087438[281] = 0.0;
   out_372251806579087438[282] = 0.0;
   out_372251806579087438[283] = 0.0;
   out_372251806579087438[284] = 0.0;
   out_372251806579087438[285] = 1.0;
   out_372251806579087438[286] = 0.0;
   out_372251806579087438[287] = 0.0;
   out_372251806579087438[288] = 0.0;
   out_372251806579087438[289] = 0.0;
   out_372251806579087438[290] = 0.0;
   out_372251806579087438[291] = 0.0;
   out_372251806579087438[292] = 0.0;
   out_372251806579087438[293] = 0.0;
   out_372251806579087438[294] = 0.0;
   out_372251806579087438[295] = 0.0;
   out_372251806579087438[296] = 0.0;
   out_372251806579087438[297] = 0.0;
   out_372251806579087438[298] = 0.0;
   out_372251806579087438[299] = 0.0;
   out_372251806579087438[300] = 0.0;
   out_372251806579087438[301] = 0.0;
   out_372251806579087438[302] = 0.0;
   out_372251806579087438[303] = 0.0;
   out_372251806579087438[304] = 1.0;
   out_372251806579087438[305] = 0.0;
   out_372251806579087438[306] = 0.0;
   out_372251806579087438[307] = 0.0;
   out_372251806579087438[308] = 0.0;
   out_372251806579087438[309] = 0.0;
   out_372251806579087438[310] = 0.0;
   out_372251806579087438[311] = 0.0;
   out_372251806579087438[312] = 0.0;
   out_372251806579087438[313] = 0.0;
   out_372251806579087438[314] = 0.0;
   out_372251806579087438[315] = 0.0;
   out_372251806579087438[316] = 0.0;
   out_372251806579087438[317] = 0.0;
   out_372251806579087438[318] = 0.0;
   out_372251806579087438[319] = 0.0;
   out_372251806579087438[320] = 0.0;
   out_372251806579087438[321] = 0.0;
   out_372251806579087438[322] = 0.0;
   out_372251806579087438[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_1740152809646530052) {
   out_1740152809646530052[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_1740152809646530052[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_1740152809646530052[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_1740152809646530052[3] = dt*state[12] + state[3];
   out_1740152809646530052[4] = dt*state[13] + state[4];
   out_1740152809646530052[5] = dt*state[14] + state[5];
   out_1740152809646530052[6] = state[6];
   out_1740152809646530052[7] = state[7];
   out_1740152809646530052[8] = state[8];
   out_1740152809646530052[9] = state[9];
   out_1740152809646530052[10] = state[10];
   out_1740152809646530052[11] = state[11];
   out_1740152809646530052[12] = state[12];
   out_1740152809646530052[13] = state[13];
   out_1740152809646530052[14] = state[14];
   out_1740152809646530052[15] = state[15];
   out_1740152809646530052[16] = state[16];
   out_1740152809646530052[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1353467089812513678) {
   out_1353467089812513678[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1353467089812513678[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1353467089812513678[2] = 0;
   out_1353467089812513678[3] = 0;
   out_1353467089812513678[4] = 0;
   out_1353467089812513678[5] = 0;
   out_1353467089812513678[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1353467089812513678[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1353467089812513678[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1353467089812513678[9] = 0;
   out_1353467089812513678[10] = 0;
   out_1353467089812513678[11] = 0;
   out_1353467089812513678[12] = 0;
   out_1353467089812513678[13] = 0;
   out_1353467089812513678[14] = 0;
   out_1353467089812513678[15] = 0;
   out_1353467089812513678[16] = 0;
   out_1353467089812513678[17] = 0;
   out_1353467089812513678[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1353467089812513678[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1353467089812513678[20] = 0;
   out_1353467089812513678[21] = 0;
   out_1353467089812513678[22] = 0;
   out_1353467089812513678[23] = 0;
   out_1353467089812513678[24] = 0;
   out_1353467089812513678[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1353467089812513678[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1353467089812513678[27] = 0;
   out_1353467089812513678[28] = 0;
   out_1353467089812513678[29] = 0;
   out_1353467089812513678[30] = 0;
   out_1353467089812513678[31] = 0;
   out_1353467089812513678[32] = 0;
   out_1353467089812513678[33] = 0;
   out_1353467089812513678[34] = 0;
   out_1353467089812513678[35] = 0;
   out_1353467089812513678[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1353467089812513678[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1353467089812513678[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1353467089812513678[39] = 0;
   out_1353467089812513678[40] = 0;
   out_1353467089812513678[41] = 0;
   out_1353467089812513678[42] = 0;
   out_1353467089812513678[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1353467089812513678[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1353467089812513678[45] = 0;
   out_1353467089812513678[46] = 0;
   out_1353467089812513678[47] = 0;
   out_1353467089812513678[48] = 0;
   out_1353467089812513678[49] = 0;
   out_1353467089812513678[50] = 0;
   out_1353467089812513678[51] = 0;
   out_1353467089812513678[52] = 0;
   out_1353467089812513678[53] = 0;
   out_1353467089812513678[54] = 0;
   out_1353467089812513678[55] = 0;
   out_1353467089812513678[56] = 0;
   out_1353467089812513678[57] = 1;
   out_1353467089812513678[58] = 0;
   out_1353467089812513678[59] = 0;
   out_1353467089812513678[60] = 0;
   out_1353467089812513678[61] = 0;
   out_1353467089812513678[62] = 0;
   out_1353467089812513678[63] = 0;
   out_1353467089812513678[64] = 0;
   out_1353467089812513678[65] = 0;
   out_1353467089812513678[66] = dt;
   out_1353467089812513678[67] = 0;
   out_1353467089812513678[68] = 0;
   out_1353467089812513678[69] = 0;
   out_1353467089812513678[70] = 0;
   out_1353467089812513678[71] = 0;
   out_1353467089812513678[72] = 0;
   out_1353467089812513678[73] = 0;
   out_1353467089812513678[74] = 0;
   out_1353467089812513678[75] = 0;
   out_1353467089812513678[76] = 1;
   out_1353467089812513678[77] = 0;
   out_1353467089812513678[78] = 0;
   out_1353467089812513678[79] = 0;
   out_1353467089812513678[80] = 0;
   out_1353467089812513678[81] = 0;
   out_1353467089812513678[82] = 0;
   out_1353467089812513678[83] = 0;
   out_1353467089812513678[84] = 0;
   out_1353467089812513678[85] = dt;
   out_1353467089812513678[86] = 0;
   out_1353467089812513678[87] = 0;
   out_1353467089812513678[88] = 0;
   out_1353467089812513678[89] = 0;
   out_1353467089812513678[90] = 0;
   out_1353467089812513678[91] = 0;
   out_1353467089812513678[92] = 0;
   out_1353467089812513678[93] = 0;
   out_1353467089812513678[94] = 0;
   out_1353467089812513678[95] = 1;
   out_1353467089812513678[96] = 0;
   out_1353467089812513678[97] = 0;
   out_1353467089812513678[98] = 0;
   out_1353467089812513678[99] = 0;
   out_1353467089812513678[100] = 0;
   out_1353467089812513678[101] = 0;
   out_1353467089812513678[102] = 0;
   out_1353467089812513678[103] = 0;
   out_1353467089812513678[104] = dt;
   out_1353467089812513678[105] = 0;
   out_1353467089812513678[106] = 0;
   out_1353467089812513678[107] = 0;
   out_1353467089812513678[108] = 0;
   out_1353467089812513678[109] = 0;
   out_1353467089812513678[110] = 0;
   out_1353467089812513678[111] = 0;
   out_1353467089812513678[112] = 0;
   out_1353467089812513678[113] = 0;
   out_1353467089812513678[114] = 1;
   out_1353467089812513678[115] = 0;
   out_1353467089812513678[116] = 0;
   out_1353467089812513678[117] = 0;
   out_1353467089812513678[118] = 0;
   out_1353467089812513678[119] = 0;
   out_1353467089812513678[120] = 0;
   out_1353467089812513678[121] = 0;
   out_1353467089812513678[122] = 0;
   out_1353467089812513678[123] = 0;
   out_1353467089812513678[124] = 0;
   out_1353467089812513678[125] = 0;
   out_1353467089812513678[126] = 0;
   out_1353467089812513678[127] = 0;
   out_1353467089812513678[128] = 0;
   out_1353467089812513678[129] = 0;
   out_1353467089812513678[130] = 0;
   out_1353467089812513678[131] = 0;
   out_1353467089812513678[132] = 0;
   out_1353467089812513678[133] = 1;
   out_1353467089812513678[134] = 0;
   out_1353467089812513678[135] = 0;
   out_1353467089812513678[136] = 0;
   out_1353467089812513678[137] = 0;
   out_1353467089812513678[138] = 0;
   out_1353467089812513678[139] = 0;
   out_1353467089812513678[140] = 0;
   out_1353467089812513678[141] = 0;
   out_1353467089812513678[142] = 0;
   out_1353467089812513678[143] = 0;
   out_1353467089812513678[144] = 0;
   out_1353467089812513678[145] = 0;
   out_1353467089812513678[146] = 0;
   out_1353467089812513678[147] = 0;
   out_1353467089812513678[148] = 0;
   out_1353467089812513678[149] = 0;
   out_1353467089812513678[150] = 0;
   out_1353467089812513678[151] = 0;
   out_1353467089812513678[152] = 1;
   out_1353467089812513678[153] = 0;
   out_1353467089812513678[154] = 0;
   out_1353467089812513678[155] = 0;
   out_1353467089812513678[156] = 0;
   out_1353467089812513678[157] = 0;
   out_1353467089812513678[158] = 0;
   out_1353467089812513678[159] = 0;
   out_1353467089812513678[160] = 0;
   out_1353467089812513678[161] = 0;
   out_1353467089812513678[162] = 0;
   out_1353467089812513678[163] = 0;
   out_1353467089812513678[164] = 0;
   out_1353467089812513678[165] = 0;
   out_1353467089812513678[166] = 0;
   out_1353467089812513678[167] = 0;
   out_1353467089812513678[168] = 0;
   out_1353467089812513678[169] = 0;
   out_1353467089812513678[170] = 0;
   out_1353467089812513678[171] = 1;
   out_1353467089812513678[172] = 0;
   out_1353467089812513678[173] = 0;
   out_1353467089812513678[174] = 0;
   out_1353467089812513678[175] = 0;
   out_1353467089812513678[176] = 0;
   out_1353467089812513678[177] = 0;
   out_1353467089812513678[178] = 0;
   out_1353467089812513678[179] = 0;
   out_1353467089812513678[180] = 0;
   out_1353467089812513678[181] = 0;
   out_1353467089812513678[182] = 0;
   out_1353467089812513678[183] = 0;
   out_1353467089812513678[184] = 0;
   out_1353467089812513678[185] = 0;
   out_1353467089812513678[186] = 0;
   out_1353467089812513678[187] = 0;
   out_1353467089812513678[188] = 0;
   out_1353467089812513678[189] = 0;
   out_1353467089812513678[190] = 1;
   out_1353467089812513678[191] = 0;
   out_1353467089812513678[192] = 0;
   out_1353467089812513678[193] = 0;
   out_1353467089812513678[194] = 0;
   out_1353467089812513678[195] = 0;
   out_1353467089812513678[196] = 0;
   out_1353467089812513678[197] = 0;
   out_1353467089812513678[198] = 0;
   out_1353467089812513678[199] = 0;
   out_1353467089812513678[200] = 0;
   out_1353467089812513678[201] = 0;
   out_1353467089812513678[202] = 0;
   out_1353467089812513678[203] = 0;
   out_1353467089812513678[204] = 0;
   out_1353467089812513678[205] = 0;
   out_1353467089812513678[206] = 0;
   out_1353467089812513678[207] = 0;
   out_1353467089812513678[208] = 0;
   out_1353467089812513678[209] = 1;
   out_1353467089812513678[210] = 0;
   out_1353467089812513678[211] = 0;
   out_1353467089812513678[212] = 0;
   out_1353467089812513678[213] = 0;
   out_1353467089812513678[214] = 0;
   out_1353467089812513678[215] = 0;
   out_1353467089812513678[216] = 0;
   out_1353467089812513678[217] = 0;
   out_1353467089812513678[218] = 0;
   out_1353467089812513678[219] = 0;
   out_1353467089812513678[220] = 0;
   out_1353467089812513678[221] = 0;
   out_1353467089812513678[222] = 0;
   out_1353467089812513678[223] = 0;
   out_1353467089812513678[224] = 0;
   out_1353467089812513678[225] = 0;
   out_1353467089812513678[226] = 0;
   out_1353467089812513678[227] = 0;
   out_1353467089812513678[228] = 1;
   out_1353467089812513678[229] = 0;
   out_1353467089812513678[230] = 0;
   out_1353467089812513678[231] = 0;
   out_1353467089812513678[232] = 0;
   out_1353467089812513678[233] = 0;
   out_1353467089812513678[234] = 0;
   out_1353467089812513678[235] = 0;
   out_1353467089812513678[236] = 0;
   out_1353467089812513678[237] = 0;
   out_1353467089812513678[238] = 0;
   out_1353467089812513678[239] = 0;
   out_1353467089812513678[240] = 0;
   out_1353467089812513678[241] = 0;
   out_1353467089812513678[242] = 0;
   out_1353467089812513678[243] = 0;
   out_1353467089812513678[244] = 0;
   out_1353467089812513678[245] = 0;
   out_1353467089812513678[246] = 0;
   out_1353467089812513678[247] = 1;
   out_1353467089812513678[248] = 0;
   out_1353467089812513678[249] = 0;
   out_1353467089812513678[250] = 0;
   out_1353467089812513678[251] = 0;
   out_1353467089812513678[252] = 0;
   out_1353467089812513678[253] = 0;
   out_1353467089812513678[254] = 0;
   out_1353467089812513678[255] = 0;
   out_1353467089812513678[256] = 0;
   out_1353467089812513678[257] = 0;
   out_1353467089812513678[258] = 0;
   out_1353467089812513678[259] = 0;
   out_1353467089812513678[260] = 0;
   out_1353467089812513678[261] = 0;
   out_1353467089812513678[262] = 0;
   out_1353467089812513678[263] = 0;
   out_1353467089812513678[264] = 0;
   out_1353467089812513678[265] = 0;
   out_1353467089812513678[266] = 1;
   out_1353467089812513678[267] = 0;
   out_1353467089812513678[268] = 0;
   out_1353467089812513678[269] = 0;
   out_1353467089812513678[270] = 0;
   out_1353467089812513678[271] = 0;
   out_1353467089812513678[272] = 0;
   out_1353467089812513678[273] = 0;
   out_1353467089812513678[274] = 0;
   out_1353467089812513678[275] = 0;
   out_1353467089812513678[276] = 0;
   out_1353467089812513678[277] = 0;
   out_1353467089812513678[278] = 0;
   out_1353467089812513678[279] = 0;
   out_1353467089812513678[280] = 0;
   out_1353467089812513678[281] = 0;
   out_1353467089812513678[282] = 0;
   out_1353467089812513678[283] = 0;
   out_1353467089812513678[284] = 0;
   out_1353467089812513678[285] = 1;
   out_1353467089812513678[286] = 0;
   out_1353467089812513678[287] = 0;
   out_1353467089812513678[288] = 0;
   out_1353467089812513678[289] = 0;
   out_1353467089812513678[290] = 0;
   out_1353467089812513678[291] = 0;
   out_1353467089812513678[292] = 0;
   out_1353467089812513678[293] = 0;
   out_1353467089812513678[294] = 0;
   out_1353467089812513678[295] = 0;
   out_1353467089812513678[296] = 0;
   out_1353467089812513678[297] = 0;
   out_1353467089812513678[298] = 0;
   out_1353467089812513678[299] = 0;
   out_1353467089812513678[300] = 0;
   out_1353467089812513678[301] = 0;
   out_1353467089812513678[302] = 0;
   out_1353467089812513678[303] = 0;
   out_1353467089812513678[304] = 1;
   out_1353467089812513678[305] = 0;
   out_1353467089812513678[306] = 0;
   out_1353467089812513678[307] = 0;
   out_1353467089812513678[308] = 0;
   out_1353467089812513678[309] = 0;
   out_1353467089812513678[310] = 0;
   out_1353467089812513678[311] = 0;
   out_1353467089812513678[312] = 0;
   out_1353467089812513678[313] = 0;
   out_1353467089812513678[314] = 0;
   out_1353467089812513678[315] = 0;
   out_1353467089812513678[316] = 0;
   out_1353467089812513678[317] = 0;
   out_1353467089812513678[318] = 0;
   out_1353467089812513678[319] = 0;
   out_1353467089812513678[320] = 0;
   out_1353467089812513678[321] = 0;
   out_1353467089812513678[322] = 0;
   out_1353467089812513678[323] = 1;
}
void h_4(double *state, double *unused, double *out_1851635960522651698) {
   out_1851635960522651698[0] = state[6] + state[9];
   out_1851635960522651698[1] = state[7] + state[10];
   out_1851635960522651698[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_3399555403264820134) {
   out_3399555403264820134[0] = 0;
   out_3399555403264820134[1] = 0;
   out_3399555403264820134[2] = 0;
   out_3399555403264820134[3] = 0;
   out_3399555403264820134[4] = 0;
   out_3399555403264820134[5] = 0;
   out_3399555403264820134[6] = 1;
   out_3399555403264820134[7] = 0;
   out_3399555403264820134[8] = 0;
   out_3399555403264820134[9] = 1;
   out_3399555403264820134[10] = 0;
   out_3399555403264820134[11] = 0;
   out_3399555403264820134[12] = 0;
   out_3399555403264820134[13] = 0;
   out_3399555403264820134[14] = 0;
   out_3399555403264820134[15] = 0;
   out_3399555403264820134[16] = 0;
   out_3399555403264820134[17] = 0;
   out_3399555403264820134[18] = 0;
   out_3399555403264820134[19] = 0;
   out_3399555403264820134[20] = 0;
   out_3399555403264820134[21] = 0;
   out_3399555403264820134[22] = 0;
   out_3399555403264820134[23] = 0;
   out_3399555403264820134[24] = 0;
   out_3399555403264820134[25] = 1;
   out_3399555403264820134[26] = 0;
   out_3399555403264820134[27] = 0;
   out_3399555403264820134[28] = 1;
   out_3399555403264820134[29] = 0;
   out_3399555403264820134[30] = 0;
   out_3399555403264820134[31] = 0;
   out_3399555403264820134[32] = 0;
   out_3399555403264820134[33] = 0;
   out_3399555403264820134[34] = 0;
   out_3399555403264820134[35] = 0;
   out_3399555403264820134[36] = 0;
   out_3399555403264820134[37] = 0;
   out_3399555403264820134[38] = 0;
   out_3399555403264820134[39] = 0;
   out_3399555403264820134[40] = 0;
   out_3399555403264820134[41] = 0;
   out_3399555403264820134[42] = 0;
   out_3399555403264820134[43] = 0;
   out_3399555403264820134[44] = 1;
   out_3399555403264820134[45] = 0;
   out_3399555403264820134[46] = 0;
   out_3399555403264820134[47] = 1;
   out_3399555403264820134[48] = 0;
   out_3399555403264820134[49] = 0;
   out_3399555403264820134[50] = 0;
   out_3399555403264820134[51] = 0;
   out_3399555403264820134[52] = 0;
   out_3399555403264820134[53] = 0;
}
void h_10(double *state, double *unused, double *out_3439758590286442773) {
   out_3439758590286442773[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_3439758590286442773[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_3439758590286442773[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_3773575779738063167) {
   out_3773575779738063167[0] = 0;
   out_3773575779738063167[1] = 9.8100000000000005*cos(state[1]);
   out_3773575779738063167[2] = 0;
   out_3773575779738063167[3] = 0;
   out_3773575779738063167[4] = -state[8];
   out_3773575779738063167[5] = state[7];
   out_3773575779738063167[6] = 0;
   out_3773575779738063167[7] = state[5];
   out_3773575779738063167[8] = -state[4];
   out_3773575779738063167[9] = 0;
   out_3773575779738063167[10] = 0;
   out_3773575779738063167[11] = 0;
   out_3773575779738063167[12] = 1;
   out_3773575779738063167[13] = 0;
   out_3773575779738063167[14] = 0;
   out_3773575779738063167[15] = 1;
   out_3773575779738063167[16] = 0;
   out_3773575779738063167[17] = 0;
   out_3773575779738063167[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_3773575779738063167[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_3773575779738063167[20] = 0;
   out_3773575779738063167[21] = state[8];
   out_3773575779738063167[22] = 0;
   out_3773575779738063167[23] = -state[6];
   out_3773575779738063167[24] = -state[5];
   out_3773575779738063167[25] = 0;
   out_3773575779738063167[26] = state[3];
   out_3773575779738063167[27] = 0;
   out_3773575779738063167[28] = 0;
   out_3773575779738063167[29] = 0;
   out_3773575779738063167[30] = 0;
   out_3773575779738063167[31] = 1;
   out_3773575779738063167[32] = 0;
   out_3773575779738063167[33] = 0;
   out_3773575779738063167[34] = 1;
   out_3773575779738063167[35] = 0;
   out_3773575779738063167[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_3773575779738063167[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_3773575779738063167[38] = 0;
   out_3773575779738063167[39] = -state[7];
   out_3773575779738063167[40] = state[6];
   out_3773575779738063167[41] = 0;
   out_3773575779738063167[42] = state[4];
   out_3773575779738063167[43] = -state[3];
   out_3773575779738063167[44] = 0;
   out_3773575779738063167[45] = 0;
   out_3773575779738063167[46] = 0;
   out_3773575779738063167[47] = 0;
   out_3773575779738063167[48] = 0;
   out_3773575779738063167[49] = 0;
   out_3773575779738063167[50] = 1;
   out_3773575779738063167[51] = 0;
   out_3773575779738063167[52] = 0;
   out_3773575779738063167[53] = 1;
}
void h_13(double *state, double *unused, double *out_6062599988059421211) {
   out_6062599988059421211[0] = state[3];
   out_6062599988059421211[1] = state[4];
   out_6062599988059421211[2] = state[5];
}
void H_13(double *state, double *unused, double *out_6611829228597152935) {
   out_6611829228597152935[0] = 0;
   out_6611829228597152935[1] = 0;
   out_6611829228597152935[2] = 0;
   out_6611829228597152935[3] = 1;
   out_6611829228597152935[4] = 0;
   out_6611829228597152935[5] = 0;
   out_6611829228597152935[6] = 0;
   out_6611829228597152935[7] = 0;
   out_6611829228597152935[8] = 0;
   out_6611829228597152935[9] = 0;
   out_6611829228597152935[10] = 0;
   out_6611829228597152935[11] = 0;
   out_6611829228597152935[12] = 0;
   out_6611829228597152935[13] = 0;
   out_6611829228597152935[14] = 0;
   out_6611829228597152935[15] = 0;
   out_6611829228597152935[16] = 0;
   out_6611829228597152935[17] = 0;
   out_6611829228597152935[18] = 0;
   out_6611829228597152935[19] = 0;
   out_6611829228597152935[20] = 0;
   out_6611829228597152935[21] = 0;
   out_6611829228597152935[22] = 1;
   out_6611829228597152935[23] = 0;
   out_6611829228597152935[24] = 0;
   out_6611829228597152935[25] = 0;
   out_6611829228597152935[26] = 0;
   out_6611829228597152935[27] = 0;
   out_6611829228597152935[28] = 0;
   out_6611829228597152935[29] = 0;
   out_6611829228597152935[30] = 0;
   out_6611829228597152935[31] = 0;
   out_6611829228597152935[32] = 0;
   out_6611829228597152935[33] = 0;
   out_6611829228597152935[34] = 0;
   out_6611829228597152935[35] = 0;
   out_6611829228597152935[36] = 0;
   out_6611829228597152935[37] = 0;
   out_6611829228597152935[38] = 0;
   out_6611829228597152935[39] = 0;
   out_6611829228597152935[40] = 0;
   out_6611829228597152935[41] = 1;
   out_6611829228597152935[42] = 0;
   out_6611829228597152935[43] = 0;
   out_6611829228597152935[44] = 0;
   out_6611829228597152935[45] = 0;
   out_6611829228597152935[46] = 0;
   out_6611829228597152935[47] = 0;
   out_6611829228597152935[48] = 0;
   out_6611829228597152935[49] = 0;
   out_6611829228597152935[50] = 0;
   out_6611829228597152935[51] = 0;
   out_6611829228597152935[52] = 0;
   out_6611829228597152935[53] = 0;
}
void h_14(double *state, double *unused, double *out_5840628258535751267) {
   out_5840628258535751267[0] = state[6];
   out_5840628258535751267[1] = state[7];
   out_5840628258535751267[2] = state[8];
}
void H_14(double *state, double *unused, double *out_316766970969447838) {
   out_316766970969447838[0] = 0;
   out_316766970969447838[1] = 0;
   out_316766970969447838[2] = 0;
   out_316766970969447838[3] = 0;
   out_316766970969447838[4] = 0;
   out_316766970969447838[5] = 0;
   out_316766970969447838[6] = 1;
   out_316766970969447838[7] = 0;
   out_316766970969447838[8] = 0;
   out_316766970969447838[9] = 0;
   out_316766970969447838[10] = 0;
   out_316766970969447838[11] = 0;
   out_316766970969447838[12] = 0;
   out_316766970969447838[13] = 0;
   out_316766970969447838[14] = 0;
   out_316766970969447838[15] = 0;
   out_316766970969447838[16] = 0;
   out_316766970969447838[17] = 0;
   out_316766970969447838[18] = 0;
   out_316766970969447838[19] = 0;
   out_316766970969447838[20] = 0;
   out_316766970969447838[21] = 0;
   out_316766970969447838[22] = 0;
   out_316766970969447838[23] = 0;
   out_316766970969447838[24] = 0;
   out_316766970969447838[25] = 1;
   out_316766970969447838[26] = 0;
   out_316766970969447838[27] = 0;
   out_316766970969447838[28] = 0;
   out_316766970969447838[29] = 0;
   out_316766970969447838[30] = 0;
   out_316766970969447838[31] = 0;
   out_316766970969447838[32] = 0;
   out_316766970969447838[33] = 0;
   out_316766970969447838[34] = 0;
   out_316766970969447838[35] = 0;
   out_316766970969447838[36] = 0;
   out_316766970969447838[37] = 0;
   out_316766970969447838[38] = 0;
   out_316766970969447838[39] = 0;
   out_316766970969447838[40] = 0;
   out_316766970969447838[41] = 0;
   out_316766970969447838[42] = 0;
   out_316766970969447838[43] = 0;
   out_316766970969447838[44] = 1;
   out_316766970969447838[45] = 0;
   out_316766970969447838[46] = 0;
   out_316766970969447838[47] = 0;
   out_316766970969447838[48] = 0;
   out_316766970969447838[49] = 0;
   out_316766970969447838[50] = 0;
   out_316766970969447838[51] = 0;
   out_316766970969447838[52] = 0;
   out_316766970969447838[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_5709026608963937263) {
  err_fun(nom_x, delta_x, out_5709026608963937263);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9100639615517483437) {
  inv_err_fun(nom_x, true_x, out_9100639615517483437);
}
void pose_H_mod_fun(double *state, double *out_372251806579087438) {
  H_mod_fun(state, out_372251806579087438);
}
void pose_f_fun(double *state, double dt, double *out_1740152809646530052) {
  f_fun(state,  dt, out_1740152809646530052);
}
void pose_F_fun(double *state, double dt, double *out_1353467089812513678) {
  F_fun(state,  dt, out_1353467089812513678);
}
void pose_h_4(double *state, double *unused, double *out_1851635960522651698) {
  h_4(state, unused, out_1851635960522651698);
}
void pose_H_4(double *state, double *unused, double *out_3399555403264820134) {
  H_4(state, unused, out_3399555403264820134);
}
void pose_h_10(double *state, double *unused, double *out_3439758590286442773) {
  h_10(state, unused, out_3439758590286442773);
}
void pose_H_10(double *state, double *unused, double *out_3773575779738063167) {
  H_10(state, unused, out_3773575779738063167);
}
void pose_h_13(double *state, double *unused, double *out_6062599988059421211) {
  h_13(state, unused, out_6062599988059421211);
}
void pose_H_13(double *state, double *unused, double *out_6611829228597152935) {
  H_13(state, unused, out_6611829228597152935);
}
void pose_h_14(double *state, double *unused, double *out_5840628258535751267) {
  h_14(state, unused, out_5840628258535751267);
}
void pose_H_14(double *state, double *unused, double *out_316766970969447838) {
  H_14(state, unused, out_316766970969447838);
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
