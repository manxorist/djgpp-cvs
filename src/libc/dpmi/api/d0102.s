/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#define USE_EBX
#include "dpmidefs.h"

	FUNC(___dpmi_resize_dos_memory)
	ENTER

	movl	ARG1, %ebx
	movl	ARG2, %edx
	DPMIce(0x0102)

	xorl	%eax,%eax
	RET

L_error:
	movl	ARG3, %ecx
	movzwl	%bx, %ebx
	movl	%ebx, (%ecx)
	movl	$-1, %eax
	
	LEAVE
