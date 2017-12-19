.CODE
Alloc_Mem       proc near

; IN - EAX : Mem size
; OUT - EAX :  mem addr

                push    ecx
                push    ebx
                push    esi
                push    edi
                mov     ebx, eax
                shr     ebx, 10h
                mov     ecx, eax
                mov     eax, 501h
                int     31h             ; DPMI Services   ax=func xxxxh
                                        ; ALLOCATE MEMORY BLOCK
                                        ; BX:CX = size in bytes
                                        ; Return: CF set on error
                                        ; CF clear if successful
                jc      @@Mem_Err
                shl     ebx, 10h
                mov     bx, cx
                mov     eax, ebx
                shl     esi, 10h
                mov     si, di
                mov     edx, esi
                pop     edi
                pop     esi
                pop     ebx
                pop     ecx
                retn

@@Mem_Err:
                lea edx,@@Mem_Err_Msg
                jmp Exec_Err

@@Mem_Err_Msg   db  'No mem$'

endp

Exec_Err:
                Textmode
                mov ah,9
                int 21h
                Quittodos

Wrap_Max macro value,max
local tt1
cmp value,max
jng tt1
sub value,max
tt1:
endm

Wrap_Min macro value,min,max
local tt1
cmp value,min
jnl tt1
add value,max
tt1:
endm

Fix_Max macro value,max
local tt1
cmp value,max
jng tt1
mov value,max
tt1:
endm

Fix_Min macro value,min
        local tt1
        cmp value,min
        jnl tt1
        mov value,min
tt1:
endm

Arccos  proc near

;               mov Tmp1,1

                ftst
                fstsw ax
                sahf
                jz @@Zero_Arg
;               jnc @@Pos_Arg
;               mov Tmp1,0
;@@Pos_Arg:

                fld1
                fld st(1)
                fmul st,st
                fsubp st(1),st
                fsqrt
                fxch st(1)
                fpatan

;               fldpi
;               fimul Tmp1
;               faddp st(1),st

                retn
@@Zero_Arg:
                fstp st
                fld PI_DIV_2
                retn
endp

PI_DIV_2        dd      1.570796326795     ;  PI/2

Fexp   macro
        call FPU_Exp
endm

FPU_Exp  proc near
        fld1
        fldl2e
        fmul st,st(2)
        fst st(2)
        fprem
        f2xm1
        faddp st(1),st
        fscale
        fstp st(1)
        retn
endp


FPU_Compare proc near
       mov @@Comp_Var,eax
       fld st
       ficomp @@Comp_Var
       fstsw ax
       sahf

      retn
.DATA?
@@Comp_Var  dd  ?
.CODE
endp

FPow  macro
   call FPU_Pow
endm

FPU_Pow proc near
fabs
        fyl2x
        fld1
        fld st(1)
        fprem
        f2xm1
        faddp st(1),st
        fscale
        fstp st(1)
        retn
endp

FTrunc  macro
   call FPU_Trunc
endm


FPU_Trunc          proc near

                fstcw   Tmp1
                push    Tmp1
;               and     Tmp1,0F3FFh
                or     Tmp1,0000110000000000b ;0FFFFh
                fldcw   Tmp1       ; ^
                frndint
                pop    Tmp1
                fldcw   Tmp1

                retn
endp
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

Line_Param      proc
; OUT :
;      ECX - PARAM

        fld   Time2
        fsub  Base_Time1
        fdivrp st(1),st

        retn
        endp

Sin_Param      proc
; OUT :
;      ECX - PARAM

        fld   Time2
        fsub  Base_Time1
        fdivrp   st(1),st
	fsin
	fmulp st(1),st

        retn
        endp
