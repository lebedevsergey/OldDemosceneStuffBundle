; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

; sequencer notes generation code

.CODE

MAX_HARMONICS_NUM  equ  10

HARMONIC_REC   struc
	HARMONIC_FREQ_MUL          dd  ?
	HARMONIC_AMP_MUL           dd  ?
ends

HARMONIZER_REC   struc
  Harmonics_Num  dd  ?
  Harmonics      db  MAX_HARMONICS_NUM*SIZE HARMONIC_REC dup(?)
ends

GET_HARMONIZER_ADDR macro  REG_      ; eax - harm num
		lea  REG_,Harmonizers
		imul eax,SIZE HARMONIZER_REC
		add  REG_,eax
endm

Setup_Harmonizer  proc 
	pushad
	xor  eax,eax
	lodsb
	GET_HARMONIZER_ADDR edi

;        mov  [edi].Harmonics_Num,0
	lea  edx,[edi].Harmonics
@@tt1:
	xor eax,eax
	mov eax,[esi]
	cmp eax,NULL_SYM
        jz @@Harmonics_End
	lodsd
	mov [edx].HARMONIC_FREQ_MUL,eax
	lodsd
	mov [edx].HARMONIC_AMP_MUL,eax
        inc [edi].Harmonics_Num
	add edx,SIZE HARMONIC_REC
	jmp @@tt1
@@Harmonics_End:
	mov Last_ESI,esi	
	popad
	mov esi,Last_ESI	
	add esi,4
	retn
endp

Harmonizer  proc
	pushad

        mov  Harm_Func_Value,0
	mov  ebx,Cur_Harmonizer_Ptr
	mov  ecx,[ebx].Harmonics_Num
	lea  ebp,[ebx].Harmonics
@@tt1:

	fld  Cur_Angle
fmul Cur_Flanger_Coef
	fmul [ebp].HARMONIC_FREQ_MUL

	fld1 
	fxch st(1)
	fprem
	fxch st(1)
	fstp  st

	mov  eax,Cur_Wave_Func
	call eax
@@Add:
	fmul [ebp].HARMONIC_AMP_MUL

	fadd Harm_Func_Value
	fstp Harm_Func_Value

	add  ebp,SIZE HARMONIC_REC
	dec  ecx
	jnz  @@tt1

	fld   Harm_Func_Value
;	fidiv [ebx].Harmonics_Num

	popad
	retn
Harmonizer  endp

Clear_Harmonizer  proc 
	pushad

        lea  edi,Harmonizers

        mov  ecx,MAX_OSC_NUM
@@Harm_Cyc1:
	mov  [edi].Harmonics_Num,1
	lea  edx,[edi].Harmonics
	fld1
	fst  [edx].HARMONIC_FREQ_MUL
	fstp [edx].HARMONIC_AMP_MUL

	add edx,SIZE HARMONIC_REC

	mov  ebx,MAX_HARMONICS_NUM-1
	xor  eax,eax	
@@Harm_Cyc2:
	mov  [edx].HARMONIC_FREQ_MUL,eax
	mov  [edx].HARMONIC_AMP_MUL,eax
	add  edx,SIZE HARMONIC_REC
	dec  ebx
	jnz  @@Harm_Cyc2

	add  edi,SIZE HARMONIZER_REC
	loop @@Harm_Cyc1

	popad
	retn
Clear_Harmonizer endp

;freqtab.inc
.CODE
locals

CHK_ERR1  equ  1

NOTE_PITCH_SYM         equ  'P'
NOTE_SUSTAIN_SYM       equ  'S'
NOTE_PITCH_SUSTAIN_SYM equ  'Q'
EMPTY_NOTE_SYM         equ  'ENON'

OCTAVES_NUM     equ  10
TONES_NUM       equ  12

TONECONST_      equ  1.059463094     ; 2^(1/12)
C_5_FREQ_       equ  261.625458      ;C_5 frequency
					; CDEFGAB + #

.CODE
locals
	
upcase  macro par
	and par,0DFh	
endm

Do_Freq_Tab proc 
	pushad

	finit

	fld1
	mov Tmp1,32
	fidiv Tmp1
	fstp Oct

	lea edi,Freq_Tab   ; i

	xor ecx,ecx     ; j
@@tt1:
	fld1
	fstp Tone

	xor ebp,ebp    ; k
@@tt2:
	fld Oct
	fmul Tone
	fmul C_5_FREQ
	fstp dword ptr [edi]
	add edi,4
	
	fld Tone
	fmul TONECONST
	fstp Tone

	inc ebp
	cmp ebp,TONES_NUM
	jb @@tt2

	fld Oct
	mov Tmp1,2
	fimul Tmp1
	fstp Oct

	inc ecx
	cmp ecx,OCTAVES_NUM
	jb  @@tt1

	popad
	retn
endp

Interp_Note proc 
; IN  : EAX: note sign
; OUT : EAX - octave
;       EBX - tone

	mov ebx,eax	

bswap ebx
mov Note_Pitch_Flg,0
upcase bl
cmp bl,NOTE_PITCH_SUSTAIN_SYM
jz @@tt7
cmp bl,NOTE_PITCH_SYM
jnz @@tt6
@@tt7:
mov Note_Pitch_Flg,1
@@tt6:

	mov ebx,eax	
upcase bl
	sub bl,'A'	
	and ebx,0FFh

IFDEF CHK_ERR1
.DATA
Err_Msg1  db  'Wrong note',0Dh,0Ah,'$'
.CODE
lea edx,Err_Msg1
cmp ebx,TONES_NUM
ja sound_err
ENDIF

	mov bl,Tones[ebx]
	cmp ah,'#'
	jnz @@tt1
	inc bl
@@tt1:
	shr eax,16
	sub al,'0'
	and eax,0FFh

IFDEF CHK_ERR1
.DATA
Err_Msg2  db  'Wrong octave',0Dh,0Ah,'$'
.CODE
lea edx,Err_Msg2
cmp eax,OCTAVES_NUM
ja sound_err
ENDIF

	retn
endp

.DATA
;int   tones[]={9,11,0,2,4,5,7};
Tones  db 9,11,0,2,4,5,7

.CODE
Get_Note_Freq proc 
	pushad

; EAX: note obozn
	call Interp_Note
; EAX: octave
; EBX: tone

	imul eax,TONES_NUM
	add  eax,ebx
	fld  Freq_Tab[eax*4]

	popad
	retn
endp

IFDEF CHK_ERR1
sound_err:
	mov ah,09h
	int 21h
	mov ah,4ch
	int 21h
ENDIF

.DATA
TONECONST  dd  TONECONST_
C_5_FREQ   dd   C_5_FREQ_

.DATA?
Oct  	  dd   ?
Tone  	  dd   ?
Freq_Tab  dd   OCTAVES_NUM*TONES_NUM dup (?)

;oscill.inc
.CODE
locals
MAX_OSC_SHAPE_PARAM  equ 100

MAX_OSC_NUM  equ  5

SIN_OSC     equ    0
SQU_OSC     equ    1
SAW_OSC     equ    2
TRI_OSC     equ    3
NOISE_OSC   equ    4

OSCILLATOR  struc
Osc_Type           dd  ?
Osc_Harmonizer_Num dd  ?
Osc_Mix_Coef       dd  ?
Osc_Shape_Param    dd  ?
Osc_Harmonizer_Ptr dd  ?
Flanger_Freq_Coef  dd  ?
	    ends

GET_OSCILL_ADDR macro  REG_      ; eax - oscill num
		lea  REG_,Oscillators
		imul eax,SIZE OSCILLATOR
		add  REG_,eax
endm


Set_Cust_Wave  proc  
	 pushad

	 lodsb
         mov    edx,1
         lea    edi,Envelope_Nodes
         call   Setup_Envelope
	 mov    Last_ESI,esi

         mov    ecx,PRECALC_WAVE_LEN+1
         lea    esi,Envelope_Nodes
         lea    edi,Cust_Wave
         call   Calc_Envelope

	 popad
	 mov   esi,Last_ESI
	 retn
Set_Cust_Wave  endp

