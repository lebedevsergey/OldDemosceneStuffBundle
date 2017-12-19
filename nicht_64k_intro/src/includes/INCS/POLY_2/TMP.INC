.CODE
locals

Prepare_Poly_Data  proc  near
; ESI - ptr to poly
		   pushad
		   push esi

		   lea  edi,Poly_Data
		   lea  ebp,[esi].PA

                   mov  edx,3                 ;  Init vertexes num
		   mov  Poly_V_Num,edx
@@Vert_Loop:
		   mov  esi,[ebp]
		   lea  esi,[esi].New_X
	           mov  ecx,SIZE POLY_VERTEX_DATA/4  ;  Needed poly data size
		   rep  movsd	
		   add  ebp,4		      ;  Next vertex
		   dec  edx	
		   jnz  @@Vert_Loop

		   pop  esi

		   mov  esi,[esi]
		   lea  esi,[esi].New_X
	           mov  ecx,SIZE POLY_VERTEX_DATA/4  ;  Needed poly data size
		   rep  movsd	              ;  First vertex again

		   popad
		   retn
Prepare_Poly_Data  endp

Calc_Poly_Data_Tex  macro
		   lea  esi,Poly_Data		  ; esi - vertex 1
		   mov  ebp,esi
		   add  ebp,SIZE POLY_VERTEX_DATA ; ebp - vertex 2
		   mov  edi,ebp
		   add  edi,SIZE POLY_VERTEX_DATA ; edi - vertex 3
Do_Fix3             
        	   fild  [ebp].P_V_SY
        	   fisub [esi].P_V_SY

        	   fild  [edi].P_V_SY
        	   fisub [esi].P_V_SY
		   fdivp st(1),st	

		   fld   TRI_FIX_P_COEF_XY_
        	   fild  [ebp].P_V_SX
        	   fild  [edi].P_V_SX
        	   fisub [esi].P_V_SX
		   fmul  st,st(3)
        	   fiadd [esi].P_V_SX
		   fsubp st(1),st
		   fdivp st(1),st
		   	
        	   Calc_Line_DParam1 U
        	   Calc_Line_DParam1 V

		   fstp  st	
		   fstp  st	

;		   retn
;Calc_Poly_Data_Tex endp
		   endm

Edge_Right_Scan    proc
		   xchg esi,ebp
		   mov  eax,[ebp].P_V_SY
		   Ceil eax
		   sar  eax,TRI_FIX_P_COEF_XY
		   mov  ebx,[esi].P_V_SY
		   Ceil ebx
		   mov  edx,ebx
		   sar  ebx,TRI_FIX_P_COEF_XY
		   sub  eax,ebx
		   jz   @@Skip_Edge
		   mov  Poly_Edge_Y_Height,eax
		   mov  Poly_Edge_Start_Y,ebx

		   mov  edi,ebx
		   shl  edi,2

                   and  edx,0FFFF0000h
		   sub  edx,[esi].P_V_SY
		   mov  Poly_Edge_Ex_Y,edx

		   mov  ecx,[ebp].P_V_SY
		   sub  ecx,[esi].P_V_SY
IFDEF DO_FIX1
                   jnz  @@Ok2
		   mov  ecx,10000h
@@Ok2:
ENDIF
IFDEF DO_FIX2_
		   cmp  ecx,10000h
                   jge  @@No_Overflow2
		   Do_Fix_Calc_X
                   jmp  @@Skip2
@@No_Overflow2:
ENDIF
		   mov  eax,[ebp].P_V_SX
		   mov  ebx,[esi].P_V_SX
		   sub  eax,ebx	
		   mov  Poly_Edge_Start_X,ebx
		   cdq
		   shld edx,eax,TRI_FIX_P_COEF_XY
		   shl  eax,TRI_FIX_P_COEF_XY
		   idiv ecx 
		   mov  Poly_Edge_DX,eax
@@Skip2:
		   mov  ecx,Poly_Edge_Y_Height
		   mov  esi,Poly_Edge_Start_X
IFDEF SUBPIXEL_
		   Do_Subpixel_Correct Poly_Edge_DX,esi
ENDIF

