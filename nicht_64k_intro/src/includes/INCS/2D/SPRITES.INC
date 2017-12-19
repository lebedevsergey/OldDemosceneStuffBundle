MAX_SPRITE_X equ 180
MAX_SPRITE_Y equ 180

;SPRITE_DRAW_FIX_COEF   equ   8
;SPRITE_DRAW_FIX_COEF__ equ   256.0

SPRITE_DRAW_FIX_COEF   equ   16
SPRITE_DRAW_FIX_COEF__ equ   65536.0

locals

.CODE
Alpha_One_Byte  macro Offset1
local @@Not_Carry3,@@Not_Draw3
; edx -  offset in texture
                mov   al,[esi+edx*4+Offset1]
                sub   al,Alpha_Sprite_Subber
                jc    @@Not_Draw3
                jz    @@Not_Draw3

                add al,[edi+Offset1]
                jnc @@Not_Carry3
                mov al,0FFh
@@Not_Carry3:
                mov [edi+Offset1],al
@@Not_Draw3:
                endm

Alpha_One_Byte_  macro Offset1
local @@Not_Carry3,@@Not_Draw3
; edx -  offset in texture
                mov   al,[esi+Offset1]
                sub   al,Alpha_Sprite_Subber
                jc    @@Not_Draw3
                jz    @@Not_Draw3

                add al,[edi+Offset1]
                jnc @@Not_Carry3
                mov al,0FFh
@@Not_Carry3:
                mov [edi+Offset1],al
@@Not_Draw3:
                endm

Alpha_One_Byte_Z_Buf  macro Offset1
local @@Not_Carry3,@@Not_Draw3
; edx -  offset in texture
                mov   al,[esi+edx*4+Offset1]
                sub   al,Alpha_Sprite_Subber
                jc    @@Not_Draw3
                jz    @@Not_Draw3

                add al,Virt_Screen_1[edi+Offset1]
                jnc @@Not_Carry3
                mov al,0FFh
@@Not_Carry3:
                mov Virt_Screen_1[edi+Offset1],al
@@Not_Draw3:
                endm


Draw_Sprite2   proc
                pushad
; esi: sprite
; edi: screen ptr
; ebx, ecx: sprite x,y

                mov  Sprite_Color,eax

mov Sprite_Clip_Adder,0

                lodsd
                mov SPRITE_X1,eax
                lodsd
                mov SPRITE_Y1,eax

                mov eax,ecx
                cmp eax,0
                jge  @@tt5
                neg eax
mov edx,eax
imul edx,SPRITE_X1
shl edx,2
add esi,edx
                mov ecx,0
                sub SPRITE_Y1 ,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt5:
                mov eax,ebx
                cmp eax,0
                jge  @@tt6
                neg eax
                mov ebx,0
mov edx,eax
shl edx,2
add esi,edx
add Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt6:

                mov eax,ecx
                add eax,SPRITE_Y1
                cmp eax,VERT_RAZR
                jle  @@tt1
                sub eax,VERT_RAZR
                sub SPRITE_Y1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt1:
                mov eax,ebx
                add eax,SPRITE_X1
                cmp eax,HOR_RAZR
                jle  @@tt2
                sub eax,HOR_RAZR
mov edx,eax
shl edx,2
add Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt2:

                imul ecx,HOR_RAZR
                add ebx,ecx
                lea edi,[ebx*4+edi]

                mov ebp,HOR_RAZR
                sub ebp,SPRITE_X1
                shl ebp,2

                mov edx,SPRITE_Y1
@@D_S_Loop1:
                mov ecx,SPRITE_X1
@@D_S_Loop2:
                lodsd
                or eax,eax
                jz @@Not_Draw2
                mov  eax,Sprite_Color
                mov [edi],eax
@@Not_Draw2:
                add  edi,4

                dec ecx
                jnz @@D_S_Loop2
add esi,Sprite_Clip_Adder
                add edi,ebp
                dec edx
                jnz @@D_S_Loop1

@@Not_Draw:
@@ex:
                popad
                retn
endp


Draw_Alpha_Sprite2   proc
; esi: sprite
; edi: screen ptr
; ebx, ecx: sprite x,y
                pushad

mov Sprite_Clip_Adder,0


                lodsd
                mov SPRITE_X1,eax
                lodsd
                mov SPRITE_Y1,eax

shr eax,1
sub ecx,eax                            ; Чтобы были координаты центра спрайта
mov eax,SPRITE_X1
shr eax,1
sub ebx,eax                            ; Чтобы были координаты центра спрайта

                mov eax,ebx
                cmp eax,0
                jge  @@tt6
                neg eax
                mov ebx,0
mov edx,eax
shl edx,2
add esi,edx
add Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt6:
                mov eax,ecx
                cmp eax,0
                jge  @@tt5
                neg eax