Setup_Oscillator  proc  
	    pushad
	    xor  eax,eax
	    lodsb  		; Берем номер осциллятора
	    push eax	
	    GET_OSCILL_ADDR edi
	    xor  eax,eax
	    lodsb  		; Берем тип осциллятора
	    mov  [edi].Osc_Type,eax
	    lodsd  		; Берем коэф. микширования
	    mov  [edi].Osc_Mix_Coef,eax	   
	    xor  eax,eax
	    lodsb  		; Берем частоту расстройки
	    mov  Tmp2,eax
	    fild Tmp2
	    fidiv Percent3
	    fmul  Flanger_Freq_Coef2
	    fadd  Flanger_Freq_Coef1
	    fstp  [edi].Flanger_Freq_Coef
	    lodsb  		; Берем  shape param
	    mov   Tmp1,ax
	    fild  Tmp1
	    fidiv Percent
	    fstp  [edi].Osc_Shape_Param

	    pop  eax	
     	    GET_HARMONIZER_ADDR ebp
	    mov  [edi].Osc_Harmonizer_Ptr,ebp

	    mov  eax,Act_Osc_Num   ; Добавляем осц. в список
	    mov  Act_Osc_Ptrs[eax*4],edi
	    inc  Act_Osc_Num

                                   ; Инитим осциллятор
	    mov  eax,[edi].Osc_Type
	    mov  eax,dword ptr Init_Sound_Gen_Proc_Table[eax*4]
	    call eax

	    popad
	    add  esi,8
	    retn
Setup_Oscillator  endp

.DATA
Flanger_Freq_Coef1  dd 0.9
Flanger_Freq_Coef2  dd 0.2
MAX_OSC_SHAPE_PARAM_ dd MAX_OSC_SHAPE_PARAM

.DATA?
Oscillators:
        db SIZE OSCILLATOR*MAX_OSC_NUM dup(?)
Oscillators_Size equ $-Oscillators

Act_Osc_Ptrs     dd   MAX_OSC_NUM dup(?)
Act_Osc_Num      dd   ?

;sequenc.inc
NOTE_ENV_SUBDIV_NUM  equ  3

.CODE
locals

Set_A_D_Coef     proc
		 xor  eax,eax
		 lodsb		
		 mov  Tmp1,ax
		 fild Tmp1
		 fidiv Percent
		 fstp Attack_Coef
		 lodsb		
		 mov  Tmp1,ax
		 fild Tmp1
		 fidiv Percent
		 fstp Decay_Coef

		 retn
Set_A_D_Coef     endp

Set_Pitch_Speed  proc
		 xor  eax,eax
		 lodsb		
		 mov  Pitch_Speed,eax
		 retn
Set_Pitch_Speed  endp

Do_Note_Picht proc
	pushad

        mov Note_Pitch_Rate,0

	cmp dword ptr [esi],NULL_SYM
	jz @@tt1
	cmp dword ptr [esi],EMPTY_NOTE_SYM
	jz  @@tt1

	cmp   Pitch_Type_Flg,1
	jz    @@Do_Pitch_Type2

	fld   st
	fsub  Prev_Note_Frq
	fidiv Note_Len
	fimul Pitch_Speed
	fstp  Note_Pitch_Rate

	fld   Prev_Note_Frq
	fxch  st(1)
	fstp  Prev_Note_Frq
@@tt1:
	popad
	retn

@@Do_Pitch_Type2:
	mov  eax,Note_Len
	mov  Pitched_Note_Len,eax
	fld   st
	mov eax,[esi]
	call Get_Note_Freq
	fsubp st(1),st
	fchs
	fidiv Note_Len
	fstp  Note_Pitch_Rate
	jmp  @@tt1
Do_Note_Picht endp

Sequencer   proc 
        pushad
        xor  eax,eax
	lodsb
	mov  Seq_Notes_Cyc_Num,eax

        lea  edi,Seq_Note_Env_Nodes
        mov  edx,AMP_ENV_MAX_VALUE
        call Setup_Envelope

        mov   edx,Seq_Amp_Env_Ptr
                       ; Указатель на огибающую нот по амплитуде
        mov   edi,Seq_Freq_Env_Ptr
                        ; Указатель на огибающую нот по частоте
        push edx
        push edi            ; Чистим все это
        CLEAR_SAMPLE
        mov  edi,edx
        CLEAR_SAMPLE
        pop  edi

        push esi

        mov Note_Count,0
@@Calc_Notes:
        lodsd
        cmp eax,NULL_SYM
        jz  @@tt0_
        inc Note_Count
        jmp @@Calc_Notes
@@tt0_:
        fild   SAMPLE_SIZE
        fidiv  Note_Count
        fst    Note_Len2
        fistp  Note_Len

        fild   SAMPLE_SIZE
	fidiv  Seq_Notes_Cyc_Num
	fistp  Seq_Notes_Cyc_Len


        call   Init_Note_Envelopes

	fild   Note_Len
	fidiv  Pitch_Speed
	fistp  Pitched_Note_Len

        pop    esi
        push   esi

	xor    ebp,ebp
	mov    Prev_Note_Frq,0
