locals

CAMERA_FOV_HALF_ equ   0.3


Correct_Cam_Matr macro

        lea esi,Cam_Pos
        lea edi,Tmp_Vec3
        call vec_neg
        lea esi,Tmp_Vec3
        lea edi,Cam_Tmp_Matr
        call Do_Mov_Mat
        lea ebx,Cam_Tmp_Matr
        lea esi,Cam_Matr
        lea edi,Cam_Matr
        call matmat_mul

        fld Cam_Roll_Angle
        lea edi,Cam_Tmp_Matr
        call do_rot_Z_mat
        lea ebx,Cam_Matr
        lea esi,Cam_Tmp_Matr
        lea edi,Cam_Matr
        call matmat_mul
endm

Right1 macro
        lea ebx,Cam_Targ
        lea esi,Cam_Pos
        lea edi,Camera_Forward_Vec
        call vecvec_sub

        lea esi,Camera_Forward_Vec
        lea edi,Camera_Right_Vec

        fldz

        cmp dword ptr [esi].VZ,0
        jnz @@tt1
        cmp dword ptr [esi].VX,0
        jnz @@tt1

        fst [edi].VX
        fld1
        fstp [edi].VZ
        jmp @@tt2
@@tt1:
        fld [esi].VZ
        fstp [edi].VX
        fld [esi].VX
        fchs
        fstp [edi].VZ
@@tt2:
        fst [edi].Ort
        fstp [edi].VY

        lea ebx,Camera_Forward_Vec
        lea esi,Camera_Right_Vec
        lea edi,Camera_Up_Vec
        call vecvec_mul2

        lea  edi,Cam_Matr
        call Create_Matr

        lea  esi,Camera_Right_Vec
        call vec_norm
        lea esi,Camera_Up_Vec
        call vec_norm

        lea esi,Camera_Forward_Vec
        vec_module
        fadd st,st
        fld FOV_Half
        fsincos
        fdivp st(1),st
        fmulp st(1),st

        fld st
        fidiv Right_Vec_Coef
        lea  esi,Camera_Right_Vec
        lea  edi,Camera_Right_Vec
        call vec_const_mul
        fstp st

        fidiv Up_Vec_Coef
        lea esi,Camera_Up_Vec
        lea edi,Camera_Up_Vec
        call vec_const_mul
        fstp st

        fild Forward_Vec_Coef
        lea esi,Camera_Forward_Vec
        lea edi,Camera_Forward_Vec
        call vec_const_div
        fstp st

endm

.DATA
Cam_Pos VECTOR4  <0.0,0.0,-20.0,0.0>
Cam_Targ VECTOR4 <0.0,0.0,0.0,0.0>

Cam_Roll_Angle    dd   0.0 ;3.1415

FOV_Half          dd  CAMERA_FOV_HALF_

Right_Vec_Coef    dw  HOR_RAZR/2
Up_Vec_Coef       dw  HOR_RAZR/2
Forward_Vec_Coef  dw  100
.CODE
Calc_Camera_Matrix  proc near
        pushad

        Right1

        lea esi,Camera_Right_Vec
        call vec_sqr
        lea esi,Camera_Right_Vec
        lea edi,Cam_Matr.El00
        call vec_const_div
        fstp st

        lea esi,Camera_Up_Vec
        call vec_sqr
        lea esi,Camera_Up_Vec
        lea edi,Cam_Matr.El10
        call vec_const_div
        fstp st

        lea esi,Camera_Forward_Vec
        call vec_sqr
        lea esi,Camera_Forward_Vec
        lea edi,Cam_Matr.El20
        call vec_const_div
        fstp st

        Correct_Cam_Matr

finit
        popad
        retn
Calc_Camera_Matrix  endp

.DATA?


endp

.DATA?
Camera_Forward_Vec VECTOR4  <?>
Camera_Up_Vec     VECTOR4   <?>
Camera_Right_Vec  VECTOR4   <?>
Cam_Matr          MATRIX4x4 <?>
Cam_Tmp_Matr      MATRIX4x4 <?>
