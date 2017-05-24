; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

; synth scripts parser functions and data

.data
Token_Tab      TOKEN_STRUC <offset End_Token,offset End_Token_Proc,END_COM>
	   TOKEN_STRUC <offset Sequencer_Token,offset Sequencer_Token_Proc,SEQ_COM>
	   TOKEN_STRUC <offset Sequencer_Token2,offset Sequencer_Token2_Proc,SEQ_COM>
	   TOKEN_STRUC <offset Sample_Filename_Token,offset Sample_Filename_Token_Proc,0>
	   TOKEN_STRUC <offset Script_Filename_Token,offset Script_Filename_Token_Proc,0>
	   TOKEN_STRUC <offset SET_BPM_SAMPLE_LEN_Token,offset SET_BPM_SAMPLE_LEN_Proc,SET_BPM_SAMPLE_LEN_COM>
	   TOKEN_STRUC <offset Osc_Token,offset Osc_Token_Proc,SETUP_OSC_COM>
	   TOKEN_STRUC <offset Osc_Token2,offset Osc_Token2_Proc,SETUP_OSC_COM>
	   TOKEN_STRUC <offset Osc_Token3,offset Osc_Token3_Proc,SETUP_OSC_COM>
	   TOKEN_STRUC <offset Cust_Sound_Token,offset Cust_Sound_Token_Proc,DO_CUSTOM_SOUND_COM>
	   TOKEN_STRUC <offset Filt_Token,offset Filt_Token_Proc,FILT_COM>
	   TOKEN_STRUC <offset Filt_Token2,offset Filt_Token2_Proc,FILT_COM>
	   TOKEN_STRUC <offset Harm_Token,offset Harm_Token_Proc,SETUP_HARM_COM>
	   TOKEN_STRUC <offset Reverb_Token,offset Reverb_Token_Proc,REVERB_COM>
	   TOKEN_STRUC <offset Dist_Token,offset Dist_Token_Proc,DISTORT_COM>
	   TOKEN_STRUC <offset Delay_Token,offset Reverb_Token_Proc,DELAY_COM>
	   TOKEN_STRUC <offset Mod_Amp_Token,offset Mod_Amp_Token_Proc,MOD_AMP_COM>
	   TOKEN_STRUC <offset Len_Mul_Token,offset Len_Mul_Token_Proc,SET_LEN_MUL_COM>
	   TOKEN_STRUC <offset Cust_Wave_Token,offset Cust_Wave_Token_Proc,SET_CUST_WAVE_COM>
	   TOKEN_STRUC <offset Delay2_Token,offset Delay2_Token_Proc,DELAY2_COM>
	   TOKEN_STRUC <offset SET_PITCH_Token,offset SET_PITCH_Token_Proc,SET_PITCH_SPEED_COM>
	   TOKEN_STRUC <offset SET_A_D_Token,offset SET_A_D_Token_Proc,SET_A_D_COM>

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
	     call Get_Float_Number
	     call Get_Float_Number
	     jmp @@tt1	
@@tt2:
	     mov edx,Cur_Inner_Script_Ptr	
	     mov eax,[esi]
	     mov [edx],eax

	     add Cur_Inner_Script_Ptr,4		     		
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

Sequencer_Token_Proc proc

	     call Simple_Token_Proc
	     mov  al,1
	     stosb          ; Notes cyc num=1	 	
	     call Get_Envelope

	     mov esi,Cur_Script_Ptr
	     mov edi,Cur_Inner_Script_Ptr	
@@tt1:
	     call Surp_Separator

	     lodsd	     	
	     stosd
	     cmp eax,NULL_SYM
	     jnz @@tt1			
	     		     	
	     mov Cur_Inner_Script_Ptr,edi 
	     mov Cur_Script_Ptr,esi

	     retn	
	     endp	

Sequencer_Token2_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     mov  edi,Cur_Inner_Script_Ptr	    

	     dec  edi	
	     movzx ecx,byte ptr [edi]	
             push  ecx
	   	             
	     call Get_Envelope
	     pop   ecx	

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
	     stosd
	     jmp @@tt1			
@@ex:	     		     	
	     mov Cur_Script_Ptr,esi

	     pop esi

	     dec ecx
	     jnz @@rep 	

	     stosd

	     mov Cur_Inner_Script_Ptr,edi 

	     retn	
	     endp	

Osc_Token_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     call Get_Float_Number

	     mov  edi,Cur_Inner_Script_Ptr
	     mov  ax,Percent2
	     shr  ax,1	
	     stosb	
	     mov  ax,MAX_OSC_SHAPE_PARAM
	     stosb
	     mov  Cur_Inner_Script_Ptr,edi 

	     retn	
Osc_Token_Proc endp

Osc_Token2_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     call Get_Float_Number
	     call Get_Byte_Number
	     mov  edi,Cur_Inner_Script_Ptr
	     mov  ax,MAX_OSC_SHAPE_PARAM
	     stosb
	     mov  Cur_Inner_Script_Ptr,edi 
	     retn	
Osc_Token2_Proc endp

Osc_Token3_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     call Get_Float_Number
	     call Get_Byte_Number
	     call Get_Byte_Number
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

SET_BPM_SAMPLE_LEN_Proc  proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     call Get_Byte_Number
;	     call Get_Byte_Number
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
	     mov  al,1
	     stosb          ; Filt cyc num=1	 			
	     mov Cur_Inner_Script_Ptr,edi
	     call Get_Envelope
	     call Get_Envelope
	     call Get_Envelope

	     retn	
	     endp	

Filt_Token2_Proc proc

	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number

	     call Get_Envelope
	     call Get_Envelope
	     call Get_Envelope

	     retn	
	     endp	

Harm_Token_Proc proc

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

	     retn	
	     endp	

Reverb_Token_Proc  proc
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

	     retn	
	     endp	

Dist_Token_Proc proc

	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     call Get_Envelope

	     retn	
	     endp	

Mod_Amp_Token_Proc proc

	     call Simple_Token_Proc
	     call Get_Envelope

	     retn	
	     endp	


Len_Mul_Token_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     retn	
	     endp	

Cust_Wave_Token_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Envelope
	     retn	
	     endp	

Delay2_Token_Proc proc

	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number

	     retn	
	     endp	

SET_PITCH_Token_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number

	     retn	
	     endp	

SET_A_D_Token_Proc proc
	     call Simple_Token_Proc
	     call Get_Byte_Number
	     call Get_Byte_Number
	     retn	
	     endp	