@@Right_Scan:
		   mov  eax,esi
		   sar  eax,TRI_FIX_P_COEF_XY
		   mov  Right_Scanline_X[edi],eax

		   add  esi,Poly_Edge_DX
		   add  edi,4

		   dec  ecx
		   jnz  @@Right_Scan
@@Skip_Edge:
		   retn
Edge_Right_Scan    endp


Scan_Edge_Tex      macro
		   pushad

		   mov  eax,[ebp].P_V_SY
		   cmp  eax,[esi].P_V_SY
		   jz   @@Skip_Edge
		   jl   @@Right_Scan_

;		   mov  eax,[ebp].P_V_SY
		   Ceil eax
		   sar  eax,TRI_FIX_P_COEF_XY
		   mov  ebx,[esi].P_V_SY
		   Ceil ebx
		   mov  edx,ebx
		   sar  ebx,TRI_FIX_P_COEF_XY
		   sub  eax,ebx
		   jz   @@Skip_Edge
		   mov  Poly_Edge_Y_Height,eax
		   mov  Poly_Edge_Start_Y,ebx

		   mov  edi,ebx
		   shl  edi,2

                   and  edx,0FFFF0000h
		   sub  edx,[esi].P_V_SY
		   mov  Poly_Edge_Ex_Y,edx

		   mov  ecx,[ebp].P_V_SY
		   sub  ecx,[esi].P_V_SY

IFDEF DO_FIX1
                   jnz  @@Ok1
		   mov  ecx,10000h
@@Ok1:
ENDIF
IFDEF DO_FIX2
		   cmp  ecx,10000h
                   jge  @@No_Overflow
		   Do_Fix_Calc_Tex
                   jmp  @@Skip
@@No_Overflow:
ENDIF
		   Calc_DParam1  SX,X
		   Calc_DParam1  U,U
		   Calc_DParam1  V,V

@@Skip:
		   mov  ecx,Poly_Edge_Y_Height
		   mov  ebx,Poly_Edge_Start_U
		   mov  ebp,Poly_Edge_Start_V
		   mov  esi,Poly_Edge_Start_X
IFDEF SUBPIXEL_
		   Do_Subpixel_Correct Poly_Edge_DU,ebx
		   Do_Subpixel_Correct Poly_Edge_DV,ebp
		   Do_Subpixel_Correct Poly_Edge_DX,esi
ENDIF

@@Left_Scan:       
		   mov  eax,esi
		   sar  eax,TRI_FIX_P_COEF_XY
		   mov  Left_Scanline_X[edi],eax
IFDEF SUBTEXEL_
		   shl  eax,TRI_FIX_P_COEF_XY
		   sub  eax,esi		   
push eax
		   imul Poly_Line_DU
		   shrd eax,edx,TRI_FIX_P_COEF_XY
		   add  eax,ebx
		   mov  Left_Scanline_U[edi],eax
pop eax
		   imul Poly_Line_DV
		   shrd eax,edx,TRI_FIX_P_COEF_XY
		   add  eax,ebp
		   mov  Left_Scanline_V[edi],eax
ELSEIF
		   mov  Left_Scanline_U[edi],ebx
		   mov  Left_Scanline_V[edi],ebp
ENDIF
		   add  esi,Poly_Edge_DX
		   add  ebx,Poly_Edge_DU
		   add  ebp,Poly_Edge_DV

		   add  edi,4
		   dec  ecx
		   jnz  @@Left_Scan
		   jmp  @@ex2
@@Right_Scan_:
		   call Edge_Right_Scan
@@Skip_Edge:
@@ex2:
		   popad
;		   retn
;Scan_Edge_Tex      endp
		    endm

Calc_Scanlines_Tex macro
		   lea  esi,Poly_Data
		   mov  ebp,esi
		   add  ebp,SIZE POLY_VERTEX_DATA ; ebp - next vertex

		   mov  ecx,Poly_V_Num
@@Vert_Cyc:
		   Scan_Edge_Tex

		   add  esi,SIZE POLY_VERTEX_DATA
		   add  ebp,SIZE POLY_VERTEX_DATA

		   dec  ecx
		   jnz  @@Vert_Cyc
