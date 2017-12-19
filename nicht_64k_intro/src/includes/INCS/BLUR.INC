.CODE

MOTBLUR equ 3

locals

Fast_Blurx320      proc    near
; edi - scr ptr
                 pushad

                 mov  edx,HOR_RAZR*(VERT_RAZR-1);*4
                 mov  ecx,HOR_RAZR*4

@@B_S_Loop1:
                 mov  eax,[edi-4]
                 mov  ebx,[edi+4]
                 mov  ebp,[edi-ecx]

                 and  eax,0FCFCFCFCh
                 and  ebx,0FCFCFCFCh
                 and  ebp,0FCFCFCFCh
                 add  eax,ebx
                 mov  ebx,[edi+ecx]
                 add  eax,ebp
                 and  ebx,0FCFCFCFCh
                 add  eax,ebx

                 shr  eax,2

                 stosd
                 dec edx
                 jnz @@B_S_Loop1


                 popad
                 retn
                endp


Motion_Blur     proc    near
; esi - scr2 ptr
; edi - scr1 ptr
                 pushad

                 mov  ecx,HOR_RAZR*VERT_RAZR
@@B_S_Loop1:
                 lodsd

IF MOTBLUR EQ  1
                 and  eax,0FCFCFCFCh
                 mov  ebx,[edi]
                 and  ebx,0FCFCFCFCh
                 lea  ebx,[ebx*2+ebx]
                 add  eax,ebx
                 shr  eax,2
                 stosd
ELSEIF MOTBLUR EQ  2
                 and  eax,0F8F8F8F8h
                 mov  ebx,[edi]
                 and  ebx,0F8F8F8F8h
                 lea  ebx,[ebx*8+ebx]
                 add  eax,ebx
                 shr  eax,3
                 stosd
ELSEIF MOTBLUR EQ  3
                 and  eax,0FEFEFEFEh
                 mov  ebx,[edi]
                 and  ebx,0FEFEFEFEh
                 add  eax,ebx
                 shr  eax,1
                 stosd

ELSEIF MOTBLUR EQ  4
                 and  eax,0F0F0F0F0h
                 mov  ebx,[edi]
                 and  ebx,0F0F0F0F0h
                 lea  ebx,[ebx*8+ebx]
                 add  eax,ebx
                 shr  eax,3
                 stosd
ENDIF
                 loop @@B_S_Loop1

                 popad
                 retn
Motion_Blur      endp