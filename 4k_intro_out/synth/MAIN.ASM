; 4k intro softsynth
;выкинут гармонайзер
.486
;DO_COMPILE	equ	1

.MODEL FLAT
Global Generate: near
;Global Init: near

.CODE
;_main:

include matem.inc			;всякая нужная математика
include synth.inc                       ;синт
include	disk.inc                        ;работа с .wav 
include	2.inc                        ;работа с .wav 

.CODE
Init macro
; Это надо вызвать перед началом работы синта
	pushad
lea	ebp,All_Data
	lea	eax,Synth_Buf		; Указатель на память для буферов синта
					; ее размер -  SYNTH_TMP_MEM_SIZE
;	call	Init_Synth              ; это типа инициализация синта
					   ;setup pointers
		lea	edi,[ebp].Ready_Sample_Ptr
		mov	ecx,5
@@Setup_Pointers:
		stosd
		add	eax,SIZE TMP_SAMPLE
		loop	@@Setup_Pointers

lea	ebx,Init_Data

;	xor	ecx,ecx     ; j
	xor	edi,edi
@@Do_Octaves:
	fld	[ebx].C_5_FREQ

	xor	edx,edx    ; k
@@Do_Tones:
	fld	st
	fmul	[ebx].Oct
	fstp	[ebp].Freq_Tab[edi*4]

	inc	edi

	fmul	[ebx].TONECONST
	inc	edx
	cmp	edx,TONES_NUM
	jb	@@Do_Tones

	fstp	st
	fld	[ebx].Oct
	fadd	st,st
	fstp	[ebx].Oct

	inc	ecx
	cmp	ecx,OCTAVES_NUM
	jb	@@Do_Octaves

	popad
;	retn
;Init	endp
endm

Generate proc
; Сгенерить сэмпл
; esi - script ptr
; ecx - script len
; edi - wav ptr

Init	
	lea	esi,Stash
	lea	ebp,Chann1
	lea	edi,Tmp_Sample1    ; буфер для сэмпла
	call	Do_Sample
	lea	esi,Drum
	lea	ebp,Chann2
	lea	edi,Tmp_Sample2    ; буфер для сэмпла
	call	Do_Sample
	lea	esi,Jizz1
	lea	ebp,Chann3
	lea	edi,Tmp_Sample3    ; буфер для сэмпла
	call	Do_Sample

	lea	esi,Jizz2
	lea	ebp,Chann4
	lea	edi,Tmp_Sample4    ; буфер для сэмпла
	call	Do_Sample
	lea	esi,Jizz1_2
	lea	ebp,Chann5
	lea	edi,Tmp_Sample5    ; буфер для сэмпла
	call	Do_Sample
	lea	esi,Jizz2_2
	lea	ebp,Chann6
	lea	edi,Tmp_Sample6    ; буфер для сэмпла
	call	Do_Sample

	lea	esi,Vomit
	lea	ebp,Chann7
	lea	edi,Tmp_Sample7    ; буфер для сэмпла
	call	Do_Sample

	lea	edi,Music
	push	edi
        mov	eax,PATTERN_LEN*PATTERNS_NUM  ;PATTERN_LEN
	call	Raw2Wav
	call	Use_Mixer
	pop	edi

;	mov	eax,Wav_Head.Riff_Len	   ; Интерфейс хочет знать
				   ; какой длины сэмпл получился
	retn
Generate	endp


.DATA
Drum	db 00ch,001h,006h,004h,002h,000h,000h,064h,07fh,008h,000h,009h,00fh
	db 00ah,010h,00ch,00ah,00fh,007h,036h,000h,000h,000h,005h,02eh,05fh
	db 003h,000h,0ffh,0ffh,0ffh

Stash	db 00ch,001h,006h,040h,002h,000h,002h,042h,07fh,002h,001h,002h,021h
	db 07bh,002h,002h,002h,03bh,083h,004h,03bh,064h,03eh,000h,03ah,0ffh
	db 0ffh,0ffh


Jizz2	db 010h,000h,04bh,00ch,004h,006h,010h,002h,000h,002h,027h,07fh,002h
	db 001h,002h,032h,089h,002h,002h,002h,02ch,078h,004h,000h,022h,037h
	db 021h,037h,021h,000h,000h,016h,03bh,03ch,03dh,03bh,03ch,016h,03ch
	db 03bh,01eh,03ch,03bh,045h,03ch,006h,03bh,016h,03bh,03ch,03dh,03bh
	db 03ch,016h,03ch,03bh,01eh,03ch,03bh,045h,03ch,006h,03bh,016h,03bh
	db 03ch,03dh,03bh,03ch,016h,03ch,03bh,01eh,03ch,03bh,045h,03ch,006h
	db 03bh,016h,03bh,03ch,03dh,03bh,03ch,016h,03ch,03bh,01eh,03ch,03bh
	db 045h,03ch,006h,03bh,0ffh,00eh,042h,003h,00ah,001h,014h,031h,042h
	db 033h,056h,000h,004h,064h,00dh,000h,015h,033h,054h,031h,013h,000h
	db 0ffh,0ffh,0ffh