;		   retn
;Calc_Scanlines_Tex endp
		   endm


Find_Max_Min_Y     proc

		   lea  esi,Poly_Data
		   mov  eax,[esi].P_V_SY
		   mov  edx,eax

		   mov  ecx,Poly_V_Num
		   dec  ecx	
@@Vert_Loop:
		   add  esi,SIZE POLY_VERTEX_DATA ;  Next vertex

		   cmp  edx,[esi].P_V_SY
		   jge  @@Not_Max
		   mov  edx,[esi].P_V_SY
@@Not_Max:
		   cmp  eax,[esi].P_V_SY
		   jle  @@Not_Min
		   mov  eax,[esi].P_V_SY
@@Not_Min:
		   dec  ecx	
		   jnz  @@Vert_Loop

		   retn		
Find_Max_Min_Y     endp

Draw_Tex_Scanline  macro
		   local @@Not_Draw
		   pushad	

		   mov  edx,Left_Scanline_X[eax]
		   mov  ecx,Right_Scanline_X[eax]
                   sub  ecx,edx
		   jz   @@Not_Draw 
js   @@Not_Draw
		   add  edi,edx

		   mov  ebp,Left_Scanline_U[eax]
		   mov  ebx,Left_Scanline_V[eax]

		   mov  esi,12345678h
Poly_Texture1      equ  $-4

		   shl  edi,2
@@Draw_Scanline:
IFNDEF DO_BILIN_INTERPOL
;		   mov  eax,ebp 
		   mov  edx,ebx
		   shld  eax,ebp,TRI_FIX_P_COEF_UV
		   shr   edx,TRI_FIX_P_COEF_UV
;		   shr  eax,TRI_FIX_P_COEF_UV
;		   shr  edx,TRI_FIX_P_COEF_UV
		   mov  dh,al
		   add  ebp,Poly_Line_DU
		   mov  eax,[esi+edx*4]
ELSE
		   Bilin_Interp
		   add  ebp,Poly_Line_DU
ENDIF
		   add  ebx,Poly_Line_DV


Poly_Virt_Screen   equ  $+2
		   mov  dword ptr Virt_Screen_1[edi],eax

		   add  edi,4
		   dec  ecx
		   jnz  @@Draw_Scanline
@@Not_Draw:
		   popad
;		   retn		
;@@Not_Draw_:
;		   int 3
;		   popad
;		   retn		

;Draw_Tex_Scanline endp
		   endm


Draw_Poly_Tex_     macro
		   call Find_Max_Min_Y

		   Ceil eax
		   sar  eax,TRI_FIX_P_COEF_XY
		   Ceil edx
		   sar  edx,TRI_FIX_P_COEF_XY

		   cmp  eax,edx
		   jz   @@Not_Draw

		   mov  edi,eax
		   imul edi,HOR_RAZR

		   shl  eax,2
		   shl  edx,2
@@Draw_Poly:
		   Draw_Tex_Scanline	
		   add  eax,4
		   add  edi,HOR_RAZR
		   cmp  eax,edx 	
		   jnz  @@Draw_Poly
		  
@@Not_Draw:
;		   retn
;Draw_Poly_Tex_     endp
		   endm



Calc_Poly_Data_Phong     macro 

		   lea   esi,Poly_Data		   ; esi - vertex 1
		   mov   ebp,esi
		   add   ebp,SIZE POLY_VERTEX_DATA ; ebp - vertex 2
		   mov   edi,ebp
		   add   edi,SIZE POLY_VERTEX_DATA ; edi - vertex 3
Do_Fix3
        	   fild  [ebp].P_V_SY
        	   fisub [esi].P_V_SY
        	   fild  [edi].P_V_SY
        	   fisub [esi].P_V_SY
		   fdivp st(1),st	

		   fld   TRI_FIX_P_COEF_XY_
        	   fild  [ebp].P_V_SX
        	   fild  [edi].P_V_SX
        	   fisub [esi].P_V_SX
		   fmul  st,st(3)
        	   fiadd [esi].P_V_SX
		   fsubp st(1),st
		   fdivp st(1),st

        	   Calc_Line_DParam1 U
        	   Calc_Line_DParam1 V
        	   Calc_Line_DParam1 FU
        	   Calc_Line_DParam1 FV

		   fstp  st	
		   fstp  st	

