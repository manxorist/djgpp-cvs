#include "test.h"
 one_line_type atanhf_vec[] = {
{34, 0, 0,__LINE__, 0xc0036072, 0xa0000000, 0xbfef8000, 0x00000000, },	/* -2.4221E+00=F( -0.9844) */
{34, 0, 0,__LINE__, 0xc0009291, 0xe0000000, 0xbfef0000, 0x00000000, },	/* -2.0716E+00=F( -0.9688) */
{34, 0, 0,__LINE__, 0xbffdd66d, 0xc0000000, 0xbfee8000, 0x00000000, },	/* -1.8649E+00=F( -0.9531) */
{34, 0, 0,__LINE__, 0xbffb78ce, 0x40000000, 0xbfee0000, 0x00000000, },	/* -1.7170E+00=F( -0.9375) */
{34, 0, 0,__LINE__, 0xbff99f39, 0x80000000, 0xbfed8000, 0x00000000, },	/* -1.6014E+00=F( -0.9219) */
{34, 0, 0,__LINE__, 0xbff8191c, 0xa0000000, 0xbfed0000, 0x00000000, },	/* -1.5061E+00=F( -0.9062) */
{34, 0, 0,__LINE__, 0xbff6cc8e, 0x20000000, 0xbfec8000, 0x00000000, },	/* -1.4249E+00=F( -0.8906) */
{34, 0, 0,__LINE__, 0xbff5aa16, 0x40000000, 0xbfec0000, 0x00000000, },	/* -1.3540E+00=F(  -0.875) */
{34, 0, 0,__LINE__, 0xbff4a7ba, 0xa0000000, 0xbfeb8000, 0x00000000, },	/* -1.2909E+00=F( -0.8594) */
{34, 0, 0,__LINE__, 0xbff3beab, 0x00000000, 0xbfeb0000, 0x00000000, },	/* -1.2340E+00=F( -0.8438) */
{34, 0, 0,__LINE__, 0xbff2ea0a, 0xe0000000, 0xbfea8000, 0x00000000, },	/* -1.1821E+00=F( -0.8281) */
{34, 0, 0,__LINE__, 0xbff22643, 0x80000000, 0xbfea0000, 0x00000000, },	/* -1.1343E+00=F( -0.8125) */
{34, 0, 0,__LINE__, 0xbff1709a, 0xc0000000, 0xbfe98000, 0x00000000, },	/* -1.0900E+00=F( -0.7969) */
{34, 0, 0,__LINE__, 0xbff0c6f1, 0xe0000000, 0xbfe90000, 0x00000000, },	/* -1.0486E+00=F( -0.7812) */
{34, 0, 0,__LINE__, 0xbff0279a, 0x80000000, 0xbfe88000, 0x00000000, },	/* -1.0097E+00=F( -0.7656) */
{34, 0, 0,__LINE__, 0xbfef2272, 0xa0000000, 0xbfe80000, 0x00000000, },	/* -9.7296E-01=F(   -0.75) */
{34, 0, 0,__LINE__, 0xbfee0564, 0xe0000000, 0xbfe78000, 0x00000000, },	/* -9.3816E-01=F( -0.7344) */
{34, 0, 0,__LINE__, 0xbfecf634, 0x80000000, 0xbfe70000, 0x00000000, },	/* -9.0505E-01=F( -0.7188) */
{34, 0, 0,__LINE__, 0xbfebf356, 0xc0000000, 0xbfe68000, 0x00000000, },	/* -8.7345E-01=F( -0.7031) */
{34, 0, 0,__LINE__, 0xbfeafb7d, 0x80000000, 0xbfe60000, 0x00000000, },	/* -8.4320E-01=F( -0.6875) */
{34, 0, 0,__LINE__, 0xbfea0d8b, 0x00000000, 0xbfe58000, 0x00000000, },	/* -8.1415E-01=F( -0.6719) */
{34, 0, 0,__LINE__, 0xbfe92889, 0x60000000, 0xbfe50000, 0x00000000, },	/* -7.8620E-01=F( -0.6562) */
{34, 0, 0,__LINE__, 0xbfe84ba3, 0x20000000, 0xbfe48000, 0x00000000, },	/* -7.5923E-01=F( -0.6406) */
{34, 0, 0,__LINE__, 0xbfe7761d, 0xe0000000, 0xbfe40000, 0x00000000, },	/* -7.3317E-01=F(  -0.625) */
{34, 0, 0,__LINE__, 0xbfe6a755, 0xa0000000, 0xbfe38000, 0x00000000, },	/* -7.0793E-01=F( -0.6094) */
{34, 0, 0,__LINE__, 0xbfe5deb9, 0xa0000000, 0xbfe30000, 0x00000000, },	/* -6.8344E-01=F( -0.5938) */
{34, 0, 0,__LINE__, 0xbfe51bc9, 0x20000000, 0xbfe28000, 0x00000000, },	/* -6.5964E-01=F( -0.5781) */
{34, 0, 0,__LINE__, 0xbfe45e11, 0x40000000, 0xbfe20000, 0x00000000, },	/* -6.3648E-01=F( -0.5625) */
{34, 0, 0,__LINE__, 0xbfe3a52a, 0xc0000000, 0xbfe18000, 0x00000000, },	/* -6.1391E-01=F( -0.5469) */
{34, 0, 0,__LINE__, 0xbfe2f0b8, 0xe0000000, 0xbfe10000, 0x00000000, },	/* -5.9189E-01=F( -0.5312) */
{34, 0, 0,__LINE__, 0xbfe24067, 0xa0000000, 0xbfe08000, 0x00000000, },	/* -5.7036E-01=F( -0.5156) */
{34, 0, 0,__LINE__, 0xbfe193ea, 0x80000000, 0xbfe00000, 0x00000000, },	/* -5.4931E-01=F(    -0.5) */
{34, 0, 0,__LINE__, 0xbfe0eafc, 0x20000000, 0xbfdf0000, 0x00000000, },	/* -5.2868E-01=F( -0.4844) */
{34, 0, 0,__LINE__, 0xbfe0455c, 0xe0000000, 0xbfde0000, 0x00000000, },	/* -5.0847E-01=F( -0.4688) */
{34, 0, 0,__LINE__, 0xbfdf45a4, 0xc0000000, 0xbfdd0000, 0x00000000, },	/* -4.8863E-01=F( -0.4531) */
{34, 0, 0,__LINE__, 0xbfde064e, 0x00000000, 0xbfdc0000, 0x00000000, },	/* -4.6913E-01=F( -0.4375) */
{34, 0, 0,__LINE__, 0xbfdccc52, 0x60000000, 0xbfdb0000, 0x00000000, },	/* -4.4997E-01=F( -0.4219) */
{34, 0, 0,__LINE__, 0xbfdb9755, 0xc0000000, 0xbfda0000, 0x00000000, },	/* -4.3111E-01=F( -0.4062) */
{34, 0, 0,__LINE__, 0xbfda6703, 0x20000000, 0xbfd90000, 0x00000000, },	/* -4.1254E-01=F( -0.3906) */
{34, 0, 0,__LINE__, 0xbfd93b0a, 0xe0000000, 0xbfd80000, 0x00000000, },	/* -3.9423E-01=F(  -0.375) */
{34, 0, 0,__LINE__, 0xbfd81323, 0x00000000, 0xbfd70000, 0x00000000, },	/* -3.7617E-01=F( -0.3594) */
{34, 0, 0,__LINE__, 0xbfd6ef06, 0x00000000, 0xbfd60000, 0x00000000, },	/* -3.5834E-01=F( -0.3438) */
{34, 0, 0,__LINE__, 0xbfd5ce72, 0xa0000000, 0xbfd50000, 0x00000000, },	/* -3.4073E-01=F( -0.3281) */
{34, 0, 0,__LINE__, 0xbfd4b12b, 0x80000000, 0xbfd40000, 0x00000000, },	/* -3.2331E-01=F( -0.3125) */
{34, 0, 0,__LINE__, 0xbfd396f6, 0xa0000000, 0xbfd30000, 0x00000000, },	/* -3.0609E-01=F( -0.2969) */
{34, 0, 0,__LINE__, 0xbfd27f9d, 0x20000000, 0xbfd20000, 0x00000000, },	/* -2.8904E-01=F( -0.2812) */
{34, 0, 0,__LINE__, 0xbfd16aeb, 0x20000000, 0xbfd10000, 0x00000000, },	/* -2.7215E-01=F( -0.2656) */
{34, 0, 0,__LINE__, 0xbfd058af, 0x00000000, 0xbfd00000, 0x00000000, },	/* -2.5541E-01=F(   -0.25) */
{34, 0, 0,__LINE__, 0xbfce9173, 0x20000000, 0xbfce0000, 0x00000000, },	/* -2.3881E-01=F( -0.2344) */
{34, 0, 0,__LINE__, 0xbfcc75bb, 0x80000000, 0xbfcc0000, 0x00000000, },	/* -2.2234E-01=F( -0.2188) */
{34, 0, 0,__LINE__, 0xbfca5de0, 0x80000000, 0xbfca0000, 0x00000000, },	/* -2.0599E-01=F( -0.2031) */
{34, 0, 0,__LINE__, 0xbfc8498e, 0xe0000000, 0xbfc80000, 0x00000000, },	/* -1.8974E-01=F( -0.1875) */
{34, 0, 0,__LINE__, 0xbfc63876, 0x60000000, 0xbfc60000, 0x00000000, },	/* -1.7360E-01=F( -0.1719) */
{34, 0, 0,__LINE__, 0xbfc42a49, 0xc0000000, 0xbfc40000, 0x00000000, },	/* -1.5754E-01=F( -0.1562) */
{34, 0, 0,__LINE__, 0xbfc21ebd, 0xa0000000, 0xbfc20000, 0x00000000, },	/* -1.4156E-01=F( -0.1406) */
{34, 0, 0,__LINE__, 0xbfc01589, 0x20000000, 0xbfc00000, 0x00000000, },	/* -1.2566E-01=F(  -0.125) */
{34, 0, 0,__LINE__, 0xbfbc1cca, 0x40000000, 0xbfbc0000, 0x00000000, },	/* -1.0981E-01=F( -0.1094) */
{34, 0, 0,__LINE__, 0xbfb81218, 0x80000000, 0xbfb80000, 0x00000000, },	/* -9.4026E-02=F(-0.09375) */
{34, 0, 0,__LINE__, 0xbfb40a74, 0x80000000, 0xbfb40000, 0x00000000, },	/* -7.8285E-02=F(-0.07812) */
{34, 0, 0,__LINE__, 0xbfb00558, 0x80000000, 0xbfb00000, 0x00000000, },	/* -6.2582E-02=F( -0.0625) */
{34, 0, 0,__LINE__, 0xbfa80481, 0x80000000, 0xbfa80000, 0x00000000, },	/* -4.6909E-02=F(-0.04688) */
{34, 0, 0,__LINE__, 0xbfa00155, 0x80000000, 0xbfa00000, 0x00000000, },	/* -3.1260E-02=F(-0.03125) */
{34, 0, 0,__LINE__, 0xbf900055, 0x60000000, 0xbf900000, 0x00000000, },	/* -1.5626E-02=F(-0.01562) */
{34, 0, 0,__LINE__, 0x00000000, 0x00000000, 0x00000000, 0x00000000, },	/* +0.0000E+00=F(      +0) */
{34, 0, 0,__LINE__, 0x3f900055, 0x60000000, 0x3f900000, 0x00000000, },	/* +1.5626E-02=F(+0.01562) */
{34, 0, 0,__LINE__, 0x3fa00155, 0x80000000, 0x3fa00000, 0x00000000, },	/* +3.1260E-02=F(+0.03125) */
{34, 0, 0,__LINE__, 0x3fa80481, 0x80000000, 0x3fa80000, 0x00000000, },	/* +4.6909E-02=F(+0.04688) */
{34, 0, 0,__LINE__, 0x3fb00558, 0x80000000, 0x3fb00000, 0x00000000, },	/* +6.2582E-02=F( +0.0625) */
{34, 0, 0,__LINE__, 0x3fb40a74, 0x80000000, 0x3fb40000, 0x00000000, },	/* +7.8285E-02=F(+0.07812) */
{34, 0, 0,__LINE__, 0x3fb81218, 0x80000000, 0x3fb80000, 0x00000000, },	/* +9.4026E-02=F(+0.09375) */
{34, 0, 0,__LINE__, 0x3fbc1cca, 0x40000000, 0x3fbc0000, 0x00000000, },	/* +1.0981E-01=F( +0.1094) */
{34, 0, 0,__LINE__, 0x3fc01589, 0x20000000, 0x3fc00000, 0x00000000, },	/* +1.2566E-01=F(  +0.125) */
{34, 0, 0,__LINE__, 0x3fc21ebd, 0xa0000000, 0x3fc20000, 0x00000000, },	/* +1.4156E-01=F( +0.1406) */
{34, 0, 0,__LINE__, 0x3fc42a49, 0xc0000000, 0x3fc40000, 0x00000000, },	/* +1.5754E-01=F( +0.1562) */
{34, 0, 0,__LINE__, 0x3fc63876, 0x60000000, 0x3fc60000, 0x00000000, },	/* +1.7360E-01=F( +0.1719) */
{34, 0, 0,__LINE__, 0x3fc8498e, 0xe0000000, 0x3fc80000, 0x00000000, },	/* +1.8974E-01=F( +0.1875) */
{34, 0, 0,__LINE__, 0x3fca5de0, 0x80000000, 0x3fca0000, 0x00000000, },	/* +2.0599E-01=F( +0.2031) */
{34, 0, 0,__LINE__, 0x3fcc75bb, 0x80000000, 0x3fcc0000, 0x00000000, },	/* +2.2234E-01=F( +0.2188) */
{34, 0, 0,__LINE__, 0x3fce9173, 0x20000000, 0x3fce0000, 0x00000000, },	/* +2.3881E-01=F( +0.2344) */
{34, 0, 0,__LINE__, 0x3fd058af, 0x00000000, 0x3fd00000, 0x00000000, },	/* +2.5541E-01=F(   +0.25) */
{34, 0, 0,__LINE__, 0x3fd16aeb, 0x20000000, 0x3fd10000, 0x00000000, },	/* +2.7215E-01=F( +0.2656) */
{34, 0, 0,__LINE__, 0x3fd27f9d, 0x20000000, 0x3fd20000, 0x00000000, },	/* +2.8904E-01=F( +0.2812) */
{34, 0, 0,__LINE__, 0x3fd396f6, 0xa0000000, 0x3fd30000, 0x00000000, },	/* +3.0609E-01=F( +0.2969) */
{34, 0, 0,__LINE__, 0x3fd4b12b, 0x80000000, 0x3fd40000, 0x00000000, },	/* +3.2331E-01=F( +0.3125) */
{34, 0, 0,__LINE__, 0x3fd5ce72, 0xa0000000, 0x3fd50000, 0x00000000, },	/* +3.4073E-01=F( +0.3281) */
{34, 0, 0,__LINE__, 0x3fd6ef06, 0x00000000, 0x3fd60000, 0x00000000, },	/* +3.5834E-01=F( +0.3438) */
{34, 0, 0,__LINE__, 0x3fd81323, 0x00000000, 0x3fd70000, 0x00000000, },	/* +3.7617E-01=F( +0.3594) */
{34, 0, 0,__LINE__, 0x3fd93b0a, 0xe0000000, 0x3fd80000, 0x00000000, },	/* +3.9423E-01=F(  +0.375) */
{34, 0, 0,__LINE__, 0x3fda6703, 0x20000000, 0x3fd90000, 0x00000000, },	/* +4.1254E-01=F( +0.3906) */
{34, 0, 0,__LINE__, 0x3fdb9755, 0xc0000000, 0x3fda0000, 0x00000000, },	/* +4.3111E-01=F( +0.4062) */
{34, 0, 0,__LINE__, 0x3fdccc52, 0x60000000, 0x3fdb0000, 0x00000000, },	/* +4.4997E-01=F( +0.4219) */
{34, 0, 0,__LINE__, 0x3fde064e, 0x00000000, 0x3fdc0000, 0x00000000, },	/* +4.6913E-01=F( +0.4375) */
{34, 0, 0,__LINE__, 0x3fdf45a4, 0xc0000000, 0x3fdd0000, 0x00000000, },	/* +4.8863E-01=F( +0.4531) */
{34, 0, 0,__LINE__, 0x3fe0455c, 0xe0000000, 0x3fde0000, 0x00000000, },	/* +5.0847E-01=F( +0.4688) */
{34, 0, 0,__LINE__, 0x3fe0eafc, 0x20000000, 0x3fdf0000, 0x00000000, },	/* +5.2868E-01=F( +0.4844) */
{34, 0, 0,__LINE__, 0x3fe193ea, 0x80000000, 0x3fe00000, 0x00000000, },	/* +5.4931E-01=F(    +0.5) */
{34, 0, 0,__LINE__, 0x3fe24067, 0xa0000000, 0x3fe08000, 0x00000000, },	/* +5.7036E-01=F( +0.5156) */
{34, 0, 0,__LINE__, 0x3fe2f0b8, 0xe0000000, 0x3fe10000, 0x00000000, },	/* +5.9189E-01=F( +0.5312) */
{34, 0, 0,__LINE__, 0x3fe3a52a, 0xc0000000, 0x3fe18000, 0x00000000, },	/* +6.1391E-01=F( +0.5469) */
{34, 0, 0,__LINE__, 0x3fe45e11, 0x40000000, 0x3fe20000, 0x00000000, },	/* +6.3648E-01=F( +0.5625) */
{34, 0, 0,__LINE__, 0x3fe51bc9, 0x20000000, 0x3fe28000, 0x00000000, },	/* +6.5964E-01=F( +0.5781) */
{34, 0, 0,__LINE__, 0x3fe5deb9, 0xa0000000, 0x3fe30000, 0x00000000, },	/* +6.8344E-01=F( +0.5938) */
{34, 0, 0,__LINE__, 0x3fe6a755, 0xa0000000, 0x3fe38000, 0x00000000, },	/* +7.0793E-01=F( +0.6094) */
{34, 0, 0,__LINE__, 0x3fe7761d, 0xe0000000, 0x3fe40000, 0x00000000, },	/* +7.3317E-01=F(  +0.625) */
{34, 0, 0,__LINE__, 0x3fe84ba3, 0x20000000, 0x3fe48000, 0x00000000, },	/* +7.5923E-01=F( +0.6406) */
{34, 0, 0,__LINE__, 0x3fe92889, 0x60000000, 0x3fe50000, 0x00000000, },	/* +7.8620E-01=F( +0.6562) */
{34, 0, 0,__LINE__, 0x3fea0d8b, 0x00000000, 0x3fe58000, 0x00000000, },	/* +8.1415E-01=F( +0.6719) */
{34, 0, 0,__LINE__, 0x3feafb7d, 0x80000000, 0x3fe60000, 0x00000000, },	/* +8.4320E-01=F( +0.6875) */
{34, 0, 0,__LINE__, 0x3febf356, 0xc0000000, 0x3fe68000, 0x00000000, },	/* +8.7345E-01=F( +0.7031) */
{34, 0, 0,__LINE__, 0x3fecf634, 0x80000000, 0x3fe70000, 0x00000000, },	/* +9.0505E-01=F( +0.7188) */
{34, 0, 0,__LINE__, 0x3fee0564, 0xe0000000, 0x3fe78000, 0x00000000, },	/* +9.3816E-01=F( +0.7344) */
{34, 0, 0,__LINE__, 0x3fef2272, 0xa0000000, 0x3fe80000, 0x00000000, },	/* +9.7296E-01=F(   +0.75) */
{34, 0, 0,__LINE__, 0x3ff0279a, 0x80000000, 0x3fe88000, 0x00000000, },	/* +1.0097E+00=F( +0.7656) */
{34, 0, 0,__LINE__, 0x3ff0c6f1, 0xe0000000, 0x3fe90000, 0x00000000, },	/* +1.0486E+00=F( +0.7812) */
{34, 0, 0,__LINE__, 0x3ff1709a, 0xc0000000, 0x3fe98000, 0x00000000, },	/* +1.0900E+00=F( +0.7969) */
{34, 0, 0,__LINE__, 0x3ff22643, 0x80000000, 0x3fea0000, 0x00000000, },	/* +1.1343E+00=F( +0.8125) */
{34, 0, 0,__LINE__, 0x3ff2ea0a, 0xe0000000, 0x3fea8000, 0x00000000, },	/* +1.1821E+00=F( +0.8281) */
{34, 0, 0,__LINE__, 0x3ff3beab, 0x00000000, 0x3feb0000, 0x00000000, },	/* +1.2340E+00=F( +0.8438) */
{34, 0, 0,__LINE__, 0x3ff4a7ba, 0xa0000000, 0x3feb8000, 0x00000000, },	/* +1.2909E+00=F( +0.8594) */
{34, 0, 0,__LINE__, 0x3ff5aa16, 0x40000000, 0x3fec0000, 0x00000000, },	/* +1.3540E+00=F(  +0.875) */
{34, 0, 0,__LINE__, 0x3ff6cc8e, 0x20000000, 0x3fec8000, 0x00000000, },	/* +1.4249E+00=F( +0.8906) */
{34, 0, 0,__LINE__, 0x3ff8191c, 0xa0000000, 0x3fed0000, 0x00000000, },	/* +1.5061E+00=F( +0.9062) */
{34, 0, 0,__LINE__, 0x3ff99f39, 0x80000000, 0x3fed8000, 0x00000000, },	/* +1.6014E+00=F( +0.9219) */
{34, 0, 0,__LINE__, 0x3ffb78ce, 0x40000000, 0x3fee0000, 0x00000000, },	/* +1.7170E+00=F( +0.9375) */
{34, 0, 0,__LINE__, 0x3ffdd66d, 0xc0000000, 0x3fee8000, 0x00000000, },	/* +1.8649E+00=F( +0.9531) */
{34, 0, 0,__LINE__, 0x40009291, 0xe0000000, 0x3fef0000, 0x00000000, },	/* +2.0716E+00=F( +0.9688) */
{34, 0, 0,__LINE__, 0x40036072, 0xa0000000, 0x3fef8000, 0x00000000, },	/* +2.4221E+00=F( +0.9844) */
{34, 1, 0,__LINE__, 0x7ff80000, 0x00000000, 0xbff40000, 0x00000000, },	/* +NaN       =F(   -1.25) */
{34, 1, 0,__LINE__, 0xfff00000, 0x00000000, 0xbff00000, 0x00000000, },	/* -Inf       =F(      -1) */
{34, 1, 0,__LINE__, 0x7ff80000, 0x00000000, 0x3ff40000, 0x00000000, },	/* +NaN       =F(   +1.25) */
{34, 1, 0,__LINE__, 0x7ff00000, 0x00000000, 0x3ff00000, 0x00000000, },	/* +Inf       =F(      +1) */
{34, 1, 0,__LINE__, 0x7ff80000, 0x00000000, 0xfff00000, 0x00000000, },	/* +NaN       =F(    -Inf) */
{34, 1, 0,__LINE__, 0x7ff80000, 0x00000000, 0x7ff00000, 0x00000000, },	/* +NaN       =F(    +Inf) */
{34, 0, 0,__LINE__, 0xfff80000, 0x00000000, 0xfff80000, 0x00000000, },	/* -NaN       =F(    -NaN) */
{34, 0, 0,__LINE__, 0x7ff80000, 0x00000000, 0x7ff80000, 0x00000000, },	/* +NaN       =F(    +NaN) */
0,};
void
test_atanhf(int m)	{ run_vector_1(m, atanhf_vec,(char *)(atanhf),"atanhf","ff");}
