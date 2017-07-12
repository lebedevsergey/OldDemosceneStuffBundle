Sound_Gen_Proc_Base_Offset:

;pow - disabled
Sin_Wave_Func  proc
;	fmul	_2PI
	fldpi
	fadd	st,st
	fmulp	st(1),st
        fsin
;disabled
;	call	Do_Pow
	retn
Sin_Wave_Func  endp

Saw_Wave_Func  proc
	fadd st,st
	fld1
	fsubp st(1),st
	retn
Saw_Wave_Func  endp  

Noise_Wave_Func  proc
fstp st	
        Random
	fild	[ebp].seed
	fmul	Noise_Norm_Coef
	retn
Noise_Wave_Func  endp

Get_Percent	proc
		 xor	eax,eax
		 lodsb		
		 mov	[ebp].Tmp2_2,eax
		 fild	[ebp].Tmp2_2
		 fidiv	Percent
		retn
Get_Percent	endp

Setup_Oscillator  proc  
;!!!eax=0!!!
;	    pushad

	    lodsb  		; Берем номер осциллятора
;	    push eax	
	    GET_OSCILL_ADDR edi

	    xor  eax,eax
	    lodsb  		; Берем тип осциллятора
	    mov  [edi].Osc_Type,eax

;	    lodsd  		; Берем коэф. микширования
	    Get_Percent_
	    fstp	[edi].Osc_Mix_Coef

;	    xor  eax,eax
	    lodsb  		; Берем частоту расстройки
	    mov  [ebp].Tmp2_2,eax
	    fild [ebp].Tmp2_2
;	    fidiv Percent3
;	    fmul  [ebx].Flanger_Freq_Coef2
	    fmul  [ebx].Flanger_Freq_Coef3
	    fadd  [ebx].Flanger_Freq_Coef1
	    fstp  [edi].Flanger_Freq_Coef

;	    lodsb  		; Берем  shape param
; disabled
;	    mov   Tmp1,ax
;	    fild  Tmp1
;	    fidiv Percent
;	    fstp  [edi].Osc_Shape_Param
; disabled
;	    pop  eax	
;     	    GET_HARMONIZER_ADDR ebp
;	    mov  [edi].Osc_Harmonizer_Ptr,ebp

	    mov  eax,[ebp].Act_Osc_Num   ; Добавляем осц. в список
	    mov  [ebp].Act_Osc_Ptrs[eax*4],edi
	    inc  [ebp].Act_Osc_Num

;	    popad
;	    add  esi,4
;lodsd
	    retn
Setup_Oscillator  endp

Set_A_D_Coef	proc
		 Get_Percent_
		 fstp	[ebp].Attack_Coef
		 Get_Percent_
		 fstp	[ebp].Decay_Coef

		 retn
Set_A_D_Coef	endp