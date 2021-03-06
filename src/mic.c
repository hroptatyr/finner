/*** mic.c -- checker for ISO10383 MIC codes
 *
 * Copyright (C) 2014-2018 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "finner.h"
#include "nifty.h"

#ifdef RAGEL_BLOCK
%%{
	machine finner;

	mic_valid =
		"3579" |
		"360T" |
		"A2XX" |
		"AACA" |
		"AAPA" |
		"AATS" |
		"ABNA" |
		"ABSI" |
		"ABUL" |
		"ACEX" |
		"ACKF" |
		"ADVT" |
		"AFDL" |
		"AFET" |
		"AFSA" |
		"AFSI" |
		"AFSL" |
		"AFSO" |
		"AFSX" |
		"AIMX" |
		"AIXK" |
		"AKIS" |
		"ALDP" |
		"ALLT" |
		"ALSI" |
		"ALTX" |
		"ALXB" |
		"ALXL" |
		"ALXP" |
		"AMPX" |
		"AMTS" |
		"AMXO" |
		"ANLP" |
		"ANTS" |
		"ANZL" |
		"APAW" |
		"APCL" |
		"APEX" |
		"APXL" |
		"AQEA" |
		"AQEU" |
		"AQUA" |
		"AQXA" |
		"AQXE" |
		"ARAX" |
		"ARCD" |
		"ARCO" |
		"ARCX" |
		"AREX" |
		"ARIA" |
		"ASEX" |
		"ASTR" |
		"ASXB" |
		"ASXC" |
		"ASXP" |
		"ASXT" |
		"ASXV" |
		"ATDF" |
		"ATFX" |
		"ATLB" |
		"AURB" |
		"AURO" |
		"AUTB" |
		"AUTO" |
		"AUTP" |
		"AUTX" |
		"AWBX" |
		"AWEX" |
		"AXSI" |
		"BAAD" |
		"BACE" |
		"BAEP" |
		"BAIP" |
		"BAJM" |
		"BALT" |
		"BAML" |
		"BAMX" |
		"BANA" |
		"BAPA" |
		"BAPE" |
		"BAPX" |
		"BARD" |
		"BARK" |
		"BARL" |
		"BARO" |
		"BART" |
		"BARU" |
		"BARX" |
		"BASE" |
		"BASI" |
		"BASX" |
		"BATD" |
		"BATE" |
		"BATF" |
		"BATO" |
		"BATP" |
		"BATS" |
		"BATY" |
		"BBIE" |
		"BBIS" |
		"BBSF" |
		"BBSI" |
		"BBVA" |
		"BCDX" |
		"BCEE" |
		"BCFS" |
		"BCMM" |
		"BCRM" |
		"BCSE" |
		"BCSI" |
		"BCSL" |
		"BCXE" |
		"BDEA" |
		"BEAM" |
		"BEEX" |
		"BEIS" |
		"BELB" |
		"BELF" |
		"BERA" |
		"BERB" |
		"BERC" |
		"BETA" |
		"BETP" |
		"BEUD" |
		"BEUE" |
		"BEUF" |
		"BEUO" |
		"BEUP" |
		"BEUT" |
		"BFEX" |
		"BGCB" |
		"BGCD" |
		"BGCF" |
		"BGCI" |
		"BGCO" |
		"BGFI" |
		"BGFX" |
		"BGSI" |
		"BHSF" |
		"BHWA" |
		"BIDS" |
		"BILT" |
		"BINV" |
		"BISI" |
		"BIVA" |
		"BKBF" |
		"BKBR" |
		"BKLF" |
		"BKLN" |
		"BKSK" |
		"BLBB" |
		"BLBF" |
		"BLBS" |
		"BLEV" |
		"BLIQ" |
		"BLKX" |
		"BLNK" |
		"BLOX" |
		"BLPX" |
		"BLTD" |
		"BLTX" |
		"BLXA" |
		"BMCL" |
		"BMEA" |
		"BMEX" |
		"BMLB" |
		"BMLX" |
		"BMSI" |
		"BMTF" |
		"BMTS" |
		"BNDD" |
		"BNPA" |
		"BNPF" |
		"BNPS" |
		"BNSX" |
		"BNTW" |
		"BNYC" |
		"BOAL" |
		"BOAT" |
		"BOFS" |
		"BOSC" |
		"BOSD" |
		"BOSP" |
		"BOTC" |
		"BOTE" |
		"BOTV" |
		"BOVA" |
		"BOVM" |
		"BPAS" |
		"BPKO" |
		"BPLC" |
		"BPOL" |
		"BPSX" |
		"BRDE" |
		"BRDL" |
		"BRFQ" |
		"BRIX" |
		"BRNX" |
		"BSEX" |
		"BSFX" |
		"BSME" |
		"BTEC" |
		"BTEE" |
		"BTFE" |
		"BTNL" |
		"BULK" |
		"BVCA" |
		"BVMF" |
		"BVUK" |
		"BVUR" |
		"BVUS" |
		"BYXD" |
		"BZXD" |
		"C2OX" |
		"CABK" |
		"CAES" |
		"CAND" |
		"CANX" |
		"CAPA" |
		"CAPI" |
		"CASI" |
		"CATS" |
		"CAVE" |
		"CBAL" |
		"CBKA" |
		"CBKD" |
		"CBKE" |
		"CBKF" |
		"CBKG" |
		"CBLC" |
		"CBNL" |
		"CBSX" |
		"CBTS" |
		"CCEU" |
		"CCFX" |
		"CCLX" |
		"CCML" |
		"CCMS" |
		"CCO2" |
		"CCRM" |
		"CCXE" |
		"CDED" |
		"CDSL" |
		"CECS" |
		"CEPL" |
		"CETI" |
		"CEUD" |
		"CEUE" |
		"CEUO" |
		"CFAU" |
		"CFBC" |
		"CFHK" |
		"CFJP" |
		"CFTW" |
		"CGCM" |
		"CGDB" |
		"CGEB" |
		"CGEC" |
		"CGEE" |
		"CGET" |
		"CGGD" |
		"CGIT" |
		"CGMA" |
		"CGMC" |
		"CGMD" |
		"CGME" |
		"CGMH" |
		"CGMI" |
		"CGML" |
		"CGMT" |
		"CGMX" |
		"CGND" |
		"CGQD" |
		"CGQT" |
		"CGTR" |
		"CHEV" |
		"CHIA" |
		"CHIC" |
		"CHID" |
		"CHIJ" |
		"CHIO" |
		"CHIS" |
		"CHIV" |
		"CHIX" |
		"CIBC" |
		"CIBP" |
		"CICX" |
		"CIMB" |
		"CIMD" |
		"CIME" |
		"CIMV" |
		"CIOI" |
		"CITD" |
		"CITX" |
		"CLAU" |
		"CLHK" |
		"CLJP" |
		"CLMX" |
		"CLPH" |
		"CLTD" |
		"CMCI" |
		"CMEC" |
		"CMEE" |
		"CMES" |
		"CMET" |
		"CMMT" |
		"CMSF" |
		"CNSI" |
		"COAL" |
		"COMG" |
		"COMM" |
		"CORE" |
		"COTC" |
		"CPTX" |
		"CRED" |
		"CRYD" |
		"CRYP" |
		"CRYX" |
		"CSAG" |
		"CSAU" |
		"CSBX" |
		"CSCF" |
		"CSEC" |
		"CSEU" |
		"CSHK" |
		"CSIN" |
		"CSJP" |
		"CSLB" |
		"CSLP" |
		"CSOT" |
		"CSSI" |
		"CSSX" |
		"CSTO" |
		"CSZH" |
		"CURX" |
		"CXAC" |
		"CXAF" |
		"CXAM" |
		"CXAN" |
		"CXAP" |
		"CXAQ" |
		"CXAR" |
		"CXAV" |
		"CXAW" |
		"CXOT" |
		"CXRT" |
		"DAIW" |
		"DAPA" |
		"DASI" |
		"DAVY" |
		"DBAG" |
		"DBCR" |
		"DBCX" |
		"DBDC" |
		"DBES" |
		"DBHK" |
		"DBIX" |
		"DBMO" |
		"DBOX" |
		"DBSE" |
		"DBSX" |
		"DCSE" |
		"DCSX" |
		"DDTX" |
		"DEAL" |
		"DEKA" |
		"DGCX" |
		"DHEL" |
		"DICE" |
		"DIFX" |
		"DKED" |
		"DKFI" |
		"DKTC" |
		"DNDK" |
		"DNFI" |
		"DNIS" |
		"DNSE" |
		"DOSE" |
		"DOTS" |
		"DOWG" |
		"DRCT" |
		"DSMD" |
		"DSTO" |
		"DUMX" |
		"DUSA" |
		"DUSB" |
		"DUSC" |
		"DUSD" |
		"DVFX" |
		"DWSF" |
		"DZBK" |
		"EBON" |
		"EBRA" |
		"EBSC" |
		"EBSD" |
		"EBSI" |
		"EBSM" |
		"EBSS" |
		"ECAG" |
		"ECAL" |
		"ECHO" |
		"EDDP" |
		"EDGA" |
		"EDGD" |
		"EDGO" |
		"EDGX" |
		"EDRF" |
		"EEAL" |
		"EESE" |
		"EGMT" |
		"EGSI" |
		"ELEU" |
		"ELIX" |
		"ELNO" |
		"ELSE" |
		"ELUK" |
		"EMBX" |
		"EMDR" |
		"EMIB" |
		"EMID" |
		"EMIR" |
		"EMLD" |
		"EMTF" |
		"EMTS" |
		"ENAX" |
		"ENCL" |
		"ENSY" |
		"ENXB" |
		"ENXL" |
		"EOTC" |
		"EPEX" |
		"EQLD" |
		"EQTA" |
		"EQTB" |
		"EQTC" |
		"EQTD" |
		"ERIS" |
		"ERST" |
		"ESPD" |
		"ESTO" |
		"ETFP" |
		"ETLX" |
		"ETPA" |
		"ETSC" |
		"EUCH" |
		"EURM" |
		"EUSC" |
		"EUSP" |
		"EUWA" |
		"EUWB" |
		"EUWX" |
		"EWSM" |
		"EXAA" |
		"EXBO" |
		"EXCP" |
		"EXDC" |
		"EXEU" |
		"EXLP" |
		"EXMP" |
		"EXOR" |
		"EXOT" |
		"EXSB" |
		"EXSD" |
		"EXSE" |
		"EXSF" |
		"EXSH" |
		"EXSI" |
		"EXSP" |
		"EXSY" |
		"EXTR" |
		"EXVP" |
		"EXYY" |
		"FAIR" |
		"FAST" |
		"FBSI" |
		"FCBT" |
		"FCME" |
		"FGEX" |
		"FICO" |
		"FICX" |
		"FIED" |
		"FINC" |
		"FINN" |
		"FINO" |
		"FINR" |
		"FINY" |
		"FISH" |
		"FISU" |
		"FMTS" |
		"FNCS" |
		"FNDK" |
		"FNEE" |
		"FNFI" |
		"FNIS" |
		"FNLT" |
		"FNLV" |
		"FNSE" |
		"FRAA" |
		"FRAB" |
		"FREI" |
		"FRTE" |
		"FSEF" |
		"FSHX" |
		"FTFM" |
		"FTFS" |
		"FXAL" |
		"FXCL" |
		"FXCM" |
		"FXGB" |
		"FXOP" |
		"FXSW" |
		"G1XX" |
		"GBOT" |
		"GEMX" |
		"GETB" |
		"GFBM" |
		"GFBO" |
		"GFIB" |
		"GFIC" |
		"GFIF" |
		"GFIN" |
		"GFIR" |
		"GFKS" |
		"GFSM" |
		"GFSO" |
		"GLBX" |
		"GLLC" |
		"GLMX" |
		"GLPS" |
		"GLPX" |
		"GMEG" |
		"GMGD" |
		"GMGL" |
		"GMNI" |
		"GMTF" |
		"GMTS" |
		"GOTC" |
		"GOVX" |
		"GREE" |
		"GRIF" |
		"GRIO" |
		"GRSE" |
		"GSAL" |
		"GSBX" |
		"GSCI" |
		"GSCO" |
		"GSEF" |
		"GSIB" |
		"GSIL" |
		"GSSI" |
		"GSXH" |
		"GSXL" |
		"GTCO" |
		"GTSX" |
		"GTXE" |
		"GTXS" |
		"GXGF" |
		"GXGM" |
		"GXGR" |
		"GXMA" |
		"HAMA" |
		"HAMB" |
		"HAML" |
		"HAMM" |
		"HAMN" |
		"HANA" |
		"HANB" |
		"HBFR" |
		"HBPL" |
		"HCHC" |
		"HDAT" |
		"HEGX" |
		"HELA" |
		"HEMO" |
		"HMOD" |
		"HMTF" |
		"HOTC" |
		"HPCO" |
		"HPCS" |
		"HPCV" |
		"HPCX" |
		"HPPO" |
		"HRFQ" |
		"HRSI" |
		"HRTF" |
		"HRTX" |
		"HSBC" |
		"HSBT" |
		"HSFX" |
		"HSTC" |
		"HSXA" |
		"HUDX" |
		"HUNG" |
		"HUPX" |
		"IATS" |
		"IBAL" |
		"IBEQ" |
		"IBEX" |
		"IBGH" |
		"IBIS" |
		"IBUL" |
		"ICBX" |
		"ICDX" |
		"ICEL" |
		"ICEO" |
		"ICES" |
		"ICOT" |
		"ICPM" |
		"ICRO" |
		"ICSU" |
		"ICXL" |
		"IECL" |
		"IEPA" |
		"IEXD" |
		"IEXG" |
		"IFCA" |
		"IFED" |
		"IFEN" |
		"IFEU" |
		"IFLL" |
		"IFLO" |
		"IFLS" |
		"IFLX" |
		"IFSG" |
		"IFSM" |
		"IFUS" |
		"IFUT" |
		"IGDL" |
		"IINX" |
		"IKBS" |
		"IMAG" |
		"IMBD" |
		"IMCC" |
		"IMCE" |
		"IMCG" |
		"IMCO" |
		"IMCR" |
		"IMCS" |
		"IMCT" |
		"IMED" |
		"IMEN" |
		"IMEQ" |
		"IMET" |
		"IMEX" |
		"IMFD" |
		"IMFX" |
		"IMGB" |
		"IMGI" |
		"IMIR" |
		"IMMH" |
		"IMMM" |
		"IMRD" |
		"IMSB" |
		"IMTF" |
		"IMTS" |
		"INCA" |
		"INCR" |
		"INGB" |
		"INGE" |
		"INGF" |
		"INGW" |
		"INSE" |
		"INVE" |
		"IOCD" |
		"IOED" |
		"IOFI" |
		"IOFX" |
		"IOGB" |
		"IOGI" |
		"IOIR" |
		"IOMM" |
		"IOTC" |
		"IOTF" |
		"IPSX" |
		"IPXP" |
		"ISBA" |
		"ISBV" |
		"ISDA" |
		"ISEX" |
		"ISSI" |
		"ISWA" |
		"ISWB" |
		"ISWC" |
		"ISWE" |
		"ISWR" |
		"ISWV" |
		"ITGI" |
		"ITGL" |
		"IVZX" |
		"JADX" |
		"JBSI" |
		"JEFS" |
		"JEFX" |
		"JISI" |
		"JLQD" |
		"JLSI" |
		"JPBX" |
		"JPCB" |
		"JPEU" |
		"JPMI" |
		"JPMX" |
		"JPSI" |
		"JSEB" |
		"JSES" |
		"JSJX" |
		"JSSI" |
		"JYSI" |
		"KAIX" |
		"KBCB" |
		"KBLM" |
		"KCCP" |
		"KELR" |
		"KLEU" |
		"KMUX" |
		"KNCM" |
		"KNEM" |
		"KNIG" |
		"KNLI" |
		"KNMX" |
		"KOTF" |
		"LASF" |
		"LASP" |
		"LBBW" |
		"LBCM" |
		"LBWL" |
		"LBWS" |
		"LCHC" |
		"LCUR" |
		"LEDG" |
		"LEUE" |
		"LEUF" |
		"LEVL" |
		"LICA" |
		"LIFI" |
		"LIGA" |
		"LIQF" |
		"LIQH" |
		"LIQU" |
		"LISX" |
		"LISZ" |
		"LIUH" |
		"LIUS" |
		"LMAD" |
		"LMAE" |
		"LMAF" |
		"LMAO" |
		"LMAX" |
		"LMEC" |
		"LMNR" |
		"LMNX" |
		"LOTC" |
		"LOUI" |
		"LOYD" |
		"LPPM" |
		"LQED" |
		"LQFI" |
		"LSSI" |
		"LTAA" |
		"LXJP" |
		"LYNX" |
		"MABX" |
		"MACB" |
		"MAEL" |
		"MAGM" |
		"MAKX" |
		"MALX" |
		"MANL" |
		"MAQH" |
		"MAQI" |
		"MAQJ" |
		"MAQL" |
		"MAQU" |
		"MAQX" |
		"MARF" |
		"MASG" |
		"MATN" |
		"MBPL" |
		"MBUL" |
		"MCRY" |
		"MCSE" |
		"MCUR" |
		"MCXR" |
		"MCXS" |
		"MCXX" |
		"MCZK" |
		"MDRV" |
		"MELO" |
		"MERD" |
		"MERF" |
		"MERK" |
		"MESQ" |
		"METZ" |
		"MFGL" |
		"MFOX" |
		"MFXA" |
		"MFXC" |
		"MFXR" |
		"MHBD" |
		"MHEL" |
		"MHIP" |
		"MIBG" |
		"MICE" |
		"MIHI" |
		"MISX" |
		"MIVX" |
		"MIZX" |
		"MKAP" |
		"MLCO" |
		"MLER" |
		"MLES" |
		"MLEX" |
		"MLIB" |
		"MLIX" |
		"MLRQ" |
		"MLSI" |
		"MLVX" |
		"MLXB" |
		"MNDK" |
		"MNFI" |
		"MNIS" |
		"MNSE" |
		"MOCX" |
		"MOSE" |
		"MOTX" |
		"MPRL" |
		"MSAL" |
		"MSCO" |
		"MSIP" |
		"MSMS" |
		"MSPL" |
		"MSRP" |
		"MSSI" |
		"MSTO" |
		"MSTX" |
		"MTAA" |
		"MTAH" |
		"MTCH" |
		"MTSA" |
		"MTSB" |
		"MTSC" |
		"MTSD" |
		"MTSF" |
		"MTSG" |
		"MTSO" |
		"MTUS" |
		"MTXA" |
		"MTXC" |
		"MTXM" |
		"MTXS" |
		"MTXX" |
		"MUBE" |
		"MUBL" |
		"MUBM" |
		"MUBP" |
		"MUFP" |
		"MUNA" |
		"MUNB" |
		"MUNC" |
		"MUND" |
		"MUSE" |
		"MUTI" |
		"MVCX" |
		"MYTR" |
		"N2EX" |
		"NABA" |
		"NABE" |
		"NABL" |
		"NABU" |
		"NAMX" |
		"NAPA" |
		"NASD" |
		"NASX" |
		"NATX" |
		"NBLX" |
		"NBOT" |
		"NCEL" |
		"NCML" |
		"NDCM" |
		"NDEX" |
		"NDXS" |
		"NEOC" |
		"NEOD" |
		"NEOE" |
		"NEON" |
		"NESI" |
		"NEXD" |
		"NEXF" |
		"NEXG" |
		"NEXL" |
		"NEXN" |
		"NEXO" |
		"NEXS" |
		"NEXT" |
		"NEXX" |
		"NEXY" |
		"NFSA" |
		"NFSC" |
		"NFSD" |
		"NGXC" |
		"NIBR" |
		"NILX" |
		"NLBX" |
		"NLPX" |
		"NMCE" |
		"NMRA" |
		"NMRJ" |
		"NMTF" |
		"NOCO" |
		"NODX" |
		"NOED" |
		"NOFF" |
		"NOFI" |
		"NOPS" |
		"NORD" |
		"NORX" |
		"NOSC" |
		"NOSI" |
		"NOTC" |
		"NPEX" |
		"NPGA" |
		"NSXB" |
		"NWMS" |
		"NWNV" |
		"NXEU" |
		"NXJP" |
		"NXSE" |
		"NXUS" |
		"NXVW" |
		"NYFX" |
		"NYMS" |
		"NYPC" |
		"NYSD" |
		"NYSI" |
		"NZFX" |
		"OAPA" |
		"OBKL" |
		"OCSI" |
		"ODDO" |
		"ODOC" |
		"OLLC" |
		"OMEL" |
		"OMGA" |
		"OMIC" |
		"OMIP" |
		"ONSE" |
		"OOTC" |
		"OPCO" |
		"OPEX" |
		"OPRA" |
		"OSLC" |
		"OTCB" |
		"OTCE" |
		"OTCM" |
		"OTCQ" |
		"OTCX" |
		"OTPB" |
		"OTXB" |
		"PARX" |
		"PAVE" |
		"PDEX" |
		"PDQX" |
		"PEEL" |
		"PFTQ" |
		"PFTS" |
		"PIEU" |
		"PINC" |
		"PINI" |
		"PINL" |
		"PINX" |
		"PIPE" |
		"PIRM" |
		"PLDX" |
		"PLPD" |
		"PLPX" |
		"PNED" |
		"PORT" |
		"POTC" |
		"PPEX" |
		"PROS" |
		"PRSE" |
		"PSGM" |
		"PTPG" |
		"PURE" |
		"PVMF" |
		"PXIL" |
		"QWIX" |
		"R5FX" |
		"RABO" |
		"RBCB" |
		"RBCC" |
		"RBCE" |
		"RBCM" |
		"RBCT" |
		"RBHU" |
		"RBIV" |
		"RBSI" |
		"RBSX" |
		"RENC" |
		"RESE" |
		"REST" |
		"RICD" |
		"RICX" |
		"RLBO" |
		"RMTS" |
		"ROCO" |
		"ROFX" |
		"ROSR" |
		"ROTC" |
		"RPDX" |
		"RPWC" |
		"RSEX" |
		"RTSI" |
		"RTSL" |
		"RTSX" |
		"RUSX" |
		"S3FM" |
		"SANT" |
		"SB1M" |
		"SBAR" |
		"SBEX" |
		"SBIJ" |
		"SBIL" |
		"SBIU" |
		"SBIV" |
		"SBSI" |
		"SCXA" |
		"SCXF" |
		"SCXM" |
		"SCXO" |
		"SCXS" |
		"SEBS" |
		"SECF" |
		"SEDX" |
		"SEED" |
		"SELC" |
		"SEMX" |
		"SEND" |
		"SEPE" |
		"SFCL" |
		"SGEX" |
		"SGMA" |
		"SGMT" |
		"SGMX" |
		"SGMY" |
		"SGOE" |
		"SHAD" |
		"SHAR" |
		"SHAW" |
		"SHSC" |
		"SIAB" |
		"SIBC" |
		"SICS" |
		"SIGA" |
		"SIGH" |
		"SIGJ" |
		"SIMV" |
		"SISI" |
		"SKSI" |
		"SLHB" |
		"SLKK" |
		"SMBB" |
		"SMBD" |
		"SMBE" |
		"SMBG" |
		"SMBP" |
		"SMEX" |
		"SMTS" |
		"SNSI" |
		"SOHO" |
		"SPEC" |
		"SPIM" |
		"SPRZ" |
		"SPXE" |
		"SQUA" |
		"SSBI" |
		"SSBM" |
		"SSBT" |
		"SSEX" |
		"SSFX" |
		"SSOB" |
		"SSTX" |
		"SSWM" |
		"STAN" |
		"STEE" |
		"STFL" |
		"STOX" |
		"STSI" |
		"STUA" |
		"STUB" |
		"STUC" |
		"STUD" |
		"SUNB" |
		"SUNO" |
		"SUNT" |
		"SVES" |
		"SVEX" |
		"SWAP" |
		"SWBI" |
		"SWEE" |
		"SWLT" |
		"SWLV" |
		"SZSC" |
		"TBSP" |
		"TCDS" |
		"TCML" |
		"TDBL" |
		"TDON" |
		"TECO" |
		"TEEG" |
		"TEFD" |
		"TEFX" |
		"TEGB" |
		"TEGI" |
		"TEIR" |
		"TEMB" |
		"TEMF" |
		"TEMI" |
		"TEMM" |
		"TEMR" |
		"TERA" |
		"TERE" |
		"TFEX" |
		"TFSA" |
		"TFSD" |
		"TFSU" |
		"TGAT" |
		"TGSI" |
		"THEM" |
		"THRE" |
		"TIRD" |
		"TMID" |
		"TNLA" |
		"TNLB" |
		"TOCP" |
		"TOMG" |
		"TPEL" |
		"TPEO" |
		"TPES" |
		"TPEU" |
		"TPIC" |
		"TPIE" |
		"TPIO" |
		"TPIR" |
		"TPIS" |
		"TPSE" |
		"TPSL" |
		"TPSO" |
		"TQEA" |
		"TQEB" |
		"TQEM" |
		"TQEX" |
		"TRAL" |
		"TRAS" |
		"TRAX" |
		"TRCK" |
		"TRDE" |
		"TRDX" |
		"TREA" |
		"TREO" |
		"TREU" |
		"TRFW" |
		"TRNL" |
		"TRPX" |
		"TRQA" |
		"TRQB" |
		"TRQC" |
		"TRQM" |
		"TRQS" |
		"TRQX" |
		"TRSI" |
		"TRU1" |
		"TRU2" |
		"TRUX" |
		"TRWB" |
		"TSAD" |
		"TSAF" |
		"TSBX" |
		"TSCB" |
		"TSCD" |
		"TSED" |
		"TSEF" |
		"TSFI" |
		"TSGB" |
		"TSGI" |
		"TSIG" |
		"TSMM" |
		"TSMR" |
		"TSRE" |
		"TWEA" |
		"TWEM" |
		"TWEO" |
		"TWEU" |
		"TWGP" |
		"TWHK" |
		"TWJP" |
		"TWJT" |
		"TWSF" |
		"TWSG" |
		"UBIM" |
		"UBIN" |
		"UBIS" |
		"UBSA" |
		"UBSB" |
		"UBSC" |
		"UBSD" |
		"UBSE" |
		"UBSF" |
		"UBSG" |
		"UBSI" |
		"UBSL" |
		"UBSP" |
		"UBSS" |
		"UBSX" |
		"UBSY" |
		"UCDE" |
		"UFEX" |
		"UKEX" |
		"UKGD" |
		"UKPX" |
		"ULTX" |
		"USWB" |
		"UTSL" |
		"VABD" |
		"VAGL" |
		"VCMO" |
		"VEGA" |
		"VERT" |
		"VFCM" |
		"VFIL" |
		"VFSI" |
		"VFXO" |
		"VIRT" |
		"VLEX" |
		"VMFX" |
		"VMTS" |
		"VONT" |
		"VPXB" |
		"VTBC" |
		"VTEX" |
		"VTLS" |
		"VTPS" |
		"VWDX" |
		"WBAH" |
		"WBCL" |
		"WBDM" |
		"WBGF" |
		"WBKP" |
		"WBON" |
		"WDER" |
		"WEED" |
		"WELN" |
		"WELS" |
		"WETP" |
		"WGAS" |
		"WIND" |
		"WINS" |
		"WINX" |
		"WIPO" |
		"WMTF" |
		"WOOD" |
		"WOPO" |
		"WQXL" |
		"WSAG" |
		"WSIL" |
		"WSIN" |
		"XABC" |
		"XABG" |
		"XADE" |
		"XADF" |
		"XADS" |
		"XAFR" |
		"XAFX" |
		"XAIM" |
		"XALG" |
		"XALT" |
		"XAMM" |
		"XAMS" |
		"XAPA" |
		"XAQS" |
		"XARM" |
		"XASE" |
		"XASX" |
		"XATH" |
		"XATL" |
		"XATS" |
		"XBAA" |
		"XBAB" |
		"XBAH" |
		"XBAN" |
		"XBAR" |
		"XBBJ" |
		"XBBK" |
		"XBCC" |
		"XBCL" |
		"XBCM" |
		"XBCV" |
		"XBCX" |
		"XBDA" |
		"XBDV" |
		"XBEL" |
		"XBER" |
		"XBES" |
		"XBEY" |
		"XBIL" |
		"XBKF" |
		"XBKK" |
		"XBLB" |
		"XBLN" |
		"XBNV" |
		"XBOG" |
		"XBOL" |
		"XBOM" |
		"XBOS" |
		"XBOT" |
		"XBOX" |
		"XBRA" |
		"XBRD" |
		"XBRM" |
		"XBRN" |
		"XBRT" |
		"XBRU" |
		"XBRV" |
		"XBSD" |
		"XBSE" |
		"XBTR" |
		"XBUD" |
		"XBUE" |
		"XBUL" |
		"XBVC" |
		"XBVM" |
		"XBVR" |
		"XBXO" |
		"XCAI" |
		"XCAL" |
		"XCAN" |
		"XCAS" |
		"XCAY" |
		"XCBF" |
		"XCBO" |
		"XCBT" |
		"XCDE" |
		"XCEC" |
		"XCET" |
		"XCFE" |
		"XCFF" |
		"XCGS" |
		"XCHG" |
		"XCHI" |
		"XCIE" |
		"XCIS" |
		"XCME" |
		"XCNF" |
		"XCNQ" |
		"XCOL" |
		"XCOR" |
		"XCRO" |
		"XCSE" |
		"XCSX" |
		"XCUE" |
		"XCUR" |
		"XCX2" |
		"XCXD" |
		"XCYO" |
		"XCYS" |
		"XDAR" |
		"XDBC" |
		"XDBV" |
		"XDBX" |
		"XDCE" |
		"XDES" |
		"XDFB" |
		"XDFM" |
		"XDHA" |
		"XDMI" |
		"XDNB" |
		"XDPA" |
		"XDRF" |
		"XDSE" |
		"XDSM" |
		"XDSX" |
		"XDUB" |
		"XDUS" |
		"XEBI" |
		"XECB" |
		"XECC" |
		"XECM" |
		"XECS" |
		"XEDA" |
		"XEEE" |
		"XEEO" |
		"XEER" |
		"XEHQ" |
		"XEID" |
		"XELX" |
		"XEMD" |
		"XEMS" |
		"XEQT" |
		"XEQY" |
		"XERE" |
		"XERT" |
		"XESM" |
		"XETA" |
		"XETB" |
		"XETR" |
		"XEUB" |
		"XEUC" |
		"XEUE" |
		"XEUI" |
		"XEUM" |
		"XEUP" |
		"XEUR" |
		"XEYE" |
		"XFCI" |
		"XFDA" |
		"XFEX" |
		"XFKA" |
		"XFNO" |
		"XFRA" |
		"XGAS" |
		"XGAT" |
		"XGCL" |
		"XGCX" |
		"XGDX" |
		"XGEM" |
		"XGHA" |
		"XGME" |
		"XGMX" |
		"XGRM" |
		"XGSE" |
		"XGSX" |
		"XGTG" |
		"XGUA" |
		"XHAM" |
		"XHAN" |
		"XHEL" |
		"XHKF" |
		"XHKG" |
		"XHNF" |
		"XHNX" |
		"XIBE" |
		"XICB" |
		"XICE" |
		"XICX" |
		"XIDX" |
		"XIEL" |
		"XIHK" |
		"XIMA" |
		"XIMC" |
		"XIMM" |
		"XINE" |
		"XINS" |
		"XINV" |
		"XIOM" |
		"XIPO" |
		"XIQS" |
		"XISA" |
		"XISE" |
		"XIST" |
		"XISX" |
		"XJAM" |
		"XJAS" |
		"XJNB" |
		"XJPX" |
		"XJSE" |
		"XKAC" |
		"XKAR" |
		"XKAZ" |
		"XKBT" |
		"XKCE" |
		"XKCM" |
		"XKEM" |
		"XKFB" |
		"XKFE" |
		"XKHA" |
		"XKHR" |
		"XKIE" |
		"XKIS" |
		"XKLS" |
		"XKON" |
		"XKOS" |
		"XKRX" |
		"XKSE" |
		"XKUW" |
		"XLAO" |
		"XLAT" |
		"XLBM" |
		"XLCH" |
		"XLDN" |
		"XLDX" |
		"XLFX" |
		"XLGT" |
		"XLIM" |
		"XLIS" |
		"XLIT" |
		"XLJM" |
		"XLJU" |
		"XLLB" |
		"XLME" |
		"XLOD" |
		"XLOM" |
		"XLON" |
		"XLSM" |
		"XLUS" |
		"XLUX" |
		"XMAB" |
		"XMAD" |
		"XMAE" |
		"XMAI" |
		"XMAL" |
		"XMAN" |
		"XMAT" |
		"XMAU" |
		"XMDG" |
		"XMDS" |
		"XMER" |
		"XMEV" |
		"XMEX" |
		"XMGE" |
		"XMIL" |
		"XMIO" |
		"XMLI" |
		"XMNT" |
		"XMNX" |
		"XMOC" |
		"XMOD" |
		"XMOL" |
		"XMON" |
		"XMOS" |
		"XMOT" |
		"XMPW" |
		"XMRV" |
		"XMSM" |
		"XMSW" |
		"XMTB" |
		"XMTS" |
		"XMUN" |
		"XMUS" |
		"XMVL" |
		"XNAF" |
		"XNAI" |
		"XNAM" |
		"XNAS" |
		"XNCD" |
		"XNCM" |
		"XNCO" |
		"XNDQ" |
		"XNDX" |
		"XNEC" |
		"XNEP" |
		"XNGM" |
		"XNGO" |
		"XNGS" |
		"XNIM" |
		"XNKS" |
		"XNLI" |
		"XNMR" |
		"XNMS" |
		"XNOR" |
		"XNSA" |
		"XNSE" |
		"XNXC" |
		"XNYE" |
		"XNYL" |
		"XNYM" |
		"XNYS" |
		"XNZE" |
		"XOAA" |
		"XOAD" |
		"XOAM" |
		"XOAS" |
		"XOCH" |
		"XODE" |
		"XOFF" |
		"XOPV" |
		"XOSA" |
		"XOSC" |
		"XOSD" |
		"XOSE" |
		"XOSJ" |
		"XOSL" |
		"XOTC" |
		"XPAC" |
		"XPAE" |
		"XPAR" |
		"XPBT" |
		"XPET" |
		"XPHL" |
		"XPHO" |
		"XPHS" |
		"XPHX" |
		"XPIC" |
		"XPMS" |
		"XPOM" |
		"XPOR" |
		"XPOS" |
		"XPOT" |
		"XPOW" |
		"XPRA" |
		"XPRI" |
		"XPRM" |
		"XPSF" |
		"XPST" |
		"XPSX" |
		"XPTY" |
		"XPXE" |
		"XQMH" |
		"XQTX" |
		"XQUI" |
		"XRAS" |
		"XRBM" |
		"XRCB" |
		"XREP" |
		"XRFQ" |
		"XRIS" |
		"XRMO" |
		"XRMZ" |
		"XROS" |
		"XROT" |
		"XROX" |
		"XRPM" |
		"XRSP" |
		"XRUS" |
		"XSAF" |
		"XSAM" |
		"XSAP" |
		"XSAT" |
		"XSAU" |
		"XSBI" |
		"XSBT" |
		"XSC1" |
		"XSC2" |
		"XSC3" |
		"XSCA" |
		"XSCE" |
		"XSCO" |
		"XSCU" |
		"XSEC" |
		"XSEF" |
		"XSES" |
		"XSFA" |
		"XSFE" |
		"XSGA" |
		"XSGE" |
		"XSGO" |
		"XSHE" |
		"XSHG" |
		"XSIB" |
		"XSIM" |
		"XSLS" |
		"XSMP" |
		"XSOP" |
		"XSPM" |
		"XSPS" |
		"XSSC" |
		"XSSE" |
		"XSTC" |
		"XSTE" |
		"XSTF" |
		"XSTM" |
		"XSTO" |
		"XSTU" |
		"XSTV" |
		"XSTX" |
		"XSVA" |
		"XSWA" |
		"XSWB" |
		"XSWM" |
		"XSWX" |
		"XTAE" |
		"XTAF" |
		"XTAI" |
		"XTAL" |
		"XTAM" |
		"XTEH" |
		"XTFF" |
		"XTIR" |
		"XTK1" |
		"XTK2" |
		"XTK3" |
		"XTKS" |
		"XTKT" |
		"XTND" |
		"XTNX" |
		"XTRN" |
		"XTRZ" |
		"XTSE" |
		"XTSX" |
		"XTUC" |
		"XTUN" |
		"XTXM" |
		"XUAX" |
		"XUBS" |
		"XUGA" |
		"XUKR" |
		"XULA" |
		"XUMP" |
		"XUNI" |
		"XUSE" |
		"XVAL" |
		"XVES" |
		"XVIE" |
		"XVPA" |
		"XVPB" |
		"XVTX" |
		"XWAR" |
		"XWBO" |
		"XWEE" |
		"XXSC" |
		"XXXX" |
		"XYIE" |
		"XZAG" |
		"XZAM" |
		"XZAP" |
		"XZCE" |
		"XZIM" |
		"YLDX" |
		"ZAPA" |
		"ZARX" |
		"ZFXM" |
		"ZKBX" |
		"ZOBX" ;

	mic = mic_valid @{c(mic)} ;
}%%
#endif	/* RAGEL_BLOCK */


fn_bid_t
fn_mic(const char *UNUSED(str), size_t len)
{
/* we trust the regexp */
	if (len != 4U) {
		return (fn_bid_t){-1};
	}
	return S("mic");
}

/* mic.c ends here */
