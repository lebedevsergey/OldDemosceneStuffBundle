.CODE

Init_Part0 proc near

                fld  Flare_Center_Light_Coef0
                fstp Flare_Center_Light
                fld  Flare_Glow_Coef0
                fstp Flare_Glow

lea edi,Light1_Sprite
mov ebx,130
mov ebp,130
mov ax,70
mov cx,40
mov dx,25
call Gen_Flare

                fld  Flare_Center_Light_Coef0_2
                fstp Flare_Center_Light
                fld  Flare_Glow_Coef0_2
                fstp Flare_Glow

lea edi,Sprite2
mov ebx,100
mov ebp,100
mov ax,35
mov cx,40
mov dx,35
;mov Center_Light_Flg,0
call Gen_Flare

lea edi,Sprite
mov ebx,120
mov ebp,120
mov ax,70
mov cx,50
mov dx,45
call Gen_Flare


        mov  eax,Time2
 ;       mov  Fade_Base_Time,eax
        mov  Base_Time1,eax

        retn
        endp

.CODE
Part0   proc near

        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        fild  Head_Mov_Coef1
        call  Line_Param
        fistp  Tmp2

                mov  edi,Virt_Screen1
                mov eax,Z_Buff_Value
                mov ebx,Tmp2
                mov ecx,0+SPRITE1_Y_POS
                lea esi,Head_Sprite
                call Draw_Sprite2

        fild  Text_Mov_Z0_Coef2
        fild  Text_Mov_Z0_Coef
        call  Sin_Param
        fstp  Mov_Vec0.VX

        lea   esi,Mov_Vec0
        lea   edi,Matr1
        call  do_mov_mat

lea  ebx,Light1_Pos.X
lea  esi,Matr1
lea  edi,Light1_Pos.New_X
call vecmat_mul

fld   Light1_Pos.New_X
fadd  X_SIZE_2
fistp Tmp2
mov   ebx,Tmp2
fld   Light1_Pos.New_Y
fadd  Y_SIZE_2
fistp Tmp2
mov   ecx,Tmp2

                 mov  edi,Virt_Screen1
                 lea esi,Light1_Sprite

                 call Draw_Alpha_Sprite_With_Z_Buf
                 fild Light_Drawed_Pixels_Count
                 fdiv Light_Drawed_Pixels_Coef1
                 fstp Light_Drawed_Pixels_Coef2

mov edi,Virt_Screen1
lea  ebx,Light1_Pos.New_X
call Draw_Flares


retn
endp

;Tab_Tex_Add1_Coef     dd  2600
;Tab_Tex_Add2_Coef     dd  2600
;Text_Angle_Z1_Coef2   dd  -0.4
;Text_Angle_Z1_Coef3    dd  0.8

;Str0                  db  'THROB',0


.DATA?
Text_Angle_Z0         dd  ?


.DATA
Mov_Vec0 VECTOR4 <0.0,0.0,0.0>
Light1_Pos    TVERTEX <0.0,0.0,320.0,1.0>

Light_Drawed_Pixels_Coef1  dd  3900.0

Flare_Center_Light_Coef0  dd  -0.001
Flare_Glow_Coef0          dd  -0.009;7

Flare_Center_Light_Coef0_2  dd  -0.03
Flare_Glow_Coef0_2          dd  -0.01


Text_Mov_Z0_Coef      dd   100000;700000
Text_Mov_Z0_Coef2     dd  -HOR_RAZR/2+20
Head_Mov_Coef1        dd   -2000;-6000


.DATA?
Light1_Sprite      dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)