/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
/*****************************************************************************\
 * Interface to 32-bit executable (from stub.asm)
 *
 *   cs:eip	according to COFF header
 *   ds		32-bit data segment for COFF program
 *   fs		selector for our data segment (fs:0 is stubinfo)
 *   ss:sp	our stack (ss to be freed)
 *   <others>	All unspecified registers have unspecified values in them.
\*****************************************************************************/

	.file "crt0.s"

#include "stubinfo.h"

	.comm	__stklen, 4
	.comm	__stubinfo, 4
	.comm	___djgpp_base_address, 4
	.comm	___djgpp_selector_limit, 4
	.lcomm	sel_buf, 8
	/* ___djgpp_ds_alias defined in go32/exceptn.s */

#define MULTIBLOCK 1

	.data

#ifdef MULTIBLOCK
___djgpp_memory_handle_pointer:
	.long	___djgpp_memory_handle_list+8		/* Next free, first for stub */
	.comm	___djgpp_memory_handle_list, 2048	/* Enough for 256 handles */
#endif

sbrk16_first_byte:
#include "sbrk16.ah"
sbrk16_last_byte:

sbrk16_api_ofs:
	.long	0
sbrk16_api_seg:
	.word	0
zero:
	.long	0

#define EXIT16 1
#if EXIT16
exit16_first_byte:
#include "exit16.ah"
exit16_last_byte:
#endif

hook_387_emulator:
	.long	___emu387_load_hook

	/* this pulls in the ident string, generated in .. */
	.long	___libc_ident_string

	/* this is for when main comes from a library */
	.long	_main

	.text

	.globl	start
start:

	pushl	%ds			/* set %es same as %ds */
	popl	%es			/* push/pop 4 bytes shorter than ax */

#if 0 /* we do this in the stub now */
	movl	$edata, %edi		/* set all BSS bytes to zero */
	movl	$end, %ecx
	subl	%edi, %ecx
	xorl	%eax, %eax		/* Zero fill value */
	shrl	$2, %ecx		/* div 4 Longwords not bytes */
	cld
	rep
	stosl
#endif

/* Enable NULL pointer protection if DPMI supports it */
	testb	$0x1, __crt0_startup_flags+1		/* include/crt0.h */
	jnz	1f
	movl	$start, %eax
	cmpl	$0x1000, %eax
	jl	1f
	movw	$0x507, %ax
	.byte 0x64 /* fs: */
	movl	STUBINFO_MEMORY_HANDLE, %esi
	xorl	%ebx, %ebx			/* Offset 0 in mem block */
	movl	$1, %ecx			/* Set one page */
	movl	$zero, %edx
	int	$0x31			/* Make null page uncommitted */
1:
/* Create an alias for DS to be used by real-mode callbacks (exception handler messes with DS itself) */

	movw	%ds, %bx
	movw	$0x000a, %ax
	int	$0x31
	jnc	ds_alias_ok
	movb	$0x4c, %ah
	int	$0x21
ds_alias_ok:
	movw	%ax, ___djgpp_ds_alias
	movl	%eax, %ebx
	movw	$0x0009, %al
	movw	%cs, %cx	/* get CPL from %cs */
	andl	$3, %ecx
	shll	$5, %ecx		/* move it into place */
	orw	$0xc093, %cx
	int	$0x31		/* set access rights for alias */

/* Maybe set our DS limit to 4Gb in size if flag set */
	testb	$0x80, __crt0_startup_flags		/* include/crt0.h */
	jz	2f
	movw	$0xffff, %cx
	movl	%ecx, %edx
	movw	$0x0008, %ax				/* reset alias limit to -1 */
	int	$0x31
	movw	%ds, %bx
	movw	$0x0008, %ax				/* reset DS limit to -1 */
	int	$0x31
	jnc	2f
	andb	$0x7f, __crt0_startup_flags		/* clear it if failure */
2:
#ifdef MULTIBLOCK
	testb	$0x8, __crt0_startup_flags+1		/* include/crt0.h */
	jz	8f
