locals
.CODE
Calc_Poly_Normal proc near
; IN : EBP - ptr to poly
        pushad

        mov ebx,[ebp].PB
        mov esi,[ebp].PA 
	lea edi,Tmp_Vect1
	call vecvec_sub

        mov ebx,[ebp].PC
        mov esi,[ebp].PB 
	lea edi,Tmp_Vect2
	call vecvec_sub

	lea ebx,Tmp_Vect1
	lea esi,Tmp_Vect2
	lea edi,[ebp].PNX
	call vecvec_mul2

	fld1
        fstp dword ptr [edi+0Ch]

	lea esi,[ebp].PNX
	call vec_norm

        popad

	retn
	endp

.DATA?
Tmp_Vect1 VECTOR4 <?>
Tmp_Vect2 VECTOR4 <?> 

