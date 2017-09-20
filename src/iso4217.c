/*** iso4217.c -- currency symbols
 *
 * Copyright (C) 2008-2017 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@fresse.org>
 *
 * This file is part of finner.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***/
#include "iso4217.h"

/* the list is pre-sorted */
const struct iso4217_s iso4217[] = {
	[UNK] = {"\0\0\0\0", 0, 0},
	[AED] = {"AED", 784, 2},
	[AFN] = {"AFN", 971, 2},
	[ALL] = {"ALL",   8, 2},
	[AMD] = {"AMD",  51, 2},
	[ANG] = {"ANG", 532, 2},
	[AOA] = {"AOA", 973, 2},
	[ARS] = {"ARS",  32, 2},
	[AUD] = {"AUD",  36, 2},
	[AWG] = {"AWG", 533, 2},
	[AZN] = {"AZN", 944, 2},
	[BAM] = {"BAM", 977, 2},
	[BBD] = {"BBD",  52, 2},
	[BDT] = {"BDT",  50, 2},
	[BGN] = {"BGN", 975, 2},
	[BHD] = {"BHD",  48, 3},
	[BIF] = {"BIF", 108, 0},
	[BMD] = {"BMD",  60, 2},
	[BND] = {"BND",  96, 2},
	[BOB] = {"BOB",  68, 2},
	[BOV] = {"BOV", 984, 2},
	[BRL] = {"BRL", 986, 2},
	[BSD] = {"BSD",  44, 2},
	[BTN] = {"BTN",  64, 2},
	[BWP] = {"BWP",  72, 2},
	[BYR] = {"BYR", 974, 0},
	[BZD] = {"BZD",  84, 2},
	[CAD] = {"CAD", 124, 2},
	[CDF] = {"CDF", 976, 2},
	[CHE] = {"CHE", 947, 2},
	[CHF] = {"CHF", 756, 2},
	[CHW] = {"CHW", 948, 2},
	[CLF] = {"CLF", 990, 0},
	[CLP] = {"CLP", 152, 0},
	[CNY] = {"CNY", 156, 2},
	[COP] = {"COP", 170, 2},
	[COU] = {"COU", 970, 2},
	[CRC] = {"CRC", 188, 2},
	[CUP] = {"CUP", 192, 2},
	[CVE] = {"CVE", 132, 2},
	[CZK] = {"CZK", 203, 2},
	[DJF] = {"DJF", 262, 0},
	[DKK] = {"DKK", 208, 2},
	[DOP] = {"DOP", 214, 2},
	[DZD] = {"DZD",  12, 2},
	[EEK] = {"EEK", 233, 2},
	[EGP] = {"EGP", 818, 2},
	[ERN] = {"ERN", 232, 2},
	[ETB] = {"ETB", 230, 2},
	[EUR] = {"EUR", 978, 2},
	[FJD] = {"FJD", 242, 2},
	[FKP] = {"FKP", 238, 2},
	[GBP] = {"GBP", 826, 2},
	[GEL] = {"GEL", 981, 2},
	[GHS] = {"GHS", 936, 2},
	[GIP] = {"GIP", 292, 2},
	[GMD] = {"GMD", 270, 2},
	[GNF] = {"GNF", 324, 0},
	[GTQ] = {"GTQ", 320, 2},
	[GYD] = {"GYD", 328, 2},
	[HKD] = {"HKD", 344, 2},
	[HNL] = {"HNL", 340, 2},
	[HRK] = {"HRK", 191, 2},
	[HTG] = {"HTG", 332, 2},
	[HUF] = {"HUF", 348, 2},
	[IDR] = {"IDR", 360, 2},
	[ILS] = {"ILS", 376, 2},
	[INR] = {"INR", 356, 2},
	[IQD] = {"IQD", 368, 3},
	[IRR] = {"IRR", 364, 2},
	[ISK] = {"ISK", 352, 2},
	[JMD] = {"JMD", 388, 2},
	[JOD] = {"JOD", 400, 3},
	[JPY] = {"JPY", 392, 0},
	[KES] = {"KES", 404, 2},
	[KGS] = {"KGS", 417, 2},
	[KHR] = {"KHR", 116, 2},
	[KMF] = {"KMF", 174, 0},
	[KPW] = {"KPW", 408, 2},
	[KRW] = {"KRW", 410, 0},
	[KWD] = {"KWD", 414, 3},
	[KYD] = {"KYD", 136, 2},
	[KZT] = {"KZT", 398, 2},
	[LAK] = {"LAK", 418, 2},
	[LBP] = {"LBP", 422, 2},
	[LKR] = {"LKR", 144, 2},
	[LRD] = {"LRD", 430, 2},
	[LSL] = {"LSL", 426, 2},
	[LTL] = {"LTL", 440, 2},
	[LVL] = {"LVL", 428, 2},
	[LYD] = {"LYD", 434, 3},
	[MAD] = {"MAD", 504, 2},
	[MDL] = {"MDL", 498, 2},
	[MGA] = {"MGA", 969, 7},
	[MKD] = {"MKD", 807, 2},
	[MMK] = {"MMK", 104, 2},
	[MNT] = {"MNT", 496, 2},
	[MOP] = {"MOP", 446, 2},
	[MRO] = {"MRO", 478, 7},
	[MUR] = {"MUR", 480, 2},
	[MVR] = {"MVR", 462, 2},
	[MWK] = {"MWK", 454, 2},
	[MXN] = {"MXN", 484, 2},
	[MXV] = {"MXV", 979, 2},
	[MYR] = {"MYR", 458, 2},
	[MZN] = {"MZN", 943, 2},
	[NAD] = {"NAD", 516, 2},
	[NGN] = {"NGN", 566, 2},
	[NIO] = {"NIO", 558, 2},
	[NOK] = {"NOK", 578, 2},
	[NPR] = {"NPR", 524, 2},
	[NZD] = {"NZD", 554, 2},
	[OMR] = {"OMR", 512, 3},
	[PAB] = {"PAB", 590, 2},
	[PEN] = {"PEN", 604, 2},
	[PGK] = {"PGK", 598, 2},
	[PHP] = {"PHP", 608, 2},
	[PKR] = {"PKR", 586, 2},
	[PLN] = {"PLN", 985, 2},
	[PYG] = {"PYG", 600, 0},
	[QAR] = {"QAR", 634, 2},
	[RON] = {"RON", 946, 2},
	[RSD] = {"RSD", 941, 2},
	[RUB] = {"RUB", 643, 2},
	[RWF] = {"RWF", 646, 0},
	[SAR] = {"SAR", 682, 2},
	[SBD] = {"SBD",  90, 2},
	[SCR] = {"SCR", 690, 2},
	[SDG] = {"SDG", 938, 2},
	[SEK] = {"SEK", 752, 2},
	[SGD] = {"SGD", 702, 2},
	[SHP] = {"SHP", 654, 2},
	[SKK] = {"SKK", 703, 2},
	[SLL] = {"SLL", 694, 2},
	[SOS] = {"SOS", 706, 2},
	[SRD] = {"SRD", 968, 2},
	[STD] = {"STD", 678, 2},
	[SYP] = {"SYP", 760, 2},
	[SZL] = {"SZL", 748, 2},
	[THB] = {"THB", 764, 2},
	[TJS] = {"TJS", 972, 2},
	[TMM] = {"TMM", 795, 2},
	[TND] = {"TND", 788, 3},
	[TOP] = {"TOP", 776, 2},
	[TRY] = {"TRY", 949, 2},
	[TTD] = {"TTD", 780, 2},
	[TWD] = {"TWD", 901, 2},
	[TZS] = {"TZS", 834, 2},
	[UAH] = {"UAH", 980, 2},
	[UGX] = {"UGX", 800, 2},
	[USD] = {"USD", 840, 2},
	[USN] = {"USN", 997, 2},
	[USS] = {"USS", 998, 2},
	[UYU] = {"UYU", 858, 2},
	[VEF] = {"VEF", 937, 2},
	[VND] = {"VND", 704, 0},
	[XAG] = {"XAG", 961, -1},
	[XAU] = {"XAU", 959, -1},
	[XBA] = {"XBA", 955, -1},
	[XBB] = {"XBB", 956, -1},
	[XBC] = {"XBC", 957, -1},
	[XBD] = {"XBD", 958, -1},
	[XCD] = {"XCD", 951, 2},
	[XDR] = {"XDR", 960, -1},
	[XFU] = {"XFU",   0, -1},
	[XOF] = {"XOF", 952, 0},
	[XPD] = {"XPD", 964, -1},
	[XPF] = {"XPF", 953, 0},
	[XPT] = {"XPT", 962, -1},
	[XTS] = {"XTS", 963, -1},
	[XXX] = {"XXX", 999, -1},
	[YER] = {"YER", 886, 2},
	[ZAR] = {"ZAR", 710, 2},
	[ZMW] = {"ZMW", 967, 2},
	[ZWL] = {"ZWD", 932, 2},

	/* obsolete ... kept for historical purposes */
	[ADF] = {"ADF",  -1, 2},
	[ADP] = {"ADP",  20, 2},
	[AFA] = {"AFA",   4, -1},
	[ALK] = {"ALK",  -1, -1},
	[AON] = {"AON",  24, -1},
	[AOR] = {"AOR", 982, -1},
	[ARA] = {"ARA",  -1, -1},
	[ARL] = {"ARL",  -1, -1},
	[ARM] = {"ARM",  -1, -1},
	[ARP] = {"ARP",  -1, -1},
	[ATS] = {"ATS",  40, 2},
	[AZM] = {"AZM",  31, -1},
	[BEC] = {"BEC", 993, 2},
	[BEF] = {"BEF",  56, 2},
	[BEL] = {"BEL", 992, 2},
	[BGJ] = {"BGJ",  -1, -1},
	[BGK] = {"BGK",  -1, -1},
	[BGL] = {"BGL", 100, -1},
	[BOP] = {"BOP",  -1, -1},
	[BRB] = {"BRB",  -1, -1},
	[BRC] = {"BRC",  -1, -1},
	[BRE] = {"BRE",  -1, -1},
	[BRN] = {"BRN",  -1, -1},
	[BRR] = {"BRR",  -1, -1},
	[BRY] = {"BRY",  -1, -1},
	[BRZ] = {"BRZ",  -1, -1},
	[CFP] = {"CFP",  -1, -1},
	[CNX] = {"CNX",  -1, -1},
	[CSD] = {"CSD", 891, -1},
	[CSJ] = {"CSJ",  -1, 2},
	[CSK] = {"CSK", 200, 2},
	[CYP] = {"CYP", 196, -1},
	[DDM] = {"DDM", 278, 2},
	[DEM] = {"DEM", 276, 2},
	[ECS] = {"ECS", 218, -1},
	[ECV] = {"ECV", 983, -1},
	[EQE] = {"EQE",  -1, -1},
	[ESA] = {"ESA", 996, 2},
	[ESB] = {"ESB", 995, 2},
	[ESP] = {"ESP", 724, 2},
	[FIM] = {"FIM", 246, -1},
	[FRF] = {"FRF", 250, 2},
	[GHC] = {"GHC", 288, -1},
	[GNE] = {"GNE",  -1, -1},
	[GRD] = {"GRD", 300, -1},
	[GWP] = {"GWP", 624, -1},
	[IEP] = {"IEP", 372, -1},
	[ILP] = {"ILP",  -1, -1},
	[ILR] = {"ILR",  -1, -1},
	[ISJ] = {"ISJ",  -1, 2},
	[ITL] = {"ITL", 380, 2},
	[LAJ] = {"LAJ",  -1, -1},
	[LUF] = {"LUF", 442, 2},
	[MAF] = {"MAF",  -1, -1},
	[MCF] = {"MCF",  -1, -1},
	[MGF] = {"MGF", 450, -1},
	[MKN] = {"MKN",  -1, -1},
	[MTL] = {"MTL", 470, -1},
	[MVQ] = {"MVQ",  -1, -1},
	[MXP] = {"MXP",  -1, -1},
	[MZM] = {"MZM", 508, -1},
	[NLG] = {"NLG", 528, 2},
	[PEH] = {"PEH",  -1, -1},
	[PEI] = {"PEI",  -1, -1},
	[PLZ] = {"PLZ", 616, 2},
	[PTE] = {"PTE", 620, 2},
	[ROL] = {"ROL", 642, -1},
	[RUR] = {"RUR", 810, -1},
	[SDD] = {"SDD", 736, -1},
	[SIT] = {"SIT", 705, -1},
	[SML] = {"SML",  -1, -1},
	[SRG] = {"SRG", 740, -1},
	[SUR] = {"SUR",  -1, -1},
	[SVC] = {"SVC", 222, -1},
	[TJR] = {"TJR", 762, -1},
	[TPE] = {"TPE", 626, -1},
	[TRL] = {"TRL", 792, -1},
	[UAK] = {"UAK", 804, -1},
	[UGS] = {"UGS",  -1, -1},
	[UYN] = {"UYN",  -1, -1},
	[VAL] = {"VAL",  -1, -1},
	[VEB] = {"VEB", 862, -1},
	[VNC] = {"VNC",  -1, -1},
	[XEU] = {"XEU", 954, 2},
	[XFO] = {"XFO",  -1, -1},
	[YDD] = {"YDD", 720, -1},
	[YUD] = {"YUD",  -1, -1},
	[YUF] = {"YUF",  -1, -1},
	[YUG] = {"YUG",  -1, -1},
	[YUM] = {"YUM", 891, -1},
	[YUN] = {"YUN",  -1, -1},
	[YUO] = {"YUO",  -1, -1},
	[YUR] = {"YUR",  -1, -1},
	[YUS] = {"YUS",  -1, -1},
	[ZAL] = {"ZAL", 991, -1},
	[ZMK] = {"ZMK", 894, 2},
	[ZRN] = {"ZRN", 180, -1},
	[ZRZ] = {"ZRZ",  -1, -1},
	[ZWC] = {"ZWC",  -1, -1},
	[ZWD] = {"ZWD", 716, 2},
	[ZWN] = {"ZWN", 942, 2},
	[ZWR] = {"ZWR", 935, 2},
};

/* iso4217.c ends here */
