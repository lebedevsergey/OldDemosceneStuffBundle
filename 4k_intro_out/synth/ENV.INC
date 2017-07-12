;envelope.inc
.CODE
locals
MAX_ENV_NODES_NUM   equ  30

ENV_NODE   struc
	NODE_VALUE	dd	?
	NODE_LEN	dd	?
        NODE_CUR_LEN	dd	?
        NODE_RATE	dd	?
ends

ENVELOPE  struc
          NODES_NUM	dd	?
	  CUR_ENV_NODE	dd	?
	  CUR_ENV_VALUE	dd	?
	  CUR_ENV_POS	dd	?

          ENV_NODES  db MAX_ENV_NODES_NUM*SIZE ENV_NODE dup(?)
ends

.CODE

Clear_Envelope	macro
; edi - env rec ptr
;        pushad
        lea	ebx,[edi].ENV_NODES
	mov	[edi].CUR_ENV_NODE,ebx

	xor	eax,eax
	mov	[edi].CUR_ENV_POS,eax
	mov	eax,[ebx].NODE_VALUE
	mov	[edi].CUR_ENV_VALUE,eax
;        popad
;	retn
;Clear_Envelope	endp
endm

Setup_Envelope    proc
; esi - envelope script
; edi - envelope record ptr
; edx - env max amp

        mov    [ebp].Env_Max_Amp,edx

        lea    edx,[edi].ENV_NODES
                                ; Собираем узлы огибающей
	xor	ebx,ebx
@@tt1:
	and	byte ptr [esi+1],NOT ENV_END_MASK
	pushf

	Get_Percent_
        fimul	[ebp].Env_Max_Amp
        fstp	[edx].NODE_VALUE
			  	; Берем node len
	Get_Percent_
        fstp	dword ptr [edx].NODE_LEN

;        add    esi,8

        inc    ebx
        add    edx,SIZE ENV_NODE

        popf
        jnz     @@tt1

        mov    [edi].NODES_NUM,ebx

@@Nodes_End:
        ret
Setup_Envelope    endp


Calc_Nodes_Rates	proc
;esi - env ptr
;ecx - env len
		mov	[ebp].Env_Len,ecx
                                       ; Считаем nodes rates
	        lea    edx,[esi].ENV_NODES
        	mov    ecx,[esi].NODES_NUM
@@Calc_Rates:
	        fld	[edx].NODE_LEN
        	fimul	[ebp].Env_Len
	        fist	[edx].NODE_CUR_LEN

        	fld	[edx+SIZE ENV_NODE].NODE_VALUE
	        fsub	[edx].NODE_VALUE
;        	fidiv	[edx].NODE_CUR_LEN
		fdivrp	st(1),st

	        fstp	[edx].NODE_RATE

        	add	edx,SIZE ENV_NODE
        	loop	@@Calc_Rates
		retn
Calc_Nodes_Rates	endp

Calc_Envelope_From_To_Pos    proc
; esi - envelope record ptr
; edi - ready envelope
; ecx - envelope len
; eax,ebx - begin,end pos
        pushad
Calc_Envelope_From_To_Pos_:
	mov	[ebp].Env_Start_Pos,eax
	mov	[ebp].Env_End_Pos,ebx
	call	Calc_Nodes_Rates
                                ; Считаем огибающую
        xor	ebx,ebx
        mov	ecx,[esi].NODES_NUM
        lea	esi,[esi].ENV_NODES
@@Calc_Env:
        fld   [esi].NODE_VALUE

        mov   edx,[esi].NODE_CUR_LEN
IFDEF DEBUG_VER
        or    edx,edx
        jz    @@Next_Node
ENDIF
@@ll2:
	cmp	ebx,[ebp].Env_Start_Pos
        jb    @@No_Env
;        fst   dword ptr [edi]
;        add   edi,4
	stosd
        fst   dword ptr [edi-4]
@@No_Env:
        fadd  [esi].NODE_RATE

        inc   ebx
        cmp   ebx,[ebp].Env_Len
        ja    @@Sample_End
	cmp	ebx,[ebp].Env_End_Pos
        ja    @@Sample_End

        dec   edx
        jns   @@ll2
@@Next_Node:
        fstp st
        add   esi,SIZE ENV_NODE
        loop  @@Calc_Env

@@Sample_End:
        fstp st
        popad
        ret
Calc_Envelope_From_To_Pos    endp

Do_Envelope   proc
;edi - ready envelope
;edx - env max amp
       push   edi
       lea    edi,[ebp].Envelope_Nodes
       call   Setup_Envelope
xchg eax,edi
       pop    edi

       pushad
       mov	ecx,[ebp].SAMPLE_SIZE
xchg esi,eax
;       lea	esi,[ebp].Envelope_Nodes
	
	xor	eax,eax
	mov	ebx,ecx
       jmp   Calc_Envelope_From_To_Pos_
;       popad
 ;      retn
Do_Envelope   endp

Init_Envelope_Deb   proc
;esi - script
;edi - envelope rec
;edx - env max amp
;ebp - ptr to All_Data
push eax
push ebx
	push   edi
	call   Setup_Envelope
	pop    edi

	pushad
	mov	esi,edi
	mov	ecx,[ebp].SAMPLE_SIZE
	call	Calc_Nodes_Rates

	Clear_Envelope

       popad
pop ebx
pop eax
	add	edi,eax ;!!!!!!!! izvrat 
       retn
Init_Envelope_Deb   endp

Get_Envelope_Deb   proc
;edi - envelope rec
        pushad
                                      ; Считаем огибающую
	mov	ebx,[edi].CUR_ENV_NODE
	mov	ecx,[edi].CUR_ENV_POS

	cmp	ecx,[ebx].NODE_CUR_LEN
	jbe	@@No_Next_Node
        add	ebx,SIZE ENV_NODE
	mov	[edi].CUR_ENV_NODE,ebx
	mov	eax,[ebx].NODE_VALUE
	mov	[edi].CUR_ENV_VALUE,eax  
	xor	ecx,ecx
@@No_Next_Node:
	fld	[edi].CUR_ENV_VALUE		
					; update				
	fld	st			
        fadd	[ebx].NODE_RATE
	fstp	[edi].CUR_ENV_VALUE

	inc	ecx
	mov	[edi].CUR_ENV_POS,ecx
       popad
       retn
Get_Envelope_Deb   endp