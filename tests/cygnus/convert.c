/* Test conversions */

#include "test.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef __DJGPP__
#define strtodf(s,e)  (float)strtod(s,e)
#define atoff(s)      (float)atof(s)
#define ecvtf         ecvt
#define fcvtf         fcvt
#define gcvtf         gcvt
#endif

static char buffer[500];

extern double_type doubles[];
extern float_type floats[];

/* TEST ATOF  ATOFF */

double_type *pd = doubles;
float_type *pf = floats;

void
_DEFUN_VOID(test_strtod)
{
  char *tail;
  double v;
  /* On average we'll loose 1/2 a bit, so the test is for within 1 bit  */
  v = strtod(pd->string, &tail);
  test_mok(v, pd->value, 64);
  test_iok(tail - pd->string, pd->endscan);
}

void
_DEFUN_VOID(test_strtodf)
{
  char *tail;
  double v;
  /* On average we'll loose 1/2 a bit, so the test is for within 1 bit  */
  v = strtod(pd->string, &tail);
  test_mok(v, pd->value, 32);
  test_iok(tail - pd->string, pd->endscan);
}

void
_DEFUN_VOID(test_strtof)
{
  char *tail;
  float v;
  /* On average we'll loose 1/2 a bit, so the test is for within 1 bit  */
  v = strtof(pf->string, &tail);
  test_mokf(v, pf->value, 32);
  test_iok(tail - pf->string, pf->endscan);
}

void
_DEFUN_VOID(test_atof)
{
  test_mok(atof(pd->string), pd->value, 64);
}

void
_DEFUN_VOID(test_atoff)
{
  test_mok(atoff(pd->string), pd->value, 32);
}


static
void 
_DEFUN(iterate,(func, name),
       void _EXFUN((*func),(void)) _AND
       char *name)
{

  newfunc(name);
  pd = doubles;
  while (pd->string) {
    line(pd->line);
    func();
    pd++;
  }
}


extern int_type ints[];

int_type *p = ints;


static void
_DEFUN(int_iterate,(func, name),
       void (*func)() _AND
       char *name)
{
  newfunc(name);

  p = ints;
  while (p->string) {
    line(p->line);
    errno = 0;
    func();
    p++;
  }
}

void
_DEFUN(test_strtol_base,(base, pi, string),
       int base _AND
       int_scan_type *pi _AND
       char *string)
{
  long r;
  char *ptr;
  errno = 0;
  r = strtol(string, &ptr, base);
  test_iok(r, pi->value);
  test_eok(errno, pi->errno_val);
  test_iok(ptr - string, pi->end);
}

void
_DEFUN_VOID(test_strtol)
{
  test_strtol_base(8,&(p->octal), p->string);
  test_strtol_base(10,&(p->decimal), p->string);
  test_strtol_base(16, &(p->hex), p->string);
  test_strtol_base(0, &(p->normal), p->string);
  test_strtol_base(26, &(p->alphabetical), p->string);
}

void
_DEFUN_VOID(test_atoi)
{
  test_iok(atoi(p->string), p->decimal.value);
  test_eok(errno, p->decimal.errno_val);
}

void
_DEFUN_VOID(test_atol)
{
  test_iok(atol(p->string), p->decimal.value);
  test_eok(errno, p->decimal.errno_val);
}

/* test ECVT and friends */
extern ddouble_type ddoubles[];
ddouble_type *pdd;
void
_DEFUN_VOID(test_ecvtbuf)
{
  int a2,a3;
  char *s;
  s =  ecvtbuf(pdd->value, pdd->e1, &a2, &a3, buffer);

  test_sok(s,pdd->estring);
  test_iok(pdd->e2,a2);
  test_iok(pdd->e3,a3);
}