mov edx,eax
imul edx,SPRITE_X1
shl edx,2
add esi,edx
                mov ecx,0
                sub SPRITE_Y1 ,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt5:

                mov eax,ecx
                add eax,SPRITE_Y1
                cmp eax,VERT_RAZR
                jle  @@tt1
                sub eax,VERT_RAZR
                sub SPRITE_Y1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt1:
                mov eax,ebx
                add eax,SPRITE_X1
                cmp eax,HOR_RAZR
                jle  @@tt2
                sub eax,HOR_RAZR
mov edx,eax
shl edx,2
add Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt2:

                mov eax,ecx
                imul eax,HOR_RAZR
                add eax,ebx
                lea edi,[eax*4+edi]

;                shl SPRITE_X1,2

                mov eax,SPRITE_X1

                shl eax,2

                mov ebp,HOR_RAZR*4

                mov edx,SPRITE_Y1
                sub ebp,eax

@@D_A_S_Loop1:
                mov ecx,SPRITE_X1
@@D_A_S_Loop2:

                Alpha_One_Byte_ 0
                Alpha_One_Byte_ 1
                Alpha_One_Byte_ 2

                add esi,4
                add edi,4

                dec ecx
                jnz @@D_A_S_Loop2
add esi,Sprite_Clip_Adder
                add edi,ebp
                dec edx
                jnz @@D_A_S_Loop1

@@Not_Draw:

                popad
                retn
endp


Draw_Scaled_Alpha_Sprite2   proc
; esi: sprite
; edi: screen ptr
; ebx, ecx: sprite x,y
                pushad

;shr ebx,SPRITE_DRAW_FIX_COEF
;shr ecx,SPRITE_DRAW_FIX_COEF

                lodsd
                mov SPRITE_X1,eax
                lodsd
                mov SPRITE_Y1,eax

                fild  SPRITE_X1
                fmul  Sprite_Scale_X
                fistp SCALED_SPRITE_X1

                cmp SCALED_SPRITE_X1,0
                jz    @@Not_Draw

                fild  SPRITE_Y1
                fmul  Sprite_Scale_Y
                fistp SCALED_SPRITE_Y1
                cmp SCALED_SPRITE_Y1,0
                jz    @@Not_Draw


mov eax,SCALED_SPRITE_X1
shr eax,1
sub ebx,eax                            ; Чтобы были координаты центра спрайта
mov eax,SCALED_SPRITE_Y1
shr eax,1
sub ecx,eax                            ; Чтобы были координаты центра спрайта

                cmp    ebx,HOR_RAZR
                jge    @@Not_Draw
                cmp    ecx,VERT_RAZR
                jge    @@Not_Draw

                fld1
                fdiv   Sprite_Scale_X
                fmul   SPRITE_DRAW_FIX_COEF_
                fistp  Sprite_DU

                fld1
                fdiv   Sprite_Scale_Y
                fmul   SPRITE_DRAW_FIX_COEF_
                fistp  Sprite_DV

                mov    Sprite_Start_U,0
                mov    Sprite_Start_V,0

                cmp    ebx,0
                jge    @@tt6
                mov    eax,ebx
                neg    eax
                sub    SCALED_SPRITE_X1,eax
                js     @@Not_Draw
                jz     @@Not_Draw
                imul   eax,Sprite_DU
                add    Sprite_Start_U,eax
                mov    ebx,0
@@tt6:
                cmp    ecx,0
                jge    @@tt7
                mov    eax,ecx
                neg    eax
                sub    SCALED_SPRITE_Y1,eax
                js     @@Not_Draw
                jz     @@Not_Draw
                imul   eax,Sprite_DV
                add    Sprite_Start_V,eax
                mov    ecx,0
@@tt7:

                mov    eax,ebx
                add    eax,SCALED_SPRITE_X1
                cmp    eax,HOR_RAZR
                jle    @@tt8
                sub    eax,HOR_RAZR
                jz     @@Not_Draw
                sub    SCALED_SPRITE_X1,eax
@@tt8:
                mov    eax,ecx
                add    eax,SCALED_SPRITE_Y1
                cmp    eax,VERT_RAZR
                jle    @@tt9
                sub    eax,VERT_RAZR
                jz     @@Not_Draw
                sub    SCALED_SPRITE_Y1,eax
@@tt9:

                mov    eax,ecx
                imul   eax,HOR_RAZR
                add    eax,ebx
                lea    edi,[eax*4+edi]

                mov    eax,SCALED_SPRITE_X1
                shl    eax,2
                mov    Sprite_Adder,HOR_RAZR*4
                sub    Sprite_Adder,eax

                mov    ebp,Sprite_Start_V
                mov    edx,0
