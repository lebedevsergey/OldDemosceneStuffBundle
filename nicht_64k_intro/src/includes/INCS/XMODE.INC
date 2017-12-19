.486p
VERT_RAZR_X        equ  400

COU1  equ 2

VIRT_SCREEN_SIZE equ  HOR_RAZR*VERT_RAZR

.CODE
locals
Cycle1           macro
local @@tt1_
                 mov  ecx,HOR_RAZR/16

@@tt1_:

mov al,byte ptr [esi+3*4*4+2]
mov bl,byte ptr [esi+3*4*4+0]
mov dl,byte ptr [esi+3*4*4+1]
mov   ah,[esi+2*4*4+2]
mov   bh,[esi+2*4*4+0]
mov   dh,[esi+2*4*4+1]

bswap eax
bswap ebx
bswap edx

mov   ah,[esi+4*4+2]
mov   bh,[esi+4*4+0]
mov   dh,[esi+4*4+1]
mov   al,[esi+2]		
mov   bl,[esi+0]		
mov   dl,[esi+1]		
and   eax,0FCFCFCFCh		
and   ebx,0FCFCFCFCh		
and   edx,0FCFCFCFCh		
shr   eax,2
shr   ebx,2
shr   edx,2
add esi,4*4*4
or    ebx,80808080h
or    edx,40404040h
mov   [edi+HOR_RAZR/4],ebx   ; green
mov   [edi+2*HOR_RAZR/4],edx    ; blue
stosd                 ; red
;mov   [edi],eax      ; red
;add edi,4

		loop @@tt1_

endm


.486p
setxmode320x400   proc near

        pushad
        mov  ax,13h
        int  10h

        ;-----
        ; unchain the video mode

        mov dx,03C4h
        mov al,04h
        out dx,al        
        inc dx
        in  al,dx
        and al,11110111b    
        or  al,00000100b    
        out dx,al

        mov dx,03CEh
        mov al,05h
        out dx,al
        inc dx
        in  al,dx
        and al,11101111b    
        out dx,al

        dec dx
        mov al,06h       
        out dx,al
        inc dx
        in  al,dx
        and al,11111101b    
        out dx,al
        
        mov dx,03C4h
        mov ax,0F02h
        out dx,ax

        mov ecx,04000h
        xor eax,eax
        mov edi,VideoPtr
        rep stosd

        mov dx,03D4h
        mov al,09h
        out dx,al
        inc dx
        in  al,dx
        and al,01100000b    
        out dx,al

        dec dx
        mov al,14h       
        out dx,al
        inc dx
        in  al,dx
        and al,10111111b    
        out dx,al

        dec dx
        mov al,17h       
        out dx,al
        inc dx
        in  al,dx
        or  al,01000000b    
        out dx,al

        popad
	retn
setxmode320x400        endp


X_Draw_Virt_Screen     proc  near


                 mov  bx,0104h
@@loop3:
		 push esi
		 push edi
		 push ebx

                 mov dx,03C4h
                 mov  al,2
                 mov  ah,bh
                 out dx,ax


                 mov  bp,VERT_RAZR_X/6
@@tt2:        
		Cycle1
		add edi,2*HOR_RAZR/4

		Cycle1
		add edi,2*HOR_RAZR/4

		add esi,1*4*HOR_RAZR

		dec bp
		jns @@tt2

		 pop  ebx
		 pop  edi
		 pop  esi
                 shl  bh,1
		 add  esi,4
			
                 dec  bl
                 jnz  @@loop3

                 retn
X_Draw_Virt_Screen endp

.DATA?
Count1  db ?
.CODE

