;harmoniz.inc
.CODE


END_NOTES_WORD	equ	0FFh
EMPTY_NOTE_WORD equ	07Fh
NOTE_SUSTAIN_WORD	equ	080h
NOTE_PITCH_WORD		equ	0200h
FIRST_NOTE_WORD		equ	0400h
LAST_NOTE_MASK		equ	0800h
OCTAVE_MASK		equ	07h  ;00000111
TONE_MASK		equ	078h ;01111000
TONE_SHIFT		equ	3

;NOTE_ENV_SUBDIV_NUM  equ  3

MAX_OSC_SHAPE_PARAM  equ 100

MAX_OSC_NUM  equ  5

OCTAVES_NUM		equ	7
TONES_NUM		equ	12
TONECONST_		equ	1.059463094     ; 2^(1/12)
C_5_FREQ_		equ	261.625458      ;C_5 frequency
					; CDEFGAB + #

SHAPE_POW_COEF		equ	30
MAX_HARMONICS_NUM	equ	10

OSCILLATOR  struc
Osc_Type           dd  ?
Osc_Mix_Coef       dd  ?
Flanger_Freq_Coef  dd  ?

; disabled
;Osc_Harmonizer_Num dd  ?
;Osc_Shape_Param    dd  ?
;Osc_Harmonizer_Ptr dd  ?
	    ends

GET_OSCILL_ADDR macro  REG_      ; eax - oscill num
		imul eax,SIZE OSCILLATOR
		lea  REG_,[ebp+eax].Oscillators
endm


;oscill.inc
.CODE
locals


Init_Note_Envelopes   macro
;!!!!!fpu - [ebp].Int_Note_Len
;		pushad

;	       fild	[ebp].Int_Note_Len

	       fld	[ebp].Attack_Coef
;	       fimul	[ebp].Int_Note_Len
		fmul	st,st(1)
	       fist	[ebp].Attack_Len

	       fld	[ebp].Decay_Coef
;	       fimul	[ebp].Int_Note_Len
		fmul	st,st(2)
	       fist	[ebp].Decay_Len       
;	       fimul	Init_Data.NOTE_ENV_SUBDIV_NUM_
;	       fistp	[ebp].Tmp_Decay_Len

		faddp	st(1),st
		fsubp	st(1),st

;	       fisub	[ebp].Decay_Len
;	       fisub	[ebp].Attack_Len
	       fistp	[ebp].Sustain_Len
       
	       lea      esi,[ebp].Seq_Note_Env_Nodes
	       mov      edi,[ebp].Seq_Note_Amp_Env_Ptr
push edi
		xor	eax,eax
		mov	ebx,[ebp].Attack_Len
mov ecx,ebx
lea ecx,[ecx+ebx*2]	;ecx=ebx*3

	       call     Calc_Envelope_From_To_Pos
	

;	       lea      esi,[ebp].Seq_Note_Env_Nodes
;	       mov      edi,[ebp].Seq_Note_Amp_Env_Ptr
        	mov	eax,[ebp].Attack_Len
		lea	edi,[edi+eax*4]	
		mov	eax,[ebp].Sustain_Len
		mov	ebx,eax
		add	ebx,eax ;Sustain_Len

;	       mov      ecx,[ebp].Tmp_Sustain_Len
mov ecx,eax
lea ecx,[ecx+eax*2]	;ecx=eax*3
		call	Calc_Envelope_From_To_Pos
	

;		lea	esi,[ebp].Seq_Note_Env_Nodes
;		mov	edi,[ebp].Seq_Note_Amp_Env_Ptr
pop edi
        	mov	eax,[ebp].Attack_Len
		add	eax,[ebp].Sustain_Len
		lea	edi,[edi+eax*4]

;		mov	eax,[ebp].Tmp_Decay_Len	
		mov	eax,[ebp].Decay_Len
		mov	ebx,eax
		add	eax,eax
		add	eax,ebx
		mov	ebx,eax
;		mov	ecx,[ebp].Tmp_Decay_Len
mov	ecx,eax
		sub	eax,[ebp].Decay_Len
		call	Calc_Envelope_From_To_Pos

;		popad
;		retn
;Init_Note_Envelopes   endp
endm

