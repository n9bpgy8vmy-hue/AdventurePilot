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
void err_fun(double *nom_x, double *delta_x, double *out_2806378377347710691) {
   out_2806378377347710691[0] = delta_x[0] + nom_x[0];
   out_2806378377347710691[1] = delta_x[1] + nom_x[1];
   out_2806378377347710691[2] = delta_x[2] + nom_x[2];
   out_2806378377347710691[3] = delta_x[3] + nom_x[3];
   out_2806378377347710691[4] = delta_x[4] + nom_x[4];
   out_2806378377347710691[5] = delta_x[5] + nom_x[5];
   out_2806378377347710691[6] = delta_x[6] + nom_x[6];
   out_2806378377347710691[7] = delta_x[7] + nom_x[7];
   out_2806378377347710691[8] = delta_x[8] + nom_x[8];
   out_2806378377347710691[9] = delta_x[9] + nom_x[9];
   out_2806378377347710691[10] = delta_x[10] + nom_x[10];
   out_2806378377347710691[11] = delta_x[11] + nom_x[11];
   out_2806378377347710691[12] = delta_x[12] + nom_x[12];
   out_2806378377347710691[13] = delta_x[13] + nom_x[13];
   out_2806378377347710691[14] = delta_x[14] + nom_x[14];
   out_2806378377347710691[15] = delta_x[15] + nom_x[15];
   out_2806378377347710691[16] = delta_x[16] + nom_x[16];
   out_2806378377347710691[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6769519321017128267) {
   out_6769519321017128267[0] = -nom_x[0] + true_x[0];
   out_6769519321017128267[1] = -nom_x[1] + true_x[1];
   out_6769519321017128267[2] = -nom_x[2] + true_x[2];
   out_6769519321017128267[3] = -nom_x[3] + true_x[3];
   out_6769519321017128267[4] = -nom_x[4] + true_x[4];
   out_6769519321017128267[5] = -nom_x[5] + true_x[5];
   out_6769519321017128267[6] = -nom_x[6] + true_x[6];
   out_6769519321017128267[7] = -nom_x[7] + true_x[7];
   out_6769519321017128267[8] = -nom_x[8] + true_x[8];
   out_6769519321017128267[9] = -nom_x[9] + true_x[9];
   out_6769519321017128267[10] = -nom_x[10] + true_x[10];
   out_6769519321017128267[11] = -nom_x[11] + true_x[11];
   out_6769519321017128267[12] = -nom_x[12] + true_x[12];
   out_6769519321017128267[13] = -nom_x[13] + true_x[13];
   out_6769519321017128267[14] = -nom_x[14] + true_x[14];
   out_6769519321017128267[15] = -nom_x[15] + true_x[15];
   out_6769519321017128267[16] = -nom_x[16] + true_x[16];
   out_6769519321017128267[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_7023130148617119261) {
   out_7023130148617119261[0] = 1.0;
   out_7023130148617119261[1] = 0.0;
   out_7023130148617119261[2] = 0.0;
   out_7023130148617119261[3] = 0.0;
   out_7023130148617119261[4] = 0.0;
   out_7023130148617119261[5] = 0.0;
   out_7023130148617119261[6] = 0.0;
   out_7023130148617119261[7] = 0.0;
   out_7023130148617119261[8] = 0.0;
   out_7023130148617119261[9] = 0.0;
   out_7023130148617119261[10] = 0.0;
   out_7023130148617119261[11] = 0.0;
   out_7023130148617119261[12] = 0.0;
   out_7023130148617119261[13] = 0.0;
   out_7023130148617119261[14] = 0.0;
   out_7023130148617119261[15] = 0.0;
   out_7023130148617119261[16] = 0.0;
   out_7023130148617119261[17] = 0.0;
   out_7023130148617119261[18] = 0.0;
   out_7023130148617119261[19] = 1.0;
   out_7023130148617119261[20] = 0.0;
   out_7023130148617119261[21] = 0.0;
   out_7023130148617119261[22] = 0.0;
   out_7023130148617119261[23] = 0.0;
   out_7023130148617119261[24] = 0.0;
   out_7023130148617119261[25] = 0.0;
   out_7023130148617119261[26] = 0.0;
   out_7023130148617119261[27] = 0.0;
   out_7023130148617119261[28] = 0.0;
   out_7023130148617119261[29] = 0.0;
   out_7023130148617119261[30] = 0.0;
   out_7023130148617119261[31] = 0.0;
   out_7023130148617119261[32] = 0.0;
   out_7023130148617119261[33] = 0.0;
   out_7023130148617119261[34] = 0.0;
   out_7023130148617119261[35] = 0.0;
   out_7023130148617119261[36] = 0.0;
   out_7023130148617119261[37] = 0.0;
   out_7023130148617119261[38] = 1.0;
   out_7023130148617119261[39] = 0.0;
   out_7023130148617119261[40] = 0.0;
   out_7023130148617119261[41] = 0.0;
   out_7023130148617119261[42] = 0.0;
   out_7023130148617119261[43] = 0.0;
   out_7023130148617119261[44] = 0.0;
   out_7023130148617119261[45] = 0.0;
   out_7023130148617119261[46] = 0.0;
   out_7023130148617119261[47] = 0.0;
   out_7023130148617119261[48] = 0.0;
   out_7023130148617119261[49] = 0.0;
   out_7023130148617119261[50] = 0.0;
   out_7023130148617119261[51] = 0.0;
   out_7023130148617119261[52] = 0.0;
   out_7023130148617119261[53] = 0.0;
   out_7023130148617119261[54] = 0.0;
   out_7023130148617119261[55] = 0.0;
   out_7023130148617119261[56] = 0.0;
   out_7023130148617119261[57] = 1.0;
   out_7023130148617119261[58] = 0.0;
   out_7023130148617119261[59] = 0.0;
   out_7023130148617119261[60] = 0.0;
   out_7023130148617119261[61] = 0.0;
   out_7023130148617119261[62] = 0.0;
   out_7023130148617119261[63] = 0.0;
   out_7023130148617119261[64] = 0.0;
   out_7023130148617119261[65] = 0.0;
   out_7023130148617119261[66] = 0.0;
   out_7023130148617119261[67] = 0.0;
   out_7023130148617119261[68] = 0.0;
   out_7023130148617119261[69] = 0.0;
   out_7023130148617119261[70] = 0.0;
   out_7023130148617119261[71] = 0.0;
   out_7023130148617119261[72] = 0.0;
   out_7023130148617119261[73] = 0.0;
   out_7023130148617119261[74] = 0.0;
   out_7023130148617119261[75] = 0.0;
   out_7023130148617119261[76] = 1.0;
   out_7023130148617119261[77] = 0.0;
   out_7023130148617119261[78] = 0.0;
   out_7023130148617119261[79] = 0.0;
   out_7023130148617119261[80] = 0.0;
   out_7023130148617119261[81] = 0.0;
   out_7023130148617119261[82] = 0.0;
   out_7023130148617119261[83] = 0.0;
   out_7023130148617119261[84] = 0.0;
   out_7023130148617119261[85] = 0.0;
   out_7023130148617119261[86] = 0.0;
   out_7023130148617119261[87] = 0.0;
   out_7023130148617119261[88] = 0.0;
   out_7023130148617119261[89] = 0.0;
   out_7023130148617119261[90] = 0.0;
   out_7023130148617119261[91] = 0.0;
   out_7023130148617119261[92] = 0.0;
   out_7023130148617119261[93] = 0.0;
   out_7023130148617119261[94] = 0.0;
   out_7023130148617119261[95] = 1.0;
   out_7023130148617119261[96] = 0.0;
   out_7023130148617119261[97] = 0.0;
   out_7023130148617119261[98] = 0.0;
   out_7023130148617119261[99] = 0.0;
   out_7023130148617119261[100] = 0.0;
   out_7023130148617119261[101] = 0.0;
   out_7023130148617119261[102] = 0.0;
   out_7023130148617119261[103] = 0.0;
   out_7023130148617119261[104] = 0.0;
   out_7023130148617119261[105] = 0.0;
   out_7023130148617119261[106] = 0.0;
   out_7023130148617119261[107] = 0.0;
   out_7023130148617119261[108] = 0.0;
   out_7023130148617119261[109] = 0.0;
   out_7023130148617119261[110] = 0.0;
   out_7023130148617119261[111] = 0.0;
   out_7023130148617119261[112] = 0.0;
   out_7023130148617119261[113] = 0.0;
   out_7023130148617119261[114] = 1.0;
   out_7023130148617119261[115] = 0.0;
   out_7023130148617119261[116] = 0.0;
   out_7023130148617119261[117] = 0.0;
   out_7023130148617119261[118] = 0.0;
   out_7023130148617119261[119] = 0.0;
   out_7023130148617119261[120] = 0.0;
   out_7023130148617119261[121] = 0.0;
   out_7023130148617119261[122] = 0.0;
   out_7023130148617119261[123] = 0.0;
   out_7023130148617119261[124] = 0.0;
   out_7023130148617119261[125] = 0.0;
   out_7023130148617119261[126] = 0.0;
   out_7023130148617119261[127] = 0.0;
   out_7023130148617119261[128] = 0.0;
   out_7023130148617119261[129] = 0.0;
   out_7023130148617119261[130] = 0.0;
   out_7023130148617119261[131] = 0.0;
   out_7023130148617119261[132] = 0.0;
   out_7023130148617119261[133] = 1.0;
   out_7023130148617119261[134] = 0.0;
   out_7023130148617119261[135] = 0.0;
   out_7023130148617119261[136] = 0.0;
   out_7023130148617119261[137] = 0.0;
   out_7023130148617119261[138] = 0.0;
   out_7023130148617119261[139] = 0.0;
   out_7023130148617119261[140] = 0.0;
   out_7023130148617119261[141] = 0.0;
   out_7023130148617119261[142] = 0.0;
   out_7023130148617119261[143] = 0.0;
   out_7023130148617119261[144] = 0.0;
   out_7023130148617119261[145] = 0.0;
   out_7023130148617119261[146] = 0.0;
   out_7023130148617119261[147] = 0.0;
   out_7023130148617119261[148] = 0.0;
   out_7023130148617119261[149] = 0.0;
   out_7023130148617119261[150] = 0.0;
   out_7023130148617119261[151] = 0.0;
   out_7023130148617119261[152] = 1.0;
   out_7023130148617119261[153] = 0.0;
   out_7023130148617119261[154] = 0.0;
   out_7023130148617119261[155] = 0.0;
   out_7023130148617119261[156] = 0.0;
   out_7023130148617119261[157] = 0.0;
   out_7023130148617119261[158] = 0.0;
   out_7023130148617119261[159] = 0.0;
   out_7023130148617119261[160] = 0.0;
   out_7023130148617119261[161] = 0.0;
   out_7023130148617119261[162] = 0.0;
   out_7023130148617119261[163] = 0.0;
   out_7023130148617119261[164] = 0.0;
   out_7023130148617119261[165] = 0.0;
   out_7023130148617119261[166] = 0.0;
   out_7023130148617119261[167] = 0.0;
   out_7023130148617119261[168] = 0.0;
   out_7023130148617119261[169] = 0.0;
   out_7023130148617119261[170] = 0.0;
   out_7023130148617119261[171] = 1.0;
   out_7023130148617119261[172] = 0.0;
   out_7023130148617119261[173] = 0.0;
   out_7023130148617119261[174] = 0.0;
   out_7023130148617119261[175] = 0.0;
   out_7023130148617119261[176] = 0.0;
   out_7023130148617119261[177] = 0.0;
   out_7023130148617119261[178] = 0.0;
   out_7023130148617119261[179] = 0.0;
   out_7023130148617119261[180] = 0.0;
   out_7023130148617119261[181] = 0.0;
   out_7023130148617119261[182] = 0.0;
   out_7023130148617119261[183] = 0.0;
   out_7023130148617119261[184] = 0.0;
   out_7023130148617119261[185] = 0.0;
   out_7023130148617119261[186] = 0.0;
   out_7023130148617119261[187] = 0.0;
   out_7023130148617119261[188] = 0.0;
   out_7023130148617119261[189] = 0.0;
   out_7023130148617119261[190] = 1.0;
   out_7023130148617119261[191] = 0.0;
   out_7023130148617119261[192] = 0.0;
   out_7023130148617119261[193] = 0.0;
   out_7023130148617119261[194] = 0.0;
   out_7023130148617119261[195] = 0.0;
   out_7023130148617119261[196] = 0.0;
   out_7023130148617119261[197] = 0.0;
   out_7023130148617119261[198] = 0.0;
   out_7023130148617119261[199] = 0.0;
   out_7023130148617119261[200] = 0.0;
   out_7023130148617119261[201] = 0.0;
   out_7023130148617119261[202] = 0.0;
   out_7023130148617119261[203] = 0.0;
   out_7023130148617119261[204] = 0.0;
   out_7023130148617119261[205] = 0.0;
   out_7023130148617119261[206] = 0.0;
   out_7023130148617119261[207] = 0.0;
   out_7023130148617119261[208] = 0.0;
   out_7023130148617119261[209] = 1.0;
   out_7023130148617119261[210] = 0.0;
   out_7023130148617119261[211] = 0.0;
   out_7023130148617119261[212] = 0.0;
   out_7023130148617119261[213] = 0.0;
   out_7023130148617119261[214] = 0.0;
   out_7023130148617119261[215] = 0.0;
   out_7023130148617119261[216] = 0.0;
   out_7023130148617119261[217] = 0.0;
   out_7023130148617119261[218] = 0.0;
   out_7023130148617119261[219] = 0.0;
   out_7023130148617119261[220] = 0.0;
   out_7023130148617119261[221] = 0.0;
   out_7023130148617119261[222] = 0.0;
   out_7023130148617119261[223] = 0.0;
   out_7023130148617119261[224] = 0.0;
   out_7023130148617119261[225] = 0.0;
   out_7023130148617119261[226] = 0.0;
   out_7023130148617119261[227] = 0.0;
   out_7023130148617119261[228] = 1.0;
   out_7023130148617119261[229] = 0.0;
   out_7023130148617119261[230] = 0.0;
   out_7023130148617119261[231] = 0.0;
   out_7023130148617119261[232] = 0.0;
   out_7023130148617119261[233] = 0.0;
   out_7023130148617119261[234] = 0.0;
   out_7023130148617119261[235] = 0.0;
   out_7023130148617119261[236] = 0.0;
   out_7023130148617119261[237] = 0.0;
   out_7023130148617119261[238] = 0.0;
   out_7023130148617119261[239] = 0.0;
   out_7023130148617119261[240] = 0.0;
   out_7023130148617119261[241] = 0.0;
   out_7023130148617119261[242] = 0.0;
   out_7023130148617119261[243] = 0.0;
   out_7023130148617119261[244] = 0.0;
   out_7023130148617119261[245] = 0.0;
   out_7023130148617119261[246] = 0.0;
   out_7023130148617119261[247] = 1.0;
   out_7023130148617119261[248] = 0.0;
   out_7023130148617119261[249] = 0.0;
   out_7023130148617119261[250] = 0.0;
   out_7023130148617119261[251] = 0.0;
   out_7023130148617119261[252] = 0.0;
   out_7023130148617119261[253] = 0.0;
   out_7023130148617119261[254] = 0.0;
   out_7023130148617119261[255] = 0.0;
   out_7023130148617119261[256] = 0.0;
   out_7023130148617119261[257] = 0.0;
   out_7023130148617119261[258] = 0.0;
   out_7023130148617119261[259] = 0.0;
   out_7023130148617119261[260] = 0.0;
   out_7023130148617119261[261] = 0.0;
   out_7023130148617119261[262] = 0.0;
   out_7023130148617119261[263] = 0.0;
   out_7023130148617119261[264] = 0.0;
   out_7023130148617119261[265] = 0.0;
   out_7023130148617119261[266] = 1.0;
   out_7023130148617119261[267] = 0.0;
   out_7023130148617119261[268] = 0.0;
   out_7023130148617119261[269] = 0.0;
   out_7023130148617119261[270] = 0.0;
   out_7023130148617119261[271] = 0.0;
   out_7023130148617119261[272] = 0.0;
   out_7023130148617119261[273] = 0.0;
   out_7023130148617119261[274] = 0.0;
   out_7023130148617119261[275] = 0.0;
   out_7023130148617119261[276] = 0.0;
   out_7023130148617119261[277] = 0.0;
   out_7023130148617119261[278] = 0.0;
   out_7023130148617119261[279] = 0.0;
   out_7023130148617119261[280] = 0.0;
   out_7023130148617119261[281] = 0.0;
   out_7023130148617119261[282] = 0.0;
   out_7023130148617119261[283] = 0.0;
   out_7023130148617119261[284] = 0.0;
   out_7023130148617119261[285] = 1.0;
   out_7023130148617119261[286] = 0.0;
   out_7023130148617119261[287] = 0.0;
   out_7023130148617119261[288] = 0.0;
   out_7023130148617119261[289] = 0.0;
   out_7023130148617119261[290] = 0.0;
   out_7023130148617119261[291] = 0.0;
   out_7023130148617119261[292] = 0.0;
   out_7023130148617119261[293] = 0.0;
   out_7023130148617119261[294] = 0.0;
   out_7023130148617119261[295] = 0.0;
   out_7023130148617119261[296] = 0.0;
   out_7023130148617119261[297] = 0.0;
   out_7023130148617119261[298] = 0.0;
   out_7023130148617119261[299] = 0.0;
   out_7023130148617119261[300] = 0.0;
   out_7023130148617119261[301] = 0.0;
   out_7023130148617119261[302] = 0.0;
   out_7023130148617119261[303] = 0.0;
   out_7023130148617119261[304] = 1.0;
   out_7023130148617119261[305] = 0.0;
   out_7023130148617119261[306] = 0.0;
   out_7023130148617119261[307] = 0.0;
   out_7023130148617119261[308] = 0.0;
   out_7023130148617119261[309] = 0.0;
   out_7023130148617119261[310] = 0.0;
   out_7023130148617119261[311] = 0.0;
   out_7023130148617119261[312] = 0.0;
   out_7023130148617119261[313] = 0.0;
   out_7023130148617119261[314] = 0.0;
   out_7023130148617119261[315] = 0.0;
   out_7023130148617119261[316] = 0.0;
   out_7023130148617119261[317] = 0.0;
   out_7023130148617119261[318] = 0.0;
   out_7023130148617119261[319] = 0.0;
   out_7023130148617119261[320] = 0.0;
   out_7023130148617119261[321] = 0.0;
   out_7023130148617119261[322] = 0.0;
   out_7023130148617119261[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5185471552589919441) {
   out_5185471552589919441[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5185471552589919441[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5185471552589919441[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5185471552589919441[3] = dt*state[12] + state[3];
   out_5185471552589919441[4] = dt*state[13] + state[4];
   out_5185471552589919441[5] = dt*state[14] + state[5];
   out_5185471552589919441[6] = state[6];
   out_5185471552589919441[7] = state[7];
   out_5185471552589919441[8] = state[8];
   out_5185471552589919441[9] = state[9];
   out_5185471552589919441[10] = state[10];
   out_5185471552589919441[11] = state[11];
   out_5185471552589919441[12] = state[12];
   out_5185471552589919441[13] = state[13];
   out_5185471552589919441[14] = state[14];
   out_5185471552589919441[15] = state[15];
   out_5185471552589919441[16] = state[16];
   out_5185471552589919441[17] = state[17];
}
void F_fun(double *state, double dt, double *out_7795547570733027780) {
   out_7795547570733027780[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7795547570733027780[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7795547570733027780[2] = 0;
   out_7795547570733027780[3] = 0;
   out_7795547570733027780[4] = 0;
   out_7795547570733027780[5] = 0;
   out_7795547570733027780[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7795547570733027780[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7795547570733027780[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_7795547570733027780[9] = 0;
   out_7795547570733027780[10] = 0;
   out_7795547570733027780[11] = 0;
   out_7795547570733027780[12] = 0;
   out_7795547570733027780[13] = 0;
   out_7795547570733027780[14] = 0;
   out_7795547570733027780[15] = 0;
   out_7795547570733027780[16] = 0;
   out_7795547570733027780[17] = 0;
   out_7795547570733027780[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7795547570733027780[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7795547570733027780[20] = 0;
   out_7795547570733027780[21] = 0;
   out_7795547570733027780[22] = 0;
   out_7795547570733027780[23] = 0;
   out_7795547570733027780[24] = 0;
   out_7795547570733027780[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7795547570733027780[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_7795547570733027780[27] = 0;
   out_7795547570733027780[28] = 0;
   out_7795547570733027780[29] = 0;
   out_7795547570733027780[30] = 0;
   out_7795547570733027780[31] = 0;
   out_7795547570733027780[32] = 0;
   out_7795547570733027780[33] = 0;
   out_7795547570733027780[34] = 0;
   out_7795547570733027780[35] = 0;
   out_7795547570733027780[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7795547570733027780[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7795547570733027780[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7795547570733027780[39] = 0;
   out_7795547570733027780[40] = 0;
   out_7795547570733027780[41] = 0;
   out_7795547570733027780[42] = 0;
   out_7795547570733027780[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7795547570733027780[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_7795547570733027780[45] = 0;
   out_7795547570733027780[46] = 0;
   out_7795547570733027780[47] = 0;
   out_7795547570733027780[48] = 0;
   out_7795547570733027780[49] = 0;
   out_7795547570733027780[50] = 0;
   out_7795547570733027780[51] = 0;
   out_7795547570733027780[52] = 0;
   out_7795547570733027780[53] = 0;
   out_7795547570733027780[54] = 0;
   out_7795547570733027780[55] = 0;
   out_7795547570733027780[56] = 0;
   out_7795547570733027780[57] = 1;
   out_7795547570733027780[58] = 0;
   out_7795547570733027780[59] = 0;
   out_7795547570733027780[60] = 0;
   out_7795547570733027780[61] = 0;
   out_7795547570733027780[62] = 0;
   out_7795547570733027780[63] = 0;
   out_7795547570733027780[64] = 0;
   out_7795547570733027780[65] = 0;
   out_7795547570733027780[66] = dt;
   out_7795547570733027780[67] = 0;
   out_7795547570733027780[68] = 0;
   out_7795547570733027780[69] = 0;
   out_7795547570733027780[70] = 0;
   out_7795547570733027780[71] = 0;
   out_7795547570733027780[72] = 0;
   out_7795547570733027780[73] = 0;
   out_7795547570733027780[74] = 0;
   out_7795547570733027780[75] = 0;
   out_7795547570733027780[76] = 1;
   out_7795547570733027780[77] = 0;
   out_7795547570733027780[78] = 0;
   out_7795547570733027780[79] = 0;
   out_7795547570733027780[80] = 0;
   out_7795547570733027780[81] = 0;
   out_7795547570733027780[82] = 0;
   out_7795547570733027780[83] = 0;
   out_7795547570733027780[84] = 0;
   out_7795547570733027780[85] = dt;
   out_7795547570733027780[86] = 0;
   out_7795547570733027780[87] = 0;
   out_7795547570733027780[88] = 0;
   out_7795547570733027780[89] = 0;
   out_7795547570733027780[90] = 0;
   out_7795547570733027780[91] = 0;
   out_7795547570733027780[92] = 0;
   out_7795547570733027780[93] = 0;
   out_7795547570733027780[94] = 0;
   out_7795547570733027780[95] = 1;
   out_7795547570733027780[96] = 0;
   out_7795547570733027780[97] = 0;
   out_7795547570733027780[98] = 0;
   out_7795547570733027780[99] = 0;
   out_7795547570733027780[100] = 0;
   out_7795547570733027780[101] = 0;
   out_7795547570733027780[102] = 0;
   out_7795547570733027780[103] = 0;
   out_7795547570733027780[104] = dt;
   out_7795547570733027780[105] = 0;
   out_7795547570733027780[106] = 0;
   out_7795547570733027780[107] = 0;
   out_7795547570733027780[108] = 0;
   out_7795547570733027780[109] = 0;
   out_7795547570733027780[110] = 0;
   out_7795547570733027780[111] = 0;
   out_7795547570733027780[112] = 0;
   out_7795547570733027780[113] = 0;
   out_7795547570733027780[114] = 1;
   out_7795547570733027780[115] = 0;
   out_7795547570733027780[116] = 0;
   out_7795547570733027780[117] = 0;
   out_7795547570733027780[118] = 0;
   out_7795547570733027780[119] = 0;
   out_7795547570733027780[120] = 0;
   out_7795547570733027780[121] = 0;
   out_7795547570733027780[122] = 0;
   out_7795547570733027780[123] = 0;
   out_7795547570733027780[124] = 0;
   out_7795547570733027780[125] = 0;
   out_7795547570733027780[126] = 0;
   out_7795547570733027780[127] = 0;
   out_7795547570733027780[128] = 0;
   out_7795547570733027780[129] = 0;
   out_7795547570733027780[130] = 0;
   out_7795547570733027780[131] = 0;
   out_7795547570733027780[132] = 0;
   out_7795547570733027780[133] = 1;
   out_7795547570733027780[134] = 0;
   out_7795547570733027780[135] = 0;
   out_7795547570733027780[136] = 0;
   out_7795547570733027780[137] = 0;
   out_7795547570733027780[138] = 0;
   out_7795547570733027780[139] = 0;
   out_7795547570733027780[140] = 0;
   out_7795547570733027780[141] = 0;
   out_7795547570733027780[142] = 0;
   out_7795547570733027780[143] = 0;
   out_7795547570733027780[144] = 0;
   out_7795547570733027780[145] = 0;
   out_7795547570733027780[146] = 0;
   out_7795547570733027780[147] = 0;
   out_7795547570733027780[148] = 0;
   out_7795547570733027780[149] = 0;
   out_7795547570733027780[150] = 0;
   out_7795547570733027780[151] = 0;
   out_7795547570733027780[152] = 1;
   out_7795547570733027780[153] = 0;
   out_7795547570733027780[154] = 0;
   out_7795547570733027780[155] = 0;
   out_7795547570733027780[156] = 0;
   out_7795547570733027780[157] = 0;
   out_7795547570733027780[158] = 0;
   out_7795547570733027780[159] = 0;
   out_7795547570733027780[160] = 0;
   out_7795547570733027780[161] = 0;
   out_7795547570733027780[162] = 0;
   out_7795547570733027780[163] = 0;
   out_7795547570733027780[164] = 0;
   out_7795547570733027780[165] = 0;
   out_7795547570733027780[166] = 0;
   out_7795547570733027780[167] = 0;
   out_7795547570733027780[168] = 0;
   out_7795547570733027780[169] = 0;
   out_7795547570733027780[170] = 0;
   out_7795547570733027780[171] = 1;
   out_7795547570733027780[172] = 0;
   out_7795547570733027780[173] = 0;
   out_7795547570733027780[174] = 0;
   out_7795547570733027780[175] = 0;
   out_7795547570733027780[176] = 0;
   out_7795547570733027780[177] = 0;
   out_7795547570733027780[178] = 0;
   out_7795547570733027780[179] = 0;
   out_7795547570733027780[180] = 0;
   out_7795547570733027780[181] = 0;
   out_7795547570733027780[182] = 0;
   out_7795547570733027780[183] = 0;
   out_7795547570733027780[184] = 0;
   out_7795547570733027780[185] = 0;
   out_7795547570733027780[186] = 0;
   out_7795547570733027780[187] = 0;
   out_7795547570733027780[188] = 0;
   out_7795547570733027780[189] = 0;
   out_7795547570733027780[190] = 1;
   out_7795547570733027780[191] = 0;
   out_7795547570733027780[192] = 0;
   out_7795547570733027780[193] = 0;
   out_7795547570733027780[194] = 0;
   out_7795547570733027780[195] = 0;
   out_7795547570733027780[196] = 0;
   out_7795547570733027780[197] = 0;
   out_7795547570733027780[198] = 0;
   out_7795547570733027780[199] = 0;
   out_7795547570733027780[200] = 0;
   out_7795547570733027780[201] = 0;
   out_7795547570733027780[202] = 0;
   out_7795547570733027780[203] = 0;
   out_7795547570733027780[204] = 0;
   out_7795547570733027780[205] = 0;
   out_7795547570733027780[206] = 0;
   out_7795547570733027780[207] = 0;
   out_7795547570733027780[208] = 0;
   out_7795547570733027780[209] = 1;
   out_7795547570733027780[210] = 0;
   out_7795547570733027780[211] = 0;
   out_7795547570733027780[212] = 0;
   out_7795547570733027780[213] = 0;
   out_7795547570733027780[214] = 0;
   out_7795547570733027780[215] = 0;
   out_7795547570733027780[216] = 0;
   out_7795547570733027780[217] = 0;
   out_7795547570733027780[218] = 0;
   out_7795547570733027780[219] = 0;
   out_7795547570733027780[220] = 0;
   out_7795547570733027780[221] = 0;
   out_7795547570733027780[222] = 0;
   out_7795547570733027780[223] = 0;
   out_7795547570733027780[224] = 0;
   out_7795547570733027780[225] = 0;
   out_7795547570733027780[226] = 0;
   out_7795547570733027780[227] = 0;
   out_7795547570733027780[228] = 1;
   out_7795547570733027780[229] = 0;
   out_7795547570733027780[230] = 0;
   out_7795547570733027780[231] = 0;
   out_7795547570733027780[232] = 0;
   out_7795547570733027780[233] = 0;
   out_7795547570733027780[234] = 0;
   out_7795547570733027780[235] = 0;
   out_7795547570733027780[236] = 0;
   out_7795547570733027780[237] = 0;
   out_7795547570733027780[238] = 0;
   out_7795547570733027780[239] = 0;
   out_7795547570733027780[240] = 0;
   out_7795547570733027780[241] = 0;
   out_7795547570733027780[242] = 0;
   out_7795547570733027780[243] = 0;
   out_7795547570733027780[244] = 0;
   out_7795547570733027780[245] = 0;
   out_7795547570733027780[246] = 0;
   out_7795547570733027780[247] = 1;
   out_7795547570733027780[248] = 0;
   out_7795547570733027780[249] = 0;
   out_7795547570733027780[250] = 0;
   out_7795547570733027780[251] = 0;
   out_7795547570733027780[252] = 0;
   out_7795547570733027780[253] = 0;
   out_7795547570733027780[254] = 0;
   out_7795547570733027780[255] = 0;
   out_7795547570733027780[256] = 0;
   out_7795547570733027780[257] = 0;
   out_7795547570733027780[258] = 0;
   out_7795547570733027780[259] = 0;
   out_7795547570733027780[260] = 0;
   out_7795547570733027780[261] = 0;
   out_7795547570733027780[262] = 0;
   out_7795547570733027780[263] = 0;
   out_7795547570733027780[264] = 0;
   out_7795547570733027780[265] = 0;
   out_7795547570733027780[266] = 1;
   out_7795547570733027780[267] = 0;
   out_7795547570733027780[268] = 0;
   out_7795547570733027780[269] = 0;
   out_7795547570733027780[270] = 0;
   out_7795547570733027780[271] = 0;
   out_7795547570733027780[272] = 0;
   out_7795547570733027780[273] = 0;
   out_7795547570733027780[274] = 0;
   out_7795547570733027780[275] = 0;
   out_7795547570733027780[276] = 0;
   out_7795547570733027780[277] = 0;
   out_7795547570733027780[278] = 0;
   out_7795547570733027780[279] = 0;
   out_7795547570733027780[280] = 0;
   out_7795547570733027780[281] = 0;
   out_7795547570733027780[282] = 0;
   out_7795547570733027780[283] = 0;
   out_7795547570733027780[284] = 0;
   out_7795547570733027780[285] = 1;
   out_7795547570733027780[286] = 0;
   out_7795547570733027780[287] = 0;
   out_7795547570733027780[288] = 0;
   out_7795547570733027780[289] = 0;
   out_7795547570733027780[290] = 0;
   out_7795547570733027780[291] = 0;
   out_7795547570733027780[292] = 0;
   out_7795547570733027780[293] = 0;
   out_7795547570733027780[294] = 0;
   out_7795547570733027780[295] = 0;
   out_7795547570733027780[296] = 0;
   out_7795547570733027780[297] = 0;
   out_7795547570733027780[298] = 0;
   out_7795547570733027780[299] = 0;
   out_7795547570733027780[300] = 0;
   out_7795547570733027780[301] = 0;
   out_7795547570733027780[302] = 0;
   out_7795547570733027780[303] = 0;
   out_7795547570733027780[304] = 1;
   out_7795547570733027780[305] = 0;
   out_7795547570733027780[306] = 0;
   out_7795547570733027780[307] = 0;
   out_7795547570733027780[308] = 0;
   out_7795547570733027780[309] = 0;
   out_7795547570733027780[310] = 0;
   out_7795547570733027780[311] = 0;
   out_7795547570733027780[312] = 0;
   out_7795547570733027780[313] = 0;
   out_7795547570733027780[314] = 0;
   out_7795547570733027780[315] = 0;
   out_7795547570733027780[316] = 0;
   out_7795547570733027780[317] = 0;
   out_7795547570733027780[318] = 0;
   out_7795547570733027780[319] = 0;
   out_7795547570733027780[320] = 0;
   out_7795547570733027780[321] = 0;
   out_7795547570733027780[322] = 0;
   out_7795547570733027780[323] = 1;
}
void h_4(double *state, double *unused, double *out_4129260781455939877) {
   out_4129260781455939877[0] = state[6] + state[9];
   out_4129260781455939877[1] = state[7] + state[10];
   out_4129260781455939877[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_1078411690355293289) {
   out_1078411690355293289[0] = 0;
   out_1078411690355293289[1] = 0;
   out_1078411690355293289[2] = 0;
   out_1078411690355293289[3] = 0;
   out_1078411690355293289[4] = 0;
   out_1078411690355293289[5] = 0;
   out_1078411690355293289[6] = 1;
   out_1078411690355293289[7] = 0;
   out_1078411690355293289[8] = 0;
   out_1078411690355293289[9] = 1;
   out_1078411690355293289[10] = 0;
   out_1078411690355293289[11] = 0;
   out_1078411690355293289[12] = 0;
   out_1078411690355293289[13] = 0;
   out_1078411690355293289[14] = 0;
   out_1078411690355293289[15] = 0;
   out_1078411690355293289[16] = 0;
   out_1078411690355293289[17] = 0;
   out_1078411690355293289[18] = 0;
   out_1078411690355293289[19] = 0;
   out_1078411690355293289[20] = 0;
   out_1078411690355293289[21] = 0;
   out_1078411690355293289[22] = 0;
   out_1078411690355293289[23] = 0;
   out_1078411690355293289[24] = 0;
   out_1078411690355293289[25] = 1;
   out_1078411690355293289[26] = 0;
   out_1078411690355293289[27] = 0;
   out_1078411690355293289[28] = 1;
   out_1078411690355293289[29] = 0;
   out_1078411690355293289[30] = 0;
   out_1078411690355293289[31] = 0;
   out_1078411690355293289[32] = 0;
   out_1078411690355293289[33] = 0;
   out_1078411690355293289[34] = 0;
   out_1078411690355293289[35] = 0;
   out_1078411690355293289[36] = 0;
   out_1078411690355293289[37] = 0;
   out_1078411690355293289[38] = 0;
   out_1078411690355293289[39] = 0;
   out_1078411690355293289[40] = 0;
   out_1078411690355293289[41] = 0;
   out_1078411690355293289[42] = 0;
   out_1078411690355293289[43] = 0;
   out_1078411690355293289[44] = 1;
   out_1078411690355293289[45] = 0;
   out_1078411690355293289[46] = 0;
   out_1078411690355293289[47] = 1;
   out_1078411690355293289[48] = 0;
   out_1078411690355293289[49] = 0;
   out_1078411690355293289[50] = 0;
   out_1078411690355293289[51] = 0;
   out_1078411690355293289[52] = 0;
   out_1078411690355293289[53] = 0;
}
void h_10(double *state, double *unused, double *out_275654426612283760) {
   out_275654426612283760[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_275654426612283760[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_275654426612283760[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_3325379419715504236) {
   out_3325379419715504236[0] = 0;
   out_3325379419715504236[1] = 9.8100000000000005*cos(state[1]);
   out_3325379419715504236[2] = 0;
   out_3325379419715504236[3] = 0;
   out_3325379419715504236[4] = -state[8];
   out_3325379419715504236[5] = state[7];
   out_3325379419715504236[6] = 0;
   out_3325379419715504236[7] = state[5];
   out_3325379419715504236[8] = -state[4];
   out_3325379419715504236[9] = 0;
   out_3325379419715504236[10] = 0;
   out_3325379419715504236[11] = 0;
   out_3325379419715504236[12] = 1;
   out_3325379419715504236[13] = 0;
   out_3325379419715504236[14] = 0;
   out_3325379419715504236[15] = 1;
   out_3325379419715504236[16] = 0;
   out_3325379419715504236[17] = 0;
   out_3325379419715504236[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_3325379419715504236[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_3325379419715504236[20] = 0;
   out_3325379419715504236[21] = state[8];
   out_3325379419715504236[22] = 0;
   out_3325379419715504236[23] = -state[6];
   out_3325379419715504236[24] = -state[5];
   out_3325379419715504236[25] = 0;
   out_3325379419715504236[26] = state[3];
   out_3325379419715504236[27] = 0;
   out_3325379419715504236[28] = 0;
   out_3325379419715504236[29] = 0;
   out_3325379419715504236[30] = 0;
   out_3325379419715504236[31] = 1;
   out_3325379419715504236[32] = 0;
   out_3325379419715504236[33] = 0;
   out_3325379419715504236[34] = 1;
   out_3325379419715504236[35] = 0;
   out_3325379419715504236[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_3325379419715504236[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_3325379419715504236[38] = 0;
   out_3325379419715504236[39] = -state[7];
   out_3325379419715504236[40] = state[6];
   out_3325379419715504236[41] = 0;
   out_3325379419715504236[42] = state[4];
   out_3325379419715504236[43] = -state[3];
   out_3325379419715504236[44] = 0;
   out_3325379419715504236[45] = 0;
   out_3325379419715504236[46] = 0;
   out_3325379419715504236[47] = 0;
   out_3325379419715504236[48] = 0;
   out_3325379419715504236[49] = 0;
   out_3325379419715504236[50] = 1;
   out_3325379419715504236[51] = 0;
   out_3325379419715504236[52] = 0;
   out_3325379419715504236[53] = 1;
}
void h_13(double *state, double *unused, double *out_8251649658839689529) {
   out_8251649658839689529[0] = state[3];
   out_8251649658839689529[1] = state[4];
   out_8251649658839689529[2] = state[5];
}
void H_13(double *state, double *unused, double *out_2133862134977039512) {
   out_2133862134977039512[0] = 0;
   out_2133862134977039512[1] = 0;
   out_2133862134977039512[2] = 0;
   out_2133862134977039512[3] = 1;
   out_2133862134977039512[4] = 0;
   out_2133862134977039512[5] = 0;
   out_2133862134977039512[6] = 0;
   out_2133862134977039512[7] = 0;
   out_2133862134977039512[8] = 0;
   out_2133862134977039512[9] = 0;
   out_2133862134977039512[10] = 0;
   out_2133862134977039512[11] = 0;
   out_2133862134977039512[12] = 0;
   out_2133862134977039512[13] = 0;
   out_2133862134977039512[14] = 0;
   out_2133862134977039512[15] = 0;
   out_2133862134977039512[16] = 0;
   out_2133862134977039512[17] = 0;
   out_2133862134977039512[18] = 0;
   out_2133862134977039512[19] = 0;
   out_2133862134977039512[20] = 0;
   out_2133862134977039512[21] = 0;
   out_2133862134977039512[22] = 1;
   out_2133862134977039512[23] = 0;
   out_2133862134977039512[24] = 0;
   out_2133862134977039512[25] = 0;
   out_2133862134977039512[26] = 0;
   out_2133862134977039512[27] = 0;
   out_2133862134977039512[28] = 0;
   out_2133862134977039512[29] = 0;
   out_2133862134977039512[30] = 0;
   out_2133862134977039512[31] = 0;
   out_2133862134977039512[32] = 0;
   out_2133862134977039512[33] = 0;
   out_2133862134977039512[34] = 0;
   out_2133862134977039512[35] = 0;
   out_2133862134977039512[36] = 0;
   out_2133862134977039512[37] = 0;
   out_2133862134977039512[38] = 0;
   out_2133862134977039512[39] = 0;
   out_2133862134977039512[40] = 0;
   out_2133862134977039512[41] = 1;
   out_2133862134977039512[42] = 0;
   out_2133862134977039512[43] = 0;
   out_2133862134977039512[44] = 0;
   out_2133862134977039512[45] = 0;
   out_2133862134977039512[46] = 0;
   out_2133862134977039512[47] = 0;
   out_2133862134977039512[48] = 0;
   out_2133862134977039512[49] = 0;
   out_2133862134977039512[50] = 0;
   out_2133862134977039512[51] = 0;
   out_2133862134977039512[52] = 0;
   out_2133862134977039512[53] = 0;
}
void h_14(double *state, double *unused, double *out_349945032698230729) {
   out_349945032698230729[0] = state[6];
   out_349945032698230729[1] = state[7];
   out_349945032698230729[2] = state[8];
}
void H_14(double *state, double *unused, double *out_2884829165984191240) {
   out_2884829165984191240[0] = 0;
   out_2884829165984191240[1] = 0;
   out_2884829165984191240[2] = 0;
   out_2884829165984191240[3] = 0;
   out_2884829165984191240[4] = 0;
   out_2884829165984191240[5] = 0;
   out_2884829165984191240[6] = 1;
   out_2884829165984191240[7] = 0;
   out_2884829165984191240[8] = 0;
   out_2884829165984191240[9] = 0;
   out_2884829165984191240[10] = 0;
   out_2884829165984191240[11] = 0;
   out_2884829165984191240[12] = 0;
   out_2884829165984191240[13] = 0;
   out_2884829165984191240[14] = 0;
   out_2884829165984191240[15] = 0;
   out_2884829165984191240[16] = 0;
   out_2884829165984191240[17] = 0;
   out_2884829165984191240[18] = 0;
   out_2884829165984191240[19] = 0;
   out_2884829165984191240[20] = 0;
   out_2884829165984191240[21] = 0;
   out_2884829165984191240[22] = 0;
   out_2884829165984191240[23] = 0;
   out_2884829165984191240[24] = 0;
   out_2884829165984191240[25] = 1;
   out_2884829165984191240[26] = 0;
   out_2884829165984191240[27] = 0;
   out_2884829165984191240[28] = 0;
   out_2884829165984191240[29] = 0;
   out_2884829165984191240[30] = 0;
   out_2884829165984191240[31] = 0;
   out_2884829165984191240[32] = 0;
   out_2884829165984191240[33] = 0;
   out_2884829165984191240[34] = 0;
   out_2884829165984191240[35] = 0;
   out_2884829165984191240[36] = 0;
   out_2884829165984191240[37] = 0;
   out_2884829165984191240[38] = 0;
   out_2884829165984191240[39] = 0;
   out_2884829165984191240[40] = 0;
   out_2884829165984191240[41] = 0;
   out_2884829165984191240[42] = 0;
   out_2884829165984191240[43] = 0;
   out_2884829165984191240[44] = 1;
   out_2884829165984191240[45] = 0;
   out_2884829165984191240[46] = 0;
   out_2884829165984191240[47] = 0;
   out_2884829165984191240[48] = 0;
   out_2884829165984191240[49] = 0;
   out_2884829165984191240[50] = 0;
   out_2884829165984191240[51] = 0;
   out_2884829165984191240[52] = 0;
   out_2884829165984191240[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_2806378377347710691) {
  err_fun(nom_x, delta_x, out_2806378377347710691);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6769519321017128267) {
  inv_err_fun(nom_x, true_x, out_6769519321017128267);
}
void pose_H_mod_fun(double *state, double *out_7023130148617119261) {
  H_mod_fun(state, out_7023130148617119261);
}
void pose_f_fun(double *state, double dt, double *out_5185471552589919441) {
  f_fun(state,  dt, out_5185471552589919441);
}
void pose_F_fun(double *state, double dt, double *out_7795547570733027780) {
  F_fun(state,  dt, out_7795547570733027780);
}
void pose_h_4(double *state, double *unused, double *out_4129260781455939877) {
  h_4(state, unused, out_4129260781455939877);
}
void pose_H_4(double *state, double *unused, double *out_1078411690355293289) {
  H_4(state, unused, out_1078411690355293289);
}
void pose_h_10(double *state, double *unused, double *out_275654426612283760) {
  h_10(state, unused, out_275654426612283760);
}
void pose_H_10(double *state, double *unused, double *out_3325379419715504236) {
  H_10(state, unused, out_3325379419715504236);
}
void pose_h_13(double *state, double *unused, double *out_8251649658839689529) {
  h_13(state, unused, out_8251649658839689529);
}
void pose_H_13(double *state, double *unused, double *out_2133862134977039512) {
  H_13(state, unused, out_2133862134977039512);
}
void pose_h_14(double *state, double *unused, double *out_349945032698230729) {
  h_14(state, unused, out_349945032698230729);
}
void pose_H_14(double *state, double *unused, double *out_2884829165984191240) {
  H_14(state, unused, out_2884829165984191240);
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
