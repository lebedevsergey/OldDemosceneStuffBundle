; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

; samples generator

include synth.def
.CODE
locals

Init_Synth   proc 
; ebp - tmp mem
					   ;setup pointers
		mov	Tmp_Samples_Ptr,ebp
		mov	Ready_Sample_Ptr,ebp
		add	ebp,SIZE TMP_SAMPLE
		mov	Seq_Amp_Env_Ptr,ebp
		add	ebp,SIZE TMP_SAMPLE
		mov	Seq_Freq_Env_Ptr,ebp
		add	ebp,SIZE TMP_SAMPLE
		mov	Filt_Gain_Env_Ptr,ebp
		add	ebp,SIZE TMP_SAMPLE
		mov	Seq_Note_Amp_Env_Ptr,ebp

;		add	ebp,SIZE TMP_SAMPLE
;		mov	Filt_Freq_Env_Ptr,ebp
;		add	ebp,SIZE TMP_SAMPLE
;		mov	Filt_Q_Env_Ptr,ebp
;		add	ebp,SIZE TMP_SAMPLE
;		mov	Sample_Copy_Ptr,ebp

		fld1	
		fidiv	SAMPLE_RATE
		fstp	Freq_Coef

		fld1
		fidiv	@@_FFFF
		fstp	Noise_Norm_Coef

		call	Do_Freq_Tab
		call	Init_Waves	

		retn
.DATA
@@_FFFF		dw	0FFFFh/2
.CODE
Init_Synth   endp

Do_Sample    proc 
; esi - script
; edi - sample
		pushad
		
		push edi
		xor  eax,eax
		mov  edi,Tmp_Samples_Ptr
                mov  ecx,Tmp_Samples_Size/4
		rep  stosd
					; Чистим место для сэмплов
		lea  edi,Oscillators
                mov  ecx,Oscillators_Size/4
		rep  stosd           ; Чистим осцилляторы

		mov  Len_Mul,1
		mov  Act_Osc_Num,0

		mov	seed,RAND_CONST     ; Инитим случайные числа
		mov	Pitch_Speed,DEFAULT_PITCH_SPEED

		mov	eax,Def_Attack_Len
		mov	Attack_Len,eax
		mov	eax,Def_Decay_Len
		mov	Decay_Len,eax


		mov  SAMPLE_SIZE,MAX_SAMPLE_SIZE_
		push esi
		call Clear_Harmonizer
		pop  esi
@@com_cycle:
		finit

		xor  eax,eax
		lodsb           ; Берем команду
		cmp  al,END_COM		
		jz   @@exit
		xchg eax,ebx

		lea  ebx,[Synth_Com_Table+ebx*4]

IFDEF CHK_SYNT_SCRIPT_ERR1
		cmp  ebx,offset Synth_Com_Table_End
		jae  Err_
		cmp  ebx,offset Synth_Com_Table
		jl  Err_
ENDIF
		call	dword ptr [ebx]
		jmp	@@com_cycle
@@exit:		
		pop	edi

		mov	esi,Ready_Sample_Ptr
                mov	ecx,SAMPLE_SIZE
@@Conv_to_16:
		fld	dword ptr [esi]
		fmul	Max_Sample_Val_2
		call	Sample_Cut_Max_Min2
		fistp	word ptr [edi]

		add	esi,4
		add	edi,2
		dec	ecx
		jnz	@@Conv_to_16

		popad
		retn

IFDEF CHK_SYNT_SCRIPT_ERR1
.DATA
Err_Msg3  db  'Wrong script command',0Dh,0Ah,'$'
.CODE
Err_:
int 3
	lea edx,Err_Msg3
	jmp sound_err
ENDIF
endp


Sample_Cut_Max_Min  proc 

        fld   st
        fcomp Max_Sample_Val_
        fstsw ax
        sahf
        jbe    @@tt6
        fstp  st
        fld Max_Sample_Val_
        jmp @@tt7
@@tt6:
        fld   st
        fcomp Min_Sample_Val_
        fstsw ax
        sahf
        jae    @@tt7
        fstp  st
        fld Min_Sample_Val_
@@tt7:
	retn
Sample_Cut_Max_Min endp

Sample_Cut_Max_Min2	proc

        fld   st
        fcomp Max_Sample_Val_2
        fstsw ax
        sahf
        jbe    @@tt6
        fstp  st
        fld Max_Sample_Val_2
        jmp @@tt7
@@tt6:
        fld   st
        fcomp Min_Sample_Val_2
        fstsw ax
        sahf
        jae    @@tt7
        fstp  st
        fld Min_Sample_Val_2
@@tt7:
	retn
Sample_Cut_Max_Min2	endp

Set_Len_Mul	proc
		xor eax,eax
		lodsb
		mov Len_Mul,ax	
                retn
Set_Len_Mul	endp

Set_Bpm_Sample_Len	proc 
		pushad
		xor	eax,eax
		lodsb
		mov	BPM_Tempo,ax	;!!!!!!! <=255 !!!!!!!
		lodsb 
		mov	BPM_Speed,ax
		lodsb
		mov	Sample_Notes_Len,ax	

		fild	SAMPLE_RATE ; 5*S_R/(2*Tempo)*Speed*Note_Len*Muller
		mov	Tmp1,5
		fimul	Tmp1
		fild	BPM_Tempo
		fadd	st,st
		fdivp	st(1),st
		fimul	BPM_Speed
		fimul	Sample_Notes_Len
		fimul	Len_Mul
		fistp	SAMPLE_SIZE

IFDEF CHK_SYNT_SCRIPT_ERR1
.DATA
Err_Msg10  db  'Too big sample length',0Dh,0Ah,'$'
.CODE
lea edx,Err_Msg10		
cmp SAMPLE_SIZE,MAX_SAMPLE_SIZE_-2
ja sound_err
ENDIF
		popad
		add esi,3
		retn
Set_Bpm_Sample_Len	endp

.DATA
Minute_Len	dw	60

Percent3	dw	254

.DATA?
BPM_Tempo	dw	?
BPM_Speed	dw	?
Len_Mul		dw	?
Tick_Len	dd	?
Sample_Notes_Len	dw	?
Freq_Coef	dd	?
Last_ESI	dd	?

Tmp_Samples_Ptr			dd	?

Ready_Sample_Ptr		dd	?
Seq_Amp_Env_Ptr			dd	?
Seq_Freq_Env_Ptr		dd	?
Seq_Note_Amp_Env_Ptr		dd	?
Filt_Gain_Env_Ptr		dd	?

;Filt_Freq_Env_Ptr		dd	?
;Filt_Q_Env_Ptr			dd	?
;Sample_Copy_Ptr		dd	?

Tmp_Samples_Size equ (($-Ready_Sample_Ptr)/4)*SIZE TMP_SAMPLE
SYNTH_TMP_MEM_SIZE = Tmp_Samples_Size	

include syn_stff.inc
include syn_effs.inc