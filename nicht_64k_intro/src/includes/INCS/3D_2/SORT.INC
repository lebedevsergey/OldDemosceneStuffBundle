.CODE
locals

Sort_All_Scene   proc 	near

		xor     esi,esi
		mov     eax,Sorted_Poly_Num
		or      eax,eax
		jz      @@tt5
		
		dec     eax

		shl     eax,2
		mov     edi,eax

qSort:
		mov     ebx,esi
qSort_:
		mov     ecx,edi

		mov     eax,ebx
		add     eax,ecx	
		shr     eax,3 ;1
;		and     al,11111100b	
		mov     edx,Z_Sum[eax*4]

@@Sort:
		sub     ebx,4
		add     ecx,4

@@tt1:
		add     ebx,4
		cmp     Z_Sum[ebx],edx
		jg      @@tt1

@@tt2:
		sub     ecx,4
		cmp     Z_Sum[ecx],edx
		jl      @@tt2

		cmp     ebx,ecx
		jg      @@tt3

		mov     eax,Z_Sum[ecx]
		xchg    eax,Z_Sum[ebx]
		mov     Z_Sum[ecx],eax

                mov     eax, dword ptr Sorted_Faces[ecx]
                xchg    eax, dword ptr Sorted_Faces[ebx]
                mov     dword ptr Sorted_Faces[ecx], eax

		cmp     ebx,ecx
		jle     @@tt1   ;Sort

@@tt3:
		cmp     ecx,esi
		jle     @@tt4_

		push    edi
		push    ebx
		mov     edi,ecx	
		call    qSort
		pop     esi
		pop     edi

		cmp     esi,edi
		jl      qSort
@@tt5:

                retn

@@tt4_:
		cmp     ebx,edi
		mov     esi,ebx	
		jl      qSort_
                retn
Sort_All_Scene  endp

.DATA?
Sorted_Poly_Num dd  ?
Sorted_Faces_Cur_Ptr  dd  ?
Z_Sum_Ptr       dd   ?

 Sorted_Obj TMODEL <?>

Sorted_Faces:  
        db MAX_POLY_SCENE_NUM*SIZE TFACE dup (?)

Z_Sum   dd MAX_POLY_SCENE_NUM dup (?)