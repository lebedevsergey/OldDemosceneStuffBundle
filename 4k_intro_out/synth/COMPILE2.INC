; synth script compiler

.data
Token_Tab      TOKEN_STRUC <offset End_Token,offset End_Token_Proc,END_COM>
	   TOKEN_STRUC <offset Sample_Filename_Token,offset Sample_Filename_Token_Proc,0>
	   TOKEN_STRUC <offset Script_Filename_Token,offset Script_Filename_Token_Proc,0>

	   TOKEN_STRUC <offset Sequencer_Token2,offset Sequencer_Token2_Proc,SEQ_COM>

	   TOKEN_STRUC <offset Osc_Token3,offset Osc_Token3_Proc,SETUP_OSC_COM>
	   TOKEN_STRUC <offset Cust_Sound_Token,offset Cust_Sound_Token_Proc,DO_CUSTOM_SOUND_COM>
	   TOKEN_STRUC <offset Filt_Token,offset Filt_Token_Proc,FILT_COM>
	   TOKEN_STRUC <offset Filt_Token2,offset Filt_Token2_Proc,FILT_COM>

	   TOKEN_STRUC <offset Len_Mul_Token,offset Len_Mul_Token_Proc,SET_LEN_MUL_COM>
	   TOKEN_STRUC <offset Delay2_Token,offset Delay2_Token_Proc,DELAY2_COM>
	   TOKEN_STRUC <offset SET_A_D_Token,offset SET_A_D_Token_Proc,SET_A_D_COM>
	   TOKEN_STRUC <offset SET_BPM_SAMPLE_LEN_Token,offset SET_BPM_SAMPLE_LEN_Proc,SET_BPM_SAMPLE_LEN_COM>

;disabled
	   TOKEN_STRUC <offset Dist_Token,offset Dist_Token_Proc,NONE>
	   TOKEN_STRUC <offset Cust_Wave_Token,offset Cust_Wave_Token_Proc,SET_CUST_WAVE_COM>
	   TOKEN_STRUC <offset Reverb_Token,offset Reverb_Token_Proc,REVERB_COM>
	   TOKEN_STRUC <offset Delay_Token,offset Reverb_Token_Proc,DELAY_COM>
	   TOKEN_STRUC <offset Mod_Amp_Token,offset Mod_Amp_Token_Proc,MOD_AMP_COM>
	   TOKEN_STRUC <offset SET_PITCH_Token,offset SET_PITCH_Token_Proc,SET_PITCH_SPEED_COM>
	   TOKEN_STRUC <offset Harm_Token,offset Harm_Token_Proc,SETUP_HARM_COM>
	       dd NULL

End_Token                    db  'END',0
Sample_Filename_Token	     db  'SAMPLE_FILENAME'
Script_Filename_Token	     db  'SCRIPT_FILENAME'
Sequencer_Token              db  'SEQUENCER'
Sequencer_Token2              db  'SEQUENCE2'
Cust_Env_Token  	     db  'CUST_ENV'
Osc_Token                    db  'OSCILLATOR'
Osc_Token2                    db  'OSCILLATO2'
Osc_Token3                    db  'OSCILLATO3'
SET_BPM_SAMPLE_LEN_TOKEN db 'SET_BPM_SAMPLE_LEN',0
Cust_Sound_Token             db  'CUSTOM_SOUND'
Filt_Token                   db  'FILTER'
Filt_Token2                   db  'FILTE2'
Harm_Token                   db  'HARMONIZER'
Reverb_Token		     db  'REVERB'
Dist_Token		     db  'DISTORT'
Delay_Token		     db  'DELAY'
Mod_Amp_Token		     db  'MOD_AMP'
Len_Mul_Token                db  'LEN_MUL' 
Cust_Wave_Token              db  'CUST_WAVE' 
Delay2_Token		     db  'DELA2'
SET_PITCH_Token		     db  'SET_PITCH_SPEED'
SET_A_D_Token		     db  'SET_A_D'

