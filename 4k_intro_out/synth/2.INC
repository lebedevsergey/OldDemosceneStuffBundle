; synth sequencer channels mixer

PATTERN_LEN	equ	20d00h
PATTERNS_NUM	equ	11

CHANNELS_NUM	equ	7

CHANNEL_REC	struc
CHAN_VOL	dd	?
CHAN_SMP_POS	dd	?
CHAN_SMP_PTR	dd	?
CHAN_SMP_LEN	dd	?
ends

.CODE

Use_Mixer:
;edi - ptr to wav

lea	ebp,All_Data

	lea	esi,Music_Script
	xor	eax,eax
	mov	Pattern_Num,al

	mov	edx,PATTERNS_NUM
@@Mix_Mus:
	mov	al,[esi]
	cmp	al,Pattern_Num
	ja	@@No_Com

	lodsb
	lodsb	
	movzx	ebx,al
	call	Get_Percent
	imul	ebx,SIZE CHANNEL_REC
add ebx,offset Chann1
	fstp	[ebx].CHAN_VOL
	jmp	@@Mix_Mus
@@No_Com:

	mov	ecx,PATTERN_LEN
@@Mix_Patt:
	pushad
	lea	edi,Chann1
	mov	ecx,CHANNELS_NUM
	fldz
@@Do_Chann:
	mov	ebx,[edi].CHAN_SMP_PTR
	mov	eax,[edi].CHAN_SMP_POS
push eax
	shl	eax,1     ;16 bit
	fild	word ptr [eax+ebx]
	fmul	[edi].CHAN_VOL
	faddp	st(1),st
pop eax
	inc	eax
	cmp	eax,[edi].CHAN_SMP_LEN
	jbe	@@No_Smp_Loop
	xor	eax,eax
@@No_Smp_Loop:
	mov	[edi].CHAN_SMP_POS,eax

	add	edi,SIZE CHANNEL_REC
	loop	@@Do_Chann

	popad	
fidiv _3
	fistp	word ptr [edi]

	inc	edi
	inc	edi
	loop	@@Mix_Patt

	inc	Pattern_Num

	dec	edx
	jnz	@@Mix_Mus
	
	retn

.DATA
_3	dw	3
Music_Script:
		db	0,0,100
		db	1,1,100
		db	2,2,100   

		db 	3,0,0
		db 	3,2,0
		db	3,4,100
		db 	4,4,0
		db 	4,2,100
		db 	5,2,0
		db 	5,3,100
		db 	5,4,100
		db 	6,3,0
		db 	6,4,0
		db 	6,5,100

		db 	7,2,100
		db 	7,3,100
		db 	7,5,0

		db 	8,2,0
		db 	8,3,0
		db 	8,4,100
		db 	8,5,100

		db 	10,4,0
		db 	10,5,0
		db 	10,1,0
		db 	10,6,100

		db	20

.DATA?
Chann1	CHANNEL_REC <?>
Chann2	CHANNEL_REC <?>
Chann3	CHANNEL_REC <?>
Chann4	CHANNEL_REC <?>
Chann5	CHANNEL_REC <?>
Chann6	CHANNEL_REC <?>
Chann7	CHANNEL_REC <?>
Pattern_Num	db	?

Music	dw	PATTERN_LEN*PATTERNS_NUM dup(?)