#endif
/* Allocate some DOS memory and copy our sbrk helper into it. */
	movl	$sbrk16_first_byte, %esi
	movzwl	8(%esi), %ebx
	shrl	$4, %ebx
	movw	$0x0100, %ax
	int	$0x31
	jnc	dos_alloc_ok
	movb	$0x4c, %ah
	int	$0x21
dos_alloc_ok:
	movw	%cs, 2(%esi)			/* store API information */
	movw	%ds, 4(%esi)
	movw	%dx, 6(%esi)			/* selector for allocated block */

	movzwl	(%esi), %eax			/* calculate API address */
	movl	%eax, sbrk16_api_ofs

	pushl	%es				/* move the data */
	movw	%dx, %es
	movl	$(sbrk16_last_byte - sbrk16_first_byte), %ecx
	shrl	$2,%ecx
	xorl	%edi, %edi
	cld
	rep
	movsl
	popl	%es

	movl	%edx, %ebx			/* dos memory selector */
	movw	$0x000b, %ax			/* get descriptor */
	movl	$sel_buf, %edi
	int	$0x31

	andb	$0xbf, sel_buf+6		/* make 16-bit */
	andb	$0xf0, sel_buf+5		/* remove old type */
	orb	$0x0a, sel_buf+5		/* set new type to code/read */

	xorl	%eax, %eax			/* allocate new selector */
	movw	$0x0001, %cx
	int	$0x31
	movw	%ax, sbrk16_api_seg

	movl	%eax, %ebx
	movw	$0x000c, %ax			/* set descriptor */
	movl	$sel_buf, %edi
	int	$0x31
#ifdef MULTIBLOCK
8:	movl	$___djgpp_memory_handle_list+8, %edi
	movl	%edi, ___djgpp_memory_handle_pointer
	xorl	%eax, %eax
9:	cmpl	%eax, (%edi)
	je	10f
	mov	%eax, (%edi)
	addl	$4, %edi
	jmp	9b
10:	movw	%cs, %bx
	movw	$0x0006,%ax
	int	$0x31
	movl	%edx,___djgpp_base_address
	movw	%cx,___djgpp_base_address+2
#endif	/* MULTIBLOCK */

/* Initialize the brk/sbrk variables */

/*	movl	$end, __what_size_app_thinks_it_is */
	.byte 0x64 /* fs: */
	movl	STUBINFO_INITIAL_SIZE, %eax
	movl	%eax, __what_size_dpmi_thinks_we_are

/* Maybe lock the initial block, expects BX:CX */
	movl	%ecx,%ebx
	movl	%edx,%ecx
	addw	$4096,%cx			/* Skip null page */
	adcl	$0,%ebx
	subl	$4096,%eax
	pushl	%eax
	call	lock_memory

	.byte 0x64 /* fs: */
	movl	STUBINFO_MEMORY_HANDLE, %eax
	movl	%eax, ___djgpp_memory_handle_list

	.byte 0x64 /* fs: */			/* copy stubinfo into local memory */
	movl	STUBINFO_SIZE, %eax
	pushl	%eax
	call	___sbrk
	movl	%eax, __stubinfo
	movl	%eax, %edi
	.byte 0x64 /* fs: */
	movl	STUBINFO_SIZE, %ecx
	shrl	$2, %ecx
	xorl	%esi, %esi			/* Zero */
	pushl	%ds
	pushl	%fs
	popl	%ds
	cld
	rep
	movsl
	popl	%ds

#if 0						/* done in crt1.c */
	.byte 0x64 /* fs: */			/* set up _go32_info_block structure */
	movzwl	STUBINFO_MINKEEP, %eax
	movl	%eax, __go32_info_block+16	/* .size_of_transfer_buffer */
	.byte 0x64 /* fs: */
	movzwl	STUBINFO_DS_SEGMENT, %eax
	shll	$4, %eax
	movl	%eax, __go32_info_block+12	/* .linear_address_of_transfer_buffer */
	xorl	%eax, %eax
	movl	$1, %ecx
	int	$0x31
	jc	no_selector
	movw	%ax, __go32_info_block+26	/* .selector_for_linear_memory */
	movl	%eax, %ebx
	movl	$8, %eax
	movl	$0x0f, %ecx
	movw	$0xffff, %dx
	int	$0x31				/* Set limit 1Mb */
