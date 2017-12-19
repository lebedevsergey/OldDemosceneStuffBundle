.CODE
locals
Alpha_Texture     Proc   Near
                pushad

                xor   eax,eax
                lodsb
                shl  eax,16+2
                xchg ebx,eax

                lodsb
                mov  @@Alpha1,al
                lodsb
                mov  @@Alpha2,al

                lea  esi,Tmp_Text1
                add  esi,ebx


                mov  ecx,TEX_SIZE*TEX_SIZE*4
@@A_V_S2:
                lodsb
                sub al,@@Alpha2
                jnc @@A_V_Lb2
                mov al,0
@@A_V_Lb2:

                mov ah,byte ptr [edi]
                sub ah,@@Alpha1
                jnc @@A_V_Lb3
                mov ah,0
@@A_V_Lb3:

                add al,ah
                jnc @@A_V_Lb1
                mov al,0FFh
@@A_V_Lb1:
                stosb
                dec  ecx
                jns  @@A_V_S2

                popad

                inc  esi
                inc  esi
                inc  esi

                retn
.DATA?
@@Alpha1      db     ?
@@Alpha2      db     ?
.CODE
                endp

Do_Color           Proc   Near

                pushad

                xor  eax,eax
                lodsb
                mov  Tmp1,ax
                fild Tmp1
                fidiv Percent
                fstp  R_Coef

                lodsb
                mov  Tmp1,ax
                fild Tmp1
                fidiv Percent
                fstp  G_Coef

                lodsb
                mov  Tmp1,ax
                fild Tmp1
                fidiv Percent
                fstp  B_Coef


                xor  ecx,ecx
                dec  cx

                mov  esi,edi
@@Loop_X2:
                xor  ax,ax
                lodsb
                mov  Tmp1,ax
                fild Tmp1
                fmul  B_Coef
                fistp Tmp1
                mov   ax,Tmp1
                stosb

                xor  ax,ax
                lodsb
                mov  Tmp1,ax
                fild Tmp1
                fmul  G_Coef
                fistp Tmp1
                mov   ax,Tmp1
                stosb

                xor  ax,ax
                lodsb
                mov  Tmp1,ax
                fild Tmp1
                fmul  R_Coef
                fistp Tmp1
                mov   ax,Tmp1
                stosb

                lodsb
                stosb

                loop @@Loop_X2

                popad

                inc  esi
                inc  esi
                inc  esi
                retn
endp

.CODE
locals


Do_tlo proc near
        pushad

        lodsw

        mov bp,ax       ;Sqr_Num

        xor eax,eax
        lodsb
        mov XSize,al

        mov edx,TEX_SIZE
        sub edx,eax
        shl edx,2

        lodsb
        mov YSize,al

        lodsb           ; Adder
        mov bl,al


 @@l3:

        mov  eax,TEX_SIZE*TEX_SIZE*4
        call Random
        add eax,100h

        mov ch,Ysize
@@l2:
        mov cl,XSize
@@l1:
        add byte ptr [edi+eax],bl
        inc ax
        add byte ptr [edi+eax],bl
        inc ax
        add byte ptr [edi+eax],bl
        inc ax
        inc ax

        dec cl
        jnz @@l1

        add eax,edx
        and eax,03FFFFh

        dec ch
        jnz @@l2

        dec bp
        jnz @@l3

mov esi,edi
add esi,256*254*4
add edi,256*256*4
mov ecx,256
rep movsd

        popad
        add esi,5
        ret
        endp

Invert_Color     Proc   Near
                pushad
                mov  esi,edi
                mov  ecx,TEX_SIZE*TEX_SIZE*4
@@A_V_S2:
                lodsb
;               xor  al,0FFh
                neg  al
                stosb
                loop @@A_V_S2

                popad
                ret
                endp


Blurx256      proc    near
;==================================================
;  IN :       EDI : Segment
;==================================================

                 pushad

                push   edi
                mov   esi,edi
                lea   edi,Tmp_Texture
                mov  ecx,TEX_SIZE*TEX_SIZE
                rep   movsd

                 pop  ebp
                 push ebp
                lea   edi,Tmp_Texture

                 mov  Count,TEX_SIZE*TEX_SIZE*4
                 mov  ecx,TEX_SIZE*4
                 xor  bx,bx
                 xor  esi,esi

@@B_S_Loop1:
                 xor  ax,ax
                 mov  al,[ebp+esi-4]
                 mov  bl,[ebp+esi+4]
                 add  ax,bx

                 mov  edx,esi
                 sub  edx,ecx
                 Wrap_Min  edx,0,TEX_SIZE*TEX_SIZE*4

                 mov  bl,[ebp+edx]
                 add  ax,bx
                 mov  bl,[ebp+edx-4]
                 add  ax,bx
                 mov  bl,[ebp+edx+4]
                 add  ax,bx

                 mov  edx,esi
                 add  edx,ecx
                 Wrap_Max  edx,TEX_SIZE*TEX_SIZE*4

                 mov  bl,[ebp+edx]
                 add  ax,bx
                 mov  bl,[ebp+edx-4]
                 add  ax,bx
                 mov  bl,[ebp+edx+4]
                 add  ax,bx

                 shr  ax,3

                 mov  [edi+esi],al
                 inc  esi
                 dec Count
                 jnz @@B_S_Loop1

                pop   edi
                lea   esi,Tmp_Texture
                mov  ecx,TEX_SIZE*TEX_SIZE
                rep   movsd

                 popad
                 retn
                endp

Clear_Tex    proc near
pushad
                mov  ecx,TEX_SIZE*TEX_SIZE
                xor  eax,eax
                rep  stosd
popad
retn
endp

include f:\dem\common\incs\texgen\wobbler.inc
include f:\dem\common\incs\texgen\twirl.inc
include f:\dem\common\incs\texgen\ripple.inc
include f:\dem\common\incs\texgen\polar.inc
include f:\dem\common\incs\texgen\fplasma.inc
include f:\dem\common\incs\texgen\desert.inc
include f:\dem\common\incs\texgen\phong.inc
include f:\demo_sb\player\incs\texgen\phong2.inc


.DATA
Percent dw 100
.DATA?
Count  dd  ?
R_Coef dd ?
G_Coef dd ?
B_Coef dd ?

XSize db ?
YSize db ?


