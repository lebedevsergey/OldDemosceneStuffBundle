FILT_FREQ_ENV_MAX_VALUE	equ	2000
GAIN_ENV_MAX_VALUE	equ	50
Q_ENV_MAX_VALUE		equ	10

ALL_DATA_REC	struc

Ready_Sample_Ptr		dd	?
Seq_Amp_Env_Ptr			dd	?
Seq_Freq_Env_Ptr		dd	?
Seq_Note_Amp_Env_Ptr		dd	?
;!!!!!!!!!!!!!! izvrat
				dd	?
				dd	?

	Tmp2_2		dd	?	
	Cur_Note	dd	?

	Cur_Note_Frq	dd	?

	Len_Mul		dd	?
	Env_Max_Amp	dd	?
	Env_Len		dd	?
	Env_Start_Pos	dd	?
	Env_End_Pos	dd	?
	SAMPLE_SIZE	dd	?
	Note_Count	dd	?
	Attack_Coef     dd	?
	Decay_Coef      dd	?

	Attack_Len      dd	?
	Decay_Len       dd	?
	Sustain_Len     dd	?
	Last_ESI	dd	?

	seed		dd	?
	Int_Note_Len	dd	?

	Seq_Notes_Cyc_Len	dd	?

	Cur_Sustain_Ptr dd  ?

Cur_Note_Pos	dd	?
Act_Osc_Num      dd   ?
;Cur_Sound         dd   ?
Cur_Angle         dd   ?


Act_Osc_Ptrs     dd   MAX_OSC_NUM dup (?)
Oscillators      db SIZE OSCILLATOR*MAX_OSC_NUM dup(?)
;Oscillators_Size equ $-Oscillators

	O1		dd	?
	O2		dd	?
;	A0		dd	?
	A1		dd	?
	A2		dd	?
	B0		dd	?
	B1		dd	?
	I1		dd	?
	I2		dd	?
	CS_		dd	?
;	SN_		dd	?
	Alpha		dd	?
;	Gain		dd	?
	Gain_		dd	?
;	Q		dd	?
;	Frequency	dd	?

	Seq_Note_Env_Nodes      ENVELOPE <?>
	Envelope_Nodes          ENVELOPE <?>

	Tmp_Env1	ENVELOPE	<?>
	Tmp_Env2	ENVELOPE	<?>
	Tmp_Env3	ENVELOPE	<?>

	Delay2_Feedback	dd	?
	Delay2_Delay1	dd	?
	Delay2_Delay2	dd	?
	Delay2_Step	dd	?
	Uni_Filter_Type	db	?
	Sample_Notes_Len	dd	?
	Freq_Tab	dd	OCTAVES_NUM*TONES_NUM dup (?)

;	Tmp_Attack_Len	dd	?
;	Tmp_Decay_Len	dd	?
;	Tmp_Sustain_Len	dd	?
;	Tone		dd	?
;	Pitched_Note_Len	dd	?
;	BPM_Tempo	dd	?
;	BPM_Speed	dd	?
;	Note_Env_State	dd	?
ENDS

INIT_DATA_REC	struc
TONECONST	dd	TONECONST_
C_5_FREQ	dd	C_5_FREQ_
Oct		dd	0.03125		;  1/32
Max_Sample_Val_		dw	MAX_SAMPLE_VAL
Min_Sample_Val_		dw	-MAX_SAMPLE_VAL
Gain_Coef1       dw    10
Gain_Coef2       dw    20
_2		dw	2
Filt_Rate_Coef	dd	2.84951714611319114599786247916508e-4 ;2PI/SAMPLE_RATE
Flanger_Freq_Coef1	dd	0.9
Flanger_Freq_Coef3	dd	7.8740157480315e-4 ;1/Percent3*Flanger_Freq_Coef2

ENDS

.DATA
Init_Data	INIT_DATA_REC	<TONECONST_,C_5_FREQ_,0.03125,MAX_SAMPLE_VAL,\
-MAX_SAMPLE_VAL,10,20,2,\
2.84951714611319114599786247916508e-4,0.9,7.8740157480315e-4>

;filter.inc
.CODE
locals

Uni_Filter proc
;!!!!!!!!!!! eax=0

		lodsb
		mov	[ebp].Uni_Filter_Type,al

;disabled
;         lodsb
 ;        mov Uni_Filter_Order,eax

	lea	edi,[ebp].Tmp_Env1
	mov	eax,SIZE ENVELOPE
	mov	edx,FILT_FREQ_ENV_MAX_VALUE
	call	Init_Envelope_Deb
	mov	edx,GAIN_ENV_MAX_VALUE
	call	Init_Envelope_Deb
	mov	edx,Q_ENV_MAX_VALUE
	call	Init_Envelope_Deb

	push	esi
;disabled
;         mov	ecx,Uni_Filter_Order
;@@Do:
;	 push	ecx

        mov	esi,[ebp].Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
;push edi
;        CLEAR_SAMPLE

	lea  edi,[ebp].O1  	;Filt_Null_Data
	mov  ecx,16 		;Filt_Null_Data_End
	xor  eax,eax
	rep  stosd	
;pop esi
;	mov	esi,[ebp].Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
	mov	edi,[ebp].Ready_Sample_Ptr
