CLIP_CONST      equ   0
CLIP_CONST_2    equ   VERT_RAZR
CLIP_CONST_4    equ   0
CLIP_CONST_3    equ   HOR_RAZR

.CODE
Chk_Need_Clip      proc

		   lea  ebp,Poly_Data
		   mov  ecx,Poly_V_Num
@@Vert_Cyc:
		   call Test_For_Clip_3D__
		   jc   @@ex
		   call Test_For_Clip_3D
		   jc   @@ex
		   call Test_For_Clip_3D_2
		   jc   @@ex
		   call Test_For_Clip_3D_3
		   jc   @@ex
		   call Test_For_Clip_3D_4
		   jc   @@ex
		   add  ebp,SIZE POLY_VERTEX_DATA
		   loop @@Vert_Cyc

     		   clc
@@ex:
		   retn
Chk_Need_Clip      endp

Calc_Edge_Intersect  proc
		   mov   esi,ebp
		   sub   esi,SIZE POLY_VERTEX_DATA

		   mov   Tmp2,CLIP_CONST SHL TRI_FIX_P_COEF_XY
		   fild  Tmp2
		   fisub [esi].P_V_SY

		   fild  [ebp].P_V_SY
		   fisub [esi].P_V_SY
		   fdivp st(1),st

		   fild  Tmp2
		   fistp [edi].P_V_SY

		   Edge_Param_Lin_Interp SX
		   Edge_Param_Lin_Interp U
		   Edge_Param_Lin_Interp V
		   Edge_Param_Lin_Interp FU
		   Edge_Param_Lin_Interp FV
		   Edge_Param_Lin_Interp Z_INT

		   fstp  st

		   inc New_Poly_V_Num

		   add  edi,SIZE POLY_VERTEX_DATA

		   retn
Calc_Edge_Intersect  endp

Copy_Vertex        proc
		   push ecx

		   mov  esi,ebp
		   mov  ecx,(SIZE POLY_VERTEX_DATA)/4
		   rep  movsd

		   inc New_Poly_V_Num

		   pop  ecx
		   retn
Copy_Vertex        endp


Clip_3D            proc
		   pushad

	           call Z_Clip_3D_Poly
	           cmp  Poly_V_Num,3	
	           jb   @@Not_Visible

		   call Clip_3D_Poly
	           cmp  Poly_V_Num,3	
	           jb   @@Not_Visible

		   call Clip_3D_Poly_2

	           cmp  Poly_V_Num,3	
	           jb   @@Not_Visible

		   call Clip_3D_Poly_3

	           cmp  Poly_V_Num,3	
	           jb   @@Not_Visible

		   call Clip_3D_Poly_4

	           cmp  Poly_V_Num,3	
	           jb   @@Not_Visible
@@Not_Visible:

		   popad
		   retn
Clip_3D            endp	

Clip_3D_Poly       proc
		   pushad

		   lea  ebp,Poly_Data
		   lea  edi,Tmp_Poly_Data
		   mov  New_Poly_V_Num,0


		   call Test_For_Clip_3D
		   setc Prev_Vert_Clip_Flg
		   jc   @@No_Copy_Vert
		   call Copy_Vertex
@@No_Copy_Vert:		   

		   add  ebp,SIZE POLY_VERTEX_DATA

		   mov  ecx,Poly_V_Num
@@Vertex_Cyc:
		   call Test_For_Clip_3D
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
		   call Calc_Edge_Intersect
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
Clip_3D_Poly       endp	

.DATA?
Prev_Vert_Clip_Flg   db   ?
New_Poly_V_Num       dd   ?
Tmp_Vec1        VECTOR4 <?>

.CODE

Test_For_Clip_3D_2  proc
; ebp:vertex
		   cmp   [ebp].P_V_SY,CLIP_CONST_2 SHL TRI_FIX_P_COEF_XY
		   jle   @@No_Clip
		   stc
		   retn
@@No_Clip:
		   clc
		   retn
Test_For_Clip_3D_2  endp


