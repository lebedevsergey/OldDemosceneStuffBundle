locals

.CODE

Do_Desert           Proc   Near


                 pushad

		 lodsb
		 cbw		
		 mov  Desert_Angle,ax

		 mov  ax,Desert_Noise
		 shl  ax,1
		 mov  Desert_Const2,ax


IF HOR_RAZR GT 320 
       mov  ecx,TEX_SIZE*4
ELSE
       mov  ecx,HOR_RAZR*4
ENDIF

@@loop0:
       mov  ax,Desert_Begin
       call random
       stosb
       loop @@loop0


       mov  ecx,0FFFFh*4
@@loop1:

       movzx  edx,byte ptr [edi-TEX_SIZE*4]

       movsx  ebx,Desert_Angle
       xor    ebp,ebp
       inc    ebp
       or     ebx,ebx
       js     @@lb1
       neg    ebp       
@@lb1:
	xor   esi,esi
        inc esi
@@loop2:
        inc esi
        movzx  eax,byte ptr [edi-TEX_SIZE*4+ebx*4]
        add edx,eax
	add ebx,ebp
	jnz @@loop2

       xchg eax,edx
       cdq
       div esi  
       xchg eax,edx

       mov  ax,Desert_Const2
       call random     
       sub  ax,Desert_Noise
       add  al,dl

       stosb

       dec ecx 
       jnz @@loop1


                 popad
		inc  esi
                retn
Do_Desert       endp

.DATA
Desert_Begin    dw 164
Desert_Noise    dw 36
Desert_Angle    dw -1

.DATA?
Desert_Const2   dw  ?