Jizz2_2	db 010h,000h,04bh,00ch,004h,006h,010h,002h,000h,002h,027h,07fh,002h
	db 001h,002h,032h,089h,002h,002h,002h,02ch,078h,004h,000h,022h,037h
	db 021h,037h,021h,000h,000h,016h,03bh,03ch,03dh,03bh,03ch,016h,03ch
	db 03bh,01eh,03ch,03bh,045h,03ch,006h,03bh,016h,03bh,03ch,03dh,03bh
	db 03ch,016h,03ch,03bh,01eh,03ch,03bh,045h,03ch,006h,03bh,016h,03bh
	db 03ch,03dh,03bh,03ch,016h,03ch,03bh,01eh,03ch,03bh,045h,03ch,006h
	db 03bh,016h,03bh,03ch,03dh,03bh,03ch,016h,03ch,03bh,01eh,03ch,03bh
	db 045h,03ch,006h,03bh,0ffh,00eh,042h,003h,00ah,001h,056h,031h,042h
	db 033h,014h,000h,00dh,064h,004h,000h,013h,033h,054h,031h,015h,000h
	db 0ffh,0ffh,0ffh


Jizz1	db 010h,007h,04bh,00ch,004h,006h,010h,002h,000h,002h,022h,07fh,002h
	db 001h,002h,027h,089h,002h,002h,002h,01fh,08fh,004h,000h,01fh,036h
	db 024h,036h,020h,000h,000h,0bch,0bdh,0bbh,05ch,0c4h,0dch,05dh,0ach
	db 03bh,03bh,0c4h,0bbh,03eh,045h,0dch,05dh,0bch,0bdh,0bbh,05ch,0c4h
	db 0dch,05dh,0ach,03bh,03bh,0c4h,0bbh,03eh,045h,0dch,05dh,0bch,0bdh
	db 0bbh,05ch,0c4h,0dch,05dh,0ach,03bh,03bh,0c4h,0bbh,03eh,045h,0dch
	db 05dh,0bch,0bdh,0bbh,05ch,0c4h,0dch,05dh,0ach,03bh,03bh,0c4h,0bbh
	db 03eh,045h,0dch,05dh,0ffh,00eh,02ch,003h,00ah,001h,00fh,064h,075h
	db 000h,006h,064h,01fh,000h,007h,064h,00ah,000h,0ffh,0ffh,0ffh,0ffh	

Jizz1_2	db 010h,007h,04bh,00ch,004h,006h,010h,002h,000h,002h,022h,07fh,002h
	db 001h,002h,027h,089h,002h,002h,002h,01fh,08fh,004h,000h,01fh,036h
	db 024h,036h,020h,000h,000h,0bch,0bdh,0bbh,05ch,0c4h,0dch,05dh,0ach
	db 03bh,03bh,0c4h,0bbh,03eh,045h,0dch,05dh,0bch,0bdh,0bbh,05ch,0c4h
	db 0dch,05dh,0ach,03bh,03bh,0c4h,0bbh,03eh,045h,0dch,05dh,0bch,0bdh
	db 0bbh,05ch,0c4h,0dch,05dh,0ach,03bh,03bh,0c4h,0bbh,03eh,045h,0dch
	db 05dh,0bch,0bdh,0bbh,05ch,0c4h,0dch,05dh,0ach,03bh,03bh,0c4h,0bbh
	db 03eh,045h,0dch,05dh,0ffh,00eh,02ch,003h,00ah,001h,075h,064h,00fh
	db 000h,01fh,064h,006h,000h,00ah,064h,007h,000h,0ffh,0ffh,0ffh,0ffh


Vomit	db 00ch,001h,006h,010h,002h,000h,002h,017h,078h,002h,001h,004h,003h
	db 07fh,002h,002h,002h,023h,07fh,008h,000h,010h,03eh,01bh,018h,018h
	db 028h,012h,016h,00fh,000h,000h,000h,00fh,018h,020h,010h,019h,00dh
	db 01ch,000h,000h,00ah,001h,000h,05eh,03eh,006h,000h,000h,000h,064h
	db 000h,000h,003h,044h,065h,020h,050h,000h,0ffh,0ffh,0ffh

.DATA?
Synth_Buf	db	SYNTH_TMP_MEM_SIZE dup (?)
Tmp_Sample1	dw	MAX_SAMPLE_SIZE_ dup(?)
Tmp_Sample2	dw	MAX_SAMPLE_SIZE_ dup(?)
Tmp_Sample3	dw	MAX_SAMPLE_SIZE_ dup(?)
Tmp_Sample4	dw	MAX_SAMPLE_SIZE_ dup(?)
Tmp_Sample5	dw	MAX_SAMPLE_SIZE_ dup(?)
Tmp_Sample6	dw	MAX_SAMPLE_SIZE_ dup(?)
Tmp_Sample7	dw	MAX_SAMPLE_SIZE_ dup(?)

Inner_Script_Buf db	INNER_SCRIPT_MAX_LEN dup (?)
Text_Script_Buf	db	SCRIPT_MAX_LEN dup (?)
Text_Script_Len	dd	?
end ;_main