@@Frq_Next_Note:
	inc  ebp

        lodsd
        cmp    eax,NULL_SYM
        jz     @@Notes_End1

	cmp eax,EMPTY_NOTE_SYM
	jz  @@Do_Empty_Note1
; EAX - note obozn
        call Get_Note_Freq

        cmp  Note_Pitch_Flg,1
        jnz  @@Do_No_Pitch
	cmp  ebp,1        ; Если нота - первая, то предыд. нету и питча - тоже
        jz   @@Do_No_Pitch

        call Do_Note_Picht	

        xor  ecx,ecx
@@tt3:
        fst  dword ptr [edi]
	cmp  ecx,Pitched_Note_Len
	jnb  @@No_Rate_Add
        fadd Note_Pitch_Rate
@@No_Rate_Add:
        add edi,4
	inc ecx
	cmp  ecx,Note_Len
        jnz @@tt3
        fstp st

        jmp @@Frq_Next_Note

@@Notes_End1:

        pop esi
        pop edx

@@Next_Note2:
        mov eax,[esi]
        cmp eax,NULL_SYM
        jz  @@Notes_End

cmp eax,EMPTY_NOTE_SYM
jz  @@Do_Empty_Note2

        mov Sustain_Notes_Count,0
@@Next_Sustain_Note:
        mov eax,[esi]
        cmp eax,NULL_SYM
        jz  @@Notes_End2        
cmp eax,EMPTY_NOTE_SYM        
jz  @@Notes_End2
        add esi,4
        inc Sustain_Notes_Count

        bswap eax
        upcase al
        cmp al,NOTE_SUSTAIN_SYM
        jz @@Next_Sustain_Note

        cmp al,NOTE_PITCH_SUSTAIN_SYM
        jz @@Next_Sustain_Note
@@Notes_End2:
        fld   Note_Len2
        fimul Sustain_Notes_Count
        fistp Cur_Note_Len

        mov  Note_Env_State,0
        mov  Cur_Sustain_Ptr,0

        mov  ecx,Cur_Note_Len
	xor  ebp,ebp
@@One_Sustain_Amp_Env:
	call Get_Note_Env_Val
        mov  [edx],eax
        add  edx,4
        inc  ebp
        dec  ecx
        jnz @@One_Sustain_Amp_Env

        jmp @@Next_Note2

@@Notes_End:

        mov Last_ESI,esi

        call Do_Sound

        popad
        mov esi,Last_ESI
        add esi,4
        retn

@@Do_Empty_Note1:
        mov eax,Note_Len
        shl eax,2
        add edi,eax
        jmp  @@Frq_Next_Note
@@Do_No_Pitch:
	fstp Prev_Note_Frq
        mov  ecx,Note_Len
	mov  eax,Prev_Note_Frq
	rep  stosd
        jmp  @@Frq_Next_Note

@@Do_Empty_Note2:
        mov eax,Note_Len
        shl eax,2
        add edx,eax
        add esi,4
        jmp  @@Next_Note2
Sequencer   endp

.DATA
Pitch_Type_Flg      db   0

.DATA?
Note_Count  dd   ?
Note_Len    dd   ?
Note_Len2   dd   ?
Cur_Note_Len dd   ?
Note_Pitch_Flg db  ?
Note_Pitch_Rate dd  ?
Sustain_Notes_Count dw  ?
Cur_Note_Envelope_End dd  ?
Pitched_Note_Len   dd  ?
Pitch_Speed  dd  ?
Prev_Note_Frq dd ?