Calc_Edge_Intersect_2  proc
		   mov   esi,ebp
		   sub   esi,SIZE POLY_VERTEX_DATA

		   mov   Tmp2,CLIP_CONST_2 SHL TRI_FIX_P_COEF_XY
		   fild  Tmp2
		   fisub [esi].P_V_SY

		   fild  [ebp].P_V_SY
		   fisub [esi].P_V_SY
		   fdivp st(1),st

		   fild  Tmp2
		   fistp [edi].P_V_SY

		   Edge_Param_Lin_Interp SX
		   Edge_Param_Lin_Interp U
		   Edge_Param_Lin_Interp V
		   Edge_Param_Lin_Interp FU
		   Edge_Param_Lin_Interp FV
		   Edge_Param_Lin_Interp Z_INT

		   fstp  st

		   inc New_Poly_V_Num

		   add  edi,SIZE POLY_VERTEX_DATA

		   retn
Calc_Edge_Intersect_2  endp

Clip_3D_2          proc
		   pushad

		   call Clip_3D_Poly_2

		   popad
		   retn
Clip_3D_2          endp	

Clip_3D_Poly_2     proc
		   pushad

		   lea  ebp,Poly_Data
		   lea  edi,Tmp_Poly_Data
		   mov  New_Poly_V_Num,0

		   call Test_For_Clip_3D_2
		   setc Prev_Vert_Clip_Flg
		   jc   @@No_Copy_Vert
		   call Copy_Vertex
@@No_Copy_Vert:		   

		   add  ebp,SIZE POLY_VERTEX_DATA

		   mov  ecx,Poly_V_Num
@@Vertex_Cyc:
		   call Test_For_Clip_3D_2
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
		   call Calc_Edge_Intersect_2
		   pop  eax

		   or   al,al
		   jz   @@Next_Vertex
		   call Copy_Vertex
@@Next_Vertex:
		   add  ebp,SIZE POLY_VERTEX_DATA
		   loop @@Vertex_Cyc

;		   dec  New_Poly_V_Num
		   mov  ecx,New_Poly_V_Num
		   mov  Poly_V_Num,ecx
		   imul ecx,SIZE POLY_VERTEX_DATA/4
		   lea  esi,Tmp_Poly_Data
		   lea  edi,Poly_Data
		   rep  movsd

		   lea  esi,Tmp_Poly_Data
		   mov  ecx,SIZE POLY_VERTEX_DATA/4
		   rep  movsd

		   popad
		   retn
Clip_3D_Poly_2     endp	


Test_For_Clip_3D_3  proc
; ebp:vertex
		   cmp   [ebp].P_V_SX,CLIP_CONST_3 SHL TRI_FIX_P_COEF_XY
		   jle   @@No_Clip
		   stc
		   retn
@@No_Clip:
		   clc
		   retn
Test_For_Clip_3D_3  endp


Calc_Edge_Intersect_3  proc
		   mov   esi,ebp
		   sub   esi,SIZE POLY_VERTEX_DATA

		   mov   Tmp2,CLIP_CONST_3 SHL TRI_FIX_P_COEF_XY
		   fild  Tmp2
		   fisub [esi].P_V_SX

		   fild  [ebp].P_V_SX
		   fisub [esi].P_V_SX
		   fdivp st(1),st

		   fild  Tmp2
		   fistp [edi].P_V_SX

		   Edge_Param_Lin_Interp SY
		   Edge_Param_Lin_Interp U
		   Edge_Param_Lin_Interp V
		   Edge_Param_Lin_Interp FU
		   Edge_Param_Lin_Interp FV
		   Edge_Param_Lin_Interp Z_INT

		   fstp  st

		   inc New_Poly_V_Num

		   add  edi,SIZE POLY_VERTEX_DATA

		   retn
Calc_Edge_Intersect_3  endp

Clip_3D_3          proc
		   pushad

		   call Clip_3D_Poly_3

		   popad
		   retn
Clip_3D_3          endp	

Clip_3D_Poly_3     proc
		   pushad

		   lea  ebp,Poly_Data
		   lea  edi,Tmp_Poly_Data
		   mov  New_Poly_V_Num,0

		   call Test_For_Clip_3D_3
		   setc Prev_Vert_Clip_Flg
		   jc   @@No_Copy_Vert
		   call Copy_Vertex
@@No_Copy_Vert:		   

		   add  ebp,SIZE POLY_VERTEX_DATA

		   mov  ecx,Poly_V_Num
