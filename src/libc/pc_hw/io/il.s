/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/asmdefs.h>

	FUNC(_inportl)
	ENTER

	movl	ARG1,%edx
	inl	%dx,%eax

	LEAVE