.code 
Interp_Note	proc

; EAX: note obozn

cmp	eax,EMPTY_NOTE_SYM
jnz	@@Yes_Note
mov	ax,EMPTY_NOTE_WORD
jmp	@@ex

@@Yes_Note:

push	eax
	mov ebx,eax	
	sub bl,'A'	
	and ebx,0FFh

	mov bl,Tones[ebx]
	cmp ah,'#'
	jnz @@No_Dies
	inc bl
@@No_Dies:
	shr eax,16
	sub al,'0'
	and eax,0FFh

	and ebx,0FFh

; EAX: octave
; EBX: tone

and	eax,OCTAVE_MASK
shl	ebx,TONE_SHIFT
and	ebx,TONE_MASK
or	eax,ebx

pop	ebx

bswap ebx

cmp bl,NOTE_PITCH_SUSTAIN_SYM
jz @@Yes_Sustain
cmp bl,NOTE_SUSTAIN_SYM
jnz @@No_Sustain
@@Yes_Sustain:
or eax,NOTE_SUSTAIN_WORD
@@No_Sustain:

@@ex:

retn
endp

Conv_To_Percent	proc
;IN - fpu - float
;out - al - integer
		 fimul	Percent
		 fistp	Tmp2
		 mov	eax,Tmp2
		and	eax,0FFh

		retn
Conv_To_Percent	endp

Get_Float_Number_Percent proc
	     call Get_Number
fld st
		call	Conv_To_Percent
	     mov edi,Cur_Inner_Script_Ptr	
	stosb	
	     inc Cur_Inner_Script_Ptr
fstp Tmp2
or al,al		;Проверяем корректность перевода
jnz @@Ok
cmp Tmp2,0
jnz  @@Percent_Err
@@Ok:
              clc
	      retn
@@Percent_Err:
	      stc
	      retn
	      endp		

Simple_Token_Proc proc
	     mov al,[ebx+ebp].Token_Com	
	     mov edx,Cur_Inner_Script_Ptr	
	     mov byte ptr [edx],al
	     inc Cur_Inner_Script_Ptr	
	    retn
endp

Get_Float_Number proc
	     call Get_Number
	     mov edx,Cur_Inner_Script_Ptr	
	     fstp dword ptr [edx]
	     add Cur_Inner_Script_Ptr,4
	      retn
	      endp		

Get_Word_Number proc
	     call Get_Number
	     mov edx,Cur_Inner_Script_Ptr	
	     fistp word ptr [edx]
	     add Cur_Inner_Script_Ptr,2
	     retn
	     endp		

Get_Byte_Number proc
	     call Get_Number
	     mov edx,Cur_Inner_Script_Ptr		     
	     fistp Tmp1
	     mov ax,Tmp1
	     mov [edx],al
	     inc Cur_Inner_Script_Ptr
	     retn
	     endp		

Get_Envelope proc
@@tt1:
	     mov esi,Cur_Script_Ptr
	     call Surp_Separator
	     mov Cur_Script_Ptr,esi

;	     mov esi,Cur_Script_Ptr
	     cmp dword ptr [esi],NULL_SYM
	     jz  @@tt2	
	     call Get_Float_Number_Percent
	     call Get_Float_Number_Percent
	     jnc  @@ok 	
		     ;У нас может получиться нулевая длина из-за
                    ;за приведения в проценты. Игнорируем эти участки
	     sub Cur_Inner_Script_Ptr,2
@@ok:
	     jmp @@tt1	
@@tt2:
	     mov edx,Cur_Inner_Script_Ptr	
	     or  byte ptr [edx-1],ENV_END_MASK
;	     mov eax,[esi]
;mov al,NULL_BYTE	     
;	     mov [edx],al

	     add Cur_Script_Ptr,4
	     retn
	     endp		


Sample_Filename_Token_Proc  proc	     	 	     	
             lea edi,Token_Buf
	     mov esi,Cur_Script_Ptr	
             call Get_One_Token
	     mov Cur_Script_Ptr,esi
	     mov byte ptr [edi],0		

             lea edi,Token_Buf	
	     call Str_Len
	     inc ecx	
;	     lea edi,Sample_File
             lea esi,Token_Buf	
	     rep movsb			     	

	     mov esi,Cur_Script_Ptr
	     retn	
	     endp	

Script_Filename_Token_Proc  proc	     	 	     	
	     mov esi,Cur_Script_Ptr
             lea edi,Token_Buf	
             call Get_One_Token
	     mov Cur_Script_Ptr,esi
	     mov byte ptr [edi],0		

             lea edi,Token_Buf	
	     call Str_Len
pushad
	     inc ecx	
;	     lea edi,Script_File_Name
             lea esi,Token_Buf	
	     rep movsb			     	
popad
	     dec ecx
	     dec ecx
	     lea edi,Inner_Script_File_Name
             lea esi,Token_Buf	
	     rep movsb			     	
	     mov byte ptr[edi],0

	     mov esi,Cur_Script_Ptr
	     retn	
	     endp	


End_Token_Proc  proc	     	 	     	
	     call Simple_Token_Proc
	     retn	
	     endp	

Sequencer_Token2_Proc proc
	     call Simple_Token_Proc
push	Cur_Inner_Script_Ptr	    
	     call Get_Byte_Number
	     mov  edi,Cur_Inner_Script_Ptr	    
	     dec  edi	
	     movzx ecx,byte ptr [edi]	
pop	Cur_Inner_Script_Ptr

             push  ecx
	   	             
	     call Get_Envelope
	     pop   ecx	

	     mov Cur_Note_Num_2,0
	     mov esi,Cur_Script_Ptr
	     mov edi,Cur_Inner_Script_Ptr
@@rep:
	     push esi 

	     mov Cur_Script_Ptr,esi
	     mov Cur_Inner_Script_Ptr,edi
@@tt1:

	     call Surp_Separator

	     lodsd	     	
	     cmp eax,NULL_SYM
	     jz @@ex
call Interp_Note
	     inc Cur_Note_Num_2
	     cmp Cur_Note_Num_2,1
	     jnz @@Not_First_Note
;disabled
;	     or ax,FIRST_NOTE_WORD
@@Not_First_Note:
	     stosb
	     jmp @@tt1			
@@ex:	     		     	
	     mov Cur_Script_Ptr,esi

	     pop esi


	     dec ecx
	     jnz @@rep 	

mov	byte ptr [edi],END_NOTES_WORD
add	edi,1
;	     stosd

	     mov Cur_Inner_Script_Ptr,edi 

	     retn	
	     endp	


Osc_Token3_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number

	     call Get_Float_Number_Percent
	     call Get_Byte_Number
push Cur_Inner_Script_Ptr
	     call Get_Byte_Number
pop Cur_Inner_Script_Ptr
	     retn	
Osc_Token3_Proc endp

Cust_Env_Token_Proc proc

	     call Simple_Token_Proc

@@tt1:
;		call Surp_Separator

	     mov esi,Cur_Script_Ptr
	     cmp dword ptr [esi],NULL_SYM
	     jz  @@tt2	
	     call Get_Word_Number
	     call Get_Word_Number
	     jmp @@tt1	
@@tt2:
	     mov edx,Cur_Inner_Script_Ptr	
	     mov eax,[esi]
	     mov [edx],eax

	     add Cur_Inner_Script_Ptr,4		     		
	     add Cur_Script_Ptr,4

	     retn	
	     endp	


Cust_Sound_Token_Proc proc

	     call Simple_Token_Proc
	     call Get_Envelope
	     call Get_Envelope

	     retn	
	     endp	

