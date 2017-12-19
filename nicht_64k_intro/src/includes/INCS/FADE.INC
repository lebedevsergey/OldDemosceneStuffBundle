FADE_NO         equ    0
FADE_IN         equ    1
FADE_OUT        equ    2
FADE_WHITE_IN   equ    3
FADE_WHITE_OUT  equ    4

.CODE
locals

Do_Fade         proc

lea ebx,Max_Fade_Val
lea edx,Min_Fade_Val

mov  edi,Virt_Screen1

                 cmp  Fade_Flg,FADE_IN
                 jnz  @@Fade_Out

                 fld   Time2
                 fsub   Fade_Base_Time

                 fdiv Fade_Coef
                 fmul  Radian_Coef
                 fsin
                 fabs
                 fild  word ptr [ebx]
                 fisub word ptr [edx]
                 fmulp st(1),st
                 fiadd word ptr [edx]
                 fistp  Tmp1
                 mov   ax,Tmp1

                 mov ah,250
                 sub ah,al

                 mov  alpha1,ah
                 mov  ecx,HOR_RAZR*VERT_RAZR
                 call Fade_Virt_Screen
                 retn

@@Fade_Out:
                 cmp  Fade_Flg,FADE_OUT
                 jnz  @@No_Fade

                 fld   Time2
                 fsub   Fade_Base_Time

                 fdiv  Fade_Coef
                 fmul  Radian_Coef
                 fsin
                 fabs
                 fild  word ptr [ebx]
                 fisub word ptr [edx]
                 fmulp st(1),st
                 fiadd word ptr [edx]
                 fistp  Tmp1
                 mov   ax,Tmp1

;                 mov ah,250
 ;                sub ah,al

                 mov  alpha1,al
                 mov  ecx,HOR_RAZR*VERT_RAZR
                 call Fade_Virt_Screen
@@No_Fade:
                 retn
Do_Fade          endp


.DATA
Fade_Coef    dd  2000.0
Min_Fade_Val dw  0
Max_Fade_Val dw  250

Fade_Flg       dd  0
Fade_White_Flg dd  0


.DATA?
Fade_Base_Time  dd  ?

.CODE

Do_Fade_White         proc

lea ebx,Max_Fade_Val
lea edx,Min_Fade_Val

mov  edi,Virt_Screen1

                 cmp  Fade_White_Flg,FADE_WHITE_IN
                 jnz  @@Fade_White_Out

                 fld   Time2
                 fsub   Fade_Base_Time

                 fdiv Fade_Coef
                 fmul  Radian_Coef
                 fsin
                 fabs
                 fild  word ptr [ebx]
                 fisub word ptr [edx]
                 fmulp st(1),st
                 fiadd word ptr [edx]
                 fistp  Tmp1
                 mov   ax,Tmp1

;                 mov ah,250
 ;                sub ah,al

                 mov  alpha1,al
                 mov  ecx,HOR_RAZR*VERT_RAZR
                 call Fade_White_Virt_Screen
                 retn

@@Fade_White_Out:
                 cmp  Fade_White_Flg,FADE_WHITE_OUT
                 jnz  @@No_Fade

                 fld   Time2
                 fsub   Fade_Base_Time

                 fdiv  Fade_Coef
                 fmul  Radian_Coef
                 fsin
                 fabs
                 fild  word ptr [ebx]
                 fisub word ptr [edx]
                 fmulp st(1),st
                 fiadd word ptr [edx]
                 fistp  Tmp1
                 mov   ax,Tmp1

                 mov ah,250
                 sub ah,al

                 mov  alpha1,ah
                 mov  ecx,HOR_RAZR*VERT_RAZR
                 call Fade_White_Virt_Screen
@@No_Fade:
                 retn
Do_Fade_White    endp