no_selector:
#endif

	movl	__stklen, %eax		/* get program-requested stack size */
	.byte 0x64 /* fs: */
	movl	STUBINFO_MINSTACK, %ecx	/* get stub-requested stack size */
	cmpl	%ecx, %eax
	jge	use_stubinfo_stack_size	/* use the larger of the two */
	movl	%ecx, %eax
	movl	%eax, __stklen		/* store the actual stack length */
use_stubinfo_stack_size:
	pushl	%eax
	call	___sbrk			/* allocate the memory */
	cmpl	$-1, %eax
	je	no_memory
	addl	__stklen, %eax
	movw	%ds, %dx		/* set stack */
	movw	%dx, %ss
	movl	%eax, %esp

	xorl	%ebp, %ebp
	call	___crt1_startup		/* run program */
	jmp	exit

no_memory:
	movb	$0xff, %al
	jmp	exit

/*-----------------------------------------------------------------------------*/

#define FREESEL(x) movw x, %bx; movw $0x0001, %ax; int $0x31

	.global	__exit
	.align	2
__exit:
	movb	4(%esp), %al
exit:
	movb	%al, %cl
	xorl	%eax,%eax
	movw	%ax,%fs
	movw	%ax,%gs
	cli				/* Just in case they didn't unhook ints */
	FREESEL(__go32_info_block+26)	/* selector for linear memory */
	FREESEL(___djgpp_ds_alias)	/* DS alias for rmcb exceptions */
#ifdef MULTIBLOCK
	testb	$0x8, __crt0_startup_flags+1		/* include/crt0.h */
	jz	9f
#endif
	FREESEL(sbrk16_api_seg)		/* sbrk cs */
	movw	sbrk16_first_byte+6,%dx /* selector for allocated DOS mem */
	movw	$0x101, %ax
	int	$0x31			/* Free block and selector */
#ifdef MULTIBLOCK
9:	movl	___djgpp_memory_handle_pointer, %ebx
	movl	$__exit, %esp		/* We will free stack! Old init code as temp stack */
	jmp	7f
6:	subl	$8, %ebx
	movl	(%ebx), %edi
	movw	2(%ebx), %si
	movw	$0x502, %ax
	int	$0x31
7:	cmpl	$___djgpp_memory_handle_list+8, %ebx
	jne	6b
#endif /* MULTIBLOCK */
	xorl	%ebp, %ebp
	movl	__stubinfo, %edx
#ifdef EXIT16
	movl	STUBINFO_CS_SELECTOR(%edx), %eax
	movw	%ax, sbrk16_api_seg
	xorl	%edi, %edi
	movl	%edi, sbrk16_api_ofs	/* Offset is zero */

	movw	STUBINFO_DS_SELECTOR(%edx), %es
	movb	%cl, %dl
	movl	$exit16_first_byte, %esi
	movl	$(exit16_last_byte - exit16_first_byte), %ecx
	cld
	rep
	movsb

	movl	___djgpp_memory_handle_list, %edi
	movl	___djgpp_memory_handle_list+2, %esi	/* Skip word prefixes */

	movw	%es,%ax
	movw	%ax,%ss
	movl	$0x400,%esp		/* Transfer buffer >= 1024 bytes */
	FREESEL(%ds)
	movw	%cs, %bx
/* Call exit procedure with BX=32-bit CS; SI+DI=32-bit handle; DL=exit status */
	.byte 0x2e
	ljmp	sbrk16_api_ofs
#else
	FREESEL(STUBINFO_DS_SELECTOR(%edx))
	FREESEL(STUBINFO_CS_SELECTOR(%edx))
	movb	%cl, %al
	movb	$0x4c, %ah
	int	$0x21
#endif

/*-----------------------------------------------------------------------------*/

