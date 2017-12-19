.CODE
locals

Make_Obj_Matr    proc    near
; ebx: object ptr
; esi: vertex matrix ptr
; ebp: normals matrix ptr
                 pushad

                 push esi
                 push ebp

                 lea  esi,[ebx].OBJECT_MOVE_VEC
                 lea  edi,Tmp_Move_Matr
                 call do_mov_mat

                 fld  [ebx].OBJECT_ANGLE_X
                 lea  edi,Tmp_Rot_Matr_X
                 call do_rot_X_mat
                 fld  [ebx].OBJECT_ANGLE_Y
                 lea  edi,Tmp_Rot_Matr_Y
                 call do_rot_Y_mat
                 fld  [ebx].OBJECT_ANGLE_Z
                 lea  edi,Tmp_Rot_Matr_Z
                 call do_rot_Z_mat

                 lea  ebx,Tmp_Rot_Matr_X
                 lea  esi,Tmp_Rot_Matr_Y
                 lea  edi,Obj_Matr1
                 call matmat_mul
                 lea  ebx,Obj_Matr1
                 lea  esi,Tmp_Rot_Matr_Z
                 lea  edi,Obj_Matr1
                 call matmat_mul

                 pop  esi

                 lea  ebx,Obj_Matr1
                 lea  edi,Obj_Norm_Matr1
                 call matmat_mul

                 lea  ebx,Obj_Matr1
                 lea  esi,Tmp_Move_Matr
                 lea  edi,Obj_Matr1
                 call matmat_mul

                 pop  esi
                 lea  ebx,Obj_Matr1
                 lea  edi,Obj_Vert_Matr1
                 call matmat_mul

                 popad
                 retn
Make_Obj_Matr    endp

Transform_Object  proc
; ebx: object ptr
; esi: vertex matrix ptr
; ebp: normals matrix ptr

                 pushad

                 call Make_Obj_Matr

                 lea esi,Obj_Vert_Matr1
                 lea ebp,Obj_Norm_Matr1

                 push ebp
                 push ebx
                                                ; Повернем вершины
                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX

cmp [ebx].OBJECT_New_Coord_Transf_Flg,1       ; Если после морфинга коорд.
jz  @@B_S_Loop1_                ; в New, то...

@@B_S_Loop1:
                 lea  ebx,[ebp].X
                 lea  edi,[ebp].New_X
                 call vecmat_mul

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@B_S_Loop1
                 jmp  @@tt1

@@B_S_Loop1_:
                 lea  ebx,[ebp].New_X
                 lea  edi,[ebp].New_X
                 call vecmat_mul

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@B_S_Loop1_
@@tt1:
                 pop  ebx
                 pop  esi

                 push ebx
                                                ; Повернем нормали вершин
                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX

@@B_S_Loop3:
                 lea  ebx,[ebp].NX
                 lea  edi,[ebp].New_NX
                 call vecmat_mul

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@B_S_Loop3

                 pop  ebx
                                                ; Повернем нормали у полигонов
                 mov  ebp,[ebx].FIRST_FACE
                 mov  eax,[ebx].LAST_FACE

@@B_S_Loop2:
                 lea  ebx,[ebp].PNX
                 mov  edi,ebx
                 call vecmat_mul

                 add  ebp, SIZE TFACE
                 cmp  ebp,eax
                 jbe  @@B_S_Loop2

                 popad
                 retn
Transform_Object endp

Project_Object  proc    near
; ebx: object ptr
                 pushad

                 mov eax,CAMERA_DIST[ebx]
                 mov Cam_Dist,eax

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX
@@Project:
                 fld   Cam_Dist
                 fld   [ebp].New_Z
                 fadd  Cam_Dist
                 fdivp st(1),st

                 fld   [ebp].New_Y
                 fld   [ebp].New_X
                 fmul  st,st(2)
                 fadd  X_SIZE_2
                 fmul  TRI_FIX_P_COEF_XY_
                 fistp [ebp].SX

                 fmulp st(1),st
                 fadd  Y_SIZE_2
                 fmul  TRI_FIX_P_COEF_XY_
                 fistp [ebp].SY

                 add  ebp,SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@Project

                 popad
                 retn
                 endp
.DATA
Cam_Dist         dd    ?

Transform_3D_Scene  proc
; edi: scene ptr
; esi: vertex matrix ptr

                 pushad

                 call Calc_Camera_Matrix

                 push edi
                 push esi
                 mov  ebx,esi
                 lea  esi,Cam_Matr
                 lea  edi,World_Matr1
                 call matmat_mul

                 pop  ebp
                 lea  esi,World_Matr1
                 pop  edi
@@Do_Scene_Objects:
                 mov  ebx,[edi]
                 cmp  ebx,NULL
                 jz   @@ex

                 call Transform_Object

                 add edi,4
                 jmp @@Do_Scene_Objects
@@ex:
                 popad
                 retn
Transform_3D_Scene endp


.DATA?
Cur_Transform_Object dd  ?
Cur_Vert_Matr_Ptr    dd  ?
Cur_Norm_Matr_Ptr    dd  ?
Obj_Matr1            MATRIX4x4 <?>
Obj_Norm_Matr1       MATRIX4x4 <?>
Obj_Vert_Matr1       MATRIX4x4 <?>
World_Matr1          MATRIX4x4 <?>
Normals_Matr_Ptr     dd  ?
Vert_Matr_Ptr        dd  ?

Tmp_Rot_Matr_X        MATRIX4x4 <?>
Tmp_Rot_Matr_Y        MATRIX4x4 <?>
Tmp_Rot_Matr_Z        MATRIX4x4 <?>
Tmp_Move_Matr         MATRIX4x4 <?>