void
_DEFUN_VOID(test_ecvt)
{
  int a2,a3;
  char *s;
  s =  ecvt(pdd->value, pdd->e1, &a2, &a3);

  test_sok(s,pdd->estring);
  test_iok(pdd->e2,a2);
  test_iok(pdd->e3,a3);

  s =  ecvtf(pdd->value, pdd->e1, &a2, &a3);

  test_sok(s,pdd->estring);
  test_iok(pdd->e2,a2);
  test_iok(pdd->e3,a3);
}

void
_DEFUN_VOID(test_fcvtbuf)
{
  int a2,a3;
  char *s;
  s =  fcvtbuf(pdd->value, pdd->f1, &a2, &a3, buffer);

  test_scok(s,pdd->fstring,10);
  test_iok(pdd->f2,a2);
  test_iok(pdd->f3,a3);
}

void
_DEFUN_VOID(test_gcvt)
{
  char *s = gcvt(pdd->value, pdd->g1, buffer);  
  test_scok(s, pdd->gstring, 9);
  
  s = gcvtf(pdd->value, pdd->g1, buffer);  
  test_scok(s, pdd->gstring, 9);

}

void
_DEFUN_VOID(test_fcvt)
{
  int a2,a3;
  char *sd;
  char *sf;
  double v1;
  double v2;
  sd =  fcvt(pdd->value, pdd->f1, &a2, &a3);

  test_scok(sd,pdd->fstring,10);
  test_iok(pdd->f2,a2);
  test_iok(pdd->f3,a3);

  /* Test the float version by converting and inspecting the numbers 3
   after reconverting */
  sf =  fcvtf(pdd->value, pdd->f1, &a2, &a3);
  sscanf(sd, "%lg", &v1);
  sscanf(sf, "%lg", &v2);
  test_mok(v1, v2,32);
  test_iok(pdd->f2,a2);
  test_iok(pdd->f3,a3);
}

static void

_DEFUN(diterate,(func, name),
       void (*func)() _AND
       char *name)
{
  newfunc(name);

  pdd = ddoubles;
  while (pdd->estring) {
    line(pdd->line);
    errno = 0;
    func();
    pdd++;
  }
}


void
_DEFUN_VOID(deltest)
{
  newfunc("rounding");
  line(1);
  sprintf(buffer,"%.2f", 9.999);
  test_sok(buffer,"10.00");
  line(2);
  sprintf(buffer,"%.2g", 1.0);
  test_sok(buffer,"1");
  line(3);
  sprintf(buffer,"%.2g", 1.2e-6);
  test_sok(buffer,"1.2e-06");
  line(4);
  sprintf(buffer,"%.0g", 1.0);
  test_sok(buffer,"1");
  line(5);
  sprintf(buffer,"%.0e",1e1);
  test_sok(buffer,"1e+01");
  line(6);  
  sprintf(buffer, "%f", 12.3456789);
  test_sok(buffer, "12.345679");
  line(7);  
  sprintf(buffer, "%6.3f", 12.3456789);
  test_sok(buffer, "12.346");
  line(8);  
  sprintf(buffer,"%.0f", 12.3456789);
  test_sok(buffer,"12");
}