;		   retn
;Calc_Poly_Data_Phong endp
		   endm



Scan_Edge_Phong    macro
		   pushad

		   mov  eax,[ebp].P_V_SY
		   cmp  eax,[esi].P_V_SY
		   jz   @@Skip_Edge
		   jl   @@Right_Scan_

;		   mov  eax,[ebp].P_V_SY
		   Ceil eax
		   sar  eax,TRI_FIX_P_COEF_XY
		   mov  ebx,[esi].P_V_SY
		   Ceil ebx
		   mov  edx,ebx
		   sar  ebx,TRI_FIX_P_COEF_XY
		   sub  eax,ebx
		   jz   @@Skip_Edge
		   mov  Poly_Edge_Y_Height,eax
		   mov  Poly_Edge_Start_Y,ebx

		   mov  edi,ebx
		   shl  edi,2

                   and  edx,0FFFF0000h
		   sub  edx,[esi].P_V_SY
		   mov  Poly_Edge_Ex_Y,edx

		   mov  ecx,[ebp].P_V_SY
		   sub  ecx,[esi].P_V_SY

IFDEF DO_FIX1
                   jnz  @@Ok1
		   mov  ecx,10000h
@@Ok1:
ENDIF
IFDEF DO_FIX2
		   cmp  ecx,10000h
                   jge  @@No_Overflow
		   Do_Fix_Calc_Phong
                   jmp  @@Skip
@@No_Overflow:
ENDIF
		   Calc_DParam1 SX,X
		   Calc_DParam1 U,U
		   Calc_DParam1 V,V
		   Calc_DParam1 FU,FU
		   Calc_DParam1 FV,FV
@@Skip:

		   mov  ecx,Poly_Edge_Y_Height
		   mov  ebx,Poly_Edge_Start_U
		   mov  ebp,Poly_Edge_Start_V
		   mov  esi,Poly_Edge_Start_X
IFDEF SUBPIXEL_
		   Do_Subpixel_Correct Poly_Edge_DU,ebx
		   Do_Subpixel_Correct Poly_Edge_DV,ebp
		   Do_Subpixel_Correct Poly_Edge_DX,esi
		   Do_Subpixel_Correct Poly_Edge_DFU,Poly_Edge_Start_FU
		   Do_Subpixel_Correct Poly_Edge_DFV,Poly_Edge_Start_FV

ENDIF

@@Left_Scan:       
		   mov  eax,esi
		   sar  eax,TRI_FIX_P_COEF_XY
		   mov  Left_Scanline_X[edi],eax
IFDEF SUBTEXEL_
push ecx
		   shl  eax,TRI_FIX_P_COEF_XY
		   sub  eax,esi
mov  ecx,eax
		   imul Poly_Line_DU
		   shrd eax,edx,TRI_FIX_P_COEF_XY
		   add  eax,ebx
		   mov  Left_Scanline_U[edi],eax
mov eax,ecx
		   imul Poly_Line_DV
		   shrd eax,edx,TRI_FIX_P_COEF_XY
		   add  eax,ebp
		   mov  Left_Scanline_V[edi],eax
mov eax,ecx
		   imul Poly_Line_DFU
		   shrd eax,edx,TRI_FIX_P_COEF_XY
		   add  eax,Poly_Edge_Start_FU
		   mov  Left_Scanline_FU[edi],eax
mov eax,ecx
		   imul Poly_Line_DFV
		   shrd eax,edx,TRI_FIX_P_COEF_XY
		   add  eax,Poly_Edge_Start_FV
		   mov  Left_Scanline_FV[edi],eax
pop ecx
ELSEIF
		   mov  Left_Scanline_U[edi],ebx
		   mov  Left_Scanline_V[edi],ebp
		   mov  eax,Poly_Edge_Start_FU
		   mov  Left_Scanline_FU[edi],eax
		   mov  eax,Poly_Edge_Start_FV
		   mov  Left_Scanline_FV[edi],eax
