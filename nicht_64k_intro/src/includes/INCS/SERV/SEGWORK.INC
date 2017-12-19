.CODE
locals

Alpha_Byte  macro
                lodsb
                mov bl,Fade_Tab2[eax]

                mov dl,[edi]
                mov bh,Fade_Tab2[edx]

                mov al,Fade_Tab1[ebx]
                stosb
endm

Fade_Byte  macro
                lodsb
                mov al,Fade_Tab2[eax]
                stosb
endm

Fade_White_Byte  macro
                lodsb
                mov al,Fade_Tab1[eax]
                stosb
endm


Anti_Alpha_Byte  macro
                lodsb
                mov bh,Fade_Tab2[eax]

                mov dl,[edi]
                mov bl,Fade_Tab2[edx]

                mov al,Fade_Tab2[ebx]
                stosb
endm

Calc_Fade_Tab     Proc   Near
                pushad

                mov  ecx,256*256
@@A_V_S2:
                mov ax,cx
                add al,ah
                jnc @@A_V_Lb2
                mov al,0FFh
@@A_V_Lb2:
                mov Fade_Tab1[ecx],al

                mov ax,cx
                sub al,ah
                jnc @@A_V_Lb3
                mov al,0
@@A_V_Lb3:
                mov Fade_Tab2[ecx],al

                dec  ecx
                jns  @@A_V_S2

                popad

                retn
endp

.DATA?
db 1000 dup (?)
Fade_Tab1  db 256*256 dup (?)
Fade_Tab2  db 256*256 dup (?)
db 2000 dup (?)


.CODE

Copy_Scr        proc near
                pushad
                mov  ecx,HOR_RAZR*VERT_RAZR*4/4
                rep  movsd
                popad
                retn
Copy_Scr        endp


Clear_Scr       Proc   Near
                pushad
                mov  ecx,HOR_RAZR*VERT_RAZR;/4
                xor  eax,eax
                rep  stosd
                popad
                retn
Clear_Scr       endp

Alpha_Virt_Screen     Proc   Near
                pushad

                mov  ecx,(HOR_RAZR*VERT_RAZR*4/4)

                xor eax,eax
                xor edx,edx
                xor ebx,ebx

                mov ah,Alpha2
                mov dh,Alpha1
@@A_V_S2:

                Alpha_Byte
                Alpha_Byte
                Alpha_Byte


                inc esi
                inc edi

                dec  ecx
                jnz  @@A_V_S2

                popad

                retn
endp

Anti_Alpha_Virt_Screen     Proc   Near
                pushad

                mov  ecx,(HOR_RAZR*VERT_RAZR*4/4)

                xor eax,eax
                xor edx,edx
                xor ebx,ebx

                mov ah,Alpha2
                mov dh,Alpha1
@@A_V_S2:

                Anti_Alpha_Byte
                Anti_Alpha_Byte
                Anti_Alpha_Byte


                inc esi
                inc edi

                dec  ecx
                jnz  @@A_V_S2

                popad

                retn
endp



Apply_Virt_Screen     Proc   Near
                pushad

                mov  ecx,(HOR_RAZR*VERT_RAZR*4/4)
@@A_V_S2:
                lodsd
                cmp  eax,NULL
                jz @@A_V_Lb1
                mov dword ptr [edi],eax
@@A_V_Lb1:
                add  edi,4
                dec  ecx
                jns  @@A_V_S2

                popad

                retn

endp


Fade_Virt_Screen     Proc   Near
                pushad
                mov esi,edi

                mov  ecx,(HOR_RAZR*VERT_RAZR*4/4)

                xor eax,eax
                xor edx,edx
                xor ebx,ebx

                mov ah,Alpha1
@@A_V_S2:

                Fade_Byte
                Fade_Byte
                Fade_Byte

                inc esi
                inc edi

                dec  ecx
                jnz  @@A_V_S2

                popad

                retn
endp

Fade_White_Virt_Screen     Proc   Near
                pushad

                mov  esi,edi
                mov  ecx,(HOR_RAZR*VERT_RAZR*4/4)

                xor eax,eax
                xor edx,edx
                xor ebx,ebx

                mov ah,Alpha1
@@A_V_S2:

                Fade_White_Byte
                Fade_White_Byte
                Fade_White_Byte

                inc esi
                inc edi

                dec  ecx
                jnz  @@A_V_S2

                popad

                retn
endp

.DATA?
Alpha1 db ?
Alpha2 db ?

