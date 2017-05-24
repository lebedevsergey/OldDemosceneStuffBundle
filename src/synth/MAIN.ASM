; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

; main module

.486
.MODEL FLAT
Global Generate: near
Global Init: near

.CODE
_main:

include	compiler.inc			;компилер внешнего скрипта
include math.inc			;всякая нужная математика
include synth.inc                       ;синт
include	disk.inc                        ;работа с .wav 

.CODE
Init	proc
; Это надо вызвать перед началом работы синта
	pushad
	lea	ebp,Synth_Buf		; Указатель на память для буферов синта
					; ее размер -  SYNTH_TMP_MEM_SIZE
	call	Init_Synth              ; это инициализация синта
	popad
	retn
Init	endp

Generate proc
; Сгенерить сэмпл
; esi - script ptr
; ecx - script len
; edi - wav ptr
	pushad

	mov	Text_Script_Len,ecx
	mov	Wav_File_Ptr,edi

	lea	edi,Text_Script_Buf
	rep	movsb

	mov	ecx,Text_Script_Len
        lea	esi,Text_Script_Buf
	lea	edi,Inner_Script_Buf
	call	Compile            ; перевод внешнего скрипта (текстовый)
				   ; во внутренний (циферки)
				   ; по идее, в интре не нужен - нефиг там
				   ; текстовый скрипт держать
				   ; тогда и compiler.inc
				   ; нужно выкинуть	

	lea	esi,Inner_Script_Buf  ;указатель на скрипт для синта 
	lea	edi,Tmp_Sample2    ; буфер для сэмпла
	call	Do_Sample

	lea	esi,Tmp_Sample2    ; загоним сэмпл в .wav (для интерфейса 
				   ; синта так удобнее)
	mov	edi,Wav_File_Ptr   ; в интре тоже не особо нужно ->
	call	Raw2Wav            ; выкидывается disk.inc

	popad
		
	mov	eax,Riff_Len	   ; Интерфейс хочет знать
				   ; какой длины сэмпл получился
	retn
Generate	endp

.DATA?
Wav_File_Ptr	dd	?
Synth_Buf	db	SYNTH_TMP_MEM_SIZE dup (?)
Tmp_Sample2	dw	MAX_SAMPLE_SIZE_ dup(?)

Inner_Script_Buf db	INNER_SCRIPT_MAX_LEN dup (?)
Text_Script_Buf	db	SCRIPT_MAX_LEN dup (?)
Text_Script_Len	dd	?
end _main