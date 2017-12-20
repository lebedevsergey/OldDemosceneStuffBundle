.code
CONST1      equ      19823
CONST2      equ      1

random proc
; IN  : AX - Random range
; OUT : AX - Random
        push   ax
        mov    ax,0A123h
seed equ $-2
        mov    dx,CONST1
        imul   dx
        add    ax,CONST2
ror ax,2
        mov    word ptr seed,ax
        pop dx
        mul dx
        xchg ax,dx
        ret
random endp