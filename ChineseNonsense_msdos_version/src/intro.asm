
NULL  equ 0FFFFFFFFh

MAX_CURVES_NUM	equ	10

HOR_RAZR        equ  320;320;320
VERT_RAZR       equ  200
Virt_Screen1  equ  0A000h 

.286
.Model tiny
.Code
.386

include macros.inc
include mac2.inc

org 100h
_main:

set13

push Virt_Screen1
pop es

Waitforit:

		 mov ax,MAX_CURVES_NUM+3
		 call Random
inc ax
;mov  Curves_Num,ax


		 cmp ax,MAX_CURVES_NUM+2
		je	@@No
		ja	@@No2
mov cx,ax

;mov  cx,Curves_Num
@@tt1:
push cx
;call 
Draw_Bezier_Seg
pop  cx
loop @@tt1

@@No:

add	Cur_Sym_X,SYM_WIDTH+SYM_X_INTERV
cmp 	Cur_Sym_X,HOR_RAZR
jb	@@Ok
@@No2:
mov 	Cur_Sym_X,0
add	Cur_Sym_Y,SYM_HEIGHT+SYM_Y_INTERV
@@Ok:

cmp 	Cur_Sym_Y,VERT_RAZR
jb	@@Ok2
mov 	Cur_Sym_X,0
mov 	Cur_Sym_Y,VERT_RAZR-SYM_HEIGHT

push ds
push es
pop ds
xor di,di
mov si,SYM_HEIGHT*HOR_RAZR
mov cx,HOR_RAZR*(VERT_RAZR-SYM_HEIGHT-SYM_Y_INTERV)
rep movsb
xor ax,ax
mov cx,HOR_RAZR*(SYM_HEIGHT+SYM_Y_INTERV)
rep stosb
pop ds
@@Ok2:

WaitRetrace

mov ah,1
int 16h
jz Waitforit

        Textmode

        Quittodos

	include 1.inc
Cur_Sym_X	dw	0
Cur_Sym_Y	dw	0

.DATA?
Tmp2      dd   ?
Tmp1      dw   ?
end _main