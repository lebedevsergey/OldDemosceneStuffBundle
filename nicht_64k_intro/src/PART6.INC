.CODE

Init_Part6 proc near

fld Min_Alpha_Z_6
fstp Min_Alpha_Z

                mov Glow_Flg,1
                mov Center_Light_Flg,1
                fld  Flare_Center_Light_Coef6_1
                fstp Flare_Center_Light
                fld  Flare_Glow_Coef6_1
                fstp Flare_Glow
mov ebx,70
mov ebp,70
lea edi,Alpha_Sprite1
mov ax,100
mov cx,60
mov dx,50
call Gen_Flare
;mov ebx,70
;mov ebp,70
lea edi,Alpha_Sprite2
mov ax,50
mov cx,100
mov dx,60
call Gen_Flare
lea edi,Alpha_Sprite3
mov ax,50
mov cx,60
mov dx,100
call Gen_Flare

        mov  eax,Time2
;        mov  Fade_Base_Time,eax
        mov  Base_Time1,eax

        lea esi,Big_Cyl3_1
        lea edi,Object6_1
        call Do_Object

        mov dword ptr Big_Cyl6_1_Patch, offset Texture4_1_2
        lea esi,Big_Cyl3_1
        lea edi,Object6_1_2
        call Do_Object

        lea esi,Man_Obj_1
        lea edi,Object6_2
        call Do_Object
        mov dword ptr Man_Obj_Offs1,offset Man2_Obj
        lea esi,Man_Obj_1
        lea edi,Object6_2_2
        call Do_Object

        lea esi,Lit_Cyl6_1
        lea edi,Object6_3
        call Do_Object

        lea esi,Part_Obj6_1
        lea edi,Part_Object6
        call Do_Object
        mov  ebp,[edi].FIRST_FACE
        mov  eax,[edi].LAST_FACE
        sub  eax,SIZE TFACE*2
@@B_S_Loop2:
        mov  [ebp].Poly_Texture_Ptr,offset Alpha_Sprite3
        mov  [ebp+SIZE TFACE].Poly_Texture_Ptr,offset Alpha_Sprite2
                 add  ebp, SIZE TFACE*3
                 cmp  ebp,eax
                 jbe  @@B_S_Loop2


        mov  eax,[edi].FIRST_VERTEX
        mov  ebx,eax
        add  eax,SIZE TVERTEX
        mov  edx,[eax].X
        mov  [ebx].X,edx
        mov  edx,[eax].Y
        mov  [ebx].Y,edx
        mov  edx,[eax].Z
        mov  [ebx].Z,edx

        lea ebx,Part_Object6
        call Clear_Particles

mov Man_Flg,0

        retn
        endp

.CODE
Part6   proc near

;        lea  ebx,Part_Object6
 ;       call Update_Particles2


        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        mov  eax,07FFFFFFFh
        mov  ecx,HOR_RAZR*VERT_RAZR
        lea  edi,ZBuffer
        rep  stosd

        lea  esi,Times_Tab6_2
cmp Man2_Flg,0
jnz @@2
        lea  esi,Times_Tab6_1
@@2:
        call Do_Time_Tab


cmp Man_Flg,0
jnz @@Man2
mov dword ptr Scene6_Patch2,offset Object6_2
mov dword ptr  Scene6_Patch3,offset Object6_1
jmp @@yy5
@@Man2:
mov dword ptr Scene6_Patch2,offset Object6_2_2
mov dword ptr  Scene6_Patch3,offset Object6_1_2
@@yy5:


        lea  edi,Matr1
        call Create_Matr

        cmp  Cam_Path_Flg6,1
        jz   @@Cam_Path2

        fld   Cam_Angle_X1_Coef3_6
        fild  Cam_Mov_X1_Coef_6
        call  Sin_Param
        fadd  Cam_Angle_X1_Coef2_6
        fstp  Angle_X

        fld   Cam_Angle_Z1_Coef3_6
        fild  Cam_Mov_Z1_Coef_6
        call  Sin_Param
        fadd  Cam_Angle_Z1_Coef2_6
        fstp  Angle_Z

        fld   Cam_Angle_Y1_Coef3_6
        fild  Cam_Mov_Y1_Coef_6
        call  Sin_Param
        fadd  Cam_Angle_Y1_Coef2_6
        fstp  Angle_Y

        fld Angle_X
        mov Tmp1,100
        fcos
        fimul Tmp1
fstp st
mov tmp1,10
fild tmp1
        fstp Cam_Pos.VX
        fld Angle_Y
        mov Tmp1,-50
        fcos
        fimul Tmp1
fstp st
mov tmp1,10
fild tmp1
        fstp Cam_Pos.VY
        fld Angle_Z
        fcos
        fimul Cam_Mov_Amp_6
        fstp Cam_Pos.VZ
        fld1
        fstp Cam_Pos.Ort

        jmp  @@tt5

