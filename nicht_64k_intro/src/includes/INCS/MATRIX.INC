; !!!!!  Optimized version, possible there are some bugs

_1   equ  3F800000h

.486p
;.model flat
locals

.code

include f:\dem\common\incs\matrix.def

do_rot_Z_mat proc

  push  edi	
  call  Create_Matr
  pop   edi

  fsincos
  fst  dword ptr [edi+00h]
  fstp dword ptr [edi+14h]
  fst dword ptr [edi+10h]
  fchs	
  fstp dword ptr [edi+4h]

  ret
endp

do_rot_X_mat proc

  push edi	
  call Create_Matr
  pop  edi

  fsincos
  fst  dword ptr [edi+14h]
  fstp dword ptr [edi+28h]
  fst dword ptr [edi+24h]
  fchs	
  fstp dword ptr [edi+18h]

  ret
endp

do_rot_Y_mat proc

  push edi	
  call Create_Matr
  pop  edi

  fsincos
  fst  dword ptr [edi+00h]
  fstp dword ptr [edi+28h]
  fst  dword ptr [edi+20h]
  fchs	
  fstp dword ptr [edi+08h]

  ret
endp

do_mov_mat proc

  push edi	
  call Create_Matr
  pop  edi

  fld  dword ptr [esi+00h]
  fld  dword ptr [esi+04h]
  fld  dword ptr [esi+08h]
  fstp dword ptr [edi+2Ch]
  fstp dword ptr [edi+1Ch]
  fstp dword ptr [edi+0Ch]

  ret
endp


Create_matr proc

       mov  eax,_1
       mov  edx,3
@@lp1:
       mov  eax,_1
       stosd
       mov  ecx,MATRIX_SIZE
       mov  eax,0
       rep  stosd
       dec  edx
       jnz  @@lp1

       mov  dword ptr[edi],_1

       ret
endp


vecmat_mul proc
       push eax
       push ecx
       push esi


       lea eax,m

       mov ecx,4
@@lp11:
      fld  dword ptr [ebx+00h]  ;0-1
      fmul dword ptr [esi+00h]  ;1-3

      fld  dword ptr [ebx+04h]  ;2-3
      fmul dword ptr [esi+04h]  ;3-5

      fld  dword ptr [ebx+08h]  ;4-5
      fmul dword ptr [esi+08h]  ;5-8

      fld dword ptr  [ebx+0Ch]  ;6-9
      fmul dword ptr [esi+0Ch]  ;7

      fxch  st(1)               ;7

      faddp st(2),st            ;8
      faddp st(2),st            ;10
      add   esi,10h

      faddp st(1),st

      fstp dword ptr [eax]


      add eax,4

   dec ecx
   jnz @@lp11

  mov  ecx,4
  lea  esi,m
  rep  movsd

  
  pop esi
  pop ecx
  pop eax
  ret
vecmat_mul endp

matmat_mul proc
      pushad


      lea eax,m

      mov edx,4
@@lp1:
       mov ecx,4
    @@lp2:

      fld dword ptr  [esi+00h]
      fmul dword ptr [ebx+00h]
      fld dword ptr  [esi+04h]
      fmul dword ptr [ebx+10h]
      fld dword ptr  [esi+08h]
      fmul dword ptr [ebx+20h]
      fld dword ptr  [esi+0Ch]
      fmul dword ptr [ebx+30h]
      fxch st(1)
      faddp st(2),st
      faddp st(2),st
      add ebx,4
      faddp st(1),st

      fstp dword ptr [eax]

      add eax,4

    dec ecx
  jnz @@lp2

  sub ebx,16
  add esi,16

  dec edx
jnz @@lp1

  mov  ecx,16
  lea  esi,m
  rep  movsd

  popad
  ret
endp


vecvec_mul proc

      fld dword ptr [ebx+00h]
      fmul dword ptr [esi+00h]
      fld dword ptr [ebx+04h]
      fmul dword ptr [esi+04h]
      fld dword ptr [ebx+08h]
      fmul dword ptr [esi+08h]
      fxch st(1)
      faddp st(2),st
      faddp st(1),st

;      fld dword ptr [ebx+0Ch]
 ;     fmul dword ptr [esi+0Ch]
  ;    faddp st(1),st  

  ret
endp


vecvec_add proc
      fld dword ptr [ebx+00h]
      fadd dword ptr [esi+00h]

      fld dword ptr [ebx+04h]
      fadd dword ptr [esi+04h]

      fld dword ptr [ebx+08h]
      fadd dword ptr [esi+08h]

;      fld dword ptr [ebx+0Ch]
 ;     fadd dword ptr [esi+0Ch]
  ;    fstp dword ptr [edi+0Ch]

      fstp dword ptr [edi+08h]
      fstp dword ptr [edi+04h]
      fstp dword ptr [edi+00h]	
  ret