/*	.lcomm	__what_size_app_thinks_it_is, 4 */
__what_size_app_thinks_it_is:
	.long	end
	.lcomm	__what_we_return_to_app_as_old_size, 4
	.lcomm	__what_size_dpmi_thinks_we_are, 4

lock_memory:
	/* BX:CX should be linear address; size is pushed on stack */
	testb	$0x10, __crt0_startup_flags+1		/* include/crt0.h */
	jz	13f
	pushl	%esi
	pushl	%edi
	pushl	%eax
	movl	16(%esp),%edi
	movw	18(%esp),%si
	movw	$0x600,%ax
	int	$0x31
	popl	%eax
	popl	%edi
	popl	%esi
13:	ret	$4			/* Pop the argument */


#if 0
brk_hook_ret:
	ret
	.globl ___sbrk_brk_hook
___sbrk_brk_hook:
	.long	brk_hook_ret
#endif

	.global	___sbrk
	.align	2
___sbrk:
	movl	__what_size_app_thinks_it_is, %eax
	movl	4(%esp), %ecx			/* Increment size */
	addl	%ecx, %eax
	jnc	brk_common
	/* Carry is only set if a negative increment or wrap happens.  Negative
	   increment is semi-OK, wrap (only for multiple zone sbrk) isn't. */
	test	$0x80000000, %ecx		/* Clears carry */
	jnz	brk_common
	stc					/* Put carry back */
	jmp	brk_common

	.globl	___brk
	.align	2
___brk:
	movl	4(%esp), %eax
	clc

brk_common:
	pushl	%esi
	pushl	%edi
	pushl	%ebx

	movl	__what_size_app_thinks_it_is, %edx		/* save info */
	movl	%edx, __what_we_return_to_app_as_old_size
	movl	%eax, __what_size_app_thinks_it_is

	jc	10f						/* Wrap for multi-zone */
	cmpl	__what_size_dpmi_thinks_we_are, %eax		/* don't bother shrinking */
	jbe	brk_nochange

#ifdef MULTIBLOCK
	testb	$0x8, __crt0_startup_flags+1		/* include/crt0.h */
	jz	10f
#endif
	addl	$0x0000ffff, %eax				/* round up to 64K block */
	andl	$0xffff0000, %eax
	push	%eax						/* size - save for later */

	movl	___djgpp_memory_handle_list, %edi		/* request new size */
	movw	___djgpp_memory_handle_list+2, %si
	movl	%eax, %ecx					/* size not limit */
	movl	%eax, %ebx					/* size not limit */
	shrl	$16, %ebx					/* BX:CX size */

	movw	$0x0900, %ax					/* disable interrupts */
	int	$0x31
	pushl	%eax

	lcall	sbrk16_api_ofs
	setc	%dl						/* Save carry */

	popl	%eax					/* restore interrupts */
	int	$0x31

	test	%dl,%dl
	popl	%edx
	jne	brk_error

	movl	%edi, ___djgpp_memory_handle_list		/* store new handle */
	movw	%si, ___djgpp_memory_handle_list+2
	movl	%ecx, ___djgpp_base_address			/* store new base address */
	movw	%bx, ___djgpp_base_address+2

	movl	%edx, %eax
	movl	__what_size_dpmi_thinks_we_are, %ecx
	subl	%ecx, %eax

	addl	___djgpp_base_address, %ecx
	movl	%ecx, %ebx
	shrl	$16, %ebx					/* BX:CX addr */
	pushl	%eax						/* Size */
	call	lock_memory

	decl	%edx						/* limit now, not size */
#ifdef MULTIBLOCK
	jmp	5f
