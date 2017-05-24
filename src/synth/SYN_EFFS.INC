; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

; synth effects code

;filter.inc
.CODE
FILT_FREQ_ENV_MAX_VALUE    equ  2000
GAIN_ENV_MAX_VALUE    equ  50
Q_ENV_MAX_VALUE       equ  10

locals

.CODE

Conv_Gain_To_Db  proc
                fld Gain_
                fdiv Gain_Coef2
                fld  Gain_Coef1
                Fpow
retn
endp

Conv_Gain_To_Db2  proc
                fld Gain_
                fdiv Gain_Coef3
                fld  Gain_Coef1
                Fpow
retn
endp


Uni_Filter_Proc0  proc
; no filter
                fld dword ptr [edi]
                retn

Uni_Filter_Init_Proc0:
                retn
                endp


Uni_Filter_Proc1 proc
; lowpass
                fld dword ptr [edi]
                fmul B0
                fld I1
                fmul B1
                faddp st(1),st
                fld I2
                fmul B0
                faddp st(1),st
                fld O1
                fmul A1
                fsubp st(1),st
                fld O2
                fmul A2
                fsubp st(1),st
                retn
                endp

Uni_Filter_Init_Proc1 proc
                call Conv_Gain_To_Db
                fstp Gain

                fld _2PI
                fmul Frequency
                fidiv Sample_Rate
                fsincos
                fstp cs_
;               fstp sn_
                fmul	_0_5
                fdiv Q
                fstp  Alpha
                fld1
                fld  Alpha
                fld1
                faddp st(1),st
                fdivp st(1),st
                fst A0
                fmul CS_
                fdiv	_0_5
                fchs
                fstp A1

                fld1
                fsub Alpha
                fmul A0
                fstp A2

                fld1
                fsub CS_
                fmul A0
                fmul Gain
                fst B1

                fmul	_0_5
                fstp B0

                retn
                endp


Uni_Filter_Init_Proc2 proc
;highpass
                call Conv_Gain_To_Db
                fstp Gain

                fld _2PI
                fmul Frequency
                fidiv Sample_Rate
                fsincos
                fstp cs_
;               fstp sn_
                fmul	_0_5
                fdiv Q
                fstp  Alpha
                fld1
                fld  Alpha
                fld1
                faddp st(1),st
                fdivp st(1),st
                fst A0
                fmul CS_
                fdiv	_0_5
                fchs
                fstp A1

                fld1
                fsub Alpha
                fmul A0
                fstp A2

                fld1
                fadd CS_
                fmul A0
                fmul Gain
                fchs
                fst B1

                fmul	_0_5
                fchs
                fstp B0

                retn
                endp


Uni_Filter_Proc3 proc
; bandpass
                fld dword ptr [edi]
                fsub I2
                fmul B0

                fld O1
                fmul A1
                fsubp st(1),st

                fld O2
                fmul A2
                fsubp st(1),st

                retn
                endp


Uni_Filter_Init_Proc3 proc
; bandpass
                call Conv_Gain_To_Db
                fstp Gain

                fld _2PI
                fmul Frequency
                fidiv Sample_Rate
                fsincos
                fstp cs_
;               fstp sn_
                fmul	_0_5
                fdiv Q
                fstp  Alpha
                fld1
                fld  Alpha
                fld1
                faddp st(1),st
                fdivp st(1),st
                fst A0
                fmul CS_
                fdiv	_0_5
                fchs
                fstp A1

                fld1
                fsub Alpha
                fmul A0
                fstp A2

                fld Alpha
                fmul A0
                fmul Gain
                fstp B0

                retn
                endp


Uni_Filter_Proc4  proc
; no filter
                fld dword ptr [edi]
                retn

Uni_Filter_Init_Proc4:
                retn
                endp


Uni_Filter_Proc5 proc
; peaking
                fld dword ptr [edi]
                fmul B0
                fld I1
                fmul A1
                faddp st(1),st
                fld I2
                fmul B2
                faddp st(1),st

                fld O1
                fmul A1
                fsubp st(1),st
                fld O2
                fmul A2
                fsubp st(1),st
                retn
                endp


