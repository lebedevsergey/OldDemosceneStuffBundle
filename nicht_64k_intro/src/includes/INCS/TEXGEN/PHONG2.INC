PHONG2_SIZE_X   equ  100
PHONG2_SIZE_Y   equ  100
Phong2_MAX_COLOR  equ  255

MIN_PHONG2  equ  0

.CODE
Do_Phong2           Proc   Near
                pushad

                xor eax,eax
                xor ebx,ebx
                lodsb
                mov bl,al
                lodsb
                imul eax,TEX_SIZE
                add eax,ebx
                shl eax,2
                add edi,eax        ; Получили координаты

                lodsd
                mov Phong2_Coef,eax

                lodsd
                mov Phong2_R_Coef,eax
                lodsd
                mov Phong2_G_Coef,eax
                lodsd
                mov Phong2_B_Coef,eax

                mov edx,(TEX_SIZE-PHONG2_SIZE_X)*4

                mov  bp,-PHONG2_SIZE_Y/2
@@Loop_Y2:

                mov  cx,-PHONG2_SIZE_X/2
@@Loop_X2:
                mov  Tmp1,255
                fild Tmp1

                mov  Tmp1,cx
                fild Tmp1
fmul st,st
                mov  Tmp1,bp
                fild Tmp1
fmul st,st
                faddp st(1),st
                fsqrt
                fsqrt
fexp
                fmul  Phong2_Coef

                fsubp st(1),st
                fistp Tmp1

cmp Tmp1,MIN_PHONG2
jge @@tt2
mov Tmp1,MIN_PHONG2
@@tt2:

fild Tmp1

call Phong2_Put_RGB

                inc cx
                cmp  cx,PHONG2_SIZE_X/2
                jnz @@Loop_X2

                add edi,edx

                inc bp
                cmp bp,PHONG2_SIZE_Y/2
                jnz @@Loop_Y2

                popad
add esi,18
                retn
endp

Phong2_Put_RGB proc

                fld st
                fmul Phong2_R_Coef
                fistp Tmp1
                mov ax,Tmp1

                cmp ax,Phong2_MAX_COLOR
                jbe @@Put_R
                mov al,Phong2_MAX_COLOR

@@Put_R:
                add al,[edi]
                jnc @@Not_Carry2
                xor eax,eax
                dec al
@@Not_Carry2:
                stosb


                fld st
                fmul Phong2_G_Coef
                fistp Tmp1
                mov ax,Tmp1

                 cmp ax,Phong2_MAX_COLOR
                 jbe @@Put_G
                 mov al,Phong2_MAX_COLOR
@@Put_G:
                add al,[edi]
                jnc @@Not_Carry3
                xor eax,eax
                dec al
@@Not_Carry3:
                stosb

;                fld st
                fmul Phong2_B_Coef
                fistp Tmp1
                mov ax,Tmp1

                 cmp ax,Phong2_MAX_COLOR
                 jbe @@Put_B
                 mov al,Phong2_MAX_COLOR
@@Put_B:

                add al,[edi]
                jnc @@Not_Carry4
                xor eax,eax
                dec al
@@Not_Carry4:
                stosb

                stosb
                retn
                endp

.DATA?
Phong2_Coef    dd  ?
Phong2_R_Coef  dd  ?
Phong2_G_Coef  dd  ?
Phong2_B_Coef  dd  ?