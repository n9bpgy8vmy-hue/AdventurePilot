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
void err_fun(double *nom_x, double *delta_x, double *out_6009606406978622996) {
   out_6009606406978622996[0] = delta_x[0] + nom_x[0];
   out_6009606406978622996[1] = delta_x[1] + nom_x[1];
   out_6009606406978622996[2] = delta_x[2] + nom_x[2];
   out_6009606406978622996[3] = delta_x[3] + nom_x[3];
   out_6009606406978622996[4] = delta_x[4] + nom_x[4];
   out_6009606406978622996[5] = delta_x[5] + nom_x[5];
   out_6009606406978622996[6] = delta_x[6] + nom_x[6];
   out_6009606406978622996[7] = delta_x[7] + nom_x[7];
   out_6009606406978622996[8] = delta_x[8] + nom_x[8];
   out_6009606406978622996[9] = delta_x[9] + nom_x[9];
   out_6009606406978622996[10] = delta_x[10] + nom_x[10];
   out_6009606406978622996[11] = delta_x[11] + nom_x[11];
   out_6009606406978622996[12] = delta_x[12] + nom_x[12];
   out_6009606406978622996[13] = delta_x[13] + nom_x[13];
   out_6009606406978622996[14] = delta_x[14] + nom_x[14];
   out_6009606406978622996[15] = delta_x[15] + nom_x[15];
   out_6009606406978622996[16] = delta_x[16] + nom_x[16];
   out_6009606406978622996[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7018273336111784530) {
   out_7018273336111784530[0] = -nom_x[0] + true_x[0];
   out_7018273336111784530[1] = -nom_x[1] + true_x[1];
   out_7018273336111784530[2] = -nom_x[2] + true_x[2];
   out_7018273336111784530[3] = -nom_x[3] + true_x[3];
   out_7018273336111784530[4] = -nom_x[4] + true_x[4];
   out_7018273336111784530[5] = -nom_x[5] + true_x[5];
   out_7018273336111784530[6] = -nom_x[6] + true_x[6];
   out_7018273336111784530[7] = -nom_x[7] + true_x[7];
   out_7018273336111784530[8] = -nom_x[8] + true_x[8];
   out_7018273336111784530[9] = -nom_x[9] + true_x[9];
   out_7018273336111784530[10] = -nom_x[10] + true_x[10];
   out_7018273336111784530[11] = -nom_x[11] + true_x[11];
   out_7018273336111784530[12] = -nom_x[12] + true_x[12];
   out_7018273336111784530[13] = -nom_x[13] + true_x[13];
   out_7018273336111784530[14] = -nom_x[14] + true_x[14];
   out_7018273336111784530[15] = -nom_x[15] + true_x[15];
   out_7018273336111784530[16] = -nom_x[16] + true_x[16];
   out_7018273336111784530[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_2069747755750682261) {
   out_2069747755750682261[0] = 1.0;
   out_2069747755750682261[1] = 0.0;
   out_2069747755750682261[2] = 0.0;
   out_2069747755750682261[3] = 0.0;
   out_2069747755750682261[4] = 0.0;
   out_2069747755750682261[5] = 0.0;
   out_2069747755750682261[6] = 0.0;
   out_2069747755750682261[7] = 0.0;
   out_2069747755750682261[8] = 0.0;
   out_2069747755750682261[9] = 0.0;
   out_2069747755750682261[10] = 0.0;
   out_2069747755750682261[11] = 0.0;
   out_2069747755750682261[12] = 0.0;
   out_2069747755750682261[13] = 0.0;
   out_2069747755750682261[14] = 0.0;
   out_2069747755750682261[15] = 0.0;
   out_2069747755750682261[16] = 0.0;
   out_2069747755750682261[17] = 0.0;
   out_2069747755750682261[18] = 0.0;
   out_2069747755750682261[19] = 1.0;
   out_2069747755750682261[20] = 0.0;
   out_2069747755750682261[21] = 0.0;
   out_2069747755750682261[22] = 0.0;
   out_2069747755750682261[23] = 0.0;
   out_2069747755750682261[24] = 0.0;
   out_2069747755750682261[25] = 0.0;
   out_2069747755750682261[26] = 0.0;
   out_2069747755750682261[27] = 0.0;
   out_2069747755750682261[28] = 0.0;
   out_2069747755750682261[29] = 0.0;
   out_2069747755750682261[30] = 0.0;
   out_2069747755750682261[31] = 0.0;
   out_2069747755750682261[32] = 0.0;
   out_2069747755750682261[33] = 0.0;
   out_2069747755750682261[34] = 0.0;
   out_2069747755750682261[35] = 0.0;
   out_2069747755750682261[36] = 0.0;
   out_2069747755750682261[37] = 0.0;
   out_2069747755750682261[38] = 1.0;
   out_2069747755750682261[39] = 0.0;
   out_2069747755750682261[40] = 0.0;
   out_2069747755750682261[41] = 0.0;
   out_2069747755750682261[42] = 0.0;
   out_2069747755750682261[43] = 0.0;
   out_2069747755750682261[44] = 0.0;
   out_2069747755750682261[45] = 0.0;
   out_2069747755750682261[46] = 0.0;
   out_2069747755750682261[47] = 0.0;
   out_2069747755750682261[48] = 0.0;
   out_2069747755750682261[49] = 0.0;
   out_2069747755750682261[50] = 0.0;
   out_2069747755750682261[51] = 0.0;
   out_2069747755750682261[52] = 0.0;
   out_2069747755750682261[53] = 0.0;
   out_2069747755750682261[54] = 0.0;
   out_2069747755750682261[55] = 0.0;
   out_2069747755750682261[56] = 0.0;
   out_2069747755750682261[57] = 1.0;
   out_2069747755750682261[58] = 0.0;
   out_2069747755750682261[59] = 0.0;
   out_2069747755750682261[60] = 0.0;
   out_2069747755750682261[61] = 0.0;
   out_2069747755750682261[62] = 0.0;
   out_2069747755750682261[63] = 0.0;
   out_2069747755750682261[64] = 0.0;
   out_2069747755750682261[65] = 0.0;
   out_2069747755750682261[66] = 0.0;
   out_2069747755750682261[67] = 0.0;
   out_2069747755750682261[68] = 0.0;
   out_2069747755750682261[69] = 0.0;
   out_2069747755750682261[70] = 0.0;
   out_2069747755750682261[71] = 0.0;
   out_2069747755750682261[72] = 0.0;
   out_2069747755750682261[73] = 0.0;
   out_2069747755750682261[74] = 0.0;
   out_2069747755750682261[75] = 0.0;
   out_2069747755750682261[76] = 1.0;
   out_2069747755750682261[77] = 0.0;
   out_2069747755750682261[78] = 0.0;
   out_2069747755750682261[79] = 0.0;
   out_2069747755750682261[80] = 0.0;
   out_2069747755750682261[81] = 0.0;
   out_2069747755750682261[82] = 0.0;
   out_2069747755750682261[83] = 0.0;
   out_2069747755750682261[84] = 0.0;
   out_2069747755750682261[85] = 0.0;
   out_2069747755750682261[86] = 0.0;
   out_2069747755750682261[87] = 0.0;
   out_2069747755750682261[88] = 0.0;
   out_2069747755750682261[89] = 0.0;
   out_2069747755750682261[90] = 0.0;
   out_2069747755750682261[91] = 0.0;
   out_2069747755750682261[92] = 0.0;
   out_2069747755750682261[93] = 0.0;
   out_2069747755750682261[94] = 0.0;
   out_2069747755750682261[95] = 1.0;
   out_2069747755750682261[96] = 0.0;
   out_2069747755750682261[97] = 0.0;
   out_2069747755750682261[98] = 0.0;
   out_2069747755750682261[99] = 0.0;
   out_2069747755750682261[100] = 0.0;
   out_2069747755750682261[101] = 0.0;
   out_2069747755750682261[102] = 0.0;
   out_2069747755750682261[103] = 0.0;
   out_2069747755750682261[104] = 0.0;
   out_2069747755750682261[105] = 0.0;
   out_2069747755750682261[106] = 0.0;
   out_2069747755750682261[107] = 0.0;
   out_2069747755750682261[108] = 0.0;
   out_2069747755750682261[109] = 0.0;
   out_2069747755750682261[110] = 0.0;
   out_2069747755750682261[111] = 0.0;
   out_2069747755750682261[112] = 0.0;
   out_2069747755750682261[113] = 0.0;
   out_2069747755750682261[114] = 1.0;
   out_2069747755750682261[115] = 0.0;
   out_2069747755750682261[116] = 0.0;
   out_2069747755750682261[117] = 0.0;
   out_2069747755750682261[118] = 0.0;
   out_2069747755750682261[119] = 0.0;
   out_2069747755750682261[120] = 0.0;
   out_2069747755750682261[121] = 0.0;
   out_2069747755750682261[122] = 0.0;
   out_2069747755750682261[123] = 0.0;
   out_2069747755750682261[124] = 0.0;
   out_2069747755750682261[125] = 0.0;
   out_2069747755750682261[126] = 0.0;
   out_2069747755750682261[127] = 0.0;
   out_2069747755750682261[128] = 0.0;
   out_2069747755750682261[129] = 0.0;
   out_2069747755750682261[130] = 0.0;
   out_2069747755750682261[131] = 0.0;
   out_2069747755750682261[132] = 0.0;
   out_2069747755750682261[133] = 1.0;
   out_2069747755750682261[134] = 0.0;
   out_2069747755750682261[135] = 0.0;
   out_2069747755750682261[136] = 0.0;
   out_2069747755750682261[137] = 0.0;
   out_2069747755750682261[138] = 0.0;
   out_2069747755750682261[139] = 0.0;
   out_2069747755750682261[140] = 0.0;
   out_2069747755750682261[141] = 0.0;
   out_2069747755750682261[142] = 0.0;
   out_2069747755750682261[143] = 0.0;
   out_2069747755750682261[144] = 0.0;
   out_2069747755750682261[145] = 0.0;
   out_2069747755750682261[146] = 0.0;
   out_2069747755750682261[147] = 0.0;
   out_2069747755750682261[148] = 0.0;
   out_2069747755750682261[149] = 0.0;
   out_2069747755750682261[150] = 0.0;
   out_2069747755750682261[151] = 0.0;
   out_2069747755750682261[152] = 1.0;
   out_2069747755750682261[153] = 0.0;
   out_2069747755750682261[154] = 0.0;
   out_2069747755750682261[155] = 0.0;
   out_2069747755750682261[156] = 0.0;
   out_2069747755750682261[157] = 0.0;
   out_2069747755750682261[158] = 0.0;
   out_2069747755750682261[159] = 0.0;
   out_2069747755750682261[160] = 0.0;
   out_2069747755750682261[161] = 0.0;
   out_2069747755750682261[162] = 0.0;
   out_2069747755750682261[163] = 0.0;
   out_2069747755750682261[164] = 0.0;
   out_2069747755750682261[165] = 0.0;
   out_2069747755750682261[166] = 0.0;
   out_2069747755750682261[167] = 0.0;
   out_2069747755750682261[168] = 0.0;
   out_2069747755750682261[169] = 0.0;
   out_2069747755750682261[170] = 0.0;
   out_2069747755750682261[171] = 1.0;
   out_2069747755750682261[172] = 0.0;
   out_2069747755750682261[173] = 0.0;
   out_2069747755750682261[174] = 0.0;
   out_2069747755750682261[175] = 0.0;
   out_2069747755750682261[176] = 0.0;
   out_2069747755750682261[177] = 0.0;
   out_2069747755750682261[178] = 0.0;
   out_2069747755750682261[179] = 0.0;
   out_2069747755750682261[180] = 0.0;
   out_2069747755750682261[181] = 0.0;
   out_2069747755750682261[182] = 0.0;
   out_2069747755750682261[183] = 0.0;
   out_2069747755750682261[184] = 0.0;
   out_2069747755750682261[185] = 0.0;
   out_2069747755750682261[186] = 0.0;
   out_2069747755750682261[187] = 0.0;
   out_2069747755750682261[188] = 0.0;
   out_2069747755750682261[189] = 0.0;
   out_2069747755750682261[190] = 1.0;
   out_2069747755750682261[191] = 0.0;
   out_2069747755750682261[192] = 0.0;
   out_2069747755750682261[193] = 0.0;
   out_2069747755750682261[194] = 0.0;
   out_2069747755750682261[195] = 0.0;
   out_2069747755750682261[196] = 0.0;
   out_2069747755750682261[197] = 0.0;
   out_2069747755750682261[198] = 0.0;
   out_2069747755750682261[199] = 0.0;
   out_2069747755750682261[200] = 0.0;
   out_2069747755750682261[201] = 0.0;
   out_2069747755750682261[202] = 0.0;
   out_2069747755750682261[203] = 0.0;
   out_2069747755750682261[204] = 0.0;
   out_2069747755750682261[205] = 0.0;
   out_2069747755750682261[206] = 0.0;
   out_2069747755750682261[207] = 0.0;
   out_2069747755750682261[208] = 0.0;
   out_2069747755750682261[209] = 1.0;
   out_2069747755750682261[210] = 0.0;
   out_2069747755750682261[211] = 0.0;
   out_2069747755750682261[212] = 0.0;
   out_2069747755750682261[213] = 0.0;
   out_2069747755750682261[214] = 0.0;
   out_2069747755750682261[215] = 0.0;
   out_2069747755750682261[216] = 0.0;
   out_2069747755750682261[217] = 0.0;
   out_2069747755750682261[218] = 0.0;
   out_2069747755750682261[219] = 0.0;
   out_2069747755750682261[220] = 0.0;
   out_2069747755750682261[221] = 0.0;
   out_2069747755750682261[222] = 0.0;
   out_2069747755750682261[223] = 0.0;
   out_2069747755750682261[224] = 0.0;
   out_2069747755750682261[225] = 0.0;
   out_2069747755750682261[226] = 0.0;
   out_2069747755750682261[227] = 0.0;
   out_2069747755750682261[228] = 1.0;
   out_2069747755750682261[229] = 0.0;
   out_2069747755750682261[230] = 0.0;
   out_2069747755750682261[231] = 0.0;
   out_2069747755750682261[232] = 0.0;
   out_2069747755750682261[233] = 0.0;
   out_2069747755750682261[234] = 0.0;
   out_2069747755750682261[235] = 0.0;
   out_2069747755750682261[236] = 0.0;
   out_2069747755750682261[237] = 0.0;
   out_2069747755750682261[238] = 0.0;
   out_2069747755750682261[239] = 0.0;
   out_2069747755750682261[240] = 0.0;
   out_2069747755750682261[241] = 0.0;
   out_2069747755750682261[242] = 0.0;
   out_2069747755750682261[243] = 0.0;
   out_2069747755750682261[244] = 0.0;
   out_2069747755750682261[245] = 0.0;
   out_2069747755750682261[246] = 0.0;
   out_2069747755750682261[247] = 1.0;
   out_2069747755750682261[248] = 0.0;
   out_2069747755750682261[249] = 0.0;
   out_2069747755750682261[250] = 0.0;
   out_2069747755750682261[251] = 0.0;
   out_2069747755750682261[252] = 0.0;
   out_2069747755750682261[253] = 0.0;
   out_2069747755750682261[254] = 0.0;
   out_2069747755750682261[255] = 0.0;
   out_2069747755750682261[256] = 0.0;
   out_2069747755750682261[257] = 0.0;
   out_2069747755750682261[258] = 0.0;
   out_2069747755750682261[259] = 0.0;
   out_2069747755750682261[260] = 0.0;
   out_2069747755750682261[261] = 0.0;
   out_2069747755750682261[262] = 0.0;
   out_2069747755750682261[263] = 0.0;
   out_2069747755750682261[264] = 0.0;
   out_2069747755750682261[265] = 0.0;
   out_2069747755750682261[266] = 1.0;
   out_2069747755750682261[267] = 0.0;
   out_2069747755750682261[268] = 0.0;
   out_2069747755750682261[269] = 0.0;
   out_2069747755750682261[270] = 0.0;
   out_2069747755750682261[271] = 0.0;
   out_2069747755750682261[272] = 0.0;
   out_2069747755750682261[273] = 0.0;
   out_2069747755750682261[274] = 0.0;
   out_2069747755750682261[275] = 0.0;
   out_2069747755750682261[276] = 0.0;
   out_2069747755750682261[277] = 0.0;
   out_2069747755750682261[278] = 0.0;
   out_2069747755750682261[279] = 0.0;
   out_2069747755750682261[280] = 0.0;
   out_2069747755750682261[281] = 0.0;
   out_2069747755750682261[282] = 0.0;
   out_2069747755750682261[283] = 0.0;
   out_2069747755750682261[284] = 0.0;
   out_2069747755750682261[285] = 1.0;
   out_2069747755750682261[286] = 0.0;
   out_2069747755750682261[287] = 0.0;
   out_2069747755750682261[288] = 0.0;
   out_2069747755750682261[289] = 0.0;
   out_2069747755750682261[290] = 0.0;
   out_2069747755750682261[291] = 0.0;
   out_2069747755750682261[292] = 0.0;
   out_2069747755750682261[293] = 0.0;
   out_2069747755750682261[294] = 0.0;
   out_2069747755750682261[295] = 0.0;
   out_2069747755750682261[296] = 0.0;
   out_2069747755750682261[297] = 0.0;
   out_2069747755750682261[298] = 0.0;
   out_2069747755750682261[299] = 0.0;
   out_2069747755750682261[300] = 0.0;
   out_2069747755750682261[301] = 0.0;
   out_2069747755750682261[302] = 0.0;
   out_2069747755750682261[303] = 0.0;
   out_2069747755750682261[304] = 1.0;
   out_2069747755750682261[305] = 0.0;
   out_2069747755750682261[306] = 0.0;
   out_2069747755750682261[307] = 0.0;
   out_2069747755750682261[308] = 0.0;
   out_2069747755750682261[309] = 0.0;
   out_2069747755750682261[310] = 0.0;
   out_2069747755750682261[311] = 0.0;
   out_2069747755750682261[312] = 0.0;
   out_2069747755750682261[313] = 0.0;
   out_2069747755750682261[314] = 0.0;
   out_2069747755750682261[315] = 0.0;
   out_2069747755750682261[316] = 0.0;
   out_2069747755750682261[317] = 0.0;
   out_2069747755750682261[318] = 0.0;
   out_2069747755750682261[319] = 0.0;
   out_2069747755750682261[320] = 0.0;
   out_2069747755750682261[321] = 0.0;
   out_2069747755750682261[322] = 0.0;
   out_2069747755750682261[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5602288806834867512) {
   out_5602288806834867512[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5602288806834867512[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5602288806834867512[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5602288806834867512[3] = dt*state[12] + state[3];
   out_5602288806834867512[4] = dt*state[13] + state[4];
   out_5602288806834867512[5] = dt*state[14] + state[5];
   out_5602288806834867512[6] = state[6];
   out_5602288806834867512[7] = state[7];
   out_5602288806834867512[8] = state[8];
   out_5602288806834867512[9] = state[9];
   out_5602288806834867512[10] = state[10];
   out_5602288806834867512[11] = state[11];
   out_5602288806834867512[12] = state[12];
   out_5602288806834867512[13] = state[13];
   out_5602288806834867512[14] = state[14];
   out_5602288806834867512[15] = state[15];
   out_5602288806834867512[16] = state[16];
   out_5602288806834867512[17] = state[17];
}
void F_fun(double *state, double dt, double *out_7448356295547946507) {
   out_7448356295547946507[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7448356295547946507[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7448356295547946507[2] = 0;
   out_7448356295547946507[3] = 0;
   out_7448356295547946507[4] = 0;
   out_7448356295547946507[5] = 0;
   out_7448356295547946507[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7448356295547946507[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7448356295547946507[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7448356295547946507[9] = 0;
   out_7448356295547946507[10] = 0;
   out_7448356295547946507[11] = 0;
   out_7448356295547946507[12] = 0;
   out_7448356295547946507[13] = 0;
   out_7448356295547946507[14] = 0;
   out_7448356295547946507[15] = 0;
   out_7448356295547946507[16] = 0;
   out_7448356295547946507[17] = 0;
   out_7448356295547946507[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7448356295547946507[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7448356295547946507[20] = 0;
   out_7448356295547946507[21] = 0;
   out_7448356295547946507[22] = 0;
   out_7448356295547946507[23] = 0;
   out_7448356295547946507[24] = 0;
   out_7448356295547946507[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7448356295547946507[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7448356295547946507[27] = 0;
   out_7448356295547946507[28] = 0;
   out_7448356295547946507[29] = 0;
   out_7448356295547946507[30] = 0;
   out_7448356295547946507[31] = 0;
   out_7448356295547946507[32] = 0;
   out_7448356295547946507[33] = 0;
   out_7448356295547946507[34] = 0;
   out_7448356295547946507[35] = 0;
   out_7448356295547946507[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7448356295547946507[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7448356295547946507[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7448356295547946507[39] = 0;
   out_7448356295547946507[40] = 0;
   out_7448356295547946507[41] = 0;
   out_7448356295547946507[42] = 0;
   out_7448356295547946507[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7448356295547946507[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7448356295547946507[45] = 0;
   out_7448356295547946507[46] = 0;
   out_7448356295547946507[47] = 0;
   out_7448356295547946507[48] = 0;
   out_7448356295547946507[49] = 0;
   out_7448356295547946507[50] = 0;
   out_7448356295547946507[51] = 0;
   out_7448356295547946507[52] = 0;
   out_7448356295547946507[53] = 0;
   out_7448356295547946507[54] = 0;
   out_7448356295547946507[55] = 0;
   out_7448356295547946507[56] = 0;
   out_7448356295547946507[57] = 1;
   out_7448356295547946507[58] = 0;
   out_7448356295547946507[59] = 0;
   out_7448356295547946507[60] = 0;
   out_7448356295547946507[61] = 0;
   out_7448356295547946507[62] = 0;
   out_7448356295547946507[63] = 0;
   out_7448356295547946507[64] = 0;
   out_7448356295547946507[65] = 0;
   out_7448356295547946507[66] = dt;
   out_7448356295547946507[67] = 0;
   out_7448356295547946507[68] = 0;
   out_7448356295547946507[69] = 0;
   out_7448356295547946507[70] = 0;
   out_7448356295547946507[71] = 0;
   out_7448356295547946507[72] = 0;
   out_7448356295547946507[73] = 0;
   out_7448356295547946507[74] = 0;
   out_7448356295547946507[75] = 0;
   out_7448356295547946507[76] = 1;
   out_7448356295547946507[77] = 0;
   out_7448356295547946507[78] = 0;
   out_7448356295547946507[79] = 0;
   out_7448356295547946507[80] = 0;
   out_7448356295547946507[81] = 0;
   out_7448356295547946507[82] = 0;
   out_7448356295547946507[83] = 0;
   out_7448356295547946507[84] = 0;
   out_7448356295547946507[85] = dt;
   out_7448356295547946507[86] = 0;
   out_7448356295547946507[87] = 0;
   out_7448356295547946507[88] = 0;
   out_7448356295547946507[89] = 0;
   out_7448356295547946507[90] = 0;
   out_7448356295547946507[91] = 0;
   out_7448356295547946507[92] = 0;
   out_7448356295547946507[93] = 0;
   out_7448356295547946507[94] = 0;
   out_7448356295547946507[95] = 1;
   out_7448356295547946507[96] = 0;
   out_7448356295547946507[97] = 0;
   out_7448356295547946507[98] = 0;
   out_7448356295547946507[99] = 0;
   out_7448356295547946507[100] = 0;
   out_7448356295547946507[101] = 0;
   out_7448356295547946507[102] = 0;
   out_7448356295547946507[103] = 0;
   out_7448356295547946507[104] = dt;
   out_7448356295547946507[105] = 0;
   out_7448356295547946507[106] = 0;
   out_7448356295547946507[107] = 0;
   out_7448356295547946507[108] = 0;
   out_7448356295547946507[109] = 0;
   out_7448356295547946507[110] = 0;
   out_7448356295547946507[111] = 0;
   out_7448356295547946507[112] = 0;
   out_7448356295547946507[113] = 0;
   out_7448356295547946507[114] = 1;
   out_7448356295547946507[115] = 0;
   out_7448356295547946507[116] = 0;
   out_7448356295547946507[117] = 0;
   out_7448356295547946507[118] = 0;
   out_7448356295547946507[119] = 0;
   out_7448356295547946507[120] = 0;
   out_7448356295547946507[121] = 0;
   out_7448356295547946507[122] = 0;
   out_7448356295547946507[123] = 0;
   out_7448356295547946507[124] = 0;
   out_7448356295547946507[125] = 0;
   out_7448356295547946507[126] = 0;
   out_7448356295547946507[127] = 0;
   out_7448356295547946507[128] = 0;
   out_7448356295547946507[129] = 0;
   out_7448356295547946507[130] = 0;
   out_7448356295547946507[131] = 0;
   out_7448356295547946507[132] = 0;
   out_7448356295547946507[133] = 1;
   out_7448356295547946507[134] = 0;
   out_7448356295547946507[135] = 0;
   out_7448356295547946507[136] = 0;
   out_7448356295547946507[137] = 0;
   out_7448356295547946507[138] = 0;
   out_7448356295547946507[139] = 0;
   out_7448356295547946507[140] = 0;
   out_7448356295547946507[141] = 0;
   out_7448356295547946507[142] = 0;
   out_7448356295547946507[143] = 0;
   out_7448356295547946507[144] = 0;
   out_7448356295547946507[145] = 0;
   out_7448356295547946507[146] = 0;
   out_7448356295547946507[147] = 0;
   out_7448356295547946507[148] = 0;
   out_7448356295547946507[149] = 0;
   out_7448356295547946507[150] = 0;
   out_7448356295547946507[151] = 0;
   out_7448356295547946507[152] = 1;
   out_7448356295547946507[153] = 0;
   out_7448356295547946507[154] = 0;
   out_7448356295547946507[155] = 0;
   out_7448356295547946507[156] = 0;
   out_7448356295547946507[157] = 0;
   out_7448356295547946507[158] = 0;
   out_7448356295547946507[159] = 0;
   out_7448356295547946507[160] = 0;
   out_7448356295547946507[161] = 0;
   out_7448356295547946507[162] = 0;
   out_7448356295547946507[163] = 0;
   out_7448356295547946507[164] = 0;
   out_7448356295547946507[165] = 0;
   out_7448356295547946507[166] = 0;
   out_7448356295547946507[167] = 0;
   out_7448356295547946507[168] = 0;
   out_7448356295547946507[169] = 0;
   out_7448356295547946507[170] = 0;
   out_7448356295547946507[171] = 1;
   out_7448356295547946507[172] = 0;
   out_7448356295547946507[173] = 0;
   out_7448356295547946507[174] = 0;
   out_7448356295547946507[175] = 0;
   out_7448356295547946507[176] = 0;
   out_7448356295547946507[177] = 0;
   out_7448356295547946507[178] = 0;
   out_7448356295547946507[179] = 0;
   out_7448356295547946507[180] = 0;
   out_7448356295547946507[181] = 0;
   out_7448356295547946507[182] = 0;
   out_7448356295547946507[183] = 0;
   out_7448356295547946507[184] = 0;
   out_7448356295547946507[185] = 0;
   out_7448356295547946507[186] = 0;
   out_7448356295547946507[187] = 0;
   out_7448356295547946507[188] = 0;
   out_7448356295547946507[189] = 0;
   out_7448356295547946507[190] = 1;
   out_7448356295547946507[191] = 0;
   out_7448356295547946507[192] = 0;
   out_7448356295547946507[193] = 0;
   out_7448356295547946507[194] = 0;
   out_7448356295547946507[195] = 0;
   out_7448356295547946507[196] = 0;
   out_7448356295547946507[197] = 0;
   out_7448356295547946507[198] = 0;
   out_7448356295547946507[199] = 0;
   out_7448356295547946507[200] = 0;
   out_7448356295547946507[201] = 0;
   out_7448356295547946507[202] = 0;
   out_7448356295547946507[203] = 0;
   out_7448356295547946507[204] = 0;
   out_7448356295547946507[205] = 0;
   out_7448356295547946507[206] = 0;
   out_7448356295547946507[207] = 0;
   out_7448356295547946507[208] = 0;
   out_7448356295547946507[209] = 1;
   out_7448356295547946507[210] = 0;
   out_7448356295547946507[211] = 0;
   out_7448356295547946507[212] = 0;
   out_7448356295547946507[213] = 0;
   out_7448356295547946507[214] = 0;
   out_7448356295547946507[215] = 0;
   out_7448356295547946507[216] = 0;
   out_7448356295547946507[217] = 0;
   out_7448356295547946507[218] = 0;
   out_7448356295547946507[219] = 0;
   out_7448356295547946507[220] = 0;
   out_7448356295547946507[221] = 0;
   out_7448356295547946507[222] = 0;
   out_7448356295547946507[223] = 0;
   out_7448356295547946507[224] = 0;
   out_7448356295547946507[225] = 0;
   out_7448356295547946507[226] = 0;
   out_7448356295547946507[227] = 0;
   out_7448356295547946507[228] = 1;
   out_7448356295547946507[229] = 0;
   out_7448356295547946507[230] = 0;
   out_7448356295547946507[231] = 0;
   out_7448356295547946507[232] = 0;
   out_7448356295547946507[233] = 0;
   out_7448356295547946507[234] = 0;
   out_7448356295547946507[235] = 0;
   out_7448356295547946507[236] = 0;
   out_7448356295547946507[237] = 0;
   out_7448356295547946507[238] = 0;
   out_7448356295547946507[239] = 0;
   out_7448356295547946507[240] = 0;
   out_7448356295547946507[241] = 0;
   out_7448356295547946507[242] = 0;
   out_7448356295547946507[243] = 0;
   out_7448356295547946507[244] = 0;
   out_7448356295547946507[245] = 0;
   out_7448356295547946507[246] = 0;
   out_7448356295547946507[247] = 1;
   out_7448356295547946507[248] = 0;
   out_7448356295547946507[249] = 0;
   out_7448356295547946507[250] = 0;
   out_7448356295547946507[251] = 0;
   out_7448356295547946507[252] = 0;
   out_7448356295547946507[253] = 0;
   out_7448356295547946507[254] = 0;
   out_7448356295547946507[255] = 0;
   out_7448356295547946507[256] = 0;
   out_7448356295547946507[257] = 0;
   out_7448356295547946507[258] = 0;
   out_7448356295547946507[259] = 0;
   out_7448356295547946507[260] = 0;
   out_7448356295547946507[261] = 0;
   out_7448356295547946507[262] = 0;
   out_7448356295547946507[263] = 0;
   out_7448356295547946507[264] = 0;
   out_7448356295547946507[265] = 0;
   out_7448356295547946507[266] = 1;
   out_7448356295547946507[267] = 0;
   out_7448356295547946507[268] = 0;
   out_7448356295547946507[269] = 0;
   out_7448356295547946507[270] = 0;
   out_7448356295547946507[271] = 0;
   out_7448356295547946507[272] = 0;
   out_7448356295547946507[273] = 0;
   out_7448356295547946507[274] = 0;
   out_7448356295547946507[275] = 0;
   out_7448356295547946507[276] = 0;
   out_7448356295547946507[277] = 0;
   out_7448356295547946507[278] = 0;
   out_7448356295547946507[279] = 0;
   out_7448356295547946507[280] = 0;
   out_7448356295547946507[281] = 0;
   out_7448356295547946507[282] = 0;
   out_7448356295547946507[283] = 0;
   out_7448356295547946507[284] = 0;
   out_7448356295547946507[285] = 1;
   out_7448356295547946507[286] = 0;
   out_7448356295547946507[287] = 0;
   out_7448356295547946507[288] = 0;
   out_7448356295547946507[289] = 0;
   out_7448356295547946507[290] = 0;
   out_7448356295547946507[291] = 0;
   out_7448356295547946507[292] = 0;
   out_7448356295547946507[293] = 0;
   out_7448356295547946507[294] = 0;
   out_7448356295547946507[295] = 0;
   out_7448356295547946507[296] = 0;
   out_7448356295547946507[297] = 0;
   out_7448356295547946507[298] = 0;
   out_7448356295547946507[299] = 0;
   out_7448356295547946507[300] = 0;
   out_7448356295547946507[301] = 0;
   out_7448356295547946507[302] = 0;
   out_7448356295547946507[303] = 0;
   out_7448356295547946507[304] = 1;
   out_7448356295547946507[305] = 0;
   out_7448356295547946507[306] = 0;
   out_7448356295547946507[307] = 0;
   out_7448356295547946507[308] = 0;
   out_7448356295547946507[309] = 0;
   out_7448356295547946507[310] = 0;
   out_7448356295547946507[311] = 0;
   out_7448356295547946507[312] = 0;
   out_7448356295547946507[313] = 0;
   out_7448356295547946507[314] = 0;
   out_7448356295547946507[315] = 0;
   out_7448356295547946507[316] = 0;
   out_7448356295547946507[317] = 0;
   out_7448356295547946507[318] = 0;
   out_7448356295547946507[319] = 0;
   out_7448356295547946507[320] = 0;
   out_7448356295547946507[321] = 0;
   out_7448356295547946507[322] = 0;
   out_7448356295547946507[323] = 1;
}
void h_4(double *state, double *unused, double *out_5722605381439183411) {
   out_5722605381439183411[0] = state[6] + state[9];
   out_5722605381439183411[1] = state[7] + state[10];
   out_5722605381439183411[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_7364011898045184863) {
   out_7364011898045184863[0] = 0;
   out_7364011898045184863[1] = 0;
   out_7364011898045184863[2] = 0;
   out_7364011898045184863[3] = 0;
   out_7364011898045184863[4] = 0;
   out_7364011898045184863[5] = 0;
   out_7364011898045184863[6] = 1;
   out_7364011898045184863[7] = 0;
   out_7364011898045184863[8] = 0;
   out_7364011898045184863[9] = 1;
   out_7364011898045184863[10] = 0;
   out_7364011898045184863[11] = 0;
   out_7364011898045184863[12] = 0;
   out_7364011898045184863[13] = 0;
   out_7364011898045184863[14] = 0;
   out_7364011898045184863[15] = 0;
   out_7364011898045184863[16] = 0;
   out_7364011898045184863[17] = 0;
   out_7364011898045184863[18] = 0;
   out_7364011898045184863[19] = 0;
   out_7364011898045184863[20] = 0;
   out_7364011898045184863[21] = 0;
   out_7364011898045184863[22] = 0;
   out_7364011898045184863[23] = 0;
   out_7364011898045184863[24] = 0;
   out_7364011898045184863[25] = 1;
   out_7364011898045184863[26] = 0;
   out_7364011898045184863[27] = 0;
   out_7364011898045184863[28] = 1;
   out_7364011898045184863[29] = 0;
   out_7364011898045184863[30] = 0;
   out_7364011898045184863[31] = 0;
   out_7364011898045184863[32] = 0;
   out_7364011898045184863[33] = 0;
   out_7364011898045184863[34] = 0;
   out_7364011898045184863[35] = 0;
   out_7364011898045184863[36] = 0;
   out_7364011898045184863[37] = 0;
   out_7364011898045184863[38] = 0;
   out_7364011898045184863[39] = 0;
   out_7364011898045184863[40] = 0;
   out_7364011898045184863[41] = 0;
   out_7364011898045184863[42] = 0;
   out_7364011898045184863[43] = 0;
   out_7364011898045184863[44] = 1;
   out_7364011898045184863[45] = 0;
   out_7364011898045184863[46] = 0;
   out_7364011898045184863[47] = 1;
   out_7364011898045184863[48] = 0;
   out_7364011898045184863[49] = 0;
   out_7364011898045184863[50] = 0;
   out_7364011898045184863[51] = 0;
   out_7364011898045184863[52] = 0;
   out_7364011898045184863[53] = 0;
}
void h_10(double *state, double *unused, double *out_8524506374835914602) {
   out_8524506374835914602[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8524506374835914602[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8524506374835914602[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5210940050657868130) {
   out_5210940050657868130[0] = 0;
   out_5210940050657868130[1] = 9.8100000000000005*cos(state[1]);
   out_5210940050657868130[2] = 0;
   out_5210940050657868130[3] = 0;
   out_5210940050657868130[4] = -state[8];
   out_5210940050657868130[5] = state[7];
   out_5210940050657868130[6] = 0;
   out_5210940050657868130[7] = state[5];
   out_5210940050657868130[8] = -state[4];
   out_5210940050657868130[9] = 0;
   out_5210940050657868130[10] = 0;
   out_5210940050657868130[11] = 0;
   out_5210940050657868130[12] = 1;
   out_5210940050657868130[13] = 0;
   out_5210940050657868130[14] = 0;
   out_5210940050657868130[15] = 1;
   out_5210940050657868130[16] = 0;
   out_5210940050657868130[17] = 0;
   out_5210940050657868130[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5210940050657868130[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5210940050657868130[20] = 0;
   out_5210940050657868130[21] = state[8];
   out_5210940050657868130[22] = 0;
   out_5210940050657868130[23] = -state[6];
   out_5210940050657868130[24] = -state[5];
   out_5210940050657868130[25] = 0;
   out_5210940050657868130[26] = state[3];
   out_5210940050657868130[27] = 0;
   out_5210940050657868130[28] = 0;
   out_5210940050657868130[29] = 0;
   out_5210940050657868130[30] = 0;
   out_5210940050657868130[31] = 1;
   out_5210940050657868130[32] = 0;
   out_5210940050657868130[33] = 0;
   out_5210940050657868130[34] = 1;
   out_5210940050657868130[35] = 0;
   out_5210940050657868130[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5210940050657868130[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5210940050657868130[38] = 0;
   out_5210940050657868130[39] = -state[7];
   out_5210940050657868130[40] = state[6];
   out_5210940050657868130[41] = 0;
   out_5210940050657868130[42] = state[4];
   out_5210940050657868130[43] = -state[3];
   out_5210940050657868130[44] = 0;
   out_5210940050657868130[45] = 0;
   out_5210940050657868130[46] = 0;
   out_5210940050657868130[47] = 0;
   out_5210940050657868130[48] = 0;
   out_5210940050657868130[49] = 0;
   out_5210940050657868130[50] = 1;
   out_5210940050657868130[51] = 0;
   out_5210940050657868130[52] = 0;
   out_5210940050657868130[53] = 1;
}
void h_13(double *state, double *unused, double *out_7279317158969388905) {
   out_7279317158969388905[0] = state[3];
   out_7279317158969388905[1] = state[4];
   out_7279317158969388905[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4151738072712852062) {
   out_4151738072712852062[0] = 0;
   out_4151738072712852062[1] = 0;
   out_4151738072712852062[2] = 0;
   out_4151738072712852062[3] = 1;
   out_4151738072712852062[4] = 0;
   out_4151738072712852062[5] = 0;
   out_4151738072712852062[6] = 0;
   out_4151738072712852062[7] = 0;
   out_4151738072712852062[8] = 0;
   out_4151738072712852062[9] = 0;
   out_4151738072712852062[10] = 0;
   out_4151738072712852062[11] = 0;
   out_4151738072712852062[12] = 0;
   out_4151738072712852062[13] = 0;
   out_4151738072712852062[14] = 0;
   out_4151738072712852062[15] = 0;
   out_4151738072712852062[16] = 0;
   out_4151738072712852062[17] = 0;
   out_4151738072712852062[18] = 0;
   out_4151738072712852062[19] = 0;
   out_4151738072712852062[20] = 0;
   out_4151738072712852062[21] = 0;
   out_4151738072712852062[22] = 1;
   out_4151738072712852062[23] = 0;
   out_4151738072712852062[24] = 0;
   out_4151738072712852062[25] = 0;
   out_4151738072712852062[26] = 0;
   out_4151738072712852062[27] = 0;
   out_4151738072712852062[28] = 0;
   out_4151738072712852062[29] = 0;
   out_4151738072712852062[30] = 0;
   out_4151738072712852062[31] = 0;
   out_4151738072712852062[32] = 0;
   out_4151738072712852062[33] = 0;
   out_4151738072712852062[34] = 0;
   out_4151738072712852062[35] = 0;
   out_4151738072712852062[36] = 0;
   out_4151738072712852062[37] = 0;
   out_4151738072712852062[38] = 0;
   out_4151738072712852062[39] = 0;
   out_4151738072712852062[40] = 0;
   out_4151738072712852062[41] = 1;
   out_4151738072712852062[42] = 0;
   out_4151738072712852062[43] = 0;
   out_4151738072712852062[44] = 0;
   out_4151738072712852062[45] = 0;
   out_4151738072712852062[46] = 0;
   out_4151738072712852062[47] = 0;
   out_4151738072712852062[48] = 0;
   out_4151738072712852062[49] = 0;
   out_4151738072712852062[50] = 0;
   out_4151738072712852062[51] = 0;
   out_4151738072712852062[52] = 0;
   out_4151738072712852062[53] = 0;
}
void h_14(double *state, double *unused, double *out_4647165386070377155) {
   out_4647165386070377155[0] = state[6];
   out_4647165386070377155[1] = state[7];
   out_4647165386070377155[2] = state[8];
}
void H_14(double *state, double *unused, double *out_3400771041705700334) {
   out_3400771041705700334[0] = 0;
   out_3400771041705700334[1] = 0;
   out_3400771041705700334[2] = 0;
   out_3400771041705700334[3] = 0;
   out_3400771041705700334[4] = 0;
   out_3400771041705700334[5] = 0;
   out_3400771041705700334[6] = 1;
   out_3400771041705700334[7] = 0;
   out_3400771041705700334[8] = 0;
   out_3400771041705700334[9] = 0;
   out_3400771041705700334[10] = 0;
   out_3400771041705700334[11] = 0;
   out_3400771041705700334[12] = 0;
   out_3400771041705700334[13] = 0;
   out_3400771041705700334[14] = 0;
   out_3400771041705700334[15] = 0;
   out_3400771041705700334[16] = 0;
   out_3400771041705700334[17] = 0;
   out_3400771041705700334[18] = 0;
   out_3400771041705700334[19] = 0;
   out_3400771041705700334[20] = 0;
   out_3400771041705700334[21] = 0;
   out_3400771041705700334[22] = 0;
   out_3400771041705700334[23] = 0;
   out_3400771041705700334[24] = 0;
   out_3400771041705700334[25] = 1;
   out_3400771041705700334[26] = 0;
   out_3400771041705700334[27] = 0;
   out_3400771041705700334[28] = 0;
   out_3400771041705700334[29] = 0;
   out_3400771041705700334[30] = 0;
   out_3400771041705700334[31] = 0;
   out_3400771041705700334[32] = 0;
   out_3400771041705700334[33] = 0;
   out_3400771041705700334[34] = 0;
   out_3400771041705700334[35] = 0;
   out_3400771041705700334[36] = 0;
   out_3400771041705700334[37] = 0;
   out_3400771041705700334[38] = 0;
   out_3400771041705700334[39] = 0;
   out_3400771041705700334[40] = 0;
   out_3400771041705700334[41] = 0;
   out_3400771041705700334[42] = 0;
   out_3400771041705700334[43] = 0;
   out_3400771041705700334[44] = 1;
   out_3400771041705700334[45] = 0;
   out_3400771041705700334[46] = 0;
   out_3400771041705700334[47] = 0;
   out_3400771041705700334[48] = 0;
   out_3400771041705700334[49] = 0;
   out_3400771041705700334[50] = 0;
   out_3400771041705700334[51] = 0;
   out_3400771041705700334[52] = 0;
   out_3400771041705700334[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_6009606406978622996) {
  err_fun(nom_x, delta_x, out_6009606406978622996);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7018273336111784530) {
  inv_err_fun(nom_x, true_x, out_7018273336111784530);
}
void pose_H_mod_fun(double *state, double *out_2069747755750682261) {
  H_mod_fun(state, out_2069747755750682261);
}
void pose_f_fun(double *state, double dt, double *out_5602288806834867512) {
  f_fun(state,  dt, out_5602288806834867512);
}
void pose_F_fun(double *state, double dt, double *out_7448356295547946507) {
  F_fun(state,  dt, out_7448356295547946507);
}
void pose_h_4(double *state, double *unused, double *out_5722605381439183411) {
  h_4(state, unused, out_5722605381439183411);
}
void pose_H_4(double *state, double *unused, double *out_7364011898045184863) {
  H_4(state, unused, out_7364011898045184863);
}
void pose_h_10(double *state, double *unused, double *out_8524506374835914602) {
  h_10(state, unused, out_8524506374835914602);
}
void pose_H_10(double *state, double *unused, double *out_5210940050657868130) {
  H_10(state, unused, out_5210940050657868130);
}
void pose_h_13(double *state, double *unused, double *out_7279317158969388905) {
  h_13(state, unused, out_7279317158969388905);
}
void pose_H_13(double *state, double *unused, double *out_4151738072712852062) {
  H_13(state, unused, out_4151738072712852062);
}
void pose_h_14(double *state, double *unused, double *out_4647165386070377155) {
  h_14(state, unused, out_4647165386070377155);
}
void pose_H_14(double *state, double *unused, double *out_3400771041705700334) {
  H_14(state, unused, out_3400771041705700334);
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