Uni_Filter_Init_Proc5 proc
;peaking
                call Conv_Gain_To_Db2
                fstp Amp_

                fld _2PI
                fmul Frequency
                fidiv Sample_Rate
                fsincos
                fstp cs_
;               fstp sn_
                fmul	_0_5
                fdiv Q
;               fst  Alpha

                fld st
                fmul Amp_
                fstp Temp1

                fdiv Amp_
                fstp Temp2

                fld1
                fld  Temp2
                fld1
                faddp st(1),st
                fdivp st(1),st
                fst A0
                fmul CS_
                fdiv	_0_5
                fchs
                fstp A1

                fld1
                fsub Temp2
                fmul A0
                fstp A2

                fld1
                fadd Temp1
                fmul A0
                fstp B0

                fld1
                fsub Temp1
                fmul A0
                fstp B2

                retn
                endp


Uni_Filter proc
         pushad
         xor eax,eax
         lodsb
         mov Uni_Filter_Type,eax
         lodsb
         mov Uni_Filter_Order,eax

         mov   edi,Seq_Note_Amp_Env_Ptr ;Filt_Freq_Env_Ptr
         mov   edx,FILT_FREQ_ENV_MAX_VALUE
         call  Do_Envelope

         mov   edi,Filt_Gain_Env_Ptr
         mov   edx,GAIN_ENV_MAX_VALUE
         call  Do_Envelope

         mov   edi,Seq_Freq_Env_Ptr  ;Filt_Q_Env_Ptr
         mov   edx,Q_ENV_MAX_VALUE
         call  Do_Envelope

         mov   Last_ESI,esi

         mov   ecx,Uni_Filter_Order
@@Do:
	 push  ecx

         mov   edi,Ready_Sample_Ptr
         mov   ebp,Seq_Note_Amp_Env_Ptr ;Filt_Freq_Env_Ptr
         mov   ebx,Filt_Gain_Env_Ptr
         mov   edx,Seq_Freq_Env_Ptr  ;Filt_Q_Env_Ptr

        finit

        push edi
        mov  edi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
        CLEAR_SAMPLE

	lea  edi,Filt_Null_Data
	mov  ecx,Filt_Null_Data_End
	xor  eax,eax
	rep  stosd	

        pop  edi

        mov  esi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr

        mov  eax,dword ptr [edi]
        mov  I1,eax
        mov  O1,eax
        mov  eax,dword ptr [edi+4]
        mov  I2,eax
        mov  O2,eax

        xor  ecx,ecx
@@ll1:
        add edi,4
        add esi,4
        add ebx,4
        add ebp,4
        add edx,4


        fld dword ptr [ebx]
        fabs
        fstp Gain_

        fld dword ptr [ebp]
        fabs
        fstp Frequency

        fld dword ptr [edx]
        fabs
        fstp Q

pushad
        mov eax,Uni_Filter_Type
        call Uni_Filter_Init_Proc_Tab[eax*4]
        mov eax,Uni_Filter_Type
        call Uni_Filter_Proc_Tab[eax*4]
popad
call Sample_Cut_Max_Min

        fst dword ptr [esi]
        mov eax,O1
        mov O2,eax
        fstp O1
        mov eax,I1
        mov I2,eax
        mov  eax,[edi]
        mov I1,eax

        inc ecx
        cmp ecx,SAMPLE_SIZE
        jb  @@ll1


        mov  esi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
        mov  edi,Ready_Sample_Ptr
        COPY_SAMPLE

	pop  ecx
	dec  ecx
	jnz  @@Do

        popad

         mov   esi,Last_ESI

        ret
endp

.DATA

Uni_Filter_Proc_Tab  dd offset Uni_Filter_Proc0
                     dd offset Uni_Filter_Proc1
                     dd offset Uni_Filter_Proc1
                     dd offset Uni_Filter_Proc3
                     dd offset Uni_Filter_Proc4
                     dd offset Uni_Filter_Proc5

