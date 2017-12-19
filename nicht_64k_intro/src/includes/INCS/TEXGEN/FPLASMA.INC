FPLAS_CONST  equ 256
.CODE
locals

include f:\dem\common\incs\texgen\fplasma.def

Subdivide       Proc   Near
;		pushad
		push bp
;		push qword ptr F_P_X1

		mov  dl,F_P_X2
		sub  dl,F_P_X1
		cmp dl,1
		jbe   @@Exit		 

		mov  dl,F_P_Y2
		sub  dl,F_P_Y1
		cmp dl,1
		jbe  @@Exit		 

		movzx  dx,F_P_X1
		movzx  bx,F_P_X2
		add    dx,bx
		shr  dx,1
;		cmp  dl,F_P_X1
;		jbe  @@Exit		 

		mov  F_P_X,dl


		movzx  dx,F_P_Y1
		movzx  bx,F_P_Y2
		add    dx,bx
		shr  dx,1
;		cmp  dl,F_P_Y1
;		jbe  @@Exit		 

		mov  F_P_Y,dl

                Getcol  F_P_X,F_P_Y1
		jnz      @@1
                Adjus    F_P_X1,F_P_Y1,F_P_X2,F_P_Y1,F_P_X,F_P_Y1
@@1:
                mov     bp,ax
	        	

                Getcol  F_P_X2,F_P_Y
		jnz      @@2
                Adjus    F_P_X2,F_P_Y1,F_P_X2,F_P_Y2,F_P_X2,F_P_Y
@@2:
                add     bp,ax


                Getcol  F_P_X,F_P_Y2
		jnz      @@3
                Adjus    F_P_X1,F_P_Y2,F_P_X2,F_P_Y2,F_P_X,F_P_Y2
@@3:
                add     bp,ax

                Getcol  F_P_X1,F_P_Y
		jnz      @@4
                Adjus    F_P_X1,F_P_Y1,F_P_X1,F_P_Y2,F_P_X1,F_P_Y
@@4:
                add     ax,bp

		shr     ax,2

                Putcol2

		movzx   edx,F_P_X
		mov     dh,F_P_Y
		mov     [edx*4+edi],al;eax


                Subdivid    F_P_X1,F_P_Y1,F_P_X,F_P_Y
                Subdivid    F_P_X,F_P_Y1,F_P_X2,F_P_Y
                Subdivid    F_P_X,F_P_Y,F_P_X2,F_P_Y2
                Subdivid    F_P_X1,F_P_Y,F_P_X,F_P_Y2
		
@@Exit:
;		pop qword ptr F_P_X1
		pop bp
;		popad
                retn
endp


Adjust          Proc   Near
;  IN : EBX = X1,Y1
;	ECX = X2,Y2
;	EDX = X3,Y3	

;		push eax
		push ebx
		push ecx
		push edx

		movzx  ax,byte ptr [ecx*4+edi]
		sub    cl,bl
		sub    ch,bh
		movzx  bx,byte ptr [ebx*4+edi]
		add    bx,ax

		add    cl,ch
		xor    ch,ch
		
		mov   eax,FPLAS_CONST
		call  Random
		mul   cx
		shr   ax,8
		
		shr   bx,1
		add   ax,bx

		cmp   ax,0FFH
		jbe    @@tt1
;		mov   al,0FFH
@@tt1:

;		cmp   al,2
;		ja    @@tt2
;		mov   al,2
@@tt2:

		pop edx

                Putcol2

		pop ecx
		pop ebx
;		pop eax
		xor ah,ah
                retn
endp


Do_FPlasma       Proc   Near
	mov cl,3
@@tt1:
	mov  F_P_X1,0
	mov  F_P_Y1,0
	mov  F_P_X2,0FFh
	mov  F_P_Y2,0FFh


	mov     al,0ffh
	Putcol 0,0,al

	mov     al,0ffh
	Putcol 0,0ffH,al

	mov     al,0ffh
	Putcol 0ffh,0,al

	mov     al,0ffh
	Putcol 0ffh,0ffh,al

	push edi
	push cx
	call Subdivide
	pop cx
	pop edi
	inc edi
	dec  cl
	jnz  @@tt1


retn
endp

.DATA?
F_P_X1 db ?
F_P_Y1 db ?

F_P_X2 db ?
F_P_Y2 db ?

F_P_X db ?
F_P_Y db ?



