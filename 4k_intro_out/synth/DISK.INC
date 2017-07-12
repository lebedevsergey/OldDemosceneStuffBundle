; converts raw sample to .wav format 
.CODE
Raw2Wav		proc 
; esi - raw sample
; edi - .wav
		shl	eax,1
		lea	esi,Wav_Head
		mov	[esi].Data_Len,eax

                mov	ecx,[esi].Riff_Len   ;SIZE RIFF_HEADER		

		add	eax,SIZE RIFF_HEADER-4
		mov	[esi].Riff_Len,eax

 		rep	movsb

		retn
Raw2Wav		endp

.DATA
RIFF_HEADER	struc
Riff_Head       db	"RIFF"
Riff_Len        dd	SIZE RIFF_HEADER
Wave_Head       db	"WAVE"
Fmt_Head        db	"fmt "
Fmt_Len         dd	16
Fmt_Category    dw	1
Fmt_Chann       dw	1
Fmt_Smp_Rate    dd	SAMPLE_RATE_
Fmt_Buff_Est    dd	SAMPLE_RATE_*2
Fmt_Blk_Align   dw	2
Fmt_Bits        dw	16;8
Data_Head       db	"data"
Data_Len        dd	?
ENDS

Wav_Head	RIFF_HEADER	<"RIFF",SIZE RIFF_HEADER,"WAVE","fmt ",16,\
1,1,SAMPLE_RATE_,SAMPLE_RATE_*2,2,16,"data",?>