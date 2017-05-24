; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

.CODE
; disk i/o code
IFDEF SAVE_SAM

Raw2Wav		proc 
; esi - raw sample
; edi - .wav
		pushad
		push	esi

                mov	eax,SAMPLE_SIZE
		shl	eax,1
		mov	Data_Len,eax		
		add	eax,RIFF_HEAD_LEN
		mov	Riff_Len,eax

		lea	esi,Riff_Head
                mov	ecx,RIFF_HEAD_LEN2
 		rep	movsb

		pop	esi
                mov	ecx,SAMPLE_SIZE
or ecx,ecx
jz @@rr
		rep  movsw
@@rr:
		popad
		retn
Raw2Wav		endp

.DATA

;Sample_File db  "sample1.wav",0,0,0,0           
  				  ; Заголовок .WAV файла
Riff_Head       db  "RIFF"
Riff_Len        dd  RIFF_HEAD_LEN

Wave_Head       db  "WAVE"

Fmt_Head        db  "fmt "
Fmt_Len         dd  18
Fmt_Category    dw  1
Fmt_Chann       dw  1
Fmt_Smp_Rate    dd  SAMPLE_RATE_
Fmt_Buff_Est    dd  SAMPLE_RATE_*2
Fmt_Blk_Align   dw  2
Fmt_Bits        dd  16;8

Data_Head       db  "data"

Data_Len        dd  MAX_SAMPLE_SIZE_*2
RIFF_HEAD_LEN  equ  $-Riff_Len

RIFF_HEAD_LEN2   equ  $-Riff_Head
ENDIF

.DATA?
Wav_File     db  RIFF_HEAD_LEN dup (?)
Data_Data    dw  MAX_SAMPLE_SIZE_ dup (?)