.CODE
Init_Note_Envelopes   proc
       pushad

       fld1
       fsub     Attack_Coef
       fsub     Decay_Coef
       fst      Sustain_Coef
       fimul	Note_Len	
       fist    Sustain_Len
       fimul    NOTE_ENV_SUBDIV_NUM_
       fistp    @@Tmp_Sustain_Len

       fld     Attack_Coef
       fimul     Note_Len
       fist    Attack_Len
       fimul    NOTE_ENV_SUBDIV_NUM_
       fistp    @@Tmp_Attack_Len

       fld     Decay_Coef
       fimul     Note_Len
       fist    Decay_Len       
       fimul    NOTE_ENV_SUBDIV_NUM_
       fistp    @@Tmp_Decay_Len

       mov      ecx,@@Tmp_Attack_Len
       lea      esi,Seq_Note_Env_Nodes
       mov      edi,Seq_Note_Amp_Env_Ptr
	xor	eax,eax
	mov	ebx,Attack_Len
       call     Calc_Envelope_From_To_Pos

       mov      ecx,@@Tmp_Sustain_Len
       lea      esi,Seq_Note_Env_Nodes
       mov      edi,Seq_Note_Amp_Env_Ptr
	mov	eax,Attack_Len
	lea	edi,[edi+eax*4]

	mov	eax,Sustain_Len
	mov	ebx,eax
	add	ebx,Sustain_Len
       call     Calc_Envelope_From_To_Pos

       mov      ecx,@@Tmp_Decay_Len
       lea      esi,Seq_Note_Env_Nodes
       mov      edi,Seq_Note_Amp_Env_Ptr
	mov	eax,Attack_Len
	add	eax,Sustain_Len
	lea	edi,[edi+eax*4]

	mov	eax,@@Tmp_Decay_Len
	mov	ebx,eax
	sub	eax,Decay_Len
       call     Calc_Envelope_From_To_Pos

       popad
       retn
.DATA?
@@Tmp_Attack_Len	dd	?
@@Tmp_Decay_Len		dd	?
@@Tmp_Sustain_Len	dd	?
.CODE
Init_Note_Envelopes   endp

Get_Note_Env_Val   proc
; ebp - inc count
; ecx - dec count
       pushad
mov	edi,Seq_Note_Amp_Env_Ptr

       mov     eax,Note_Env_State
       jmp     @@Jmp_Tab1[eax*4]
@@Attack:
       cmp     ebp,Attack_Len
       jb      @@Ok2
       mov     Note_Env_State,1
       jmp     @@Sustain
@@Ok2:
       mov     eax,[ebp*4+edi]
       jmp     @@Done
@@Sustain:
       cmp     ecx,Decay_Len
       ja      @@Ok3
       mov     Note_Env_State,2
       jmp     @@Decay
@@Ok3:
       mov     ebx,Cur_Sustain_Ptr
       mov	eax,Attack_Len
	add	eax,ebx
       mov     eax,[eax*4+edi]
       inc     Cur_Sustain_Ptr
       cmp     ebx,Sustain_Len
       jb      @@Ok
       mov     Cur_Sustain_Ptr,0
@@Ok:
       jmp     @@Done
@@Decay:
       mov	eax,Attack_Len
       add	eax,Sustain_Len
       add	eax,Decay_Len
       sub	eax,ecx
       mov     eax,[eax*4+edi]
@@Done:
       mov     Tmp2,eax
       popad
       mov   eax,Tmp2
       retn
.DATA
@@Jmp_Tab1    dd @@Attack
	      dd @@Sustain
	      dd @@Decay
.DATA?
Note_Env_State  dd  ?
.CODE
Get_Note_Env_Val   endp

.DATA
NOTE_ENV_SUBDIV_NUM_  dw  NOTE_ENV_SUBDIV_NUM

.DATA?
Seq_Notes_Cyc_Num dd  ?
Seq_Notes_Cyc_Len dd  ?

Sustain_Coef    dd  ?
Attack_Len      dd  ?
Decay_Len       dd  ?
Sustain_Len     dd  ?
Cur_Sustain_Ptr dd  ?
Attack_Coef     dd  ?
Decay_Coef      dd  ?

;sound.inc
.CODE
locals

SHAPE_POW_COEF  equ 30

Init_Pow proc
; edi - osc rec ptr
	fld [edi].Osc_Shape_Param
	fld1
	fcomp st(1)
        fstsw ax
        sahf
        jae   @@No_Change
	fimul SHAPE_POW_COEF_
	fisub SHAPE_POW_COEF_
@@No_Change:
	 fstp [edi].Osc_Shape_Param
	 retn
.DATA
SHAPE_POW_COEF_  dw  SHAPE_POW_COEF
Init_Pow endp

Do_Pow  proc
	push eax
	mov  @@Sign,1
	ftst
	fstsw ax
	sahf
	jz   @@No_Pow
	ja   @@No_Sign
	fabs
	mov  @@Sign,-1
@@No_Sign:
	fld Shape_Param
	fxch st(1)
	Fpow
	fimul @@Sign
@@No_Pow:
	pop  eax
	retn
.DATA?
@@Sign dw  ?
.CODE
Do_Pow  endp

Init_Waves  proc
	xor ecx,ecx
	mov Cur_Angle,0
	fld1
	fidiv PRECALC_WAVE_LEN_
	fstp Cur_Angle_Adder
