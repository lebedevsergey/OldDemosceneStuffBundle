		mov ebp,dword ptr SurfaceDescription+DDSURFACEDESC.lPitch
		mov esi,dword ptr [VIRT_SCREEN1]
                mov edx,VERT_RAZR

		cmp TIME,VOM_TIME
		jbe	@@No
		mov	ecx,HOR_RAZR*VERT_RAZR
		xor	eax,eax
push edi
		mov	edi,esi
		rep	stosd
pop edi

@@No:
		mov eax,Vidmode_Flg
;		mov eax,Blit_Proc_Tab[eax*4]
		jmp  [Blit_Proc_Tab[eax*4]]		

Blit_32bpp:
		sub ebp,HOR_RAZR*4
@@tt1:
                mov ecx,HOR_RAZR

		rep movsd
		add edi,ebp
		dec edx
		jnz @@tt1
		jmp @@Blit_End

Blit_24bpp:
		sub ebp,HOR_RAZR*3
@@tt2_:
                mov ecx,HOR_RAZR
@@tt1_:
                mov eax,[esi]                                                           
                mov [edi],eax                                                           
                add esi,4                                                               
                add edi,3                                                               
		loop @@tt1_
		add edi,ebp
		dec edx
		jnz @@tt2_
		jmp @@Blit_End

Blit_16bpp:
		sub ebp,HOR_RAZR*2

		xor ebx,ebx
@@tt1__:

                mov ecx,HOR_RAZR
@@tt2__:
		xor eax,eax

		mov bh,[esi+2]
	        shld ax,bx,5
	        mov bh,[esi+1]
	        shld ax,bx,6
	        mov bh,[esi]
	        shld ax,bx,5	
		stosw
		lodsd
		loop @@tt2__

		add edi,ebp

		dec edx
		jnz @@tt1__
@@Blit_End: