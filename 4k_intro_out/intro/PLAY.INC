
.CODE
;call	INIT
call	GENERATE

push	SND_ASYNC OR SND_LOOP OR SND_MEMORY
xor	eax,eax
push	eax
push	edi 
call	PlaySoundA