Uni_Filter_Init_Proc_Tab  dd offset Uni_Filter_Init_Proc0
                     dd offset Uni_Filter_Init_Proc1
                     dd offset Uni_Filter_Init_Proc2
                     dd offset Uni_Filter_Init_Proc3
                     dd offset Uni_Filter_Init_Proc4
                     dd offset Uni_Filter_Init_Proc5

.DATA
Gain_Coef1       dd    10.0
Gain_Coef2       dd    20.0
Gain_Coef3       dd    40.0

.DATA?
Uni_Filter_Type  dd  ?
Uni_Filter_Order dd  ?

I1               dd    ?
I2               dd    ?
O1               dd    ?
O2               dd    ?
Filt_Null_Data:
Q                dd    ?
Gain             dd    ?
Gain_            dd    ?
Frequency        dd    ?
A0               dd    ?
A1               dd    ?
A2               dd    ?
B0               dd    ?
B1               dd    ?
B2               dd    ?

B3               dd    ?
B4               dd    ?

Old_X      dd ?
Y1     dd ?
Old_Y1      dd ?
Y2     dd ?
Old_Y2      dd ?
Y3     dd ?
Old_Y3      dd ? 
Y4       dd ?
Old_Y4      dd ?

CS_              dd    ?
SN_              dd    ?
Alpha            dd    ?
Amp_             dd    ?
Temp1            dd    ?
Temp2            dd    ?
Filt_Null_Data_End equ ($-Filt_Null_Data)/4


;distort.inc
.CODE
locals

Distort_Sample  proc
; esi - old sample
; edi - distorted sample
; ebp - distortion func rec
        pushad

	xor    eax,eax
	lodsb	
	mov    @@Dry_Coef,eax
	lodsb
	mov    @@Wet_Coef,eax
	fild   @@Dry_Coef
	fidiv  Percent
	fstp   @@Dry_Coef
	fild   @@Wet_Coef
	fidiv  Percent
	fstp   @@Wet_Coef

        lea    edi,Distort_Nodes
        mov    edx,__1
	call   Setup_Envelope
                                       ; Считаем интервалы
        lea    edi,Distort_Nodes
        lea    edx,[edi].ENV_NODES
        mov    ecx,[edi].NODES_NUM
      	fldz
@@Calc_Interv:
	fld    st
        fstp   [edx].NODE_CUR_LEN

        fld    [edx+SIZE ENV_NODE].NODE_VALUE
        fsub   [edx].NODE_VALUE
        fld    [edx].NODE_LEN
        fdivp  st(1),st
        fstp   [edx].NODE_RATE

        fadd   [edx].NODE_LEN

        add    edx,SIZE ENV_NODE
        dec    ecx
        jnz    @@Calc_Interv
	fstp   st
                                      ; Дистортим
	mov   edi,Ready_Sample_Ptr
	xor   ecx,ecx
@@ll1:
	fld   dword ptr [edi]
call Sample_Cut_Max_Min
	fabs
	
        push  ecx
                                       ; Ищем интервал, куда попадает тек. значение
        lea    ebp,Distort_Nodes
        lea    edx,[ebp].ENV_NODES
        mov    ecx,[ebp].NODES_NUM
	dec    ecx
@@Find_Interv:
	fld    st
        fcomp  [edx+SIZE ENV_NODE].NODE_CUR_LEN
        fstsw  ax
	sahf
	ja     @@Next_Interv
        fsub   [edx].NODE_CUR_LEN
	fmul   [edx].NODE_RATE
	fadd   [edx].NODE_VALUE

	fld    dword ptr [edi]
	Fsign
	fmulp  st(1),st

        jmp    @@Found_Interv
@@Next_Interv:
        add   edx,SIZE ENV_NODE
        dec   ecx
        jnz   @@Find_Interv
	fstp  st             ; Ни фига не нашли - по идее, такого быть не должно
	fldz
@@Found_Interv:

        pop   ecx

	fmul  @@Wet_Coef
	fld   dword ptr [edi]
	fmul  @@Dry_Coef
        faddp st(1),st

	fstp  dword ptr [edi]
	add   edi,4
	inc   ecx
        cmp  ecx,SAMPLE_SIZE
        jbe   @@ll1

        popad

        mov esi,Last_ESI
        add esi,4
        ret
