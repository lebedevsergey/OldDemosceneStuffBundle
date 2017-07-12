INV_COEF_	equ 5875/2+100

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


Fade_White_Virt_Screen   macro
                pushad

                mov  ecx,(HOR_RAZR*VERT_RAZR*4/4)*4
@@A_V_S2:
                mov al,byte ptr [edi]
                add al,[ebp].Alpha
                jnc @@A_V_Lb2
                mov al,0FFh
@@A_V_Lb2:

		stosb
;                dec  ecx
		loop  @@A_V_S2

                popad

;		retn
		endm


CONST1      equ      98212323h

.CODE
random macro
; IN  : AX - Random range
; OUT : AX - Random
        push   edx

        xchg   Seed,eax
        imul   eax,CONST1
	inc    eax
        xchg   Seed,eax
	mul seed
        xchg   eax,edx

        pop  edx
endm



Invert_Virt_Screen   macro
.code
                pushad

fild  inv_coef
fild dword ptr _TIME
	fprem
fistp [ebp].tmp4
fstp st

cmp [ebp].tmp4,INV_COEF_/2
jb @@No3

                mov  ecx,HOR_RAZR*VERT_RAZR*4
@@A_V_S223:
not byte ptr [edi]
inc edi
		loop  @@A_V_S223

@@No3:
popad

;		retn
		endm