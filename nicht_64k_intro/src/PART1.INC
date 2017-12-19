Credits_Color    equ  0DFh*01010101h
Cred_X1          equ  117
Cred_Y1          equ  0
Cred_X2          equ  5
Cred_Y2          equ  162

Cred_X3          equ  139;185
Cred_Y3          equ  0
Cred_X4          equ  5
Cred_Y4          equ  162

.CODE

Init_Part1 proc near

        mov  eax,Time2
;        mov  Fade_Base_Time,eax
        mov  Base_Time1,eax

        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        lea esi,Thing1
        lea edi,Text_Obj1
        call Do_Object

                lea  ebx,Font1
                mov [ebx].Font_X_Interval,-6
                mov  ecx,100
                mov  edx,-130
                mov  eax,50;30
                lea  esi,Str1
                lea  ebp,Text_Obj1
                call Gen_Text_Obj

                fld  Flare_Center_Light_Coef1
                fstp Flare_Center_Light
                fld  Flare_Glow_Coef1
                fstp Flare_Glow

                mov ebx,45;50
                mov ebp,45;50
                lea edi,Alpha_Sprite1
                mov ax,100
                mov cx,60
                mov dx,50
                call Gen_Flare

                lea  esi,Font_Particles
                mov  ecx,MY_FONT_X_SIZE*MY_FONT_Y_SIZE
                call Init_Font_Particles

           mov Font1.Font_X_Interval,2

        retn
        endp

.CODE
Part1   proc near


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

;        fild  Text_Angle_Z1_Coef
 ;       call  Line_Param
  ;      fadd  Text_Angle_Z1_Coef2

        fld   Text_Angle_Z1_Coef3
        fild  Text_Mov_Z1_Coef
        call  Sin_Param
        fadd  Text_Angle_Z1_Coef2

        fstp  Text_Angle_Z1

        fild  Text_Mov_Z1_Coef2
        fild  Text_Mov_Z1_Coef
        call  Sin_Param
        fstp  Mov_Vec1.VZ

        fld   Text_Angle_Z1
        lea   edi,Matr1
        call  do_rot_Z_mat
        lea   esi,Mov_Vec1
        lea   edi,Matr2
        call  do_mov_mat

        lea   ebx,Matr2
        lea   esi,Matr1
        lea   edi,Matr1
        call  matmat_mul

        lea   esi,Matr1
        lea   ebx,Text_Obj1
        call  transform_alpha_obj

        lea   ebx,Text_Obj1
        mov   edi,Virt_Screen1
        call  draw_alpha_obj

cmp Cred1_Flg,1
jnz @@tt1
             lea  ebx,AA_Font1
             mov eax,Credits_Color
             mov ecx,Cred_X1
             mov edx,Cred_Y1
             lea  esi,Cred_Str1
             mov  edi,Virt_Screen1
             call Draw_Proportional_String;2
@@tt1:

cmp Cred2_Flg,1
jnz @@tt2

             lea  ebx,AA_Font1
             mov eax,Credits_Color
             mov ecx,Cred_X2
             mov edx,Cred_Y2
             lea  esi,Cred_Str2
             mov  edi,Virt_Screen1
             call Draw_Proportional_String;2
@@tt2:

cmp Cred3_Flg,1
jnz @@tt3
             lea  ebx,AA_Font1
             mov eax,Credits_Color
             mov ecx,Cred_X3
             mov edx,Cred_Y3
             lea  esi,Cred_Str3
             mov  edi,Virt_Screen1
             call Draw_Proportional_String;2
@@tt3:

cmp Cred4_Flg,1
jnz @@tt4

             lea  ebx,AA_Font1
             mov eax,Credits_Color
             mov ecx,Cred_X4
             mov edx,Cred_Y4
             lea  esi,Cred_Str4
             mov  edi,Virt_Screen1
             call Draw_Proportional_String;2
@@tt4:
;        call Do_Fade

        lea  esi,Times_Tab1
        call Do_Time_Tab

retn
endp

Tab_Tex_Add1_Coef     dd  2600
Tab_Tex_Add2_Coef     dd  2600
Text_Angle_Z1_Coef2   dd  -0.4
Text_Angle_Z1_Coef3    dd  0.8

;Str1                  db  'ACB',0
Str1                  db   'ACB',0
Cred_Str1             db  'VISUAL BASIC',0
Cred_Str2             db  'SUNNFIRE',0
Cred_Str3             db  'PROTRACKER',0
Cred_Str4             db  'MUNNWER',0
Cred_Str5             db  'THROUJOBB',0

Text_Mov_Z1_Coef      dd  500000;700000
Text_Mov_Z1_Coef2      dd  -40


.DATA?
Text_Angle_Z1         dd  ?

Base_Time1           dd  ?
Base_Time2           dd  ?
Base_Time3           dd  ?

Text_Obj1  TMODEL <?>

Matr1  MATRIX4x4 <?>
Matr2  MATRIX4x4 <?>

.DATA
Mov_Vec1 VECTOR4 <0.0,0.0,100.0>
Flare_Center_Light_Coef1  dd  -0.01
Flare_Glow_Coef1          dd  -0.09

;FLARE_MAX_COLOR     dw FLARE_MAX_COLOR_

Cred1_Flg  db  0
Cred2_Flg  db  0
Cred3_Flg  db  0
Cred4_Flg  db  0