.DATA?
@@Wet_Coef   dd   ?
@@Dry_Coef   dd   ?
.CODE
Distort_Sample   endp


.DATA?
Distort_Nodes    ENVELOPE <?>


;effs1.inc
.CODE
locals

MAX_DELAYS_NUM  equ  5

DELAY_REC   struc
	DELAY_VALUE  dw  ?
	DELAY_SIGNAL_MUL  dd  ?
ends

.CODE
Delay_Sum_All_Delays  proc 
	pushad

	mov	edx,Delays_Num
        or	edx,edx
	jz	@@ex	

	lea	ebp,Delays
@@tt1:
	mov	Tmp2,ecx
	fild	Tmp2
	fisub	[ebp].DELAY_VALUE 
	fistp	Tmp2
	mov	ebx,Tmp2

	cmp	ebx,0
	jnl	@@tt3_1
	add	ebx,SAMPLE_SIZE	
@@tt3_1:
	cmp	ebx,SAMPLE_SIZE
	jng	@@tt3_2
	sub	ebx,SAMPLE_SIZE	
@@tt3_2:

	fld   dword ptr [edi+ebx*4]
        fmul  [ebp].DELAY_SIGNAL_MUL
	faddp st(1),st

	add ebp,SIZE DELAY_REC
	dec edx
	jnz @@tt1
@@ex:
	popad
	retn
Delay_Sum_All_Delays  endp

Reverb_Sum_All_Delays  proc 
	pushad

	mov	edx,Delays_Num
        or	edx,edx
	jz	@@ex	

	lea	ebp,Delays
@@tt1:
	mov	Tmp2,ecx
	fild	Tmp2
	fisub	[ebp].DELAY_VALUE 
	fistp	Tmp2
	mov	ebx,Tmp2

	cmp	ebx,0
	jnl	@@tt3_1
	add	ebx,SAMPLE_SIZE	
@@tt3_1:
	cmp	ebx,SAMPLE_SIZE
	jng	@@tt3_2
	sub	ebx,SAMPLE_SIZE	
@@tt3_2:

	fld	dword ptr [esi+ebx*4]
        fmul	[ebp].DELAY_SIGNAL_MUL
	faddp	st(1),st

	add	ebp,SIZE DELAY_REC
	dec	edx
	jnz	@@tt1
@@ex:
	popad
	retn
Reverb_Sum_All_Delays  endp

Get_All_Delays  proc 
        mov Delays_Num,0
	lea  edx,Delays
@@tt1:

	mov eax,[esi]
	cmp eax,NULL_SYM
        jz @@Delays_End

	lodsw
	mov [edx].DELAY_VALUE,ax
	xor eax,eax
	lodsb
	mov [edx].DELAY_SIGNAL_MUL,eax
        fild [edx].DELAY_SIGNAL_MUL
	fidiv Percent
        fstp [edx].DELAY_SIGNAL_MUL	
        inc Delays_Num
	add edx,SIZE DELAY_REC
	jmp @@tt1
@@Delays_End:
	add esi,4
	retn
endp

Delay_  proc
	pushad

	mov  edi,Ready_Sample_Ptr

        xor   eax,eax
        lodsb           ; Берем операнд
        mov   Old_Signal_Amplify,eax
        fild  Old_Signal_Amplify
	fidiv Percent
        fstp  Old_Signal_Amplify

	call Get_All_Delays

	mov Last_ESI,esi

	push edi

	mov  esi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
	xor  ecx,ecx
@@ll1:
        fld  dword ptr [edi+ecx*4]
        fmul  Old_Signal_Amplify
	call Delay_Sum_All_Delays
call Sample_Cut_Max_Min
        fstp   dword ptr [esi+ecx*4]
	inc  ecx
        cmp  ecx,SAMPLE_SIZE
        jb   @@ll1

	mov  esi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
	pop edi
	Copy_Sample
	
	popad

	mov esi,Last_ESI

        ret

