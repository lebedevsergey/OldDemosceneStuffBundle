.CODE

Init_Part3 proc near

mov ebx,30
mov ebp,30
lea edi,Alpha_Sprite1
mov ax,100
mov cx,60
mov dx,50
call Gen_Flare

        mov  eax,Time2
;        mov  Fade_Base_Time,eax
        mov  Base_Time1,eax

        lea esi,Big_Cyl3_1
        lea edi,Object4_6
        call Do_Object

        lea esi,Cup3_1
        lea edi,Object4_5
        call Do_Object

        lea esi,Lit_Cyl3_1
        lea edi,Object4_1
        call Do_Object

        lea esi,Lit_Cyl3_2
        lea edi,Object4_2
        call Do_Object

        lea esi,Lit_Cyl3_3
        lea edi,Object4_3
        call Do_Object

        lea esi,Lit_Cyl3_4
        lea edi,Object4_4
        call Do_Object

        lea esi,Part_Obj3_4
        lea edi,Part_Object4
        call Do_Object

        lea esi,Top3_1
        lea edi,Object4_7
        call Do_Object

        mov dword ptr [Poly_Texture1],offset Texture4_1
        mov dword ptr [Poly_Texture2],offset Texture4_2

        lea  ebx,Part_Object4
        call Init_Particles2

                fld  Flare_Center_Light_Coef3
                fstp Flare_Center_Light
                fld  Flare_Glow_Coef3
                fstp Flare_Glow

lea edi,Light3_Sprite
mov ebx,50
mov ebp,50
mov ax,70
mov cx,40
mov dx,25
call Gen_Flare

        retn
        endp

.CODE
Part3   proc near

        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        mov  eax,07FFFFFFFh
        mov  ecx,HOR_RAZR*VERT_RAZR
        lea  edi,ZBuffer
        rep  stosd

        lea  ebx,Part_Object4
        call Update_Particles2

        lea  edi,Matr1
        call Create_Matr

        fld   Cam_Angle_X1_Coef3
        fild  Cam_Mov_X1_Coef
        call  Sin_Param
        fadd  Cam_Angle_X1_Coef2
        fstp  Angle_X

        fld   Cam_Angle_Z1_Coef3
        fild  Cam_Mov_Z1_Coef
        call  Sin_Param
        fadd  Cam_Angle_Z1_Coef2
        fstp  Angle_Z

        fld   Cam_Angle_Y1_Coef3
        fild  Cam_Mov_Y1_Coef
        call  Sin_Param
        fadd  Cam_Angle_Y1_Coef2
        fstp  Angle_Y

        fld Angle_X
        mov Tmp1,100
        fcos
        fimul Tmp1
        fstp Cam_Pos.VX
        fld Angle_Y
        mov Tmp1,-50
        fcos
        fimul Tmp1
        fstp Cam_Pos.VY
        fld Angle_Z
        mov Tmp1,80
        fcos
        fimul Tmp1
        fstp Cam_Pos.VZ
        fld1
        fstp Cam_Pos.Ort


;fld Angle_Z
;fstp Cam_Roll_Angle

        fld  Angle_X_Coef2
        fild  Angle_X_Coef1
        call  Sin_Param
        fstp  Angle_X
;        fld  Angle_Y_Coef2
        fild  Angle_Y_Coef1
        call  Line_Param
        fstp  Angle_Y

        fld Angle_X
        lea  edi,Matr1
        call do_rot_X_mat

        fld Angle_Y
        lea  edi,Matr2
        call do_rot_Y_mat

        lea  ebx,Matr1
        lea  esi,Matr2
        lea  edi,Matr1
        call matmat_mul

        lea  esi,Matr1
        lea  edi,Scene4
        call transform_3D_scene

mov Cur_Particle,offset Particles2

        mov edi,Virt_Screen1
        lea esi,Scene4
        call Draw_3D_Scene

;        call Do_Fade

        fldz
        lea  edi,Matr1
        call do_rot_Y_mat

lea  ebx,Light3_Pos.X
lea  esi,Matr1
lea  edi,Light3_Pos.New_X
call vecmat_mul

fld   Light3_Pos.New_X
fadd  X_SIZE_2
fistp Tmp2
mov   ebx,Tmp2
fld   Light3_Pos.New_Y
fadd  Y_SIZE_2
fistp Tmp2
mov   ecx,Tmp2

                 mov  edi,Virt_Screen1

                 fld  Light3_Pos.New_Z
                 fmul TRI_FIX_P_COEF_XY_
                 fistp Cur_Poly_Z

                 lea esi,Light3_Sprite
                 call Draw_Alpha_Sprite_With_Z_Buf2
                 fild Light_Drawed_Pixels_Count
                 fdiv Light_Drawed_Pixels_Coef1
                 fstp Light_Drawed_Pixels_Coef2

                 fild Light_Drawed_Pixels_Count
                 fdiv Light_Drawed_Pixels_Coef4
                 fstp Light_Drawed_Pixels_Coef2

mov tmp1,40
fild tmp1
fstp Flare_Camera_Pos.VY
mov tmp1,20
fild tmp1
fstp Flare_Camera_Pos.VX

mov edi,Virt_Screen1
lea  ebx,Light3_Pos.New_X
call Draw_Flares

retn
endp

.DATA
Angle_X   dd  0.0
Angle_Y   dd  0.0
Angle_Z   dd  0.0

Light_Drawed_Pixels_Coef4  dd  1900.0;3990.0

Scene4 dd Object4_1, Object4_2, Object4_3,Object4_4,\
          Object4_5, Object4_6,Object4_7,Part_Object4,NULL

Angle_X_Coef1  dd 1000000
Angle_X_Coef2  dd 0.1
Angle_Y_Coef1  dd 100000
Angle_Y_Coef2  dd 6.28

Cam_Angle_Z1_Coef   dd  100000
Cam_Angle_Z1_Coef2   dd  -0.5
Cam_Angle_Z1_Coef3    dd  0.4
Cam_Mov_Z1_Coef      dd  100000;700000

Cam_Angle_X1_Coef   dd  100000
Cam_Angle_X1_Coef2   dd  -0.5
Cam_Angle_X1_Coef3    dd  0.4
Cam_Mov_X1_Coef      dd  100000;700000

Cam_Angle_Y1_Coef   dd  100000
Cam_Angle_Y1_Coef2   dd  -3.4
Cam_Angle_Y1_Coef3    dd  3.8
Cam_Mov_Y1_Coef      dd  200000;700000

Light3_Pos    TVERTEX <1.0,20.0,100.1,1.0>

Flare_Center_Light_Coef3  dd  -0.004
Flare_Glow_Coef3          dd  -0.1

.DATA?
Object4_1 TMODEL <?>
Object4_2 TMODEL <?>
Object4_3 TMODEL <?>
Object4_4 TMODEL <?>
Object4_5 TMODEL <?>
Object4_6 TMODEL <?>
Object4_7 TMODEL <?>
Part_Object4 TMODEL <?>

Light3_Sprite      dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)