/* This tests that NaN and Inf are produced and printed with a corect sign.  */
void
_DEFUN_VOID(test_nan_inf)
{
  __ieee_float_shape_type  flt_inf_nan;
  __ieee_double_shape_type inf_nan;

#ifdef __DJGPP__
  _fpreset();
#endif
  newfunc("nan");
  line(1);
  inf_nan.value = nan();
  sprintf(buffer, "%08lx%08lx", inf_nan.parts.msw, inf_nan.parts.lsw);
  test_sok(buffer, "7ff8000000000000");
  line(2);
  inf_nan.value = -nan();
  sprintf(buffer, "%08lx%08lx", inf_nan.parts.msw, inf_nan.parts.lsw);
  test_sok(buffer, "fff8000000000000");
  newfunc("nan print");
  line(1);
  sprintf(buffer,"%e", nan());
  test_sok(buffer,"nan");
  line(2);
  sprintf(buffer,"%e", -nan());
  test_sok(buffer,"nan");
  line(3);
  sprintf(buffer,"%+e", nan());
  test_sok(buffer,"+nan");
  line(4);
  sprintf(buffer,"%+e", -nan());
  test_sok(buffer,"-nan");
  newfunc("NAN print");
  line(1);
  sprintf(buffer,"%E", nan());
  test_sok(buffer,"NAN");
  line(2);
  sprintf(buffer,"%E", -nan());
  test_sok(buffer,"NAN");
  line(3);
  sprintf(buffer,"%+E", nan());
  test_sok(buffer,"+NAN");
  line(4);
  sprintf(buffer,"%+E", -nan());
  test_sok(buffer,"-NAN");
  newfunc("nanf");
  line(1);
  flt_inf_nan.value = nanf();
  sprintf(buffer, "%08lx", flt_inf_nan.p1);
  test_sok(buffer, "7fc00000");
  line(2);
  flt_inf_nan.value = -nanf();
  sprintf(buffer, "%08lx", flt_inf_nan.p1);
  test_sok(buffer, "ffc00000");
  newfunc("nanf print");
  line(1);
  sprintf(buffer,"%f", nanf());
  test_sok(buffer,"nan");
  line(2);
  sprintf(buffer,"%f", -nanf());
  test_sok(buffer,"nan");
  line(3);
  sprintf(buffer,"%+f", nanf());
  test_sok(buffer,"+nan");
  line(4);
  sprintf(buffer,"%+f", -nanf());
  test_sok(buffer,"-nan");
  newfunc("NANf print");
  line(1);
  sprintf(buffer,"%F", nanf());
  test_sok(buffer,"NAN");
  line(2);
  sprintf(buffer,"%F", -nanf());
  test_sok(buffer,"NAN");
  line(3);
  sprintf(buffer,"%+F", nanf());
  test_sok(buffer,"+NAN");
  line(4);
  sprintf(buffer,"%+F", -nanf());
  test_sok(buffer,"-NAN");
  newfunc("infinity");
  line(1);
  inf_nan.value = infinity();
  sprintf(buffer, "%08lx%08lx", inf_nan.parts.msw, inf_nan.parts.lsw);
  test_sok(buffer, "7ff0000000000000");
  line(2);
  inf_nan.value = -infinity();
  sprintf(buffer, "%08lx%08lx", inf_nan.parts.msw, inf_nan.parts.lsw);
  test_sok(buffer, "fff0000000000000");
  newfunc("infinity print");
  line(1);
  sprintf(buffer,"%e", infinity());
  test_sok(buffer,"inf");
  line(2);
  sprintf(buffer,"%e", -infinity());
  test_sok(buffer,"-inf");
  line(3);
  sprintf(buffer,"%+e", infinity());
  test_sok(buffer,"+inf");
  line(4);
  sprintf(buffer,"%+e", -infinity());
  test_sok(buffer,"-inf");
  newfunc("INFINITY print");
  line(1);
  sprintf(buffer,"%E", infinity());
  test_sok(buffer,"INF");
  line(2);
  sprintf(buffer,"%E", -infinity());
  test_sok(buffer,"-INF");
  line(3);
  sprintf(buffer,"%+E", infinity());
  test_sok(buffer,"+INF");
  line(4);
  sprintf(buffer,"%+E", -infinity());
  test_sok(buffer,"-INF");
  newfunc("infinityf");
  line(1);
  flt_inf_nan.value = infinityf();
  sprintf(buffer, "%08lx", flt_inf_nan.p1);
  test_sok(buffer, "7f800000");
  line(2);
  flt_inf_nan.value = -infinityf();
  sprintf(buffer, "%08lx", flt_inf_nan.p1);
  test_sok(buffer, "ff800000");
  newfunc("infinityf print");
  line(1);
  sprintf(buffer,"%f", infinityf());
  test_sok(buffer,"inf");
  line(2);
  sprintf(buffer,"%f", -infinityf());
  test_sok(buffer,"-inf");
  line(3);
  sprintf(buffer,"%+f", infinityf());
  test_sok(buffer,"+inf");
  line(4);
  sprintf(buffer,"%+f", -infinityf());
  test_sok(buffer,"-inf");
  newfunc("INFINITYf print");
  line(1);
  sprintf(buffer,"%F", infinityf());
  test_sok(buffer,"INF");
  line(2);
  sprintf(buffer,"%F", -infinityf());
  test_sok(buffer,"-INF");
  line(3);
  sprintf(buffer,"%+F", infinityf());
  test_sok(buffer,"+INF");
  line(4);
  sprintf(buffer,"%+F", -infinityf());
  test_sok(buffer,"-INF");
#ifdef __DJGPP__
  _fpreset();
#endif
}

