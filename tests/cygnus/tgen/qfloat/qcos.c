/*							qcos.c		*/

/* cosine check routine */

#include "qhead.h"
extern QELT qpi[], qone[];

int qcos( x, y )
QELT *x, *y;
{
QELT a[NQ];

/* cos(x) = sin( pi/2 - x ) */
qmov( qpi, a );
a[1] -= 1;
qsub( x, a, a );
qsin( a, y );
return 0;
}



/* cos(x) - 1 */

int qcosm1( x, y )
QELT *x, *y;
{
QELT a[NQ], s[NQ], x2[NQ], n[NQ];
int sign;

qmul( x, x, x2 );
qclear(s);
qmov( qone, a );
qmov( qone, n );
sign = -1;
do
	{
	qadd( qone, n, n );
	qdiv( n, a, a );
	qmul( x2, a, a );
	if( sign > 0 )
		qadd( a, s, s );
	else
		qsub( a, s, s );
	sign = -sign;
	qadd( qone, n, n );
	qdiv( n, a, a );
	}
while( ((int) s[1] - (int) a[1]) < NBITS );

qmov( s, y );
return 0;
}