@@Do_Wave1:
	fld  Cur_Angle	
	fadd Cur_Angle_Adder
	fstp Cur_Angle	
	
	inc ecx
	cmp ecx,PRECALC_WAVE_LEN+1
	jna @@Do_Wave1

	retn
Init_Waves  endp

Sin_Wave_Func  proc
	fmul _2PI
        fsin
	call Do_Pow
	retn
Sin_Wave_Func  endp

Square_Wave_Func  proc
	push   eax
        fld  Period_3
	fmul Shape_Param
	fcomp st(1)
        fstsw ax
        sahf
	fstp  st
	fld1
        jae   @@tt2
        fchs
@@tt2:
	pop   eax
	retn
.CODE
Square_Wave_Func  endp

Triangle_Wave_Func  proc
	push  eax

	fld  st
        fcomp  Period_2
        fstsw ax
        sahf
        ja    @@tt2
fadd st,st
fadd st,st
	call Do_Pow
	jmp   @@Done
@@tt2:
	fld  st
        fcomp  Period_3
        fstsw ax
        sahf
        ja    @@tt3
        fsub  Period_2
fadd st,st
fadd st,st
	call Do_Pow
        fld1
	fxch  st(1)
	fsubp st(1),st
	jmp   @@Done
@@tt3:
	fld  st
        fcomp  Period_4
        fstsw ax
        sahf
        ja    @@tt4
        fsub  Period_3
fadd st,st
fadd st,st
	call Do_Pow
	fchs
	jmp   @@Done
@@tt4:
        fsub  Period_4
fadd st,st
fadd st,st
	call Do_Pow
        fld1
	fxch  st(1)
	fsubp st(1),st
        fchs
@@Done:
	pop  eax
	retn
Triangle_Wave_Func  endp

Saw_Wave_Func  proc
	call Do_Pow
	fadd st,st
	fld1
	fsubp st(1),st
	retn
Saw_Wave_Func  endp  

TB_Saw_Wave_Func  proc
	fld  st
        fcomp  Period_2
        fstsw ax
        sahf
        ja    @@tt1
	fstp  st
	fld1
	fchs
	jmp   @@Done
@@tt1:
	fld  st
        fcomp  Period_3
        fstsw ax
        sahf
        ja    @@tt2
        fsub  Period_2
	fadd st,st
	fadd st,st
	call Do_Pow
	fld1
	fsubp st(1),st
	jmp   @@Done
@@tt2:
	fld  st
        fcomp  Period_4
        fstsw ax
        sahf
        ja    @@tt3
        fsub  Period_3
	fadd st,st
	fadd st,st
	call Do_Pow
	jmp   @@Done
@@tt3:
	fstp  st
	fld1
@@Done:
	retn
TB_Saw_Wave_Func  endp

Cust_Wave_Func  proc
	push  eax	
	fimul PRECALC_WAVE_LEN_

	fistp Tmp2
	mov   eax,Tmp2
	fld   dword ptr Cust_Wave[eax*4]
        pop  eax
	retn
Cust_Wave_Func  endp

TBL_Saw_Wave2_Func  proc
	fld st
	fmul st,st
	fld st
	fmul st,st
	faddp st(1),st
	faddp st(1),st
	fdiv @@Coef1
                
	fld1
	fsubp st(1),st

	retn
.DATA
@@Coef1  dd 1.5
	retn
TBL_Saw_Wave2_Func  endp  

Noise_Wave_Func  proc
fstp st	
        mov  eax,0FFFFFFFFh
        call Random

	mov  Tmp1,ax
	fild  Tmp1
	fmul Noise_Norm_Coef
	retn
Noise_Wave_Func  endp

Do_Sound proc
	 pushad

         cmp   Act_Osc_Num,0
         jz    @@ex

	 mov   eax,0
	 lea   eax,Sound_Gen_Proc_Table[eax*4] 	
	 mov   edx,Seq_Amp_Env_Ptr
	 mov   ebp,Seq_Freq_Env_Ptr
	 mov   edi,Ready_Sample_Ptr

         mov   Cur_Angle,0
         xor   ecx,ecx
@@Do_Wave: 
         fldz
	 fstp  Cur_Sound

         mov   ebx,Act_Osc_Num
         dec   ebx