endp

vecvec_sub proc

      fld dword ptr [ebx+00h]
      fsub dword ptr [esi+00h]

      fld dword ptr [ebx+04h]
      fsub dword ptr [esi+04h]

      fld dword ptr [ebx+08h]
      fsub dword ptr [esi+08h]

      fld dword ptr [ebx+0Ch]
      fsub dword ptr [esi+0Ch]

      fstp dword ptr [edi+0Ch]
      fstp dword ptr [edi+08h]
      fstp dword ptr [edi+04h]
      fstp dword ptr [edi+00h]	

  ret
endp

vecvec_mul2 proc
;optimized cross product; 22 cycles 
; from Abrash' book
        fld    dword ptr     [ebx+4]        ;starts & ends on cycle 0
        fmul   dword ptr     [esi+8]        ;starts on cycle 1
        fld    dword ptr     [ebx+8]        ;starts & ends on cycle 2
        fmul   dword ptr     [esi+0]        ;starts on cycle 3
        fld    dword ptr     [ebx+0]        ;starts & ends on cycle 4
        fmul   dword ptr     [esi+4]        ;starts on cycle 5
        fld    dword ptr    [ebx+8]         ;starts & ends on cycle 6
        fmul   dword ptr    [esi+4]         ;starts on cycle 7
        fld    dword ptr     [ebx+0]        ;starts & ends on cycle 8
        fmul   dword ptr     [esi+8]        ;starts on cycle 9
        fld    dword ptr     [ebx+4]        ;starts & ends on cycle 10
        fmul   dword ptr     [esi+0]        ;starts on cycle 11
        fxch      st(2)           ;no cost
        fsubrp    st(5),st(0)     ;starts on cycle 12
        fsubrp    st(3),st(0)     ;starts on cycle 13
        fsubrp    st(1),st(0)     ;starts on cycle 14
        fxch      st(2)           ;no cost
                                  ;stalls for cycle 15
        fstp   dword ptr     [edi+0]        ;starts on cycle 16,
                                      ; ends on cycle 17
        fstp   dword ptr     [edi+4]        ;starts on cycle 18,
                                  ; ends on cycle 19
        fstp   dword ptr     [edi+8]        ;starts on cycle 20,
                                  ; ends on cycle 21
  ret
endp

vec_module macro

      fld dword ptr [esi+00h]
      fmul st,st

      fld dword ptr [esi+04h]
      fmul st,st

      fld dword ptr [esi+08h]
      fmul st,st
      fxch st(1)
	
      faddp st(2),st
      faddp st(1),st
	
      fsqrt
endm

vec_norm proc

      fld  __1
      vec_module   	 	
      fdivp st(1),st

      fld dword ptr [esi+00h]
      fmul st,st(1)
      fld dword ptr [esi+04h]
      fmul st,st(2)
      fld dword ptr [esi+08h]
      fmul st,st(3)
fxch st(2)
      fstp dword ptr [esi+00h]
      fstp dword ptr [esi+04h]
      fstp dword ptr [esi+08h]
fstp st

      ret
endp


vec_const_mul proc

      fld  dword ptr [esi+00h]
      fmul st,st(1)
      fld  dword ptr [esi+04h]
      fmul st,st(2)
      fld dword ptr  [esi+08h]
      fmul st,st(3)

      fstp dword ptr [edi+08h]
      fstp dword ptr [edi+04h]
      fstp dword ptr [edi+00h]

;      fld dword ptr [esi+0Ch]
 ;     fmul st,st(1)
  ;    fstp dword ptr [edi+0Ch]

       ret
endp


vec_const_div proc

      fld  __1
      fdiv st,st(1)	

      fld dword ptr [esi+00h]
      fmul st,st(1)
      fstp dword ptr [edi+00h]

      fld dword ptr [esi+04h]
      fmul st,st(1)
      fstp dword ptr [edi+04h]

      fld dword ptr [esi+08h]
      fmul st,st(1)
      fstp dword ptr [edi+08h]

;      fld dword ptr [esi+0Ch]
 ;     fdiv st,st(1)
  ;    fstp dword ptr [edi+0Ch]

    ret
endp


vec_neg proc

      fld dword ptr [esi+00h]
      fchs
      fstp dword ptr [edi+00h]

      fld dword ptr [esi+04h]
      fchs
      fstp dword ptr [edi+04h]

      fld dword ptr [esi+08h]
      fchs
      fstp dword ptr [edi+08h]
    ret
endp

vec_sqr proc

      fld  dword ptr [esi+00h]
      fmul st,st
      fld  dword ptr [esi+04h]
      fmul st,st
      fld  dword ptr [esi+08h]
      fmul st,st

      fxch  st(1)
      faddp st(2),st
      faddp st(1),st

	retn	
endp

.DATA
__1   dd    1.0

.DATA?
m  dd 16 dup (?)