pushad
        mov	eax,dword ptr [edi]
        mov	[ebp].I1,eax
        mov	[ebp].O1,eax
        mov	eax,dword ptr [edi+4]
        mov	[ebp].I2,eax
        mov	[ebp].O2,eax

;        xor	ecx,ecx
@@Do_Filter:

;        add edi,4
 ;       add esi,4
	cmpsd

	fld	dword ptr [edi]

	cmp	[ebp].Uni_Filter_Type,1
	jnz	@@No_Filt

pushad
	lea	edi,[ebp].Tmp_Env1
	mov	eax,SIZE ENVELOPE
	call	Get_Envelope_Deb
;        fstp	[ebp].Frequency
		fmul	[ebx].Filt_Rate_Coef
;                fmul	[ebp].Frequency
                fsincos
                fstp	[ebp].CS_
;               fstp	[ebp].SN_
		fidiv	[ebx]._2

		add	edi,eax
		call	Get_Envelope_Deb
                fidiv	[ebx].Gain_Coef2
                fild	[ebx].Gain_Coef1
                Fpow
        	fstp	[ebp].Gain_

		add	edi,eax
		call	Get_Envelope_Deb
;        	fstp	[ebp].Q
;                fdiv	[ebp].Q

		fdivp	st(1),st
                fst	[ebp].Alpha

                fld1
		faddp	st(1),st
                fld1
;                fadd	[ebp].Alpha
                fdivrp	st(1),st

;                fst	[ebp].A0
		fld	st
                fmul	[ebp].CS_
		fadd	st,st
                fstp	[ebp].A1

                fld1
                fsub	[ebp].Alpha
;                fmul	[ebp].A0
		fmul	st,st(1)
                fstp	[ebp].A2

                fld1
                fsub	[ebp].CS_
;                fmul	[ebp].A0
		fmulp	st(1),st

                fld	[ebp].Gain_
		fmulp	st(1),st

                fst	[ebp].B1

		fidiv	[ebx]._2
                fst	[ebp].B0
		fmulp	st(1),st

                fld	[ebp].I1
                fmul	[ebp].B1
                faddp	st(1),st
                fld	[ebp].I2
                fmul	[ebp].B0
                faddp	st(1),st
                fld	[ebp].O1
                fmul	[ebp].A1
                faddp	st(1),st
                fld	[ebp].O2
                fmul	[ebp].A2
                fsubp	st(1),st
popad
@@No_Filt:

IFDEF DEBUG_VER
  Sample_Cut_Max_Min
ENDIF
        	fst	dword ptr [esi]
        	fld	[ebp].O1
        	fstp	[ebp].O2
        	fstp	[ebp].O1

		mov	eax,[edi]
		xchg	[ebp].I1,eax
		mov	[ebp].I2,eax

		inc	ecx
		cmp	ecx,[ebp].SAMPLE_SIZE
		jb	@@Do_Filter

popad
;        mov  esi,[ebp].Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
 ;       mov  edi,[ebp].Ready_Sample_Ptr

        COPY_SAMPLE

;disabled
;	lea	edi,[ebp].Tmp_Env1
;	call	Clear_Envelope
;	lea	edi,[ebp].Tmp_Env2
;	call	Clear_Envelope
;	lea	edi,[ebp].Tmp_Env3
;	call	Clear_Envelope
;	pop  ecx
;	dec  ecx
;	jnz  @@Do

	pop	esi

        ret
endp

Delay2  proc
;	pushad

	Get_Percent_
	fstp  [ebp].Delay2_Feedback
        lodsb           ; Берем операнд
        mov   [ebp].Delay2_Step,eax

IFDEF	DEBUG_VER
	cmp   eax,Note_Count
	jae   @@ex
ENDIF

	fild	[ebp].Delay2_Step
	fild	[ebp].Note_Count
;	fidiv	Seq_Notes_Cyc_Num
fidiv	[ebp].Len_Mul
	fsub	st,st(1)
;	fisub	[ebp].Delay2_Step
	fimul	[ebp].Int_Note_Len
	fistp	[ebp].Delay2_Delay2

;	fild	[ebp].Delay2_Step
	fimul	[ebp].Int_Note_Len
	fistp	[ebp].Delay2_Delay1

	mov	edi,[ebp].Ready_Sample_Ptr
;	mov	ecx,Seq_Notes_Cyc_Num  ; Задержку делаем по циклам
	mov	ecx,[ebp].Len_Mul
@@Notes_Cyc_Cyc:
	xor	edx,edx
@@Notes_Cyc:
        fld	dword ptr [edi+edx*4]
	mov	eax,edx
	sub	eax,[ebp].Delay2_Delay1
	jns	@@Ok
	mov	eax,[ebp].Delay2_Delay2
	add	eax,edx
@@Ok:
        fld	dword ptr [edi+eax*4]
	fmul	[ebp].Delay2_Feedback

	faddp	st(1),st

        fstp   dword ptr [edi+edx*4]

	inc	edx
        cmp	edx,[ebp].Seq_Notes_Cyc_Len
        jb	@@Notes_Cyc

	mov	eax,[ebp].Seq_Notes_Cyc_Len
	shl	eax,2
	add	edi,eax

	loop  @@Notes_Cyc_Cyc
@@ex:	
;	popad
;	inc esi
;	inc esi
        ret
endp

.DATA?
All_Data	ALL_DATA_REC	<?>