ENDIF
		   add  esi,Poly_Edge_DX
		   add  ebx,Poly_Edge_DU
		   add  ebp,Poly_Edge_DV

		   mov  eax,Poly_Edge_DFU
		   add  Poly_Edge_Start_FU,eax
		   mov  eax,Poly_Edge_DFV
		   add  Poly_Edge_Start_FV,eax

		   add  edi,4
		   dec  ecx
		   jnz  @@Left_Scan
		   jmp  @@ex2
@@Right_Scan_:
		   call Edge_Right_Scan
@@Skip_Edge:
@@ex2:
		   popad
;		   retn
;Scan_Edge_Phong      endp
		   endm

Calc_Scanlines_Phong macro
		   lea  esi,Poly_Data
		   mov  ebp,esi
		   add  ebp,SIZE POLY_VERTEX_DATA ; ebp - next vertex

		   mov  ecx,Poly_V_Num
@@Vert_Cyc:
		   Scan_Edge_Phong

		   add  esi,SIZE POLY_VERTEX_DATA
		   add  ebp,SIZE POLY_VERTEX_DATA

		   dec  ecx
		   jnz  @@Vert_Cyc
;		   retn
;Calc_Scanlines_Phong endp
		   endm

Draw_Phong_Scanline  macro
		   local @@Not_Draw
		   pushad	

		   mov  edx,Left_Scanline_X[eax]
		   mov  ecx,Right_Scanline_X[eax]
                   sub  ecx,edx
		   jz   @@Not_Draw 
js   @@Not_Draw
		   add  edi,edx

		   mov  ebp,Left_Scanline_U[eax]
		   mov  ebx,Left_Scanline_V[eax]
		   shl  edi,2
		   mov  edx,Left_Scanline_FV[eax]
		   mov  esi,Left_Scanline_FU[eax]
@@Draw_Scanline:
;		   mov  Scanline_FV,edx
		   push edx
		   mov  eax,esi
		   shr  edx,TRI_FIX_P_COEF_UV
		   shr  eax,TRI_FIX_P_COEF_UV
		   mov  dh,al
		   ror  ebx,TRI_FIX_P_COEF_UV
		   mov  edx,dword ptr Texture2[edx*4]

		   mov  eax,ebp 
and edx,0FEFEFEFEh
		   shr  eax,TRI_FIX_P_COEF_UV
		   add  ebp,Poly_Line_DU
		   mov  ah,bl
Poly_Texture2      equ  $+3
		   mov  eax,dword ptr Texture[eax*4]

		   rol  ebx,TRI_FIX_P_COEF_UV
and eax,0FEFEFEFEh
		   add  esi,Poly_Line_DFU
add eax,edx
		   add  ebx,Poly_Line_DV
shr eax,1

                   pop  edx
;		   mov  edx,Scanline_FV

Poly_Virt_Screen2  equ  $+2
		   mov  dword ptr Virt_Screen_1[edi],eax

		   add  edx,Poly_Line_DFV

		   add  edi,4
		   dec  ecx
		   jnz  @@Draw_Scanline
@@Not_Draw:
		   popad
;		   retn		
;@@Not_Draw_:
;		   int 3
;		   popad
;		   retn		

;Draw_Phong_Scanline endp
		   endm


Draw_Poly_Phong_   macro
		   call Find_Max_Min_Y

		   Ceil eax
		   sar  eax,TRI_FIX_P_COEF_XY
		   Ceil edx
		   sar  edx,TRI_FIX_P_COEF_XY

		   cmp  eax,edx
		   jz   @@Not_Draw

		   mov  edi,eax
		   imul edi,HOR_RAZR

		   shl  eax,2
		   shl  edx,2
@@Draw_Poly:
		   Draw_Phong_Scanline	
		   add  eax,4
		   add  edi,HOR_RAZR
		   cmp  eax,edx 	
		   jnz  @@Draw_Poly
		  
@@Not_Draw:
;		    retn
;Draw_Poly_Phong_     endp
		   endm

.DATA?
Scanline_FV       dd      ?
