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
void err_fun(double *nom_x, double *delta_x, double *out_6129668094290001530) {
   out_6129668094290001530[0] = delta_x[0] + nom_x[0];
   out_6129668094290001530[1] = delta_x[1] + nom_x[1];
   out_6129668094290001530[2] = delta_x[2] + nom_x[2];
   out_6129668094290001530[3] = delta_x[3] + nom_x[3];
   out_6129668094290001530[4] = delta_x[4] + nom_x[4];
   out_6129668094290001530[5] = delta_x[5] + nom_x[5];
   out_6129668094290001530[6] = delta_x[6] + nom_x[6];
   out_6129668094290001530[7] = delta_x[7] + nom_x[7];
   out_6129668094290001530[8] = delta_x[8] + nom_x[8];
   out_6129668094290001530[9] = delta_x[9] + nom_x[9];
   out_6129668094290001530[10] = delta_x[10] + nom_x[10];
   out_6129668094290001530[11] = delta_x[11] + nom_x[11];
   out_6129668094290001530[12] = delta_x[12] + nom_x[12];
   out_6129668094290001530[13] = delta_x[13] + nom_x[13];
   out_6129668094290001530[14] = delta_x[14] + nom_x[14];
   out_6129668094290001530[15] = delta_x[15] + nom_x[15];
   out_6129668094290001530[16] = delta_x[16] + nom_x[16];
   out_6129668094290001530[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8035171941691199446) {
   out_8035171941691199446[0] = -nom_x[0] + true_x[0];
   out_8035171941691199446[1] = -nom_x[1] + true_x[1];
   out_8035171941691199446[2] = -nom_x[2] + true_x[2];
   out_8035171941691199446[3] = -nom_x[3] + true_x[3];
   out_8035171941691199446[4] = -nom_x[4] + true_x[4];
   out_8035171941691199446[5] = -nom_x[5] + true_x[5];
   out_8035171941691199446[6] = -nom_x[6] + true_x[6];
   out_8035171941691199446[7] = -nom_x[7] + true_x[7];
   out_8035171941691199446[8] = -nom_x[8] + true_x[8];
   out_8035171941691199446[9] = -nom_x[9] + true_x[9];
   out_8035171941691199446[10] = -nom_x[10] + true_x[10];
   out_8035171941691199446[11] = -nom_x[11] + true_x[11];
   out_8035171941691199446[12] = -nom_x[12] + true_x[12];
   out_8035171941691199446[13] = -nom_x[13] + true_x[13];
   out_8035171941691199446[14] = -nom_x[14] + true_x[14];
   out_8035171941691199446[15] = -nom_x[15] + true_x[15];
   out_8035171941691199446[16] = -nom_x[16] + true_x[16];
   out_8035171941691199446[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_1839861344935556007) {
   out_1839861344935556007[0] = 1.0;
   out_1839861344935556007[1] = 0.0;
   out_1839861344935556007[2] = 0.0;
   out_1839861344935556007[3] = 0.0;
   out_1839861344935556007[4] = 0.0;
   out_1839861344935556007[5] = 0.0;
   out_1839861344935556007[6] = 0.0;
   out_1839861344935556007[7] = 0.0;
   out_1839861344935556007[8] = 0.0;
   out_1839861344935556007[9] = 0.0;
   out_1839861344935556007[10] = 0.0;
   out_1839861344935556007[11] = 0.0;
   out_1839861344935556007[12] = 0.0;
   out_1839861344935556007[13] = 0.0;
   out_1839861344935556007[14] = 0.0;
   out_1839861344935556007[15] = 0.0;
   out_1839861344935556007[16] = 0.0;
   out_1839861344935556007[17] = 0.0;
   out_1839861344935556007[18] = 0.0;
   out_1839861344935556007[19] = 1.0;
   out_1839861344935556007[20] = 0.0;
   out_1839861344935556007[21] = 0.0;
   out_1839861344935556007[22] = 0.0;
   out_1839861344935556007[23] = 0.0;
   out_1839861344935556007[24] = 0.0;
   out_1839861344935556007[25] = 0.0;
   out_1839861344935556007[26] = 0.0;
   out_1839861344935556007[27] = 0.0;
   out_1839861344935556007[28] = 0.0;
   out_1839861344935556007[29] = 0.0;
   out_1839861344935556007[30] = 0.0;
   out_1839861344935556007[31] = 0.0;
   out_1839861344935556007[32] = 0.0;
   out_1839861344935556007[33] = 0.0;
   out_1839861344935556007[34] = 0.0;
   out_1839861344935556007[35] = 0.0;
   out_1839861344935556007[36] = 0.0;
   out_1839861344935556007[37] = 0.0;
   out_1839861344935556007[38] = 1.0;
   out_1839861344935556007[39] = 0.0;
   out_1839861344935556007[40] = 0.0;
   out_1839861344935556007[41] = 0.0;
   out_1839861344935556007[42] = 0.0;
   out_1839861344935556007[43] = 0.0;
   out_1839861344935556007[44] = 0.0;
   out_1839861344935556007[45] = 0.0;
   out_1839861344935556007[46] = 0.0;
   out_1839861344935556007[47] = 0.0;
   out_1839861344935556007[48] = 0.0;
   out_1839861344935556007[49] = 0.0;
   out_1839861344935556007[50] = 0.0;
   out_1839861344935556007[51] = 0.0;
   out_1839861344935556007[52] = 0.0;
   out_1839861344935556007[53] = 0.0;
   out_1839861344935556007[54] = 0.0;
   out_1839861344935556007[55] = 0.0;
   out_1839861344935556007[56] = 0.0;
   out_1839861344935556007[57] = 1.0;
   out_1839861344935556007[58] = 0.0;
   out_1839861344935556007[59] = 0.0;
   out_1839861344935556007[60] = 0.0;
   out_1839861344935556007[61] = 0.0;
   out_1839861344935556007[62] = 0.0;
   out_1839861344935556007[63] = 0.0;
   out_1839861344935556007[64] = 0.0;
   out_1839861344935556007[65] = 0.0;
   out_1839861344935556007[66] = 0.0;
   out_1839861344935556007[67] = 0.0;
   out_1839861344935556007[68] = 0.0;
   out_1839861344935556007[69] = 0.0;
   out_1839861344935556007[70] = 0.0;
   out_1839861344935556007[71] = 0.0;
   out_1839861344935556007[72] = 0.0;
   out_1839861344935556007[73] = 0.0;
   out_1839861344935556007[74] = 0.0;
   out_1839861344935556007[75] = 0.0;
   out_1839861344935556007[76] = 1.0;
   out_1839861344935556007[77] = 0.0;
   out_1839861344935556007[78] = 0.0;
   out_1839861344935556007[79] = 0.0;
   out_1839861344935556007[80] = 0.0;
   out_1839861344935556007[81] = 0.0;
   out_1839861344935556007[82] = 0.0;
   out_1839861344935556007[83] = 0.0;
   out_1839861344935556007[84] = 0.0;
   out_1839861344935556007[85] = 0.0;
   out_1839861344935556007[86] = 0.0;
   out_1839861344935556007[87] = 0.0;
   out_1839861344935556007[88] = 0.0;
   out_1839861344935556007[89] = 0.0;
   out_1839861344935556007[90] = 0.0;
   out_1839861344935556007[91] = 0.0;
   out_1839861344935556007[92] = 0.0;
   out_1839861344935556007[93] = 0.0;
   out_1839861344935556007[94] = 0.0;
   out_1839861344935556007[95] = 1.0;
   out_1839861344935556007[96] = 0.0;
   out_1839861344935556007[97] = 0.0;
   out_1839861344935556007[98] = 0.0;
   out_1839861344935556007[99] = 0.0;
   out_1839861344935556007[100] = 0.0;
   out_1839861344935556007[101] = 0.0;
   out_1839861344935556007[102] = 0.0;
   out_1839861344935556007[103] = 0.0;
   out_1839861344935556007[104] = 0.0;
   out_1839861344935556007[105] = 0.0;
   out_1839861344935556007[106] = 0.0;
   out_1839861344935556007[107] = 0.0;
   out_1839861344935556007[108] = 0.0;
   out_1839861344935556007[109] = 0.0;
   out_1839861344935556007[110] = 0.0;
   out_1839861344935556007[111] = 0.0;
   out_1839861344935556007[112] = 0.0;
   out_1839861344935556007[113] = 0.0;
   out_1839861344935556007[114] = 1.0;
   out_1839861344935556007[115] = 0.0;
   out_1839861344935556007[116] = 0.0;
   out_1839861344935556007[117] = 0.0;
   out_1839861344935556007[118] = 0.0;
   out_1839861344935556007[119] = 0.0;
   out_1839861344935556007[120] = 0.0;
   out_1839861344935556007[121] = 0.0;
   out_1839861344935556007[122] = 0.0;
   out_1839861344935556007[123] = 0.0;
   out_1839861344935556007[124] = 0.0;
   out_1839861344935556007[125] = 0.0;
   out_1839861344935556007[126] = 0.0;
   out_1839861344935556007[127] = 0.0;
   out_1839861344935556007[128] = 0.0;
   out_1839861344935556007[129] = 0.0;
   out_1839861344935556007[130] = 0.0;
   out_1839861344935556007[131] = 0.0;
   out_1839861344935556007[132] = 0.0;
   out_1839861344935556007[133] = 1.0;
   out_1839861344935556007[134] = 0.0;
   out_1839861344935556007[135] = 0.0;
   out_1839861344935556007[136] = 0.0;
   out_1839861344935556007[137] = 0.0;
   out_1839861344935556007[138] = 0.0;
   out_1839861344935556007[139] = 0.0;
   out_1839861344935556007[140] = 0.0;
   out_1839861344935556007[141] = 0.0;
   out_1839861344935556007[142] = 0.0;
   out_1839861344935556007[143] = 0.0;
   out_1839861344935556007[144] = 0.0;
   out_1839861344935556007[145] = 0.0;
   out_1839861344935556007[146] = 0.0;
   out_1839861344935556007[147] = 0.0;
   out_1839861344935556007[148] = 0.0;
   out_1839861344935556007[149] = 0.0;
   out_1839861344935556007[150] = 0.0;
   out_1839861344935556007[151] = 0.0;
   out_1839861344935556007[152] = 1.0;
   out_1839861344935556007[153] = 0.0;
   out_1839861344935556007[154] = 0.0;
   out_1839861344935556007[155] = 0.0;
   out_1839861344935556007[156] = 0.0;
   out_1839861344935556007[157] = 0.0;
   out_1839861344935556007[158] = 0.0;
   out_1839861344935556007[159] = 0.0;
   out_1839861344935556007[160] = 0.0;
   out_1839861344935556007[161] = 0.0;
   out_1839861344935556007[162] = 0.0;
   out_1839861344935556007[163] = 0.0;
   out_1839861344935556007[164] = 0.0;
   out_1839861344935556007[165] = 0.0;
   out_1839861344935556007[166] = 0.0;
   out_1839861344935556007[167] = 0.0;
   out_1839861344935556007[168] = 0.0;
   out_1839861344935556007[169] = 0.0;
   out_1839861344935556007[170] = 0.0;
   out_1839861344935556007[171] = 1.0;
   out_1839861344935556007[172] = 0.0;
   out_1839861344935556007[173] = 0.0;
   out_1839861344935556007[174] = 0.0;
   out_1839861344935556007[175] = 0.0;
   out_1839861344935556007[176] = 0.0;
   out_1839861344935556007[177] = 0.0;
   out_1839861344935556007[178] = 0.0;
   out_1839861344935556007[179] = 0.0;
   out_1839861344935556007[180] = 0.0;
   out_1839861344935556007[181] = 0.0;
   out_1839861344935556007[182] = 0.0;
   out_1839861344935556007[183] = 0.0;
   out_1839861344935556007[184] = 0.0;
   out_1839861344935556007[185] = 0.0;
   out_1839861344935556007[186] = 0.0;
   out_1839861344935556007[187] = 0.0;
   out_1839861344935556007[188] = 0.0;
   out_1839861344935556007[189] = 0.0;
   out_1839861344935556007[190] = 1.0;
   out_1839861344935556007[191] = 0.0;
   out_1839861344935556007[192] = 0.0;
   out_1839861344935556007[193] = 0.0;
   out_1839861344935556007[194] = 0.0;
   out_1839861344935556007[195] = 0.0;
   out_1839861344935556007[196] = 0.0;
   out_1839861344935556007[197] = 0.0;
   out_1839861344935556007[198] = 0.0;
   out_1839861344935556007[199] = 0.0;
   out_1839861344935556007[200] = 0.0;
   out_1839861344935556007[201] = 0.0;
   out_1839861344935556007[202] = 0.0;
   out_1839861344935556007[203] = 0.0;
   out_1839861344935556007[204] = 0.0;
   out_1839861344935556007[205] = 0.0;
   out_1839861344935556007[206] = 0.0;
   out_1839861344935556007[207] = 0.0;
   out_1839861344935556007[208] = 0.0;
   out_1839861344935556007[209] = 1.0;
   out_1839861344935556007[210] = 0.0;
   out_1839861344935556007[211] = 0.0;
   out_1839861344935556007[212] = 0.0;
   out_1839861344935556007[213] = 0.0;
   out_1839861344935556007[214] = 0.0;
   out_1839861344935556007[215] = 0.0;
   out_1839861344935556007[216] = 0.0;
   out_1839861344935556007[217] = 0.0;
   out_1839861344935556007[218] = 0.0;
   out_1839861344935556007[219] = 0.0;
   out_1839861344935556007[220] = 0.0;
   out_1839861344935556007[221] = 0.0;
   out_1839861344935556007[222] = 0.0;
   out_1839861344935556007[223] = 0.0;
   out_1839861344935556007[224] = 0.0;
   out_1839861344935556007[225] = 0.0;
   out_1839861344935556007[226] = 0.0;
   out_1839861344935556007[227] = 0.0;
   out_1839861344935556007[228] = 1.0;
   out_1839861344935556007[229] = 0.0;
   out_1839861344935556007[230] = 0.0;
   out_1839861344935556007[231] = 0.0;
   out_1839861344935556007[232] = 0.0;
   out_1839861344935556007[233] = 0.0;
   out_1839861344935556007[234] = 0.0;
   out_1839861344935556007[235] = 0.0;
   out_1839861344935556007[236] = 0.0;
   out_1839861344935556007[237] = 0.0;
   out_1839861344935556007[238] = 0.0;
   out_1839861344935556007[239] = 0.0;
   out_1839861344935556007[240] = 0.0;
   out_1839861344935556007[241] = 0.0;
   out_1839861344935556007[242] = 0.0;
   out_1839861344935556007[243] = 0.0;
   out_1839861344935556007[244] = 0.0;
   out_1839861344935556007[245] = 0.0;
   out_1839861344935556007[246] = 0.0;
   out_1839861344935556007[247] = 1.0;
   out_1839861344935556007[248] = 0.0;
   out_1839861344935556007[249] = 0.0;
   out_1839861344935556007[250] = 0.0;
   out_1839861344935556007[251] = 0.0;
   out_1839861344935556007[252] = 0.0;
   out_1839861344935556007[253] = 0.0;
   out_1839861344935556007[254] = 0.0;
   out_1839861344935556007[255] = 0.0;
   out_1839861344935556007[256] = 0.0;
   out_1839861344935556007[257] = 0.0;
   out_1839861344935556007[258] = 0.0;
   out_1839861344935556007[259] = 0.0;
   out_1839861344935556007[260] = 0.0;
   out_1839861344935556007[261] = 0.0;
   out_1839861344935556007[262] = 0.0;
   out_1839861344935556007[263] = 0.0;
   out_1839861344935556007[264] = 0.0;
   out_1839861344935556007[265] = 0.0;
   out_1839861344935556007[266] = 1.0;
   out_1839861344935556007[267] = 0.0;
   out_1839861344935556007[268] = 0.0;
   out_1839861344935556007[269] = 0.0;
   out_1839861344935556007[270] = 0.0;
   out_1839861344935556007[271] = 0.0;
   out_1839861344935556007[272] = 0.0;
   out_1839861344935556007[273] = 0.0;
   out_1839861344935556007[274] = 0.0;
   out_1839861344935556007[275] = 0.0;
   out_1839861344935556007[276] = 0.0;
   out_1839861344935556007[277] = 0.0;
   out_1839861344935556007[278] = 0.0;
   out_1839861344935556007[279] = 0.0;
   out_1839861344935556007[280] = 0.0;
   out_1839861344935556007[281] = 0.0;
   out_1839861344935556007[282] = 0.0;
   out_1839861344935556007[283] = 0.0;
   out_1839861344935556007[284] = 0.0;
   out_1839861344935556007[285] = 1.0;
   out_1839861344935556007[286] = 0.0;
   out_1839861344935556007[287] = 0.0;
   out_1839861344935556007[288] = 0.0;
   out_1839861344935556007[289] = 0.0;
   out_1839861344935556007[290] = 0.0;
   out_1839861344935556007[291] = 0.0;
   out_1839861344935556007[292] = 0.0;
   out_1839861344935556007[293] = 0.0;
   out_1839861344935556007[294] = 0.0;
   out_1839861344935556007[295] = 0.0;
   out_1839861344935556007[296] = 0.0;
   out_1839861344935556007[297] = 0.0;
   out_1839861344935556007[298] = 0.0;
   out_1839861344935556007[299] = 0.0;
   out_1839861344935556007[300] = 0.0;
   out_1839861344935556007[301] = 0.0;
   out_1839861344935556007[302] = 0.0;
   out_1839861344935556007[303] = 0.0;
   out_1839861344935556007[304] = 1.0;
   out_1839861344935556007[305] = 0.0;
   out_1839861344935556007[306] = 0.0;
   out_1839861344935556007[307] = 0.0;
   out_1839861344935556007[308] = 0.0;
   out_1839861344935556007[309] = 0.0;
   out_1839861344935556007[310] = 0.0;
   out_1839861344935556007[311] = 0.0;
   out_1839861344935556007[312] = 0.0;
   out_1839861344935556007[313] = 0.0;
   out_1839861344935556007[314] = 0.0;
   out_1839861344935556007[315] = 0.0;
   out_1839861344935556007[316] = 0.0;
   out_1839861344935556007[317] = 0.0;
   out_1839861344935556007[318] = 0.0;
   out_1839861344935556007[319] = 0.0;
   out_1839861344935556007[320] = 0.0;
   out_1839861344935556007[321] = 0.0;
   out_1839861344935556007[322] = 0.0;
   out_1839861344935556007[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5572599306340567392) {
   out_5572599306340567392[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5572599306340567392[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5572599306340567392[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5572599306340567392[3] = dt*state[12] + state[3];
   out_5572599306340567392[4] = dt*state[13] + state[4];
   out_5572599306340567392[5] = dt*state[14] + state[5];
   out_5572599306340567392[6] = state[6];
   out_5572599306340567392[7] = state[7];
   out_5572599306340567392[8] = state[8];
   out_5572599306340567392[9] = state[9];
   out_5572599306340567392[10] = state[10];
   out_5572599306340567392[11] = state[11];
   out_5572599306340567392[12] = state[12];
   out_5572599306340567392[13] = state[13];
   out_5572599306340567392[14] = state[14];
   out_5572599306340567392[15] = state[15];
   out_5572599306340567392[16] = state[16];
   out_5572599306340567392[17] = state[17];
}
void F_fun(double *state, double dt, double *out_7997319965218444246) {
   out_7997319965218444246[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7997319965218444246[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7997319965218444246[2] = 0;
   out_7997319965218444246[3] = 0;
   out_7997319965218444246[4] = 0;
   out_7997319965218444246[5] = 0;
   out_7997319965218444246[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7997319965218444246[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7997319965218444246[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7997319965218444246[9] = 0;
   out_7997319965218444246[10] = 0;
   out_7997319965218444246[11] = 0;
   out_7997319965218444246[12] = 0;
   out_7997319965218444246[13] = 0;
   out_7997319965218444246[14] = 0;
   out_7997319965218444246[15] = 0;
   out_7997319965218444246[16] = 0;
   out_7997319965218444246[17] = 0;
   out_7997319965218444246[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7997319965218444246[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7997319965218444246[20] = 0;
   out_7997319965218444246[21] = 0;
   out_7997319965218444246[22] = 0;
   out_7997319965218444246[23] = 0;
   out_7997319965218444246[24] = 0;
   out_7997319965218444246[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7997319965218444246[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7997319965218444246[27] = 0;
   out_7997319965218444246[28] = 0;
   out_7997319965218444246[29] = 0;
   out_7997319965218444246[30] = 0;
   out_7997319965218444246[31] = 0;
   out_7997319965218444246[32] = 0;
   out_7997319965218444246[33] = 0;
   out_7997319965218444246[34] = 0;
   out_7997319965218444246[35] = 0;
   out_7997319965218444246[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7997319965218444246[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7997319965218444246[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7997319965218444246[39] = 0;
   out_7997319965218444246[40] = 0;
   out_7997319965218444246[41] = 0;
   out_7997319965218444246[42] = 0;
   out_7997319965218444246[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7997319965218444246[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7997319965218444246[45] = 0;
   out_7997319965218444246[46] = 0;
   out_7997319965218444246[47] = 0;
   out_7997319965218444246[48] = 0;
   out_7997319965218444246[49] = 0;
   out_7997319965218444246[50] = 0;
   out_7997319965218444246[51] = 0;
   out_7997319965218444246[52] = 0;
   out_7997319965218444246[53] = 0;
   out_7997319965218444246[54] = 0;
   out_7997319965218444246[55] = 0;
   out_7997319965218444246[56] = 0;
   out_7997319965218444246[57] = 1;
   out_7997319965218444246[58] = 0;
   out_7997319965218444246[59] = 0;
   out_7997319965218444246[60] = 0;
   out_7997319965218444246[61] = 0;
   out_7997319965218444246[62] = 0;
   out_7997319965218444246[63] = 0;
   out_7997319965218444246[64] = 0;
   out_7997319965218444246[65] = 0;
   out_7997319965218444246[66] = dt;
   out_7997319965218444246[67] = 0;
   out_7997319965218444246[68] = 0;
   out_7997319965218444246[69] = 0;
   out_7997319965218444246[70] = 0;
   out_7997319965218444246[71] = 0;
   out_7997319965218444246[72] = 0;
   out_7997319965218444246[73] = 0;
   out_7997319965218444246[74] = 0;
   out_7997319965218444246[75] = 0;
   out_7997319965218444246[76] = 1;
   out_7997319965218444246[77] = 0;
   out_7997319965218444246[78] = 0;
   out_7997319965218444246[79] = 0;
   out_7997319965218444246[80] = 0;
   out_7997319965218444246[81] = 0;
   out_7997319965218444246[82] = 0;
   out_7997319965218444246[83] = 0;
   out_7997319965218444246[84] = 0;
   out_7997319965218444246[85] = dt;
   out_7997319965218444246[86] = 0;
   out_7997319965218444246[87] = 0;
   out_7997319965218444246[88] = 0;
   out_7997319965218444246[89] = 0;
   out_7997319965218444246[90] = 0;
   out_7997319965218444246[91] = 0;
   out_7997319965218444246[92] = 0;
   out_7997319965218444246[93] = 0;
   out_7997319965218444246[94] = 0;
   out_7997319965218444246[95] = 1;
   out_7997319965218444246[96] = 0;
   out_7997319965218444246[97] = 0;
   out_7997319965218444246[98] = 0;
   out_7997319965218444246[99] = 0;
   out_7997319965218444246[100] = 0;
   out_7997319965218444246[101] = 0;
   out_7997319965218444246[102] = 0;
   out_7997319965218444246[103] = 0;
   out_7997319965218444246[104] = dt;
   out_7997319965218444246[105] = 0;
   out_7997319965218444246[106] = 0;
   out_7997319965218444246[107] = 0;
   out_7997319965218444246[108] = 0;
   out_7997319965218444246[109] = 0;
   out_7997319965218444246[110] = 0;
   out_7997319965218444246[111] = 0;
   out_7997319965218444246[112] = 0;
   out_7997319965218444246[113] = 0;
   out_7997319965218444246[114] = 1;
   out_7997319965218444246[115] = 0;
   out_7997319965218444246[116] = 0;
   out_7997319965218444246[117] = 0;
   out_7997319965218444246[118] = 0;
   out_7997319965218444246[119] = 0;
   out_7997319965218444246[120] = 0;
   out_7997319965218444246[121] = 0;
   out_7997319965218444246[122] = 0;
   out_7997319965218444246[123] = 0;
   out_7997319965218444246[124] = 0;
   out_7997319965218444246[125] = 0;
   out_7997319965218444246[126] = 0;
   out_7997319965218444246[127] = 0;
   out_7997319965218444246[128] = 0;
   out_7997319965218444246[129] = 0;
   out_7997319965218444246[130] = 0;
   out_7997319965218444246[131] = 0;
   out_7997319965218444246[132] = 0;
   out_7997319965218444246[133] = 1;
   out_7997319965218444246[134] = 0;
   out_7997319965218444246[135] = 0;
   out_7997319965218444246[136] = 0;
   out_7997319965218444246[137] = 0;
   out_7997319965218444246[138] = 0;
   out_7997319965218444246[139] = 0;
   out_7997319965218444246[140] = 0;
   out_7997319965218444246[141] = 0;
   out_7997319965218444246[142] = 0;
   out_7997319965218444246[143] = 0;
   out_7997319965218444246[144] = 0;
   out_7997319965218444246[145] = 0;
   out_7997319965218444246[146] = 0;
   out_7997319965218444246[147] = 0;
   out_7997319965218444246[148] = 0;
   out_7997319965218444246[149] = 0;
   out_7997319965218444246[150] = 0;
   out_7997319965218444246[151] = 0;
   out_7997319965218444246[152] = 1;
   out_7997319965218444246[153] = 0;
   out_7997319965218444246[154] = 0;
   out_7997319965218444246[155] = 0;
   out_7997319965218444246[156] = 0;
   out_7997319965218444246[157] = 0;
   out_7997319965218444246[158] = 0;
   out_7997319965218444246[159] = 0;
   out_7997319965218444246[160] = 0;
   out_7997319965218444246[161] = 0;
   out_7997319965218444246[162] = 0;
   out_7997319965218444246[163] = 0;
   out_7997319965218444246[164] = 0;
   out_7997319965218444246[165] = 0;
   out_7997319965218444246[166] = 0;
   out_7997319965218444246[167] = 0;
   out_7997319965218444246[168] = 0;
   out_7997319965218444246[169] = 0;
   out_7997319965218444246[170] = 0;
   out_7997319965218444246[171] = 1;
   out_7997319965218444246[172] = 0;
   out_7997319965218444246[173] = 0;
   out_7997319965218444246[174] = 0;
   out_7997319965218444246[175] = 0;
   out_7997319965218444246[176] = 0;
   out_7997319965218444246[177] = 0;
   out_7997319965218444246[178] = 0;
   out_7997319965218444246[179] = 0;
   out_7997319965218444246[180] = 0;
   out_7997319965218444246[181] = 0;
   out_7997319965218444246[182] = 0;
   out_7997319965218444246[183] = 0;
   out_7997319965218444246[184] = 0;
   out_7997319965218444246[185] = 0;
   out_7997319965218444246[186] = 0;
   out_7997319965218444246[187] = 0;
   out_7997319965218444246[188] = 0;
   out_7997319965218444246[189] = 0;
   out_7997319965218444246[190] = 1;
   out_7997319965218444246[191] = 0;
   out_7997319965218444246[192] = 0;
   out_7997319965218444246[193] = 0;
   out_7997319965218444246[194] = 0;
   out_7997319965218444246[195] = 0;
   out_7997319965218444246[196] = 0;
   out_7997319965218444246[197] = 0;
   out_7997319965218444246[198] = 0;
   out_7997319965218444246[199] = 0;
   out_7997319965218444246[200] = 0;
   out_7997319965218444246[201] = 0;
   out_7997319965218444246[202] = 0;
   out_7997319965218444246[203] = 0;
   out_7997319965218444246[204] = 0;
   out_7997319965218444246[205] = 0;
   out_7997319965218444246[206] = 0;
   out_7997319965218444246[207] = 0;
   out_7997319965218444246[208] = 0;
   out_7997319965218444246[209] = 1;
   out_7997319965218444246[210] = 0;
   out_7997319965218444246[211] = 0;
   out_7997319965218444246[212] = 0;
   out_7997319965218444246[213] = 0;
   out_7997319965218444246[214] = 0;
   out_7997319965218444246[215] = 0;
   out_7997319965218444246[216] = 0;
   out_7997319965218444246[217] = 0;
   out_7997319965218444246[218] = 0;
   out_7997319965218444246[219] = 0;
   out_7997319965218444246[220] = 0;
   out_7997319965218444246[221] = 0;
   out_7997319965218444246[222] = 0;
   out_7997319965218444246[223] = 0;
   out_7997319965218444246[224] = 0;
   out_7997319965218444246[225] = 0;
   out_7997319965218444246[226] = 0;
   out_7997319965218444246[227] = 0;
   out_7997319965218444246[228] = 1;
   out_7997319965218444246[229] = 0;
   out_7997319965218444246[230] = 0;
   out_7997319965218444246[231] = 0;
   out_7997319965218444246[232] = 0;
   out_7997319965218444246[233] = 0;
   out_7997319965218444246[234] = 0;
   out_7997319965218444246[235] = 0;
   out_7997319965218444246[236] = 0;
   out_7997319965218444246[237] = 0;
   out_7997319965218444246[238] = 0;
   out_7997319965218444246[239] = 0;
   out_7997319965218444246[240] = 0;
   out_7997319965218444246[241] = 0;
   out_7997319965218444246[242] = 0;
   out_7997319965218444246[243] = 0;
   out_7997319965218444246[244] = 0;
   out_7997319965218444246[245] = 0;
   out_7997319965218444246[246] = 0;
   out_7997319965218444246[247] = 1;
   out_7997319965218444246[248] = 0;
   out_7997319965218444246[249] = 0;
   out_7997319965218444246[250] = 0;
   out_7997319965218444246[251] = 0;
   out_7997319965218444246[252] = 0;
   out_7997319965218444246[253] = 0;
   out_7997319965218444246[254] = 0;
   out_7997319965218444246[255] = 0;
   out_7997319965218444246[256] = 0;
   out_7997319965218444246[257] = 0;
   out_7997319965218444246[258] = 0;
   out_7997319965218444246[259] = 0;
   out_7997319965218444246[260] = 0;
   out_7997319965218444246[261] = 0;
   out_7997319965218444246[262] = 0;
   out_7997319965218444246[263] = 0;
   out_7997319965218444246[264] = 0;
   out_7997319965218444246[265] = 0;
   out_7997319965218444246[266] = 1;
   out_7997319965218444246[267] = 0;
   out_7997319965218444246[268] = 0;
   out_7997319965218444246[269] = 0;
   out_7997319965218444246[270] = 0;
   out_7997319965218444246[271] = 0;
   out_7997319965218444246[272] = 0;
   out_7997319965218444246[273] = 0;
   out_7997319965218444246[274] = 0;
   out_7997319965218444246[275] = 0;
   out_7997319965218444246[276] = 0;
   out_7997319965218444246[277] = 0;
   out_7997319965218444246[278] = 0;
   out_7997319965218444246[279] = 0;
   out_7997319965218444246[280] = 0;
   out_7997319965218444246[281] = 0;
   out_7997319965218444246[282] = 0;
   out_7997319965218444246[283] = 0;
   out_7997319965218444246[284] = 0;
   out_7997319965218444246[285] = 1;
   out_7997319965218444246[286] = 0;
   out_7997319965218444246[287] = 0;
   out_7997319965218444246[288] = 0;
   out_7997319965218444246[289] = 0;
   out_7997319965218444246[290] = 0;
   out_7997319965218444246[291] = 0;
   out_7997319965218444246[292] = 0;
   out_7997319965218444246[293] = 0;
   out_7997319965218444246[294] = 0;
   out_7997319965218444246[295] = 0;
   out_7997319965218444246[296] = 0;
   out_7997319965218444246[297] = 0;
   out_7997319965218444246[298] = 0;
   out_7997319965218444246[299] = 0;
   out_7997319965218444246[300] = 0;
   out_7997319965218444246[301] = 0;
   out_7997319965218444246[302] = 0;
   out_7997319965218444246[303] = 0;
   out_7997319965218444246[304] = 1;
   out_7997319965218444246[305] = 0;
   out_7997319965218444246[306] = 0;
   out_7997319965218444246[307] = 0;
   out_7997319965218444246[308] = 0;
   out_7997319965218444246[309] = 0;
   out_7997319965218444246[310] = 0;
   out_7997319965218444246[311] = 0;
   out_7997319965218444246[312] = 0;
   out_7997319965218444246[313] = 0;
   out_7997319965218444246[314] = 0;
   out_7997319965218444246[315] = 0;
   out_7997319965218444246[316] = 0;
   out_7997319965218444246[317] = 0;
   out_7997319965218444246[318] = 0;
   out_7997319965218444246[319] = 0;
   out_7997319965218444246[320] = 0;
   out_7997319965218444246[321] = 0;
   out_7997319965218444246[322] = 0;
   out_7997319965218444246[323] = 1;
}
void h_4(double *state, double *unused, double *out_1088883195134300204) {
   out_1088883195134300204[0] = state[6] + state[9];
   out_1088883195134300204[1] = state[7] + state[10];
   out_1088883195134300204[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_7104500321122265335) {
   out_7104500321122265335[0] = 0;
   out_7104500321122265335[1] = 0;
   out_7104500321122265335[2] = 0;
   out_7104500321122265335[3] = 0;
   out_7104500321122265335[4] = 0;
   out_7104500321122265335[5] = 0;
   out_7104500321122265335[6] = 1;
   out_7104500321122265335[7] = 0;
   out_7104500321122265335[8] = 0;
   out_7104500321122265335[9] = 1;
   out_7104500321122265335[10] = 0;
   out_7104500321122265335[11] = 0;
   out_7104500321122265335[12] = 0;
   out_7104500321122265335[13] = 0;
   out_7104500321122265335[14] = 0;
   out_7104500321122265335[15] = 0;
   out_7104500321122265335[16] = 0;
   out_7104500321122265335[17] = 0;
   out_7104500321122265335[18] = 0;
   out_7104500321122265335[19] = 0;
   out_7104500321122265335[20] = 0;
   out_7104500321122265335[21] = 0;
   out_7104500321122265335[22] = 0;
   out_7104500321122265335[23] = 0;
   out_7104500321122265335[24] = 0;
   out_7104500321122265335[25] = 1;
   out_7104500321122265335[26] = 0;
   out_7104500321122265335[27] = 0;
   out_7104500321122265335[28] = 1;
   out_7104500321122265335[29] = 0;
   out_7104500321122265335[30] = 0;
   out_7104500321122265335[31] = 0;
   out_7104500321122265335[32] = 0;
   out_7104500321122265335[33] = 0;
   out_7104500321122265335[34] = 0;
   out_7104500321122265335[35] = 0;
   out_7104500321122265335[36] = 0;
   out_7104500321122265335[37] = 0;
   out_7104500321122265335[38] = 0;
   out_7104500321122265335[39] = 0;
   out_7104500321122265335[40] = 0;
   out_7104500321122265335[41] = 0;
   out_7104500321122265335[42] = 0;
   out_7104500321122265335[43] = 0;
   out_7104500321122265335[44] = 1;
   out_7104500321122265335[45] = 0;
   out_7104500321122265335[46] = 0;
   out_7104500321122265335[47] = 1;
   out_7104500321122265335[48] = 0;
   out_7104500321122265335[49] = 0;
   out_7104500321122265335[50] = 0;
   out_7104500321122265335[51] = 0;
   out_7104500321122265335[52] = 0;
   out_7104500321122265335[53] = 0;
}
void h_10(double *state, double *unused, double *out_308386678055815256) {
   out_308386678055815256[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_308386678055815256[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_308386678055815256[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_105212834104445393) {
   out_105212834104445393[0] = 0;
   out_105212834104445393[1] = 9.8100000000000005*cos(state[1]);
   out_105212834104445393[2] = 0;
   out_105212834104445393[3] = 0;
   out_105212834104445393[4] = -state[8];
   out_105212834104445393[5] = state[7];
   out_105212834104445393[6] = 0;
   out_105212834104445393[7] = state[5];
   out_105212834104445393[8] = -state[4];
   out_105212834104445393[9] = 0;
   out_105212834104445393[10] = 0;
   out_105212834104445393[11] = 0;
   out_105212834104445393[12] = 1;
   out_105212834104445393[13] = 0;
   out_105212834104445393[14] = 0;
   out_105212834104445393[15] = 1;
   out_105212834104445393[16] = 0;
   out_105212834104445393[17] = 0;
   out_105212834104445393[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_105212834104445393[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_105212834104445393[20] = 0;
   out_105212834104445393[21] = state[8];
   out_105212834104445393[22] = 0;
   out_105212834104445393[23] = -state[6];
   out_105212834104445393[24] = -state[5];
   out_105212834104445393[25] = 0;
   out_105212834104445393[26] = state[3];
   out_105212834104445393[27] = 0;
   out_105212834104445393[28] = 0;
   out_105212834104445393[29] = 0;
   out_105212834104445393[30] = 0;
   out_105212834104445393[31] = 1;
   out_105212834104445393[32] = 0;
   out_105212834104445393[33] = 0;
   out_105212834104445393[34] = 1;
   out_105212834104445393[35] = 0;
   out_105212834104445393[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_105212834104445393[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_105212834104445393[38] = 0;
   out_105212834104445393[39] = -state[7];
   out_105212834104445393[40] = state[6];
   out_105212834104445393[41] = 0;
   out_105212834104445393[42] = state[4];
   out_105212834104445393[43] = -state[3];
   out_105212834104445393[44] = 0;
   out_105212834104445393[45] = 0;
   out_105212834104445393[46] = 0;
   out_105212834104445393[47] = 0;
   out_105212834104445393[48] = 0;
   out_105212834104445393[49] = 0;
   out_105212834104445393[50] = 1;
   out_105212834104445393[51] = 0;
   out_105212834104445393[52] = 0;
   out_105212834104445393[53] = 1;
}
void h_13(double *state, double *unused, double *out_2800280990455093987) {
   out_2800280990455093987[0] = state[3];
   out_2800280990455093987[1] = state[4];
   out_2800280990455093987[2] = state[5];
}
void H_13(double *state, double *unused, double *out_506130887194435594) {
   out_506130887194435594[0] = 0;
   out_506130887194435594[1] = 0;
   out_506130887194435594[2] = 0;
   out_506130887194435594[3] = 1;
   out_506130887194435594[4] = 0;
   out_506130887194435594[5] = 0;
   out_506130887194435594[6] = 0;
   out_506130887194435594[7] = 0;
   out_506130887194435594[8] = 0;
   out_506130887194435594[9] = 0;
   out_506130887194435594[10] = 0;
   out_506130887194435594[11] = 0;
   out_506130887194435594[12] = 0;
   out_506130887194435594[13] = 0;
   out_506130887194435594[14] = 0;
   out_506130887194435594[15] = 0;
   out_506130887194435594[16] = 0;
   out_506130887194435594[17] = 0;
   out_506130887194435594[18] = 0;
   out_506130887194435594[19] = 0;
   out_506130887194435594[20] = 0;
   out_506130887194435594[21] = 0;
   out_506130887194435594[22] = 1;
   out_506130887194435594[23] = 0;
   out_506130887194435594[24] = 0;
   out_506130887194435594[25] = 0;
   out_506130887194435594[26] = 0;
   out_506130887194435594[27] = 0;
   out_506130887194435594[28] = 0;
   out_506130887194435594[29] = 0;
   out_506130887194435594[30] = 0;
   out_506130887194435594[31] = 0;
   out_506130887194435594[32] = 0;
   out_506130887194435594[33] = 0;
   out_506130887194435594[34] = 0;
   out_506130887194435594[35] = 0;
   out_506130887194435594[36] = 0;
   out_506130887194435594[37] = 0;
   out_506130887194435594[38] = 0;
   out_506130887194435594[39] = 0;
   out_506130887194435594[40] = 0;
   out_506130887194435594[41] = 1;
   out_506130887194435594[42] = 0;
   out_506130887194435594[43] = 0;
   out_506130887194435594[44] = 0;
   out_506130887194435594[45] = 0;
   out_506130887194435594[46] = 0;
   out_506130887194435594[47] = 0;
   out_506130887194435594[48] = 0;
   out_506130887194435594[49] = 0;
   out_506130887194435594[50] = 0;
   out_506130887194435594[51] = 0;
   out_506130887194435594[52] = 0;
   out_506130887194435594[53] = 0;
}
void h_14(double *state, double *unused, double *out_1589465505812499279) {
   out_1589465505812499279[0] = state[6];
   out_1589465505812499279[1] = state[7];
   out_1589465505812499279[2] = state[8];
}
void H_14(double *state, double *unused, double *out_3141259464782780806) {
   out_3141259464782780806[0] = 0;
   out_3141259464782780806[1] = 0;
   out_3141259464782780806[2] = 0;
   out_3141259464782780806[3] = 0;
   out_3141259464782780806[4] = 0;
   out_3141259464782780806[5] = 0;
   out_3141259464782780806[6] = 1;
   out_3141259464782780806[7] = 0;
   out_3141259464782780806[8] = 0;
   out_3141259464782780806[9] = 0;
   out_3141259464782780806[10] = 0;
   out_3141259464782780806[11] = 0;
   out_3141259464782780806[12] = 0;
   out_3141259464782780806[13] = 0;
   out_3141259464782780806[14] = 0;
   out_3141259464782780806[15] = 0;
   out_3141259464782780806[16] = 0;
   out_3141259464782780806[17] = 0;
   out_3141259464782780806[18] = 0;
   out_3141259464782780806[19] = 0;
   out_3141259464782780806[20] = 0;
   out_3141259464782780806[21] = 0;
   out_3141259464782780806[22] = 0;
   out_3141259464782780806[23] = 0;
   out_3141259464782780806[24] = 0;
   out_3141259464782780806[25] = 1;
   out_3141259464782780806[26] = 0;
   out_3141259464782780806[27] = 0;
   out_3141259464782780806[28] = 0;
   out_3141259464782780806[29] = 0;
   out_3141259464782780806[30] = 0;
   out_3141259464782780806[31] = 0;
   out_3141259464782780806[32] = 0;
   out_3141259464782780806[33] = 0;
   out_3141259464782780806[34] = 0;
   out_3141259464782780806[35] = 0;
   out_3141259464782780806[36] = 0;
   out_3141259464782780806[37] = 0;
   out_3141259464782780806[38] = 0;
   out_3141259464782780806[39] = 0;
   out_3141259464782780806[40] = 0;
   out_3141259464782780806[41] = 0;
   out_3141259464782780806[42] = 0;
   out_3141259464782780806[43] = 0;
   out_3141259464782780806[44] = 1;
   out_3141259464782780806[45] = 0;
   out_3141259464782780806[46] = 0;
   out_3141259464782780806[47] = 0;
   out_3141259464782780806[48] = 0;
   out_3141259464782780806[49] = 0;
   out_3141259464782780806[50] = 0;
   out_3141259464782780806[51] = 0;
   out_3141259464782780806[52] = 0;
   out_3141259464782780806[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_6129668094290001530) {
  err_fun(nom_x, delta_x, out_6129668094290001530);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_8035171941691199446) {
  inv_err_fun(nom_x, true_x, out_8035171941691199446);
}
void pose_H_mod_fun(double *state, double *out_1839861344935556007) {
  H_mod_fun(state, out_1839861344935556007);
}
void pose_f_fun(double *state, double dt, double *out_5572599306340567392) {
  f_fun(state,  dt, out_5572599306340567392);
}
void pose_F_fun(double *state, double dt, double *out_7997319965218444246) {
  F_fun(state,  dt, out_7997319965218444246);
}
void pose_h_4(double *state, double *unused, double *out_1088883195134300204) {
  h_4(state, unused, out_1088883195134300204);
}
void pose_H_4(double *state, double *unused, double *out_7104500321122265335) {
  H_4(state, unused, out_7104500321122265335);
}
void pose_h_10(double *state, double *unused, double *out_308386678055815256) {
  h_10(state, unused, out_308386678055815256);
}
void pose_H_10(double *state, double *unused, double *out_105212834104445393) {
  H_10(state, unused, out_105212834104445393);
}
void pose_h_13(double *state, double *unused, double *out_2800280990455093987) {
  h_13(state, unused, out_2800280990455093987);
}
void pose_H_13(double *state, double *unused, double *out_506130887194435594) {
  H_13(state, unused, out_506130887194435594);
}
void pose_h_14(double *state, double *unused, double *out_1589465505812499279) {
  h_14(state, unused, out_1589465505812499279);
}
void pose_H_14(double *state, double *unused, double *out_3141259464782780806) {
  H_14(state, unused, out_3141259464782780806);
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