@@Vertex_Cyc:
		   call Test_For_Clip_3D_3
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
		   call Calc_Edge_Intersect_3
		   pop  eax

		   or   al,al
		   jz   @@Next_Vertex
		   call Copy_Vertex
@@Next_Vertex:
		   add  ebp,SIZE POLY_VERTEX_DATA
		   loop @@Vertex_Cyc

;		   dec  New_Poly_V_Num
		   mov  ecx,New_Poly_V_Num
		   mov  Poly_V_Num,ecx
		   imul ecx,SIZE POLY_VERTEX_DATA/4
		   lea  esi,Tmp_Poly_Data
		   lea  edi,Poly_Data
		   rep  movsd

		   lea  esi,Tmp_Poly_Data
		   mov  ecx,SIZE POLY_VERTEX_DATA/4
		   rep  movsd

		   popad
		   retn
Clip_3D_Poly_3     endp	

Test_For_Clip_3D   proc
; ebp:vertex
		   cmp   [ebp].P_V_SY,CLIP_CONST SHL TRI_FIX_P_COEF_XY
		   jge   @@No_Clip
		   stc
		   retn
@@No_Clip:
		   clc
		   retn
Test_For_Clip_3D   endp


Test_For_Clip_3D_4  proc
; ebp:vertex
		   cmp   [ebp].P_V_SX,CLIP_CONST_4 SHL TRI_FIX_P_COEF_XY
		   jge   @@No_Clip
		   stc
		   retn
@@No_Clip:
		   clc
		   retn
Test_For_Clip_3D_4  endp


Calc_Edge_Intersect_4  proc
		   mov   esi,ebp
		   sub   esi,SIZE POLY_VERTEX_DATA

		   mov   Tmp2,CLIP_CONST_4 SHL TRI_FIX_P_COEF_XY
		   fild  Tmp2
		   fisub [esi].P_V_SX

		   fild  [ebp].P_V_SX
		   fisub [esi].P_V_SX
		   fdivp st(1),st

		   fild  Tmp2
		   fistp [edi].P_V_SX

		   Edge_Param_Lin_Interp SY
		   Edge_Param_Lin_Interp U
		   Edge_Param_Lin_Interp V
		   Edge_Param_Lin_Interp FU
		   Edge_Param_Lin_Interp FV
		   Edge_Param_Lin_Interp Z_INT

		   fstp  st

		   inc New_Poly_V_Num

		   add  edi,SIZE POLY_VERTEX_DATA

		   retn
Calc_Edge_Intersect_4  endp

Clip_3D_4          proc
		   pushad

		   call Clip_3D_Poly_4

		   popad
		   retn
Clip_3D_4          endp	

Clip_3D_Poly_4     proc
		   pushad

		   lea  ebp,Poly_Data
		   lea  edi,Tmp_Poly_Data
		   mov  New_Poly_V_Num,0

		   call Test_For_Clip_3D_4
		   setc Prev_Vert_Clip_Flg
		   jc   @@No_Copy_Vert
		   call Copy_Vertex
@@No_Copy_Vert:		   

		   add  ebp,SIZE POLY_VERTEX_DATA

		   mov  ecx,Poly_V_Num
@@Vertex_Cyc:
		   call Test_For_Clip_3D_4
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
		   call Calc_Edge_Intersect_4
		   pop  eax

		   or   al,al
		   jz   @@Next_Vertex
		   call Copy_Vertex
@@Next_Vertex:
		   add  ebp,SIZE POLY_VERTEX_DATA
		   loop @@Vertex_Cyc

;		   dec  New_Poly_V_Num
		   mov  ecx,New_Poly_V_Num
		   mov  Poly_V_Num,ecx
		   imul ecx,SIZE POLY_VERTEX_DATA/4
		   lea  esi,Tmp_Poly_Data
		   lea  edi,Poly_Data
		   rep  movsd

		   lea  esi,Tmp_Poly_Data
		   mov  ecx,SIZE POLY_VERTEX_DATA/4
		   rep  movsd

		   popad
		   retn
Clip_3D_Poly_4     endp	

;Triangle_Phong proc
int 3
retn
;endp

;Triangle_Tex proc
int 3
retn
;endp
