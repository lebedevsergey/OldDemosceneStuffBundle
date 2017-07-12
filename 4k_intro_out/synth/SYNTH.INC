include synth.def
.CODE
locals

Synth_Base_Offset:

Do_Sample    proc 
; esi - script
; edi - sample
		xor	eax,eax
		pushad
lea	ebp,All_Data		
		push	edi

		mov	edi,[ebp].Ready_Sample_Ptr
 		mov	ecx,Tmp_Samples_Size/4
		rep	stosd
					; Чистим место для сэмплов
;		lea	edi,[ebp].Oscillators
 ;               mov	ecx,Oscillators_Size/4
;		rep	stosd           ; Чистим осцилляторы

;		mov	[ebp].Act_Osc_Num,0
;		mov	[ebp].Len_Mul,1
		mov	[ebp].Act_Osc_Num,eax
		inc	eax
		mov	[ebp].Len_Mul,eax

		mov	[ebp].seed,CONST1     ; Инитим случайные числа
;		mov	Pitch_Speed,DEFAULT_PITCH_SPEED

;		mov	eax,Def_Attack_Len
;		mov	[ebp].Attack_Len,eax
;		mov	eax,Def_Decay_Len
;		mov	[ebp].Decay_Len,eax


;		mov  [ebp].SAMPLE_SIZE,MAX_SAMPLE_SIZE_
@@com_cycle:
		fninit

lea	ecx,Get_Percent
lea	ebx,Init_Data
		xor  eax,eax
		lodsb           ; Берем команду
		cmp  al,END_COM		
		jz   @@exit

;		xchg eax,edx
;disabled	lea  edx,[Synth_Com_Table+edx*4]

		movzx	edx,word ptr [offset Synth_Com_Table+eax]
		add	edx,offset Synth_Base_Offset
		xor	eax,eax
		call	edx
		jmp	@@com_cycle
@@exit:	

		pop	edi
;ebx->Init_Data
		mov	esi,[ebp].Ready_Sample_Ptr
                mov	ecx,[ebp].SAMPLE_SIZE
@@Conv_to_16:
		fld	dword ptr [esi]
		Sample_Cut_Max_Min

		fimul	Max_Sample_Val_2
		fistp	word ptr [edi]

;		add	esi,4
;		add	edi,2
lodsd
inc	edi
inc	edi
		loop	@@Conv_to_16

		popad
		mov	[ebp].CHAN_SMP_POS,eax
		mov	[ebp].CHAN_VOL,eax
		mov	[ebp].CHAN_SMP_PTR,edi
		mov	eax,All_Data.SAMPLE_SIZE
		mov	[ebp].CHAN_SMP_LEN,eax
Empty: 
		retn

IFDEF CHK_ERR1
.DATA
Err_Msg3  db  'Wrong script command',0Dh,0Ah,'$'
.CODE
Err_:
int 3
	lea edx,Err_Msg3
	jmp sound_err
ENDIF
endp

Set_Len_Mul	proc
		lodsb
		mov [ebp].Len_Mul,eax	
                retn
Set_Len_Mul	endp

Set_Bpm_Sample_Len	proc 
		pushad
;disabled
;		lodsb
;		mov	[ebp].BPM_Tempo,eax	;!!!!!!! <=255 !!!!!!!
;		lodsb 
;		mov	[ebp].BPM_Speed,eax

		lodsb
		mov	[ebp].Sample_Notes_Len,eax	

;		fild	SAMPLE_RATE ; 5*S_R/(2*Tempo)*Speed*Note_Len*Muller
;		fimul	@@_5
;		fild	[ebp].BPM_Tempo
;		fadd	st,st
;		fdivp	st(1),st
;		fimul	[ebp].BPM_Speed

fild	@@Coef1
		fimul	[ebp].Sample_Notes_Len
fist	[ebp].Seq_Notes_Cyc_Len
		fimul	[ebp].Len_Mul
		fistp	[ebp].SAMPLE_SIZE

		popad
;		add esi,3
;lodsd
;dec esi
inc	esi
		retn
.DATA
;@@_5		dw	5
;@@Coef1		dw	2067;.1875	;5*S_R/(2*Tempo)*Speed
@@Coef1		dw	2100 ;2567

.CODE
Set_Bpm_Sample_Len	endp

.DATA
Percent		dw	100

Tmp_Samples_Size =6*SIZE TMP_SAMPLE
SYNTH_TMP_MEM_SIZE = Tmp_Samples_Size	

include syn_stff.inc
include syn_effs.inc