
;SAVE_TEX equ 1

.CODE
locals

Do_Texture    proc near
                pushad

        mov    eax,0A1232134h
        mov    dword ptr seed,eax


        push  edi
        lea  edi,Tmp_Text1
        xor  eax,eax
        mov  ecx,TEX_SIZE*TEX_SIZE
        rep  stosd
                                     ; Чистим место для текстуры

        lea  edi,Tmp_Text2
        mov  ecx,TEX_SIZE*TEX_SIZE
        rep  stosd

@@tt1:
                lea  edi,Tmp_Text1

call debug_l

@@com_cycle:
                lea  edi,Tmp_Text1

                xor  eax,eax
                lodsb           ; Берем команду
                cmp  al,END_COM
                jz   @@exit
                xchg eax,ebx

                xor  eax,eax
                lodsb           ; Берем операнд
                shl  eax,16+2
                add  edi,eax
                call dword ptr [Com_Table+ebx*4]

                jmp  @@com_cycle

@@exit:

                pop  edi
                push edi

                xor  eax,eax
                lodsb           ; Берем операнд
                shl  eax,16+2

                lea  esi,Tmp_Text1

                add  esi,eax

                mov  ecx,TEX_SIZE*TEX_SIZE
                rep  movsd

                pop edx

IFDEF SAVE_TEX
                call Write_Texture
ENDIF

                popad
                retn
Do_Texture2:
                pushad

                push edi
                jmp @@tt1

endp


IFDEF SAVE_TEX

Write_Texture    proc near
                pushad

                mov esi,edx
                lea edi,Tex_Buf2
                mov  ecx,TEX_SIZE*TEX_SIZE
@@ll1:
                lodsw
                stosw
                lodsw
                stosb
                loop @@ll1

                lea edx,Tex_File
                mov ah,3ch
                int 21h

                xchg ax,bx

                lea edx,Tex_Buf
                mov ecx,Tex_Len
                mov ah,40h
                int 21h

                mov ah,3eh
                int 21h


                popad
                retn
endp

.DATA
Tex_File db 'tex1.tga'

Tex_Buf   db 0,0,2,0,0,0,0,0,0,0,0,0,0,1,0,1,18h,20h
Tex_Buf2:
          db TEX_SIZE*TEX_SIZE*3 dup (?)
Tex_Len equ $-Tex_Buf

ENDIF

.DATA?
db 200h dup (?)
Tmp_Text1 dd TEX_SIZE*TEX_SIZE dup (?)
db 200h dup (?)
Tmp_Text2 dd TEX_SIZE*TEX_SIZE dup (?)
db 200h dup (?)