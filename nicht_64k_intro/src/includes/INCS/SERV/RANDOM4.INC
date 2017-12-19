CONST1      equ      98212323h

.CODE
random proc
; IN  : AX - Random range
; OUT : AX - Random
        push   ebx
        push   edx

        mov    ebx,eax
        mov    eax,0A1232134h
seed equ $-4
        mov    edx,CONST1
        imul   edx
	inc    eax
        mov    dword ptr seed,eax
        mul    ebx
        xchg   eax,edx

        pop  edx
        pop  ebx
        ret
random endp

randomize proc near
        mov    ah,2ch
        int    21h
        mov    ax,cx
        shl    ax,16
        mov    ax,dx
        mov    dword ptr seed,eax
	retn
endp
