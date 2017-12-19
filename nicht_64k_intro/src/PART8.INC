.CODE

Cred_X1_2          equ  0
Cred_Y1_2          equ  3
Cred_X2_2          equ  158
Cred_Y2_2          equ  3
Cred_X3_2          equ  80
Cred_Y3_2          equ  158


Init_Part8 proc near


        mov  eax,Time2
;        mov  Fade_Base_Time,eax
        mov  Base_Time1,eax

        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        mov X_Pixel_Interval,9
        mov Y_Pixel_Interval,10

                lea  ebx,Font1
                mov [ebx].Font_X_Interval,-6
                mov  ecx,100
                mov  edx,-130
                mov  eax,10;30
                lea  esi,Str1
                lea  ebp,Text_Obj1
                call Gen_Text_Obj

                fld  Flare_Center_Light_Coef1
                fstp Flare_Center_Light
                fld  Flare_Glow_Coef1
                fstp Flare_Glow

                lea  edi,Alpha_Sprite1
                mov  ecx,MAX_SPRITE_X*MAX_SPRITE_Y
                xor  eax,eax
                rep  stosd

                mov ebx,35;50
                mov ebp,35;50
                lea edi,Alpha_Sprite1
                mov ax,50
                mov cx,100
                mov dx,60
                call Gen_Flare

                lea  esi,Font_Particles
                mov  ecx,MY_FONT_X_SIZE*MY_FONT_Y_SIZE
                call Init_Font_Particles

        retn
        endp

.CODE
Part8   proc near

                lea  esi,Font_Particles
                mov  ecx,MY_FONT_X_SIZE*MY_FONT_Y_SIZE
        call Update_Font_Particles

        fild  Tab_Tex_Add1_Coef
        call  Line_Param
        fistp Table_Tex_Adder

        mov ecx,0
        mov edx,VERT_RAZR
        lea ebp,Texture1_1
        lea esi,Lookup_Table
        mov edi,Virt_Screen1
        call Draw_Table_Tex

        fild  Tab_Tex_Add2_Coef
        call  Line_Param
        fistp Table_Tex_Adder

        mov ecx,HOR_RAZR
        mov edx,0
        lea ebp,Texture1_2
        lea esi,Lookup_Table
        mov edi,Virt_Screen3
        call Draw_Table_Tex

        mov alpha1,50;80
        mov alpha2,50;80
        mov esi,Virt_Screen3
        mov edi,Virt_Screen1
        call Alpha_Virt_Screen


        fld   Text_Angle_Z1_Coef3_8
        fild  Text_Angle_Z1_Coef_8
        call  Sin_Param
        fsub  Text_Angle_Z1_Coef2_8
        fchs

        fadd   Text_Angle_Z_8

;        fld   Text_Angle_Z_8
        lea   edi,Matr1
        call  do_rot_Z_mat

        lea   esi,Mov_Vec8
        lea   edi,Matr2
        call  do_mov_mat

        lea   ebx,Matr1
        lea   esi,Matr2
        lea   edi,Matr1
        call  matmat_mul

        lea   esi,Matr1
        lea   ebx,Text_Obj1
        call  transform_alpha_obj

        lea   ebx,Text_Obj1
        mov   edi,Virt_Screen1
        call  draw_alpha_obj

        fild  Text_Mov_X_Coef2_8
        fild  Text_Mov_X_Coef_8
        call  Sin_Param
        fstp  Mov_Vec8_2.VX

        fild  Text_Mov_Y_Coef2_8
        fild  Text_Mov_Y_Coef_8
        call  Sin_Param
        fstp  Mov_Vec8_2.VY

        lea   esi,Mov_Vec8_2
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

mov Light_Drawed_Pixels_Count,2
                 fild Light_Drawed_Pixels_Count
   ;              fdiv Light_Drawed_Pixels_Coef1
                 fstp Light_Drawed_Pixels_Coef2

mov edi,Virt_Screen1
lea  ebx,Light1_Pos.New_X
call Draw_Flares

             lea  ebx,AA_Font1
             mov eax,Credits_Color
             mov ecx,Cred_X1_2
             mov edx,Cred_Y1_2
             lea  esi,Cred_Str2
             mov  edi,Virt_Screen1
             call Draw_Proportional_String;2
             mov ecx,Cred_X2_2
             mov edx,Cred_Y2_2
             lea  esi,Cred_Str4
             call Draw_Proportional_String;2
             mov ecx,Cred_X3_2
             mov edx,Cred_Y3_2
             lea  esi,Cred_Str5
             call Draw_Proportional_String;2

retn
endp

.DATA
Mov_Vec8 VECTOR4 <0.0,0.0,25.0>
;Text_Z_8  dd  100.0

Text_Mov_X_Coef_8       dd  100000;700000
Text_Mov_X_Coef2_8      dd  360

Text_Mov_Y_Coef_8       dd  80000;700000
Text_Mov_Y_Coef2_8      dd  200

Text_Angle_Z_8          dd  0.75

Mov_Vec8_2 VECTOR4 <0.0,0.0,0.0>

Text_Angle_Z1_Coef2_8   dd  -0.2
Text_Angle_Z1_Coef3_8    dd  0.8
Text_Angle_Z1_Coef_8      dd  1000000;700000
