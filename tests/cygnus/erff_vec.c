#include "test.h"
 one_line_type erff_vec[] = {
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc01c0000, 0x00000000, },	/* -1.0000E+00=F(      -7) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc01b0000, 0x00000000, },	/* -1.0000E+00=F(   -6.75) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc01a0000, 0x00000000, },	/* -1.0000E+00=F(    -6.5) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc0190000, 0x00000000, },	/* -1.0000E+00=F(   -6.25) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc0180000, 0x00000000, },	/* -1.0000E+00=F(      -6) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc0170000, 0x00000000, },	/* -1.0000E+00=F(   -5.75) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc0160000, 0x00000000, },	/* -1.0000E+00=F(    -5.5) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc0150000, 0x00000000, },	/* -1.0000E+00=F(   -5.25) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc0140000, 0x00000000, },	/* -1.0000E+00=F(      -5) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc0100000, 0x00000000, },	/* -1.0000E+00=F(      -4) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xc00f8000, 0x00000000, },	/* -1.0000E+00=F(  -3.938) */
{34, 0, 0,__LINE__, 0xbfefffff, 0xe0000000, 0xc00f0000, 0x00000000, },	/* -1.0000E+00=F(  -3.875) */
{34, 0, 0,__LINE__, 0xbfefffff, 0xe0000000, 0xc00e8000, 0x00000000, },	/* -1.0000E+00=F(  -3.812) */
{34, 0, 0,__LINE__, 0xbfefffff, 0xc0000000, 0xc00e0000, 0x00000000, },	/* -1.0000E+00=F(   -3.75) */
{34, 0, 0,__LINE__, 0xbfefffff, 0xa0000000, 0xc00d8000, 0x00000000, },	/* -1.0000E+00=F(  -3.688) */
{34, 0, 0,__LINE__, 0xbfefffff, 0x60000000, 0xc00d0000, 0x00000000, },	/* -1.0000E+00=F(  -3.625) */
{34, 0, 0,__LINE__, 0xbfefffff, 0x00000000, 0xc00c8000, 0x00000000, },	/* -1.0000E+00=F(  -3.562) */
{34, 0, 0,__LINE__, 0xbfeffffe, 0x80000000, 0xc00c0000, 0x00000000, },	/* -1.0000E+00=F(    -3.5) */
{34, 0, 0,__LINE__, 0xbfeffffd, 0x80000000, 0xc00b8000, 0x00000000, },	/* -1.0000E+00=F(  -3.438) */
{34, 0, 0,__LINE__, 0xbfeffffc, 0x40000000, 0xc00b0000, 0x00000000, },	/* -1.0000E+00=F(  -3.375) */
{34, 0, 0,__LINE__, 0xbfeffffa, 0x20000000, 0xc00a8000, 0x00000000, },	/* -1.0000E+00=F(  -3.312) */
{34, 0, 0,__LINE__, 0xbfeffff7, 0x00000000, 0xc00a0000, 0x00000000, },	/* -1.0000E+00=F(   -3.25) */
{34, 0, 0,__LINE__, 0xbfeffff2, 0x40000000, 0xc0098000, 0x00000000, },	/* -9.9999E-01=F(  -3.188) */
{34, 0, 0,__LINE__, 0xbfefffeb, 0x40000000, 0xc0090000, 0x00000000, },	/* -9.9999E-01=F(  -3.125) */
{34, 0, 0,__LINE__, 0xbfefffe0, 0xe0000000, 0xc0088000, 0x00000000, },	/* -9.9999E-01=F(  -3.062) */
{34, 0, 0,__LINE__, 0xbfefffd1, 0xa0000000, 0xc0080000, 0x00000000, },	/* -9.9998E-01=F(      -3) */
{34, 0, 0,__LINE__, 0xbfefffbb, 0x80000000, 0xc0078000, 0x00000000, },	/* -9.9997E-01=F(  -2.938) */
{34, 0, 0,__LINE__, 0xbfefff9b, 0xa0000000, 0xc0070000, 0x00000000, },	/* -9.9995E-01=F(  -2.875) */
{34, 0, 0,__LINE__, 0xbfefff6d, 0xe0000000, 0xc0068000, 0x00000000, },	/* -9.9993E-01=F(  -2.812) */
{34, 0, 0,__LINE__, 0xbfefff2d, 0x00000000, 0xc0060000, 0x00000000, },	/* -9.9990E-01=F(   -2.75) */
{34, 0, 0,__LINE__, 0xbfeffed1, 0x60000000, 0xc0058000, 0x00000000, },	/* -9.9986E-01=F(  -2.688) */
{34, 0, 0,__LINE__, 0xbfeffe51, 0x40000000, 0xc0050000, 0x00000000, },	/* -9.9979E-01=F(  -2.625) */
{34, 0, 0,__LINE__, 0xbfeffd9f, 0x80000000, 0xc0048000, 0x00000000, },	/* -9.9971E-01=F(  -2.562) */
{34, 0, 0,__LINE__, 0xbfeffcaa, 0x80000000, 0xc0040000, 0x00000000, },	/* -9.9959E-01=F(    -2.5) */
{34, 0, 0,__LINE__, 0xbfeffb5b, 0xe0000000, 0xc0038000, 0x00000000, },	/* -9.9943E-01=F(  -2.438) */
{34, 0, 0,__LINE__, 0xbfeff996, 0x00000000, 0xc0030000, 0x00000000, },	/* -9.9922E-01=F(  -2.375) */
{34, 0, 0,__LINE__, 0xbfeff733, 0x80000000, 0xc0028000, 0x00000000, },	/* -9.9893E-01=F(  -2.312) */
{34, 0, 0,__LINE__, 0xbfeff404, 0x80000000, 0xc0020000, 0x00000000, },	/* -9.9854E-01=F(   -2.25) */
{34, 0, 0,__LINE__, 0xbfefefcc, 0xe0000000, 0xc0018000, 0x00000000, },	/* -9.9802E-01=F(  -2.188) */
{34, 0, 0,__LINE__, 0xbfefea42, 0x20000000, 0xc0010000, 0x00000000, },	/* -9.9735E-01=F(  -2.125) */
{34, 0, 0,__LINE__, 0xbfefe308, 0x00000000, 0xc0008000, 0x00000000, },	/* -9.9646E-01=F(  -2.062) */
{34, 0, 0,__LINE__, 0xbfefd9ae, 0x20000000, 0xc0000000, 0x00000000, },	/* -9.9532E-01=F(      -2) */
{34, 0, 0,__LINE__, 0xbfefcdac, 0xc0000000, 0xbfff0000, 0x20000000, },	/* -9.9386E-01=F(  -1.938) */
{34, 0, 0,__LINE__, 0xbfefbe62, 0x00000000, 0xbffe0000, 0x20000000, },	/* -9.9199E-01=F(  -1.875) */
{34, 0, 0,__LINE__, 0xbfefab0d, 0xe0000000, 0xbffd0000, 0x20000000, },	/* -9.8963E-01=F(  -1.813) */
{34, 0, 0,__LINE__, 0xbfef92d0, 0x80000000, 0xbffc0000, 0x20000000, },	/* -9.8667E-01=F(   -1.75) */
{34, 0, 0,__LINE__, 0xbfef74a6, 0xe0000000, 0xbffb0000, 0x20000000, },	/* -9.8299E-01=F(  -1.688) */
{34, 0, 0,__LINE__, 0xbfef4f69, 0x40000000, 0xbffa0000, 0x20000000, },	/* -9.7844E-01=F(  -1.625) */
{34, 0, 0,__LINE__, 0xbfef21ca, 0x00000000, 0xbff90000, 0x20000000, },	/* -9.7287E-01=F(  -1.563) */
{34, 0, 0,__LINE__, 0xbfeeea55, 0x60000000, 0xbff80000, 0x20000000, },	/* -9.6611E-01=F(    -1.5) */
{34, 0, 0,__LINE__, 0xbfeea773, 0x20000000, 0xbff70000, 0x20000000, },	/* -9.5794E-01=F(  -1.438) */
{34, 0, 0,__LINE__, 0xbfee5768, 0xc0000000, 0xbff60000, 0x20000000, },	/* -9.4817E-01=F(  -1.375) */
{34, 0, 0,__LINE__, 0xbfedf85e, 0xc0000000, 0xbff50000, 0x20000000, },	/* -9.3657E-01=F(  -1.313) */
{34, 0, 0,__LINE__, 0xbfed8865, 0xe0000000, 0xbff40000, 0x20000000, },	/* -9.2290E-01=F(   -1.25) */
{34, 0, 0,__LINE__, 0xbfed0580, 0xc0000000, 0xbff30000, 0x20000000, },	/* -9.0692E-01=F(  -1.188) */
{34, 0, 0,__LINE__, 0xbfec6dad, 0x40000000, 0xbff20000, 0x20000000, },	/* -8.8839E-01=F(  -1.125) */
{34, 0, 0,__LINE__, 0xbfebbef1, 0x20000000, 0xbff10000, 0x20000000, },	/* -8.6706E-01=F(  -1.063) */
{34, 0, 0,__LINE__, 0xbfeaf767, 0xc0000000, 0xbff00000, 0x20000000, },	/* -8.4270E-01=F(      -1) */
{34, 0, 0,__LINE__, 0xbfea1551, 0xc0000000, 0xbfee0000, 0x40000000, },	/* -8.1510E-01=F( -0.9375) */
{34, 0, 0,__LINE__, 0xbfe91724, 0xc0000000, 0xbfec0000, 0x40000000, },	/* -7.8408E-01=F(  -0.875) */
{34, 0, 0,__LINE__, 0xbfe7fb9c, 0x20000000, 0xbfea0000, 0x40000000, },	/* -7.4946E-01=F( -0.8125) */
{34, 0, 0,__LINE__, 0xbfe6c1c9, 0xa0000000, 0xbfe80000, 0x40000000, },	/* -7.1116E-01=F(   -0.75) */
{34, 0, 0,__LINE__, 0xbfe56924, 0x60000000, 0xbfe60000, 0x40000000, },	/* -6.6908E-01=F( -0.6875) */
{34, 0, 0,__LINE__, 0xbfe3f197, 0x00000000, 0xbfe40000, 0x40000000, },	/* -6.2324E-01=F(  -0.625) */
{34, 0, 0,__LINE__, 0xbfe25b8a, 0xc0000000, 0xbfe20000, 0x40000000, },	/* -5.7367E-01=F( -0.5625) */
{34, 0, 0,__LINE__, 0xbfe0a7ef, 0xa0000000, 0xbfe00000, 0x40000000, },	/* -5.2050E-01=F(    -0.5) */
{34, 0, 0,__LINE__, 0xbfddb082, 0x40000000, 0xbfdc0000, 0x80000000, },	/* -4.6390E-01=F( -0.4375) */
{34, 0, 0,__LINE__, 0xbfd9dd0d, 0xa0000000, 0xbfd80000, 0x80000000, },	/* -4.0412E-01=F(  -0.375) */
{34, 0, 0,__LINE__, 0xbfd5da9f, 0xc0000000, 0xbfd40000, 0x80000000, },	/* -3.4147E-01=F( -0.3125) */
{34, 0, 0,__LINE__, 0xbfd1af55, 0x60000000, 0xbfd00000, 0x80000000, },	/* -2.7633E-01=F(   -0.25) */
{34, 0, 0,__LINE__, 0xbfcac45f, 0x40000000, 0xbfc80001, 0x00000000, },	/* -2.0912E-01=F( -0.1875) */
{34, 0, 0,__LINE__, 0xbfc1f5e2, 0xc0000000, 0xbfc00001, 0x00000000, },	/* -1.4032E-01=F(  -0.125) */
{34, 0, 0,__LINE__, 0xbfb207d6, 0xc0000000, 0xbfb00002, 0x00000000, },	/* -7.0432E-02=F( -0.0625) */
{34, 0, 0,__LINE__, 0xbe820dd7, 0x60000000, 0xbe800000, 0x00000000, },	/* -1.3451E-07=F(-1.192e-07) */
{34, 0, 0,__LINE__, 0x3e820dd7, 0x60000000, 0x3e800000, 0x00000000, },	/* +1.3451E-07=F(+1.192e-07) */
{34, 0, 0,__LINE__, 0x3fb207d6, 0xc0000000, 0x3fb00002, 0x00000000, },	/* +7.0432E-02=F( +0.0625) */
{34, 0, 0,__LINE__, 0x3fc1f5e2, 0xc0000000, 0x3fc00001, 0x00000000, },	/* +1.4032E-01=F(  +0.125) */
{34, 0, 0,__LINE__, 0x3fcac45f, 0x40000000, 0x3fc80001, 0x00000000, },	/* +2.0912E-01=F( +0.1875) */
{34, 0, 0,__LINE__, 0x3fd1af55, 0x60000000, 0x3fd00000, 0x80000000, },	/* +2.7633E-01=F(   +0.25) */
{34, 0, 0,__LINE__, 0x3fd5da9f, 0xc0000000, 0x3fd40000, 0x80000000, },	/* +3.4147E-01=F( +0.3125) */
{34, 0, 0,__LINE__, 0x3fd9dd0d, 0xa0000000, 0x3fd80000, 0x80000000, },	/* +4.0412E-01=F(  +0.375) */
{34, 0, 0,__LINE__, 0x3fddb082, 0x40000000, 0x3fdc0000, 0x80000000, },	/* +4.6390E-01=F( +0.4375) */
{34, 0, 0,__LINE__, 0x3fe0a7ef, 0xa0000000, 0x3fe00000, 0x40000000, },	/* +5.2050E-01=F(    +0.5) */
{34, 0, 0,__LINE__, 0x3fe25b8a, 0xc0000000, 0x3fe20000, 0x40000000, },	/* +5.7367E-01=F( +0.5625) */
{34, 0, 0,__LINE__, 0x3fe3f197, 0x00000000, 0x3fe40000, 0x40000000, },	/* +6.2324E-01=F(  +0.625) */
{34, 0, 0,__LINE__, 0x3fe56924, 0x60000000, 0x3fe60000, 0x40000000, },	/* +6.6908E-01=F( +0.6875) */
{34, 0, 0,__LINE__, 0x3fe6c1c9, 0xa0000000, 0x3fe80000, 0x40000000, },	/* +7.1116E-01=F(   +0.75) */
{34, 0, 0,__LINE__, 0x3fe7fb9c, 0x20000000, 0x3fea0000, 0x40000000, },	/* +7.4946E-01=F( +0.8125) */
{34, 0, 0,__LINE__, 0x3fe91724, 0xc0000000, 0x3fec0000, 0x40000000, },	/* +7.8408E-01=F(  +0.875) */
{34, 0, 0,__LINE__, 0x3fea1551, 0xc0000000, 0x3fee0000, 0x40000000, },	/* +8.1510E-01=F( +0.9375) */
{34, 0, 0,__LINE__, 0x3feaf767, 0xc0000000, 0x3ff00000, 0x20000000, },	/* +8.4270E-01=F(      +1) */
{34, 0, 0,__LINE__, 0x3febbef1, 0x20000000, 0x3ff10000, 0x20000000, },	/* +8.6706E-01=F(  +1.063) */
{34, 0, 0,__LINE__, 0x3fec6dad, 0x40000000, 0x3ff20000, 0x20000000, },	/* +8.8839E-01=F(  +1.125) */
{34, 0, 0,__LINE__, 0x3fed0580, 0xc0000000, 0x3ff30000, 0x20000000, },	/* +9.0692E-01=F(  +1.188) */
{34, 0, 0,__LINE__, 0x3fed8865, 0xe0000000, 0x3ff40000, 0x20000000, },	/* +9.2290E-01=F(   +1.25) */
{34, 0, 0,__LINE__, 0x3fedf85e, 0xc0000000, 0x3ff50000, 0x20000000, },	/* +9.3657E-01=F(  +1.313) */
{34, 0, 0,__LINE__, 0x3fee5768, 0xc0000000, 0x3ff60000, 0x20000000, },	/* +9.4817E-01=F(  +1.375) */
{34, 0, 0,__LINE__, 0x3feea773, 0x20000000, 0x3ff70000, 0x20000000, },	/* +9.5794E-01=F(  +1.438) */
{34, 0, 0,__LINE__, 0x3feeea55, 0x60000000, 0x3ff80000, 0x20000000, },	/* +9.6611E-01=F(    +1.5) */
{34, 0, 0,__LINE__, 0x3fef21ca, 0x00000000, 0x3ff90000, 0x20000000, },	/* +9.7287E-01=F(  +1.563) */
{34, 0, 0,__LINE__, 0x3fef4f69, 0x40000000, 0x3ffa0000, 0x20000000, },	/* +9.7844E-01=F(  +1.625) */
{34, 0, 0,__LINE__, 0x3fef74a6, 0xe0000000, 0x3ffb0000, 0x20000000, },	/* +9.8299E-01=F(  +1.688) */
{34, 0, 0,__LINE__, 0x3fef92d0, 0x80000000, 0x3ffc0000, 0x20000000, },	/* +9.8667E-01=F(   +1.75) */
{34, 0, 0,__LINE__, 0x3fefab0d, 0xe0000000, 0x3ffd0000, 0x20000000, },	/* +9.8963E-01=F(  +1.813) */
{34, 0, 0,__LINE__, 0x3fefbe62, 0x00000000, 0x3ffe0000, 0x20000000, },	/* +9.9199E-01=F(  +1.875) */
{34, 0, 0,__LINE__, 0x3fefcdac, 0xc0000000, 0x3fff0000, 0x20000000, },	/* +9.9386E-01=F(  +1.938) */
{34, 0, 0,__LINE__, 0x3fefd9ae, 0x20000000, 0x40000000, 0x00000000, },	/* +9.9532E-01=F(      +2) */
{34, 0, 0,__LINE__, 0x3fefe308, 0x00000000, 0x40008000, 0x00000000, },	/* +9.9646E-01=F(  +2.062) */
{34, 0, 0,__LINE__, 0x3fefea42, 0x20000000, 0x40010000, 0x00000000, },	/* +9.9735E-01=F(  +2.125) */
{34, 0, 0,__LINE__, 0x3fefefcc, 0xe0000000, 0x40018000, 0x00000000, },	/* +9.9802E-01=F(  +2.188) */
{34, 0, 0,__LINE__, 0x3feff404, 0x80000000, 0x40020000, 0x00000000, },	/* +9.9854E-01=F(   +2.25) */
{34, 0, 0,__LINE__, 0x3feff733, 0x80000000, 0x40028000, 0x00000000, },	/* +9.9893E-01=F(  +2.312) */
{34, 0, 0,__LINE__, 0x3feff996, 0x00000000, 0x40030000, 0x00000000, },	/* +9.9922E-01=F(  +2.375) */
{34, 0, 0,__LINE__, 0x3feffb5b, 0xe0000000, 0x40038000, 0x00000000, },	/* +9.9943E-01=F(  +2.438) */
{34, 0, 0,__LINE__, 0x3feffcaa, 0x80000000, 0x40040000, 0x00000000, },	/* +9.9959E-01=F(    +2.5) */
{34, 0, 0,__LINE__, 0x3feffd9f, 0x80000000, 0x40048000, 0x00000000, },	/* +9.9971E-01=F(  +2.562) */
{34, 0, 0,__LINE__, 0x3feffe51, 0x40000000, 0x40050000, 0x00000000, },	/* +9.9979E-01=F(  +2.625) */
{34, 0, 0,__LINE__, 0x3feffed1, 0x60000000, 0x40058000, 0x00000000, },	/* +9.9986E-01=F(  +2.688) */
{34, 0, 0,__LINE__, 0x3fefff2d, 0x00000000, 0x40060000, 0x00000000, },	/* +9.9990E-01=F(   +2.75) */
{34, 0, 0,__LINE__, 0x3fefff6d, 0xe0000000, 0x40068000, 0x00000000, },	/* +9.9993E-01=F(  +2.812) */
{34, 0, 0,__LINE__, 0x3fefff9b, 0xa0000000, 0x40070000, 0x00000000, },	/* +9.9995E-01=F(  +2.875) */
{34, 0, 0,__LINE__, 0x3fefffbb, 0x80000000, 0x40078000, 0x00000000, },	/* +9.9997E-01=F(  +2.938) */
{34, 0, 0,__LINE__, 0x3fefffd1, 0xa0000000, 0x40080000, 0x00000000, },	/* +9.9998E-01=F(      +3) */
{34, 0, 0,__LINE__, 0x3fefffe0, 0xe0000000, 0x40088000, 0x00000000, },	/* +9.9999E-01=F(  +3.062) */
{34, 0, 0,__LINE__, 0x3fefffeb, 0x40000000, 0x40090000, 0x00000000, },	/* +9.9999E-01=F(  +3.125) */
{34, 0, 0,__LINE__, 0x3feffff2, 0x40000000, 0x40098000, 0x00000000, },	/* +9.9999E-01=F(  +3.188) */
{34, 0, 0,__LINE__, 0x3feffff7, 0x00000000, 0x400a0000, 0x00000000, },	/* +1.0000E+00=F(   +3.25) */
{34, 0, 0,__LINE__, 0x3feffffa, 0x20000000, 0x400a8000, 0x00000000, },	/* +1.0000E+00=F(  +3.312) */
{34, 0, 0,__LINE__, 0x3feffffc, 0x40000000, 0x400b0000, 0x00000000, },	/* +1.0000E+00=F(  +3.375) */
{34, 0, 0,__LINE__, 0x3feffffd, 0x80000000, 0x400b8000, 0x00000000, },	/* +1.0000E+00=F(  +3.438) */
{34, 0, 0,__LINE__, 0x3feffffe, 0x80000000, 0x400c0000, 0x00000000, },	/* +1.0000E+00=F(    +3.5) */
{34, 0, 0,__LINE__, 0x3fefffff, 0x00000000, 0x400c8000, 0x00000000, },	/* +1.0000E+00=F(  +3.562) */
{34, 0, 0,__LINE__, 0x3fefffff, 0x60000000, 0x400d0000, 0x00000000, },	/* +1.0000E+00=F(  +3.625) */
{34, 0, 0,__LINE__, 0x3fefffff, 0xa0000000, 0x400d8000, 0x00000000, },	/* +1.0000E+00=F(  +3.688) */
{34, 0, 0,__LINE__, 0x3fefffff, 0xc0000000, 0x400e0000, 0x00000000, },	/* +1.0000E+00=F(   +3.75) */
{34, 0, 0,__LINE__, 0x3fefffff, 0xe0000000, 0x400e8000, 0x00000000, },	/* +1.0000E+00=F(  +3.812) */
{34, 0, 0,__LINE__, 0x3fefffff, 0xe0000000, 0x400f0000, 0x00000000, },	/* +1.0000E+00=F(  +3.875) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x400f8000, 0x00000000, },	/* +1.0000E+00=F(  +3.938) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x40100000, 0x00000000, },	/* +1.0000E+00=F(      +4) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x40140000, 0x00000000, },	/* +1.0000E+00=F(      +5) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x40150000, 0x00000000, },	/* +1.0000E+00=F(   +5.25) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x40160000, 0x00000000, },	/* +1.0000E+00=F(    +5.5) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x40170000, 0x00000000, },	/* +1.0000E+00=F(   +5.75) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x40180000, 0x00000000, },	/* +1.0000E+00=F(      +6) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x40190000, 0x00000000, },	/* +1.0000E+00=F(   +6.25) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x401a0000, 0x00000000, },	/* +1.0000E+00=F(    +6.5) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x401b0000, 0x00000000, },	/* +1.0000E+00=F(   +6.75) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x401c0000, 0x00000000, },	/* +1.0000E+00=F(      +7) */
{34, 0, 0,__LINE__, 0x00000000, 0x00000000, 0x00000000, 0x00000000, },	/* +0.0000E+00=F(      +0) */
{34, 0, 0,__LINE__, 0x3ff00000, 0x00000000, 0x7ff00000, 0x00000000, },	/* +1.0000E+00=F(    +Inf) */
{34, 0, 0,__LINE__, 0xbff00000, 0x00000000, 0xfff00000, 0x00000000, },	/* -1.0000E+00=F(    -Inf) */
0,};
void
test_erff(int m)	{ run_vector_1(m, erff_vec,(char *)(erff),"erff","ff");}