endp

Reverb_ proc 
	pushad

        xor   eax,eax
        lodsb           ; Берем операнд
        mov   Old_Signal_Amplify,eax
        fild  Old_Signal_Amplify
	fidiv Percent
        fstp  Old_Signal_Amplify

	call Get_All_Delays

	mov Last_ESI,esi

	mov  edi,Ready_Sample_Ptr

	pushad
	mov  esi,edi
	mov  edi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
	COPY_SAMPLE
	popad

	push edi

	mov  esi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
	xor  ecx,ecx
@@ll1:
	fld  dword ptr [esi+ecx*4]
	fmul Old_Signal_Amplify
	call Reverb_Sum_All_Delays
        fstp dword ptr [esi+ecx*4]
	inc  ecx
        cmp  ecx,SAMPLE_SIZE
        jb   @@ll1

	pop edi

	mov  esi,Seq_Amp_Env_Ptr ;Sample_Copy_Ptr
	xor  ecx,ecx
@@ll1_:
        fld    dword ptr [edi+ecx*4]
        fmul   Old_Signal_Amplify
        fadd   dword ptr [esi+ecx*4]
call Sample_Cut_Max_Min
        fstp   dword ptr [edi+ecx*4]
	inc  ecx
        cmp  ecx,SAMPLE_SIZE
        jb   @@ll1_

	popad

	mov esi,Last_ESI
        ret
endp


.DATA?
Old_Signal_Amplify  dd  ?
Flanger_Amplify2    dd  ?

Delays:
		rept MAX_DELAYS_NUM
		 DELAY_REC <?,?>
		endm


.DATA?
Delays_Num	  dd  ?	

.CODE
Modulate_Amp proc
	pushad

	mov   edi,Seq_Amp_Env_Ptr
        mov   edx,1
	call  Do_Envelope
	mov   Last_ESI,esi

	mov  esi,Seq_Amp_Env_Ptr
	mov  edi,Ready_Sample_Ptr
	xor  ecx,ecx	
@@ll1:
        fld   dword ptr [esi]
        fmul  dword ptr [edi]
        fstp  dword ptr [edi]

	add esi,4
	add edi,4

	inc ecx
        cmp ecx,SAMPLE_SIZE
        jb  @@ll1

	popad

        mov esi,Last_ESI

        ret
endp

Delay2  proc
	pushad

        xor   eax,eax
        lodsb           ; Берем операнд
        mov   @@Feedback,eax
        lodsb           ; Берем операнд
        mov   @@Step,eax
	cmp   eax,Note_Count
	jae   @@ex

        fild  @@Feedback
	fidiv Percent
        fstp  @@Feedback

	fild  @@Step
	fimul Note_Len
	fistp @@Delay1

	fild  Note_Count
	fidiv Seq_Notes_Cyc_Num
	fisub @@Step
	fimul Note_Len
	fistp @@Delay2

	mov  esi,Ready_Sample_Ptr
	mov  ecx,Seq_Notes_Cyc_Num  ; Задержку делаем по циклам
@@Notes_Cyc_Cyc:
        push ecx

	xor  ecx,ecx
@@ll1:
        fld  dword ptr [esi+ecx*4]
	mov  eax,ecx
	sub  eax,@@Delay1
	jns  @@Ok
	mov  eax,@@Delay2
	add  eax,ecx
@@Ok:
        fld  dword ptr [esi+eax*4]
	fmul @@Feedback
	faddp st(1),st
call Sample_Cut_Max_Min
        fstp   dword ptr [esi+ecx*4]

	inc  ecx
        cmp  ecx,Seq_Notes_Cyc_Len
        jb   @@ll1

        pop  ecx

	mov  eax,Seq_Notes_Cyc_Len
	shl  eax,2
	add  esi,eax

	dec  ecx
	jnz  @@Notes_Cyc_Cyc
@@ex:	
	popad

	inc esi
	inc esi
        ret
.DATA?
@@Feedback  dd  ?
@@Delay1    dd  ?
@@Delay2    dd  ?
@@Step      dd   ?
endp