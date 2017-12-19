locals
.CODE
PARTICLES2_NUM    equ  3000
MAX_PART_X       equ  17
MAX_PART_Y       equ  -30
MAX_PART_Z       equ  17
PART_MAX_Y_VELOCITY  equ  7

PARTICLE2_REC   STRUC
  Part_Bright_Subber           dd    ?
  Part_Create_Time             dd    ?
  Part_Y_Velocity              dd    ?
  Part_Init_X                  dd    ?
  Part_Init_Y                  dd    ?
  Part_Init_Z                  dd    ?
ENDS

Init_One_Particle2  proc near
; esi - ptr to particles
; ebp - ptr to vertex
                pushad

                fld  Time2
                fstp [esi].Part_Create_Time
                mov  eax,PART_MAX_Y_VELOCITY
                call Random
                mov  Tmp2,eax
                fild Tmp2
                fmul Part_Y_Velocity_Coef1
                fstp [esi].Part_Y_Velocity
                mov  eax,MAX_PART_Z*2
                call Random
                sub  eax,MAX_PART_Z
                mov  Tmp2,eax
                fild Tmp2
                fst  [ebp].VZ
                fstp [esi].Part_Init_Z
                mov  eax,MAX_PART_Y
;                call Random
                mov  Tmp2,eax
                fild Tmp2

                fst  [ebp].VY
                fstp [esi].Part_Init_Y
                mov  eax,MAX_PART_X*2
                call Random
                sub  eax,MAX_PART_X
                mov  Tmp2,eax
                fild Tmp2
                fst  [ebp].VX
                fstp [esi].Part_Init_X
                popad
                retn
Init_One_Particle2  endp

Update_Particles2  proc near
; esi - ptr to particles
; ebx - ptr to particle object
                pushad

                lea  esi,Particles2

                fld  Time2

                mov  ebp,[ebx].FIRST_VERTEX
                mov  edi,[ebx].LAST_VERTEX
@@Part_Cyc:
                fld  st
                fsub [esi].Part_Create_Time
                fld  st
                fcomp Particle_Die_Time_
                fstsw ax
                sahf
                jc   @@Not_Die
                call Init_One_Particle2
                fstp st
                jmp  @@Next
@@Not_Die:
                fld   Particle_Die_Time_
                fsub  st,st(1)
                fmul  Part_Bright_Coef6
                fstp  [esi].Part_Bright_Subber

                fmul [esi].Part_Y_Velocity
                fadd [esi].Part_Init_Y
                fstp [ebp].VY

;               fld  [esi].Part_Init_Z
;               fstp [ebp].VZ
;               fld  [esi].Part_Init_X
;               fstp [ebp].VX
@@Next:
                add  esi,SIZE PARTICLE2_REC
                add  ebp,SIZE TVERTEX
                cmp  ebp,edi
                jnz  @@Part_Cyc

                fstp  st
                popad
                retn
Update_Particles2  endp

Init_Particles2  proc near
                pushad
                lea  esi,Particles2

                mov  ebp,[ebx].FIRST_VERTEX
                mov  edi,[ebx].LAST_VERTEX
@@tt1:
                call Init_One_Particle2

                fld  Particle_Die_Time_
                fistp Tmp2
                mov  eax,Tmp2
                call Random
                mov  Tmp2,eax
                fld  Time2
                fisub Tmp2
                fstp [esi].Part_Create_Time

                add  esi,SIZE PARTICLE2_REC
                add  ebp,SIZE TVERTEX
                cmp  ebp,edi
                jnz  @@tt1
                popad
                retn
Init_Particles2 endp

.DATA
Particle_Die_Time_       dd   309507.0
Part_Bright_Coef6        dd   0.000001
Part_Y_Velocity_Coef1    dd   0.00005

.DATA?
Particles2               db  PARTICLES2_NUM*SIZE PARTICLE2_REC dup(?)

.CODE
Clear_Particles  proc near
; esi - ptr to particles
; ebx - ptr to particle object
                pushad

                lea  esi,Particles2

mov ecx,PARTICLES2_NUM
                mov  ebp,[ebx].FIRST_VERTEX
                mov  edi,[ebx].LAST_VERTEX
@@Part_Cyc:
                fld1
                fstp  [esi].Part_Bright_Subber

;                add  esi,SIZE PARTICLE2_REC
 ;               add  ebp,SIZE TVERTEX
  ;              cmp  ebp,edi
  dec ecx
                jns  @@Part_Cyc

                popad
                retn
Clear_Particles  endp