Sequencer	proc 
;!!!!!!!!!!! eax=0
;		mov	[ebp].Prev_Note_Frq,eax
		mov	[ebp].Note_Count,eax

;		lodsb
;		mov	Seq_Notes_Cyc_Num,eax

		lea	edi,[ebp].Seq_Note_Env_Nodes
		mov	edx,AMP_ENV_MAX_VALUE
		call	Setup_Envelope

		mov	edi,[ebp].Seq_Freq_Env_Ptr
                		; Указатель на огибающую нот по частоте

		pushad
@@Calc_Notes_Num:
		lodsb
		cmp	al,END_NOTES_WORD
		jz	@@All_Notes
		inc	[ebp].Note_Count
		jmp	@@Calc_Notes_Num
@@All_Notes:
		fild	[ebp].SAMPLE_SIZE
		fidiv	[ebp].Note_Count
		fist	[ebp].Int_Note_Len

;		fidiv	Pitch_Speed
;		fistp	[ebp].Pitched_Note_Len

;		fild	SAMPLE_SIZE
;		fidiv	Seq_Notes_Cyc_Num
;		fistp	[ebp].Seq_Notes_Cyc_Len
;!!!!!fpu - [ebp].Int_Note_Len
		Init_Note_Envelopes

		popad

		push	esi

		mov	ecx,[ebp].Note_Count
@@Do_Frq:
	        lodsb
; EAX: note obozn
		mov	ebx,eax
		and	eax,OCTAVE_MASK
		and	ebx,TONE_MASK
		shr	ebx,TONE_SHIFT
; EAX: octave
; EBX: tone
		imul	eax,TONES_NUM
		add	eax,ebx

push ecx
		mov	eax,[ebp].Freq_Tab[eax*4]
		mov	ecx,[ebp].Int_Note_Len
		rep	stosd
pop ecx
		loop	@@Do_Frq

	        pop	esi

		mov	edx,[ebp].Seq_Amp_Env_Ptr
               			; Указатель на огибающую нот по амплитуде
				;do seq amp env
@@Do_Amp_Env:
		lodsb
		cmp	al,END_NOTES_WORD
        	jz	@@Notes_End
		cmp	al,EMPTY_NOTE_WORD
		jnz	@@Do_Notes
        	mov	eax,[ebp].Int_Note_Len	;Do Empty Note
		lea	edx,[edx+eax*4]
        	jmp	@@Do_Amp_Env
@@Do_Notes:
		dec	esi

		xor	ecx,ecx
@@Next_Sustain_Note:
        	mov	al,[esi]
        	cmp	al,END_NOTES_WORD
        	jz	@@Notes_End2
		cmp	al,EMPTY_NOTE_WORD      
		jz	@@Notes_End2
		inc	esi
		add	ecx,[ebp].Int_Note_Len
	        test	al,NOTE_SUSTAIN_WORD
        	jnz	@@Next_Sustain_Note
@@Notes_End2:
		xor	ebx,ebx
        	mov	[ebp].Cur_Sustain_Ptr,ebx; 0
@@One_Sustain_Amp_Env:
		pushad
		mov	edi,[ebp].Seq_Note_Amp_Env_Ptr
Do_Attack:
		mov     eax,[ebx*4+edi]
		cmp     ebx,[ebp].Attack_Len
		jb	@@Done
Do_Sustain:
		cmp     ecx,[ebp].Decay_Len
		jna	Do_Decay
		mov	ecx,[ebp].Cur_Sustain_Ptr
		mov	eax,[ebp].Attack_Len
		add	eax,ecx
		mov     eax,[eax*4+edi]

		inc	ecx	;Cur_Sustain_Ptr
		cmp	ecx,[ebp].Sustain_Len		;loop enelope
		jb	@@No_Loop
		xor	ecx,ecx
@@No_Loop:
		mov	[ebp].Cur_Sustain_Ptr,ecx
		jmp     @@Done
Do_Decay:
		mov	eax,[ebp].Int_Note_Len
		sub	eax,ecx
		mov     eax,[eax*4+edi]