@@D_A_S_Loop1:
                push  edx

                mov    ebx,Sprite_Start_U
                mov    ecx,0
@@D_A_S_Loop2:
                ror   ebp,SPRITE_DRAW_FIX_COEF
                ror   ebx,SPRITE_DRAW_FIX_COEF

                movzx edx,bp
                imul  edx,SPRITE_X1
                movzx eax,bx
                add   edx,eax

                rol   ebp,SPRITE_DRAW_FIX_COEF
                rol   ebx,SPRITE_DRAW_FIX_COEF

                Alpha_One_Byte 0
                Alpha_One_Byte 1
                Alpha_One_Byte 2
;                Alpha_One_Byte 3

                add edi,4

                add ebx,Sprite_DU

                inc ecx
                cmp ecx,SCALED_SPRITE_X1
                jnz @@D_A_S_Loop2

                add ebp,Sprite_DV
                add edi,Sprite_Adder

                pop edx

                inc edx
                cmp edx,SCALED_SPRITE_Y1
                jnz @@D_A_S_Loop1
@@Not_Draw:

                popad
                retn
endp


.DATA
Sprite_Coord_X  dd  200
Sprite_Coord_Y  dd  100

Alpha_Sprite_Subber  db  100

Sprite_Scale_X dd  0.5
Sprite_Scale_Y dd  0.5

SPRITE_DRAW_FIX_COEF_  dd SPRITE_DRAW_FIX_COEF__

.DATA?
Sprite      dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)
Sprite2      dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)
Sprite3      dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)

Sprite_Clip_Adder  dd ?
Sprite_Adder       dd ?

SPRITE_X1 dd ?
SPRITE_Y1 dd ?
SCALED_SPRITE_X1   dd ?
SCALED_SPRITE_Y1   dd ?

Sprite_Color       dd  ?

Sprite_Start_X     dd  ?
Sprite_Start_Y     dd  ?

Sprite_Start_U     dd  ?
Sprite_Start_V     dd  ?
Sprite_DV          dd  ?
Sprite_DU          dd  ?

Sprite_X_Ex        dd  ?
Sprite_Y_Ex        dd  ?


.DATA?
Sprite_Scr_Adder  dd  ?


.CODE
locals

MIDDLE_BRIGHTNESS equ 1000; 2500;1500
MIN_BRIGHTNESS equ 0; 2500;1500


MIN_COLOR equ 0
MIDDLE_COLOR equ 130
MAX_COLOR equ 250 ;180

MAX_RADIUS equ 10 ;120


.CODE
Form1           proc    near
          ; F=sqrt(X*X+Y*Y+|XY|)

                mov Tmp1,bx
                fild Tmp1
 ;               fmul st(0),st
fabs


                mov Tmp1,bp
                fild Tmp1
;                fmul st(0),st
fabs


                faddp st(1),st

                fild Tmp1
                mov Tmp1,bx
                fimul Tmp1
fabs
                faddp st(1),st

                fsqrt

                retn
                endp

Form2           proc    near
          ; F=sqrt(X*X+Y*Y)

                mov Tmp1,bx
                fild Tmp1
                fmul st(0),st

                mov Tmp1,bp
                fild Tmp1
                fmul st(0),st

                faddp st(1)

                fsqrt

                retn
                endp



.CODE



Gen_Alpha_Sprite2        Proc   Near
; ebx - Sprite x size
; ebp - Sprite y size
; edi - sprite ptr

                pushad

                pushad
                mov ecx,ebx;SPRITE_X
                imul ecx,ebp;SPRITE_Y
                shl ecx,2
                xor eax,eax
                add edi,8
                rep stosd
                popad


                mov eax,ebx;SPRITE_X
                mov edx,eax
                shr edx,1
                stosd

                mov eax,ebp;SPRITE_Y
                mov esi,eax
                shr esi,1
                stosd

                 mov ebx,edx
                 neg ebx
D_M_P_LOOP_1:

                mov ebp,esi
                neg ebp
D_M_P_LOOP_2:
                finit

                or   ebp,ebp
                jnz  @@tt20
                or   ebx,ebx
                jnz   @@tt20
                mov  Tmp1,MAX_COLOR
                fild Tmp1
                jmp @@tt4
@@tt20:
                call [Alpha_Sprite_Formula]

                fist Tmp1
                cmp Tmp1,MAX_RADIUS
                jbe @@tt3

                fstp st
                mov  Tmp1,0
                fldz
                jmp @@tt4
@@tt3:

                fist Tmp1
                cmp Tmp1,MIDDLE_COLOR
                jae tt14

                mov Tmp2,MIDDLE_BRIGHTNESS

                jmp tt15