Filt_Token_Proc proc

	     call Simple_Token_Proc
	     call Get_Byte_Number
	     mov edi,Cur_Inner_Script_Ptr
;disabled
;	     mov  al,1
;	     stosb          ; Filt cyc num=1	 			

	     mov Cur_Inner_Script_Ptr,edi
	     call Get_Envelope
	     call Get_Envelope
	     call Get_Envelope

	     retn	
	     endp	

Filt_Token2_Proc proc

	     call Simple_Token_Proc
	     call Get_Byte_Number

;disabled
push Cur_Inner_Script_Ptr
	     call Get_Byte_Number
pop Cur_Inner_Script_Ptr

	     call Get_Envelope
	     call Get_Envelope
	     call Get_Envelope

	     retn	
	     endp	




Len_Mul_Token_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     retn	
	     endp	


Delay2_Token_Proc proc

	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number

	     retn	
	     endp	
SET_A_D_Token_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     retn	
	     endp	
SET_BPM_SAMPLE_LEN_Proc  proc
	     call Simple_Token_Proc
;disabled
push Cur_Inner_Script_Ptr
	     call Get_Byte_Number
	     call Get_Byte_Number
pop Cur_Inner_Script_Ptr
	     call Get_Byte_Number
;	     call Get_Byte_Number
	     retn	
	     endp	

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DISABLED
SET_PITCH_Token_Proc proc
;!!!!!!!!!!! effect disabled
push Cur_Inner_Script_Ptr
	     call Simple_Token_Proc
	     call Get_Byte_Number
pop Cur_Inner_Script_Ptr
	     retn	
	     endp	
Dist_Token_Proc proc
;!!!!!!!!!!! effect disabled
push Cur_Inner_Script_Ptr
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     call Get_Envelope
pop Cur_Inner_Script_Ptr

	     retn	
	     endp	
Reverb_Token_Proc  proc
push Cur_Inner_Script_Ptr
	     call Simple_Token_Proc
	     call Get_Byte_Number
@@tt1:
	     mov esi,Cur_Script_Ptr
		call Surp_Separator
	     mov Cur_Script_Ptr,esi

	     mov esi,Cur_Script_Ptr

	     cmp dword ptr [esi],NULL_SYM
	     jz  @@tt2	
	     call Get_Word_Number
	     call Get_Byte_Number
	     jmp @@tt1	
@@tt2:
	     mov edx,Cur_Inner_Script_Ptr
	     mov eax,[esi]
	     mov [edx],eax

	     add Cur_Inner_Script_Ptr,4		     		
	     add Cur_Script_Ptr,4
pop Cur_Inner_Script_Ptr
	     retn	
	     endp	
Cust_Wave_Token_Proc proc
push Cur_Inner_Script_Ptr
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Envelope
pop Cur_Inner_Script_Ptr
	     retn	
	     endp	

Mod_Amp_Token_Proc proc
push Cur_Inner_Script_Ptr
	     call Simple_Token_Proc
	     call Get_Envelope
pop Cur_Inner_Script_Ptr
	     retn	
	     endp	

Harm_Token_Proc proc
push Cur_Inner_Script_Ptr
	     call Simple_Token_Proc
	     call Get_Byte_Number
@@tt1:
	     mov esi,Cur_Script_Ptr
		call Surp_Separator
	     mov Cur_Script_Ptr,esi

	     mov esi,Cur_Script_Ptr

	     cmp dword ptr [esi],NULL_SYM
	     jz  @@tt2	
	     call Get_Float_Number
	     call Get_Float_Number
	     jmp @@tt1	
@@tt2:
	     mov edx,Cur_Inner_Script_Ptr
	     mov eax,[esi]
	     mov [edx],eax

	     add Cur_Inner_Script_Ptr,4		     		
	     add Cur_Script_Ptr,4
pop Cur_Inner_Script_Ptr
	     retn	
	     endp	