/* Current allocation not large enough, get another block */
10:	movl	%ecx, %eax					/* Add amt */
	pushl	%eax						/* Save orig */
	addl	$0x0000ffff, %eax				/* round up to 64K block */
	andl	$0xffff0000, %eax
	movl	%eax, %edx					/* Save size */
	movl	%eax, %ecx
	movl	%eax, %ebx
	shrl	$16, %ebx					/* BX:CX size */
	movw	$0x501,%ax
	int	$0x31
	popl	%eax						/* Orig size */
	jc	brk_error

	pushl	%edx						/* Size */
	call	lock_memory

	pushw	%bx
	pushw	%cx
	popl	%ecx						/* Linear address */
	subl	___djgpp_base_address, %ecx			/* New dpmi size */
	cmpl	%ecx, __what_size_dpmi_thinks_we_are		/* Back to back ? */
	je	4f
	movl	%ecx, __what_size_dpmi_thinks_we_are
	movl	%ecx, __what_we_return_to_app_as_old_size
4:
	movl	__what_we_return_to_app_as_old_size, %ebx	/* Base for new block */
	addl	%ebx, %eax					/* Final address */
	movl	%eax, __what_size_app_thinks_it_is
/* Note - save adjusted memory base and memory handle SI:DI here */
	movl	___djgpp_memory_handle_pointer, %ebx
	movl	%edi, (%ebx)
	movw	%si, 2(%ebx)
	movl	%ecx, 4(%ebx)
	addl	$8, %ebx
	cmpl	$___djgpp_memory_handle_list+2040, %ebx		/* At end? */
	je	11f
	movl	%ebx, ___djgpp_memory_handle_pointer		/* Only if not at end */
11:
	addl	%ecx, %edx					/* Final address */
	decl	%edx						/* Limit to end */
/* If we get a block at a lower address we must skip the limit change */
	cmpl	___djgpp_selector_limit, %edx
	jbe	12f
#endif
5:	movl	%edx, ___djgpp_selector_limit
	movw	$0x0008, %ax					/* reset CS limit */
	movw	%cs, %bx
	movl	%edx, %ecx
	shrl	$16, %ecx
	int	$0x31						/* CX:DX is limit */

	testb	$0x80, __crt0_startup_flags			/* include/crt0.h */
	jnz	3f
	movw	$0x0008, %ax					/* reset DS limit */
	movw	%ds, %bx
	int	$0x31

	movw	$0x0008, %ax					/* reset DS alias limit */
	movl	___djgpp_ds_alias, %ebx
	int	$0x31
3:
	movw	$0x0007, %ax					/* reset DS alias base */
	movl	___djgpp_ds_alias, %ebx
	movl	___djgpp_base_address, %edx
	movw	___djgpp_base_address+2, %cx
	int	$0x31

	movl	___djgpp_selector_limit, %edx
12:	incl	%edx						/* Size not limit */
	testb	$0x60, __crt0_startup_flags	/* include/crt0.h */
	jz	no_fill_sbrk_memory
	pushl	%ds
	popl	%es

	movl	__what_size_dpmi_thinks_we_are, %edi		/* set all newly resized bytes zero */
	movl	%edx, %ecx					/* Limit */
	subl	%edi, %ecx			/* Adjust count for base */
	xorl	%eax, %eax
	testb	$0x40, __crt0_startup_flags
	jz	no_deadbeef
	movl	$0xdeadbeef, %eax		/* something really easy to spot */
no_deadbeef:
	shrl	$2, %ecx			/* div 4 Longwords not bytes */
	cld
	rep
	stosl
no_fill_sbrk_memory:
	movl	%edx, __what_size_dpmi_thinks_we_are

#if 0						/* No purpose */
	pushl	___djgpp_memory_handle_list
	pushl	___djgpp_base_address
	movl	___sbrk_brk_hook, %eax
	call	%eax
	addl	$8, %esp
#endif

brk_nochange:					/* successful return */
	movl	__what_we_return_to_app_as_old_size, %eax
	jmp	brk_return

brk_error:					/* error return */
	movl	__what_we_return_to_app_as_old_size, %eax
	movl	%eax, __what_size_app_thinks_it_is
	movl	$-1, %eax

brk_return:
	popl	%ebx
	popl	%edi
	popl	%esi
	ret

	.globl	__crt0_init_mcount
__crt0_init_mcount:
#ifdef IN_GCRT0
	jmp	__mcount_init
#else
	ret
#endif
