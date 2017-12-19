PULSE_TIME   equ  7000

.CODE

Init_Part7 proc near

                 fld Radian_Coef
                 fmul Wobble_Freq
                 fstp Wobble_Freq_

        lea esi,Part_Obj7
        lea edi,Part_Object7
        call Do_Object

        lea esi,Part_Obj7_2
        lea edi,Part_Object7_2
        call Do_Object

        lea ebx,Part_Object7
        call Clear_Particles


call Init_Blobs

mov  Meta_Tex,offset Texture7_1

fld Min_Alpha_Z_6
fstp Min_Alpha_Z

        mov  eax,Time2
;        mov  Fade_Base_Time,eax
        mov  Base_Time1,eax
        fld  Time2
        fisub PULSE_TIME_-10
        fstp  PULSE_BASE_TIME

        lea esi,Lit_Cyl7_1
        lea edi,Object7_1
        call Do_Object
        lea esi,Lit_Cyl7_2
        lea edi,Object7_2
        call Do_Object

        lea ebx,Part_Object7
        call Clear_Particles

mov ebx,70
mov ebp,70
lea edi,Alpha_Sprite1
mov ax,50
mov cx,60
mov dx,100
call Gen_Flare

mov ebx,100
mov ebp,100
lea edi,Alpha_Sprite2
mov ax,100
mov cx,80
mov dx,70
call Gen_Flare

        retn
        endp

.CODE
Part7   proc near

        lea  esi,Times_Tab7_1
        call Do_Time_Tab


        mov Wobble_Amp,0

        fld Time2
        fsub PULSE_BASE_TIME
        ficomp PULSE_TIME_
        fstsw ax
        sahf
        jnc @@yy5
        mov eax,Wobble_Amp_2
        mov Wobble_Amp,eax
@@yy5:
        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        mov  eax,07FFFFFFFh
        mov  ecx,HOR_RAZR*VERT_RAZR
        lea  edi,ZBuffer
        rep  stosd

        lea  edi,Matr1
        call Create_Matr

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
        fimul Cam_Mov_Amp_7
        fiadd Cam_Mov_Add_7
        fstp Cam_Pos.VZ
        fld1
        fstp Cam_Pos.Ort

        fldz
        fstp  Cam_Roll_Angle

fld Angle_Z
fstp Cam_Roll_Angle

        fld  Angle_X_Coef2
        fild  Angle_X_Coef1
        call  Sin_Param
        fstp  Angle_X
;        fld  Angle_Y_Coef2
        fild  Angle_Y_Coef1
        call  Line_Param
        fstp  Angle_Y

        fild  Man_Angle_Y_Coef
        call  Line_Param
        fst   Part_Object6.OBJECT_ANGLE_Y
        fstp  Object6_2.OBJECT_ANGLE_Y


lea esi,Blob1
lea ebp,Blob1_Coefs
mov ecx,BLOBS_NUM
@@Calc1:
        fild  Blob_X_Amp
        fild   dword ptr [ebp]
        call  Sin_Param
        fstp  [esi].Blob_X
        fild  Blob_Y_Amp
        fild   dword ptr [ebp+4]
        call  Sin_Param
        fstp  [esi].Blob_Y
        fild  Blob_Z_Amp
        fild   dword ptr [ebp+8]
        call  Sin_Param
        fstp  [esi].Blob_Z

    add ebp,3*4
    add esi,SIZE BLOB_STRUC
    dec ecx
    jnz @@Calc1

call Do_Surface

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

lea ebx,Part_Object7
call Wobble_Object

        lea  esi,Matr1
        lea  edi,Scene7
        call transform_3D_scene

;        lea  ebx,Part_Object7
 ;       call Update_Particles2

mov Cur_Particle,offset Particles2
        mov edi,Virt_Screen1
        lea esi,Scene7
        call Draw_3D_Scene


;        call Do_Fade

retn
endp

Scene7 dd Object6_1,Object7_1,Object7_2,Meta_Object,Part_Object7,Part_Object7_2,NULL

Cam_Mov_Amp_7  dw  145
Cam_Mov_Add_7  dw  205

Blob1_Coefs:

dd 160000
dd 60000
dd 160000

dd -160000
dd 130000
dd -180000

dd -100000
dd 160000
dd 160000

dd -260000
dd 100000
dd -110000

dd 160000
dd -150000
dd 80000

dd 60000
dd 150000
dd 100000

Blob_X_Amp  dw   35
Blob_Y_Amp  dw   55
Blob_Z_Amp  dw   35

PULSE_Coef1   dd  PULSE_TIME
PULSE_TIME_   dd  PULSE_TIME

Wobble_Amp_2  dd  0.5

.DATA?
PULSE_BASE_TIME  dd  ?
Object7_1 TMODEL <?>
Object7_2 TMODEL <?>
Part_Object7 TMODEL <?>
Part_Object7_2 TMODEL <?>