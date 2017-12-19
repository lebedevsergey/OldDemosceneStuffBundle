
FLARE_MAX_COLOR_     equ  255
Flare_Ray_Array_Num  equ  257
Flare_Ray_Array_Val_  equ  255 ;0FFFFh

Do_Rays  macro

push  esi

fild  Flare_Ray_Pow

fld  Flare_Angle
fld st
ftrunc
fist    Ray_I
fsubp   st(1),st
mov     esi,Ray_I
shl     esi,1
mov     ax,Flare_Ray_Array[esi+2]
sub     ax,Flare_Ray_Array[esi]
mov     Tmp1,ax
fild    Tmp1
fmulp   st(1),st
fiadd   word ptr Flare_Ray_Array[esi]
fidiv   Flare_Ray_Array_Val

fpow
fld Flare_Rad
fmul Flare_Rays_Coef1
fadd Flare_Rays_Coef2
fdivp st(1),st

faddp st(1),st

pop esi

endm

.CODE
Gen_Flare        Proc   Near
; ebx - Sprite x size
; ebp - Sprite y size
; edi - sprite ptr
; ax:cx:dx - RGB

                pushad

                pushad       ; Чистим спрайт
                mov  ecx,MAX_SPRITE_X*MAX_SPRITE_Y
                xor  eax,eax
                rep  stosd
                popad

                mov  Tmp1,ax
                fild Tmp1
                fidiv Percent
                fstp  Flare_R_Coef

                mov  Tmp1,cx
                fild Tmp1
                fidiv Percent
                fstp  Flare_G_Coef

                mov  Tmp1,dx
                fild Tmp1
                fidiv Percent
                fstp  Flare_B_Coef

fild FLARE_MAX_COLOR
fmul Gen_Sprite3_R_Coef
fistp MAX_COLOR_R
fild FLARE_MAX_COLOR
fmul Gen_Sprite3_G_Coef
fistp MAX_COLOR_G
fild FLARE_MAX_COLOR
fmul Gen_Sprite3_B_Coef
fistp MAX_COLOR_B

                mov eax,ebx
                stosd
shr eax,1
mov X_Subber,eax
                mov eax,ebp
                stosd
shr eax,1
mov Y_Subber,eax
                mov Tmp1,128
                fild Tmp1
                fldpi
                fdivp st(1),st
                fstp Flare_Radian_Coef

                                           ; Генерим лучи
                lea esi,Flare_Ray_Array
                mov ecx,Flare_Ray_Array_Num
@@tt1:
                mov  eax,Flare_Ray_Array_Val_
                call Random
                mov  [esi],ax
                inc  esi
                inc  esi
                loop  @@tt1

                mov esi,ebx
@@D_M_P_LOOP_1:
                mov ebx,esi
@@D_M_P_LOOP_2:

finit

mov Tmp2,ebp
fild Tmp2
fisub Y_Subber
fmul st,st
mov Tmp2,ebx
fild Tmp2
fisub X_Subber
fmul st,st

faddp st(1),st
fst Flare_Rad_Sqr
fsqrt
fstp Flare_Rad

mov Tmp2,ebp
fild Tmp2
fisub Y_Subber
mov Tmp2,ebx
fild Tmp2
fisub X_Subber
fpatan
fldpi
faddp st(1),st
fmul Flare_Radian_Coef
fstp  Flare_Angle

fldz
                                           ; Делаем свет в центре
cmp Center_Light_Flg,1
jnz @@No_Center_Light

fld Flare_Rad_Sqr
fmul Flare_Center_Light
fexp
faddp st(1),st

@@No_Center_Light:

                                           ; Делаем свечение
cmp Glow_Flg,1
jnz @@No_Glow

fld Flare_Rad
fmul Flare_Glow
fexp
faddp st(1),st

@@No_Glow:

                                           ; Делаем кольцо
cmp Ring_Flg,1
jnz @@No_Ring2

fld Flare_Rad
fsub Flare_Ring_Radius
fdiv Flare_Ring_Width
fabs

mov eax,0
call FPU_Compare
jb @@No_Ring
mov eax,1
call FPU_Compare
ja @@No_Ring

fstp Flare_RX
fld1
fld Flare_Ring_Coef3
fld Flare_RX
fmul Flare_Ring_Coef4
fsubp st(1),st
fmul Flare_RX
fmul Flare_RX
fsubp st(1),st
fdiv Flare_Ring_Bright

faddp st(1),st

@@No_Ring2:

                                           ; Делаем лучи
cmp Rays_Flg,1
jnz @@No_Rays

Do_Rays

@@No_Rays:

fimul FLARE_MAX_COLOR

                fld st
                fmul Flare_B_Coef
                fistp Tmp1
                mov ax,Tmp1

;                cmp ax,MAX_COLOR_B
cmp ax,FLARE_MAX_COLOR
                jle @@Put_B
mov ax,FLARE_MAX_COLOR
;               mov ax,MAX_COLOR_B
@@Put_B:
                stosb

                fld st
                fmul Flare_G_Coef
                fistp Tmp1
                mov ax,Tmp1

;                 cmp ax,MAX_COLOR_G
cmp ax,FLARE_MAX_COLOR
                 jle @@Put_G
mov ax,FLARE_MAX_COLOR
;                 mov ax,MAX_COLOR_G
@@Put_G:
                stosb

                fmul Flare_R_Coef
                fistp Tmp1
                mov ax,Tmp1

;                 cmp ax,MAX_COLOR_R
cmp ax,FLARE_MAX_COLOR
                 jle @@Put_R
;                 mov ax,MAX_COLOR_R
mov ax,FLARE_MAX_COLOR
@@Put_R:
                stosb

                stosb

                 dec ebx
                 jnz @@D_M_P_LOOP_2

                 dec  ebp
                 jnz @@D_M_P_LOOP_1


                popad
                retn

@@No_Ring:
                fstp st
                jmp @@No_Ring2

endp

.DATA
Flare_Center_Light  dd  -0.03
Flare_Glow          dd  -0.08
FLARE_MAX_COLOR     dw FLARE_MAX_COLOR_

Flare_Ring_Radius  dd 20.0 ;13.0
Flare_Ring_Width   dd 2.0 ;1.0
Flare_Ring_Coef3   dd 3.0
Flare_Ring_Coef4   dd 2.0
Flare_Ring_Bright  dd 5.0

Flare_Ray_Pow      dw 5
Flare_Rays_Coef1   dd 0.1
Flare_Rays_Coef2   dd 12.0

Flare_Ray_Array_Val dd Flare_Ray_Array_Val_

Rays_Flg           db 1
Ring_Flg           db 1
Center_Light_Flg   db 1
Glow_Flg           db 1

.DATA?
Flare_R_Coef  dd ?
Flare_B_Coef  dd ?
Flare_G_Coef  dd ?

Flare_Rad      dd   ?
Flare_Rad_Sqr  dd   ?
Flare_RX       dd   ?
Flare_Angle    dd   ?
X_Subber       dd   ?
Y_Subber       dd   ?

Ray_I          dd   ?
Flare_Ray_Array dw Flare_Ray_Array_Num dup (?)
Flare_Radian_Coef  dd  ?