@@Sum_Osc:
	 mov   esi,Act_Osc_Ptrs[ebx*4]

	 mov   eax,[esi].Osc_Harmonizer_Ptr
	 mov   Cur_Harmonizer_Ptr,eax
	 mov   eax,[esi].Flanger_Freq_Coef
	 mov   Cur_Flanger_Coef,eax
	 mov   eax,[esi].Osc_Shape_Param
	 mov   Shape_Param,eax

	 mov   eax,[esi].Osc_Type	
	 mov   eax,dword ptr Sound_Gen_Proc_Table[eax*4] 		
	 mov   Cur_Wave_Func,eax
	 call  Harmonizer             ;  Генерим
         fmul dword ptr [edx]     ; Огибающая по амплитуде
         fmul  [esi].Osc_Mix_Coef
         fadd  Cur_Sound
         fstp  Cur_Sound
         dec   ebx
         jns   @@Sum_Osc

         fld   Cur_Sound
         fstp  dword ptr [edi]

	 fld   dword ptr [ebp]    ; Огибающая по частоте
         fmul  Freq_Coef
         fadd   Cur_Angle
         fstp  Cur_Angle

	 add edx,4
         add edi,4
	 add ebp,4

         inc ecx  
         cmp ecx,SAMPLE_SIZE
         jb  @@Do_Wave
@@ex:
	 popad

         retn
Do_Sound endp

Do_Custom_Sound  proc
	 pushad
	 mov   edi,Seq_Amp_Env_Ptr
         mov   edx,AMP_ENV_MAX_VALUE
	 call  Do_Envelope
	 mov   edi,Seq_Freq_Env_Ptr
         mov   edx,FREQ_ENV_MAX_VALUE
	 call  Do_Envelope

	 mov   Last_ESI,esi
	 call  Do_Sound

	 popad
	 mov   esi,Last_ESI
         retn
Do_Custom_Sound  endp

.DATA
Init_Sound_Gen_Proc_Table:
                  dd  Init_Pow
	          dd  Empty
		  dd  Init_Pow
		  dd  Init_Pow
		  dd  Empty
		  dd  Init_Pow
		  dd  Empty
		  dd  Empty

Sound_Gen_Proc_Table:
                  dd  Sin_Wave_Func
	          dd  Square_Wave_Func
		  dd  Saw_Wave_Func
		  dd  Triangle_Wave_Func
		  dd  Noise_Wave_Func
		  dd  TB_Saw_Wave_Func
		  dd  Cust_Wave_Func
		  dd  TBL_Saw_Wave2_Func
.DATA
;Period_1  dd   0.0
Period_2  dd   0.25
Period_3  dd   0.5
Period_4  dd   0.75
Shape_Param_Deb dd  1.0

.DATA?
Noise_Norm_Coef   dd   ?
Shape_Param       dd   ?
Cur_Sound         dq   ?
Cur_Flanger_Coef  dd   ?
Cur_Wave_Func     dd   ?
Cur_Angle         dd   ?
Cur_Angle_Adder   dd   ?

Cust_Wave  dd  PRECALC_WAVE_LEN+5 dup (?)

;envelope.inc
.CODE
locals

MAX_ENV_NODES_NUM   equ  30

ENV_NODE   struc
	NODE_VALUE   dd  ?
	NODE_LEN     dd  ?
        NODE_CUR_LEN dd  ?
        NODE_RATE    dd  ?
ends

ENVELOPE  struc
          NODES_NUM  dd  ?
          ENV_NODES  db MAX_ENV_NODES_NUM*SIZE ENV_NODE dup(?)
ends

.CODE
Setup_Envelope    proc
; esi - envelope script
; edi - envelope record ptr
; edx - env max amp
        pushad

        mov    Env_Max_Amp,edx

        lea    edx,[edi].ENV_NODES
                                ; Собираем узлы огибающей
        mov    [edi].NODES_NUM,0
@@tt1:
        mov    eax,[esi]
        cmp    eax,NULL_SYM
        jz     @@Nodes_End

        fld    dword ptr [esi]
        fimul  Env_Max_Amp
        fstp   [edx].NODE_VALUE

        fld    dword ptr [esi+4]    ; Берем node len
        fstp   dword ptr [edx].NODE_LEN

        add    esi,8

        inc    [edi].NODES_NUM
        add    edx,SIZE ENV_NODE
        jmp    @@tt1

@@Nodes_End:
        mov    Last_ESI,esi

        popad
        mov esi,Last_ESI
        add esi,4

        ret
