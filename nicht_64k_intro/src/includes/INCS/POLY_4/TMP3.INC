CAM_DIST_   equ 250.0
Min_Z3_     equ -100.0

CAM_DIST__  equ 201.0      ; Cam_Dist+Small_Value
X_SIZE_2_   equ 140.0
Y_SIZE_2_   equ 90.0

Project_Vertex   macro
; edi - vertex ptr 

		 fld   Tmp_Cam_Dist
		 fld   [edi].P_V_Z		
		 fadd  Tmp_Cam_Dist
		 fdivp st(1),st

		 fld   [edi].P_V_X		
		 fmul  st,st(1)
		 fadd  X_SIZE_2
		 fmul  TRI_FIX_P_COEF_XY_
		 fistp [edi].P_V_SX

		 fld   [edi].P_V_Y		
		 fmulp st(1),st
		 fadd  Y_SIZE_2
		 fmul  TRI_FIX_P_COEF_XY_
		 fistp [edi].P_V_SY

;		 retn
;Project_Vertex   endp
		 endm

Z_Clip_3D_Poly     proc
		   pushad

		   lea  ebp,Poly_Data
		   lea  edi,Tmp_Poly_Data
		   mov  New_Poly_V_Num,0

		   call Test_For_Clip_3D__
		   setc Prev_Vert_Clip_Flg
		   jc   @@No_Copy_Vert
		   call Copy_Vertex
@@No_Copy_Vert:		   

		   add  ebp,SIZE POLY_VERTEX_DATA

		   mov  ecx,Poly_V_Num
@@Vertex_Cyc:
		   call Test_For_Clip_3D__
		   mov  al,Prev_Vert_Clip_Flg
		   setc Prev_Vert_Clip_Flg
		   cmp  al,Prev_Vert_Clip_Flg
		   jnz  @@Clip_Edge
		   or   al,al
		   jnz  @@Next_Vertex
		   call Copy_Vertex
		   jmp  @@Next_Vertex
@@Clip_Edge:
		   push eax	
		   call Calc_Edge_Intersect__
		   pop  eax

		   or   al,al
		   jz   @@Next_Vertex
		   call Copy_Vertex
@@Next_Vertex:
		   add  ebp,SIZE POLY_VERTEX_DATA
		   loop @@Vertex_Cyc

;		   dec New_Poly_V_Num
		   mov ecx,New_Poly_V_Num
		   mov Poly_V_Num,ecx
		   imul ecx,SIZE POLY_VERTEX_DATA/4
		   lea  esi,Tmp_Poly_Data
		   lea  edi,Poly_Data
		   rep  movsd

		   lea  esi,Tmp_Poly_Data
		   mov  ecx,SIZE POLY_VERTEX_DATA/4
		   rep  movsd

		   popad
		   retn
Z_Clip_3D_Poly     endp


Calc_Edge_Intersect__  proc

		   mov   esi,ebp
		   sub   esi,SIZE POLY_VERTEX_DATA

		   fld   Min_Z3
		   fsub  [esi].P_V_Z
		   fld   [ebp].P_V_Z
		   fsub  [esi].P_V_Z
		   fdivp st(1),st

		   Edge_Param_Lin_Interp_Float X
		   Edge_Param_Lin_Interp_Float Y
		   Edge_Param_Lin_Interp_Float Z
		   Edge_Param_Lin_Interp U
		   Edge_Param_Lin_Interp V
		   Edge_Param_Lin_Interp FU
		   Edge_Param_Lin_Interp FV
		   Edge_Param_Lin_Interp Z_INT
fstp st
		   Project_Vertex

		   inc New_Poly_V_Num
		   add  edi,SIZE POLY_VERTEX_DATA

		   retn
Calc_Edge_Intersect__  endp

Test_For_Clip_3D__  proc
; ebp:vertex

		    fld   [ebp].P_V_Z
		    fcomp Min_Z3
                    fstsw ax
		    sahf
		    ja    @@No_Clip
		    stc
		    retn
@@No_Clip:
		    clc
		    retn
Test_For_Clip_3D__  endp


.DATA
Tmp_Cam_Dist       dd   CAM_DIST_

Min_Z3             dd   Min_Z3_

Vis_Plane1_Orig VECTOR4 <0.0,0.0,0.0,0.0>
Vis_Plane1_Norm VECTOR4 <0.0,100.0,0.0,0.0>

.DATA?
Edge_Vert1         dd    ?
Edge_Vert2         dd    ?
New_Vert           dd    ?

Tmp_Vec2 VECTOR4   <?>