@@Done:
;!!!!!!!!!!!!!!!!!!!!!!!!!!
        	mov	[edx],eax
		popad

	        add	edx,4
		inc	ebx
		loop	@@One_Sustain_Amp_Env

		jmp	@@Do_Amp_Env
@@Notes_End:
;	 	jmp	Do_Sound

Sequencer	endp


;sound.inc

Do_Sound proc
		 pushad

		mov	edi,[ebp].Ready_Sample_Ptr
IFDEF	DEBUG_VER
		cmp   [ebp].Act_Osc_Num,0
		jz    @@ex
ENDIF
		xor	ecx,ecx
		mov	[ebp].Cur_Angle,ecx
@@Do_Wave: 
;		xor	eax,eax
;		mov	[ebp].Cur_Sound,eax
		fldz

		mov	ebx,[ebp].Act_Osc_Num
;		dec	ebx
@@Sum_Osc:
		 mov   esi,[ebp].Act_Osc_Ptrs[ebx*4-4]

; disabled
;	 	mov   eax,[esi].Osc_Harmonizer_Ptr
;	 	mov   Cur_Harmonizer_Ptr,eax

;	 	mov   eax,[esi].Flanger_Freq_Coef
;	 	mov   Cur_Flanger_Coef,eax
; disabled
;	 	mov   eax,[esi].Osc_Shape_Param
;	 	mov   Shape_Param,eax

	 	mov	eax,[esi].Osc_Type	
		movzx	eax,byte ptr Sound_Gen_Proc_Table[eax] 		
		add	eax,offset Sound_Gen_Proc_Base_Offset

	fld1 
	fld  [ebp].Cur_Angle
;fmul Cur_Flanger_Coef
fmul [esi].Flanger_Freq_Coef
	fprem
	fxch	st(1)
	fstp  st

	 call  eax            ;  Генерим
	 mov   edx,[ebp].Seq_Amp_Env_Ptr

         fmul dword ptr [edx+ecx*4]     ; Огибающая по амплитуде
         fmul  [esi].Osc_Mix_Coef
;         fadd  [ebp].Cur_Sound
 ;        fstp  [ebp].Cur_Sound
	faddp	st(1),st

		dec   ebx
		jnz   @@Sum_Osc

		stosd
		fstp  dword ptr [edi-4]

	 mov	edx,[ebp].Seq_Freq_Env_Ptr
         fld	dword ptr [edx+ecx*4]    ; Огибающая по частоте
         fidiv	SAMPLE_RATE
         fadd	[ebp].Cur_Angle
         fstp	[ebp].Cur_Angle

         inc	ecx  
         cmp	ecx,[ebp].SAMPLE_SIZE
         jb	@@Do_Wave
@@ex:
	 popad

         retn
Do_Sound endp

Do_Custom_Sound  proc
	 mov   edi,[ebp].Seq_Amp_Env_Ptr
         mov   edx,AMP_ENV_MAX_VALUE*5
	 call  Do_Envelope
	 mov   edi,[ebp].Seq_Freq_Env_Ptr
         mov   edx,FREQ_ENV_MAX_VALUE
	 call  Do_Envelope
	 jmp	Do_Sound
Do_Custom_Sound  endp

.CODE
include syn_read.inc
include env.inc

.DATA
;disabled
Sound_Gen_Proc_Table:
                  db  Sin_Wave_Func-offset Sound_Gen_Proc_Base_Offset
	          db  0   ;Square_Wave_Func
		  db  Saw_Wave_Func-offset Sound_Gen_Proc_Base_Offset
		  db  0   ;Triangle_Wave_Func
		  db  Noise_Wave_Func-offset Sound_Gen_Proc_Base_Offset

Noise_Norm_Coef	dd	2.3283064370807973754314699618685e-10  ;1/0FFFFFFFFh/2

SAMPLE_RATE		dw	SAMPLE_RATE_
Max_Sample_Val_2	dw	MAX_SAMPLE_VAL2

.DATA?
; disabled
;Harm_Func_Value		dd	?
;Cur_Harmonizer_Ptr	dd	?
;Harmonizers		db	SIZE HARMONIZER_REC*MAX_OSC_NUM dup(?)
;Seq_Notes_Cyc_Num dd  ?
;Sustain_Coef    dd  ?