@@Cam_Path2:

        fld   Cam_Angle_X1_Coef3_6_2
        fild  Cam_Mov_X1_Coef_6_2
        call  Sin_Param
        fadd  Cam_Angle_X1_Coef2_6_2
        fstp  Angle_X

        fld   Cam_Angle_Z1_Coef3_6_2
        fild  Cam_Mov_Z1_Coef_6_2
        call  Sin_Param
        fadd  Cam_Angle_Z1_Coef2_6_2
        fstp  Angle_Z

        fld   Cam_Angle_Y1_Coef3_6_2
        fild  Cam_Mov_Y1_Coef_6_2
        call  Sin_Param
        fadd  Cam_Angle_Y1_Coef2_6_2
        fstp  Angle_Y

        fld Angle_X
        mov Tmp1,150
        fcos
        fimul Tmp1
;fstp st
;mov tmp1,10
;fild tmp1
        fstp Cam_Pos.VX
        fld Angle_Y
        mov Tmp1,-30
        fcos
        fimul Tmp1
;fstp st
;mov tmp1,10
;fild tmp1
        fstp Cam_Pos.VY
        fld Angle_Z
        fcos
        fabs
        fimul Cam_Mov_Amp_6_2
        fiadd Cam_Mov_Adder_6_2
        fstp Cam_Pos.VZ
        fld1
        fstp Cam_Pos.Ort

        fld   Cam_Roll_Z1_Coef3_6
        fild  Cam_Roll_Z1_Coef_6
        call  Sin_Param
;        fadd  Cam_Roll_Z1_Coef2_6
        fstp  Cam_Roll_Angle
@@tt5:

        fld  Angle_X_Coef2
        fild  Angle_X_Coef1
        call  Sin_Param
        fstp  Angle_X
;        fld  Angle_Y_Coef2
        fild  Angle_Y_Coef1
        call  Line_Param
        fstp  Angle_Y

        fild  Part_Object6_Y_Coef
        call  Line_Param
        fstp   Part_Object6.OBJECT_ANGLE_Y

        fild  Man_Angle_Y_Coef
        call  Line_Param
        fst  Object6_2.OBJECT_ANGLE_Y
        fstp  Object6_2_2.OBJECT_ANGLE_Y

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
        lea  edi,Scene6
        call transform_3D_scene

mov Cur_Particle,offset Particles2
        mov edi,Virt_Screen1
        lea esi,Scene6
        call Draw_3D_Scene

;        call Do_Fade

        lea  esi,Times_Tab6
        call Do_Time_Tab

retn
endp

Scene6:
Scene6_Patch3:
dd Object6_1
Scene6_Patch2:
dd  Object6_2, Object6_3
Scene6_Patch:
dd NULL,NULL

Man_Angle_Y_Coef           dd   20000
Part_Object6_Y_Coef        dd   100000

Cam_Angle_Z1_Coef_6   dd  1700000
Cam_Angle_Z1_Coef2_6   dd  -1.5
Cam_Angle_Z1_Coef3_6    dd  1.4
Cam_Mov_Z1_Coef_6      dd  250000;700000

Cam_Angle_X1_Coef_6   dd  1000000
Cam_Angle_X1_Coef2_6   dd  0.0
Cam_Angle_X1_Coef3_6    dd  0.0
Cam_Mov_X1_Coef_6      dd  100000;700000

Cam_Angle_Y1_Coef_6   dd  500000
Cam_Angle_Y1_Coef2_6   dd  0.0
Cam_Angle_Y1_Coef3_6    dd  10.0
Cam_Mov_Y1_Coef_6      dd  170000;700000

Cam_Mov_Amp_6       dw    150

Min_Alpha_Z_6       dd    1.0

Flare_Center_Light_Coef6_1  dd  -0.0007;-0.001;03
Flare_Glow_Coef6_1  dd  -0.9;03

Cam_Path_Flg6       db    0

Cam_Angle_Z1_Coef_6_2   dd  1700000
Cam_Angle_Z1_Coef2_6_2   dd  -1.5
Cam_Angle_Z1_Coef3_6_2    dd  1.4
Cam_Mov_Z1_Coef_6_2      dd  100000;700000

Cam_Angle_X1_Coef_6_2   dd  1000000
Cam_Angle_X1_Coef2_6_2   dd  0.0
Cam_Angle_X1_Coef3_6_2    dd  0.0
Cam_Mov_X1_Coef_6_2      dd  100000;700000

Cam_Angle_Y1_Coef_6_2   dd  500000
Cam_Angle_Y1_Coef2_6_2   dd  0.0
Cam_Angle_Y1_Coef3_6_2    dd  10.0
Cam_Mov_Y1_Coef_6_2      dd  200000;700000

Cam_Roll_Z1_Coef3_6     dd  0.8
Cam_Roll_Z1_Coef_6      dd  160000
Cam_Mov_Amp_6_2         dd  140
Cam_Mov_Adder_6_2       dd  3
Man2_Flg    db  0

.DATA?
Object6_1 TMODEL <?>
Object6_1_2 TMODEL <?>
Object6_2 TMODEL <?>
Object6_3 TMODEL <?>
Object6_2_2 TMODEL <?>
Part_Object6 TMODEL <?>
Man_Flg   db   ?
