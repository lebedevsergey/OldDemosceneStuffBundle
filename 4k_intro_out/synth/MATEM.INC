CONST1      equ      98212323h
;CONST2      equ      1

_1   equ  3F800000h
_NAN  equ 808ch


.CODE
random	macro
; IN  : AX - Random range
; OUT : AX - Random
        mov    eax,[ebp].seed
        imul   eax,CONST1
	inc    eax
        mov    [ebp].seed,eax
endm

Fpow macro
        fyl2x
        fld1
        fld st(1)
        fprem
        f2xm1
        faddp st(1),st
        fscale
        fstp st(1)
endm