tt14:
                mov Tmp2,MIN_BRIGHTNESS

tt15:
                fld st(0)
                fild Tmp2

                fdiv st(0),st(1)
                fdiv st(0),st(1)

@@tt4:
                fld st
                fmul Gen_Sprite_B_Coef
                fistp Tmp1
                mov ax,Tmp1

                cmp ax,MAX_COLOR
                jle Put_B
                mov al,MAX_COLOR
Put_B:
                stosb

                fld st
                fmul Gen_Sprite_G_Coef
                fistp Tmp1
                mov ax,Tmp1

                 cmp ax,MAX_COLOR
                 jle Put_G
                 mov al,MAX_COLOR
Put_G:
                stosb


                fld st
                fmul Gen_Sprite_R_Coef
                fistp Tmp1
                mov ax,Tmp1

                 cmp ax,MAX_COLOR
                 jle Put_R
                 mov al,MAX_COLOR
Put_R:
                stosb

                stosb

                 inc ebp
;                 cmp ebp,SPRITE_Y/2   ; esi
                 cmp ebp,esi
                 jl D_M_P_LOOP_2

                 inc  ebx
;                 cmp ebx,SPRITE_X/2    ; edx
                 cmp ebx,edx
                 jl D_M_P_LOOP_1


                popad
                retn
endp


Gen_Alpha_Sprite3        Proc   Near
; ebx - Sprite x size
; ebp - Sprite y size
; edi - sprite ptr
; ax:cx:dx - RGB

                pushad

                mov  Tmp1,ax
                fild Tmp1
                fidiv Percent
                fstp  Gen_Sprite3_R_Coef

                mov  Tmp1,cx
                fild Tmp1
                fidiv Percent
                fstp  Gen_Sprite3_G_Coef

                mov  Tmp1,dx
                fild Tmp1
                fidiv Percent
                fstp  Gen_Sprite3_B_Coef

mov Tmp1,MAX_COLOR
fild Tmp1
fmul Gen_Sprite3_R_Coef
fistp MAX_COLOR_R
fild Tmp1
fmul Gen_Sprite3_G_Coef
fistp MAX_COLOR_G
fild Tmp1
fmul Gen_Sprite3_B_Coef
fistp MAX_COLOR_B

fldpi

mov Tmp2,ebx
fild Tmp2
fdiv st,st(1)
fstp Alpha_Sprite_X_Coef

mov Tmp2,ebp
fild Tmp2
fdiv st,st(1)
fstp Alpha_Sprite_Y_Coef

fstp st



                mov eax,ebx
                stosd

                mov eax,ebp
                stosd

                mov esi,ebx

@@D_M_P_LOOP_1:
                mov ebx,esi
@@D_M_P_LOOP_2:

finit

mov Tmp2,ebp
fild Tmp2
fdiv Alpha_Sprite_Y_Coef
fsin

mov Tmp2,ebx
fild Tmp2
fdiv Alpha_Sprite_X_Coef
fsin

fmulp st(1),st

fmul st,st
fmul st,st

fmul Alpha_Sprite_Bright_Coef

                fld st
                fmul Gen_Sprite3_B_Coef
                fistp Tmp1
                mov ax,Tmp1

                cmp ax,MAX_COLOR_B
                jle @@Put_B
                mov ax,MAX_COLOR_B
@@Put_B:
                stosb

                fld st
                fmul Gen_Sprite3_G_Coef
                fistp Tmp1
                mov ax,Tmp1

                 cmp ax,MAX_COLOR_G
                 jle @@Put_G
                 mov ax,MAX_COLOR_G
@@Put_G:
                stosb

                fmul Gen_Sprite3_R_Coef
                fistp Tmp1
                mov ax,Tmp1

                 cmp ax,MAX_COLOR_R
                 jle @@Put_R
                 mov ax,MAX_COLOR_R
@@Put_R:
                 stosb

                 stosb

                 dec ebx
                 jnz @@D_M_P_LOOP_2

                 dec  ebp
                 jnz @@D_M_P_LOOP_1


                popad
                retn
endp


.DATA
Gen_Sprite_R_Coef  dd 0.5
Gen_Sprite_B_Coef  dd 1.0
Gen_Sprite_G_Coef  dd 1.0
Alpha_Sprite_Formula   dd  Form1

Alpha_Sprite_Bright_Coef  dd  255.0

.DATA?
Alpha_Sprite_X_Coef  dd  ?
Alpha_Sprite_Y_Coef  dd  ?
MAX_COLOR_R          dw  ?
MAX_COLOR_G          dw  ?
MAX_COLOR_B          dw  ?

Gen_Sprite3_R_Coef  dd ?
Gen_Sprite3_B_Coef  dd ?
Gen_Sprite3_G_Coef  dd ?
