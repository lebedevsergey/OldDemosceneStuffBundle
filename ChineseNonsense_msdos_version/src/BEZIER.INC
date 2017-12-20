TVERTEX struc
  X   dw   ?           ;  X,Y,Z вершины
  Y   dw   ?
ENDS

;SIN_WIDTH  equ 1

LINE_COL_    equ 0Fh

SYM_WIDTH	equ	10;10
SYM_HEIGHT	equ	10;15
SYM_X_INTERV	equ	0;6
SYM_Y_INTERV	equ	0;5

CONTROL_POINTS_NUM   equ  4
Bezier_Point_Num_    equ  1400

Pow3     macro
                 fld  st
                 fmul st,st
                 fmulp st(1),st
endm    

.code
Draw_Bezier  proc
; fpu - u param
                 pusha

                 fst  @@U
                 fld1 
                 fsub st,st(1)
		 fst  @@1_U
		 Pow3
                 fstp Bezier_Coefs[0*4]
		 Pow3
                 fstp Bezier_Coefs[3*4]

		 fld  @@1_U
                 fmul st,st
                 fimul @@__3
                 fmul @@U
                 fstp Bezier_Coefs[1*4]

		 fld  @@U
                 fmul st,st
                 fmul @@1_U
                 fimul @@__3
                 fstp Bezier_Coefs[2*4]
		 
; (1-U)^3  3U(1-U)^2 3U^2(1-U) U^3
; 3(1-U)^2 6U(1-U)   6U-9U^2   3U^2
;                    3U(2-3U)

                 fldz
                 fldz

                 lea  si,Control_Points
                 lea  bp,Bezier_Coefs
                 mov  cx,CONTROL_POINTS_NUM
@@Add_Loop:
                 fild  [si].X
                 fmul dword ptr [bp]
                 faddp st(1),st
                 fxch  st(1)
                 fild  [si].Y
                 fmul dword ptr [bp]
                 faddp st(1),st
                 fxch  st(1)

                 add  bp,4
		 add  si,4
                 loop  @@Add_Loop

                 fistp X_L_1
                 fistp Y_L_1

  mov bx,Y_L_1
add bx,Cur_Sym_Y
  imul bx,HOR_RAZR
  add bx,X_L_1
add bx,Cur_Sym_X
  mov byte ptr es:[bx],LINE_COL_

                 popa
                 retn
.DATA
@@__3            dw    3
.DATA?
@@U              dd    ?
@@1_U            dd    ?
.CODE
		 endp


.DATA
Bezier_Point_Num  dw  Bezier_Point_Num_


.DATA?
Control_Points:       db SIZE TVERTEX*CONTROL_POINTS_NUM dup(?)
Bezier_Coefs          dd CONTROL_POINTS_NUM dup(?) 

X_L_1   dw ?
X_L_2   dw ?
Y_L_1   dw ?
Y_L_2   dw ?