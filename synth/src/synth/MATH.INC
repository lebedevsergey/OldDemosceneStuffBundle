; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

; math functions

;IFNDEF _MATH
;_MATH	equ	1

.CODE
INIT_SEED_VAL	equ      0A1232134h
RAND_CONST	equ	 98212323h

__1		equ	3F800000h  ;1.0
_NAN		equ	808ch
NULL		equ	0FFFFFFFFh

Wrap_Max macro value,max
local tt1
cmp value,max
jng tt1
sub value,max
tt1:
endm

Wrap_Min macro value,min,max
local tt1
cmp value,min
jnl tt1
add value,max
tt1:
endm

Fix_Max macro value,max
local tt1
cmp value,max
jng tt1
mov value,max
tt1:
endm

Fix_Min macro value,min
local tt1
cmp value,min
jnl tt1
mov value,min
tt1:
endm

Arccos		proc near
;		mov Tmp1,1

		ftst
                fstsw ax
		sahf
		jz @@Zero_Arg				
;		jnc @@Pos_Arg
;		mov Tmp1,0
;@@Pos_Arg:

		fld1
		fld st(1)
		fmul st,st
		fsubp st(1),st
		fabs
		fsqrt	
		fxch st(1)
		fpatan

;		fldpi
;		fimul Tmp1
;		faddp st(1),st		

		retn
@@Zero_Arg:
		fstp st
		fldpi
		mov Tmp1,2
		fidiv Tmp1
		retn
endp


FPU_Exp  proc near
        fld1 
        fldl2e 
        fmul st,st(2) 
        fst st(2) 
        fprem 
        f2xm1 
        faddp st(1),st 
        fscale 
        fstp st(1)
	retn
endp	 

Fexp   macro
	call FPU_Exp
endm


FPU_Compare proc near
       mov @@Comp_Var,eax	
       fld st
       ficomp @@Comp_Var
       fstsw ax
       sahf

      retn
.DATA?
@@Comp_Var  dd  ?
.CODE
endp


FPU_Pow proc near
fabs
        fyl2x
        fld1
        fld st(1)
        fprem
        f2xm1
        faddp st(1),st
        fscale
	fxch st(1)	
        fstp st
	retn
endp

FPow  macro
   call FPU_Pow  	
endm


FPU_Trunc      proc near
     fsub @@half
     retn
.DATA
@@half dd 0.5
.CODE
endp

FTrunc  macro
   call FPU_Trunc
endm


.CODE
random proc
; IN  : AX - Random range
; OUT : AX - Random
        push   ebx
        push   edx

        mov    ebx,eax
        mov    eax,seed
        imul   eax,RAND_CONST
	inc    eax
        mov    dword ptr seed,eax
        mul    ebx
        xchg   eax,edx

        pop  edx
        pop  ebx
        ret
random endp

srandom proc near
        mov    dword ptr seed,eax
	retn
endp

Fsign macro
call Fsign_
endm

Fsign_   proc
        ftst  
        fstsw ax
        sahf
	jz    @@ex
	fstp  st
	fld1
        ja    @@ex
        fchs
@@ex:
retn
endp

.DATA
seed		dd	INIT_SEED_VAL

_2PI		dd      6.28318530718
PI		dd	3.141592653589
Inv_PI		dd	0.31830988618379067
_1		dd	__1
_0_5		dd	0.5
Percent		dw	100

.DATA?
Tmp1		dw 	?
Tmp2		dd 	?