Setup_Envelope    endp


Calc_Envelope    proc
; esi - envelope record ptr
; edi - ready envelope
; ecx - envelope len
        pushad
        mov    Env_Len,ecx
                                       ; Считаем nodes rates
        lea    edx,[esi].ENV_NODES
        mov    ecx,[esi].NODES_NUM
        xor    ebx,ebx
@@Calc_Rates:
        fld   [edx].NODE_LEN
        fimul Env_Len
        fistp [edx].NODE_CUR_LEN

        fld   [edx+SIZE ENV_NODE].NODE_VALUE
        fsub  [edx].NODE_VALUE
        fidiv [edx].NODE_CUR_LEN

        fstp  [edx].NODE_RATE

        add   edx,SIZE ENV_NODE
        dec   ecx
        jnz   @@Calc_Rates

                                ; Считаем огибающую
        xor   ebx,ebx
        mov   edx,[esi].NODES_NUM
        lea   esi,[esi].ENV_NODES
@@Calc_Env:
        fld   [esi].NODE_VALUE

        mov   ecx,[esi].NODE_CUR_LEN
        or    ecx,ecx
        jz    @@Next_Node
@@ll2:
        fst   dword ptr [edi]
        fadd  [esi].NODE_RATE
        add   edi,4

        inc   ebx
        cmp   ebx,Env_Len
        ja    @@Sample_End

        dec   ecx
        jns   @@ll2
@@Next_Node:
        add   esi,SIZE ENV_NODE
        fstp st

        dec  edx
        jnz  @@Calc_Env

@@Sample_End:
        fstp st
        popad
        ret
Calc_Envelope    endp


Calc_Envelope_From_To_Pos    proc
; esi - envelope record ptr
; edi - ready envelope
; ecx - envelope len
; eax,ebx - begin,end pos
        pushad

	mov	@@Start_Pos,eax
	mov	@@End_Pos,ebx

        mov    Env_Len,ecx
                                       ; Считаем nodes rates
        lea    edx,[esi].ENV_NODES
        mov    ecx,[esi].NODES_NUM
        xor    ebx,ebx
@@Calc_Rates:
        fld   [edx].NODE_LEN
        fimul Env_Len
        fistp [edx].NODE_CUR_LEN

        fld   [edx+SIZE ENV_NODE].NODE_VALUE
        fsub  [edx].NODE_VALUE
        fidiv [edx].NODE_CUR_LEN

        fstp  [edx].NODE_RATE

        add   edx,SIZE ENV_NODE
        dec   ecx
        jnz   @@Calc_Rates

                                ; Считаем огибающую
        xor   ebx,ebx
        mov   edx,[esi].NODES_NUM
        lea   esi,[esi].ENV_NODES
@@Calc_Env:
        fld   [esi].NODE_VALUE

        mov   ecx,[esi].NODE_CUR_LEN
        or    ecx,ecx
        jz    @@Next_Node
@@ll2:
	cmp	ebx,@@Start_Pos
        jb    @@No_Env
        fst   dword ptr [edi]
        add   edi,4
@@No_Env:

        fadd  [esi].NODE_RATE

        inc   ebx
        cmp   ebx,Env_Len
        ja    @@Sample_End
	cmp	ebx,@@End_Pos
        ja    @@Sample_End

        dec   ecx
        jns   @@ll2
@@Next_Node:
        add   esi,SIZE ENV_NODE
        fstp st

        dec  edx
        jnz  @@Calc_Env

@@Sample_End:
        fstp st
        popad
        ret
.DATA?
@@Start_Pos	dd	?
@@End_Pos	dd	?
.CODE
Calc_Envelope_From_To_Pos    endp

Do_Envelope   proc
;edi - ready envelope
;edx - env max amp
       push   edi
       lea    edi,Envelope_Nodes
       call   Setup_Envelope

       pop    edi

       pushad
       mov    ecx,SAMPLE_SIZE
       lea    esi,Envelope_Nodes
       call   Calc_Envelope
       popad
       retn
Do_Envelope   endp

.DATA?
Env_Len                 dd        ?
Env_Max_Amp             dd        ?
Seq_Note_Env_Nodes      ENVELOPE <?>
Envelope_Nodes          ENVELOPE <?>

Harm_Func_Value		dd	?
Cur_Harmonizer_Ptr	dd	?
Harmonizers		db	SIZE HARMONIZER_REC*MAX_OSC_NUM dup(?)