/* Most of what sprint does is tested with the tests of
   fcvt/ecvt/gcvt, but here are some more */
void
_DEFUN_VOID(test_sprint)
{
  extern sprint_double_type sprint_doubles[];
  sprint_double_type *s = sprint_doubles;
  extern sprint_int_type sprint_ints[];
  sprint_int_type *si = sprint_ints;


  newfunc( "sprintf");  


  while (s->line) 
  {
    line( s->line);
    sprintf(buffer, s->format_string, s->value);
    test_scok(buffer, s->result, strpbrk(s->result, "+-") ? 14 : 13);
    s++;
  }

  while (si->line) 
  {
    line( si->line);
    sprintf(buffer, si->format_string, si->value);
    test_sok(buffer, si->result);
    si++;
  }  
}

/* Scanf calls strtod etc tested elsewhere, but also has some pattern matching skills */
void
_DEFUN_VOID(test_scan)
{
  int i,j;
  extern sprint_double_type sprint_doubles[];
  sprint_double_type *s = sprint_doubles;
  extern sprint_int_type sprint_ints[];
  sprint_int_type *si = sprint_ints;

  newfunc( "scanf");  
  
  /* Test scanf by converting all the numbers in the sprint vectors
     to and from their source and making sure nothing breaks */

  while (s->line) 
  {

    double d0,d1;
    line( s->line);
    sscanf(s->result, "%lg", &d0);
    sprintf(buffer, "%20.17e", d0);
    sscanf(buffer, "%lg", &d1);
    test_mok(d0,d1, 64);
    s++;
  }

  /* And integers too */
  while (si->line) 
  {

    long d0,d1;
    
    line(si->line);
    sscanf(si->result, "%ld", &d0);
    sprintf(buffer, "%ld", d0);
    sscanf(buffer, "%ld", &d1);
    test_iok(d0,d1);
    si++;
  }

  /* And the string matching */

  sscanf("    9","%d", &i);
  test_iok(i, 9);
  sscanf("foo bar 123 zap 456","foo bar %d zap %d", &i, &j);
  test_iok(i, 123);
  test_iok(j, 456);
  
  sscanf("magicXYZZYfoobar","magic%[XYZ]", buffer);
  test_sok("XYZZY", buffer);
  sscanf("magicXYZZYfoobar","%[^XYZ]", buffer);
  test_sok("magic", buffer);
}

void
_DEFUN_VOID(test_cvt)
{
  deltest();

  diterate(test_fcvtbuf,"fcvtbuf");
  diterate(test_fcvt,"fcvt/fcvtf");

  diterate(test_gcvt,"gcvt/gcvtf");
  diterate(test_ecvtbuf,"ecvtbuf");
  diterate(test_ecvt,"ecvt/ecvtf");
  
  iterate(test_strtod, "strtod");
  iterate(test_strtof, "strtof");

  test_nan_inf();

  test_scan();
  test_sprint();  
  iterate(test_atof, "atof");
  iterate(test_atoff, "atoff");

  iterate(test_strtodf, "strtodf");

  int_iterate(test_atoi,"atoi");
  int_iterate(test_atol,"atol");
  int_iterate(test_strtol, "strtol");
}
