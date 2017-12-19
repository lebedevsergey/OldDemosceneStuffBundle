.CODE

Init_Part4 proc near

lea edi,Font_Particles
mov ecx,MY_FONT_X_SIZE*MY_FONT_Y_SIZE*SIZE FONT_PARTICLE/4
xor eax,eax
rep stosd

        mov  eax,Time2
        mov  Base_Time1,eax

        lea esi,Thing2
        lea edi,Text_Obj5_1
        call Do_Object
        lea esi,Thing2
        lea edi,Text_Obj5_2
        call Do_Object

                lea  ebx,Text_Obj5_2
                call Gen_Part_Obj1
                lea  ebx,Text_Obj5_1
                call Gen_Part_Obj1

mov X_Pixel_Interval,12;7
mov Y_Pixel_Interval,13;7
                lea  ebx,Font1
                mov [ebx].Font_X_Interval,-5
                mov  ecx,110
                mov  edx,-130
                mov  eax,10
                lea  esi,Str1
                lea  ebp,Text_Obj5_2
push Text_Obj5_2.LAST_VERTEX
                call Gen_Text_Obj
pop Text_Obj5_2.LAST_VERTEX

                mov Rays_Flg,0
                mov Ring_Flg,0
                mov Glow_Flg,1
                mov Center_Light_Flg,1


                fld  Flare_Center_Light_Coef5_1
                fstp Flare_Center_Light
                fld  Flare_Glow_Coef5_1
                fstp Flare_Glow

                mov Glow_Flg,1
                mov Center_Light_Flg,1
                mov ebx,45;65
                mov ebp,45;65
                lea edi,Alpha_Sprite1
                mov ax,100
                mov cx,100
                mov dx,100
                call Gen_Flare

        retn
        endp

.CODE
Part4   proc near

        fld   Text_Angle_Z5
        lea   edi,Matr1
        call  do_rot_Z_mat
        lea   esi,Mov_Vec5
        lea   edi,Matr2
        call  do_mov_mat

        lea   ebx,Matr2
        lea   esi,Matr1
        lea   edi,Matr1
        call  matmat_mul

        fld1
        fild  Text_Morf_Coef1
        call  Sin_Param
        fabs
;        fstp
 ;       fld1

        lea   esi,Matr1
        lea   ebx,Text_Obj5_1
        lea   edi,Text_Obj5_2
        call  morf_trans_alpha_obj

        mov edi,Virt_Screen3
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd
        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        mov   Cur_Alpha_Sprite,offset Alpha_Sprite1
        lea   ebx,Text_Obj5_1
        mov   edi,Virt_Screen3
        call  draw_alpha_obj3

        lea   ebp,Texture5_1
        mov   esi,Virt_Screen3
        mov   edi,Virt_Screen1
        call  Bump

        mov edi,offset Virt_Screen1+HOR_RAZR*VERT_RAZR*4
        xor eax,eax
        mov [edi],eax

retn
endp

.DATA
Phong1_Coef_5   dd  0.004;2;1

Flare_Center_Light_Coef5_1  dd  -0.02;03
Flare_Glow_Coef5_1  dd  -1.9;03

Mov_Vec5 VECTOR4 <0.0,0.0,-10.0>
Text_Angle_Z5         dd  0.0

Text_Morf_Coef1      dd  300000

.DATA?
Text_Obj5_1  TMODEL <?>
Text_Obj5_2  TMODEL <?>
Alpha_Sprite5         dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)