.CODE
locals
TEX_SIZE   equ 256

SQR_NUM  equ  4000
SQR_ADDER  equ  20

SQR_X_SIZE  equ  30
SQR_Y_SIZE  equ  40


TEX_SIZE  equ 256


Blurx256     macro

		 mov cl,20
@@do:
pushad
                 mov  ecx,TEX_SIZE*TEX_SIZE*4
                 mov  ebp,TEX_SIZE*4

                 xor  esi,esi
                 xor  ebx,ebx         
@@B_S_Loop1:
                 movzx  eax, byte ptr [edi+esi-4]
                 mov  bl,[edi+esi+4]
                 add  eax,ebx

                 mov	edx,esi
                 sub	edx,ebp
and edx,3ffffh

                 mov  bl,[edi+edx-4]
                 add  eax,ebx
                 mov  bl,[edi+edx]
                 add  eax,ebx
                 mov  bl,[edi+edx+4]
                 add  eax,ebx

                 lea  edx,[esi+ebp]
and edx,3ffffh
                 mov  bl,[edi+edx-4]
                 add  eax,ebx
                 mov  bl,[edi+edx]
                 add  eax,ebx
                 mov  bl,[edi+edx+4]
                 add  eax,ebx

                 shr  eax,3

                 mov  [edi+esi],al

                 inc  esi                           

                 loop @@B_S_Loop1
popad           
		dec	cl
		jnz	@@do

                endm



Do_tlo	proc
        pushad

        mov	ecx,Sqr_Num

 @@l3:  
        mov	eax,TEX_SIZE*TEX_SIZE*4
        Random

        mov	dh,SQR_Y_SIZE
@@l2:   
        mov	dl,SQR_X_SIZE
@@l1:   
        and	eax,03FFFFh
        add	byte ptr [edi+eax],SQR_ADDER
	add	eax,4
        dec	dl
        jnz	@@l1

        add	eax,(TEX_SIZE-SQR_X_SIZE)*4
or bh,bh
jnz @@tt
add eax,5
@@tt:
        dec	dh
        jnz	@@l2

        loop @@l3

        popad

	Blurx256

        ret     
        endp

