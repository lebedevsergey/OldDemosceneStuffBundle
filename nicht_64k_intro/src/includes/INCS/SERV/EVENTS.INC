PARAM_TIME0   equ  0

locals

.CODE
locals

include f:\demo_sb\player\incs\serv\events.def

Exec_Part proc near
;             xor ecx,ecx
              mov eax,Time
              cmp eax,Next_Part_Time
              jb  Ex_P_Exit

              pushad
              mov esi,Cur_Part_Record
              cmp [esi],NULL

              je  Abort_

              mov eax,[esi].Proc_Time
              mov Next_Part_Time,eax

              mov eax,[esi].Init_Proc_Ptr
              mov Cur_Init_Part,eax

              mov eax,[esi].Proc_Ptr
              mov Cur_Part,eax

              add Cur_Part_Record,SIZE PROC_RECORD


              call Clear_Idle_Proc_Query
              call [Cur_Init_Part]

              popad
Ex_P_Exit:

              mov eax,Time
              cmp eax,Next_Param_Proc_Time
              jb  Ex_P_Exit2

              pushad
              mov esi,Cur_Param_Proc_Record
              cmp [esi],NULL

              je  @@tt1

              mov eax,[esi].Param_Proc_Time
              mov Next_Param_Proc_Time,eax

              mov eax,[esi].Param_Proc_Ptr
              mov Cur_Param_Proc,eax

              add Cur_Param_Proc_Record,SIZE PARAM_PROC_RECORD

@@tt1:
              popad
Ex_P_Exit2:

              ret

Abort_:
              popad
              pop eax
              jmp Abort

endp


Do_Time_Tab  proc
        mov ebx,[esi].Time_Tab_Cur_Ptr
        cmp dword ptr [ebx],NULL
        jz  @@Ev_End
        mov eax,[ebx].Event_Time
        add eax,[esi].Time_Tab_Base_Time
        cmp eax,Time__
        jg  @@No_Ev
        add [esi].Time_Tab_Cur_Ptr,SIZE TIME_TAB_REC
        mov [ebx].Event_Flg,1

        mov eax,[ebx].Event_Proc
        call eax

@@No_Ev:

        retn

@@Ev_End:
        cmp [esi].Time_Tab_Reuse_Flg,1
        jnz @@No_Ev

        mov eax,Time__
        mov [esi].Time_Tab_Base_Time,eax
        mov eax,[esi].Time_Tab_Begin_Ptr
        mov [esi].Time_Tab_Cur_Ptr,eax

        retn
        endp


Empty proc near
        retn
endp

Clear_Idle_Proc_Query  proc near
        mov Idle_Proc_Query,NULL
        mov Idle_Proc_Query_End,offset Idle_Proc_Query
        mov Idle_Proc_Query_Begin,offset Idle_Proc_Query
        retn
        endp

Del_Idle_Proc  proc near
        pushad

        cmp esi,Idle_Proc_Query_End
        jz @@end

        mov edi,esi

        add esi,SIZE IDLE_PROC_REC
@@tt1:
        mov ecx,SIZE IDLE_PROC_REC
        rep movsb
        cmp esi,Idle_Proc_Query_End
        jnz @@tt1
@@end:
        sub esi,SIZE IDLE_PROC_REC
        mov Idle_Proc_Query_End,esi

        popad
        retn
        endp


Add_Idle_Proc  proc near
        mov edi,Idle_Proc_Query_End

        mov [edi].Idle_Proc_End_Time,eax
        mov [edi].Idle_Proc_Ptr,esi

        mov [edi].Idle_Proc_Param1,ebx
        mov [edi].Idle_Proc_Param2,ecx
        mov eax,Time2
        mov [edi].Idle_Proc_Base_Time,eax

        add Idle_Proc_Query_End,SIZE IDLE_PROC_REC

        retn
        endp

Do_Idle_Proc  proc near

        mov esi,Idle_Proc_Query_Begin
@@tt1:
        cmp esi,Idle_Proc_Query_End
        jae  @@ex

        mov  eax,Time2
        sub  eax,[esi].Idle_Proc_Base_Time
        cmp  eax,[esi].Idle_Proc_End_Time
        jge  @@Proc_End

        mov  edx,[esi].Idle_Proc_Ptr
        call edx
        jmp  @@tt2

@@Proc_End:
;       call Del_Idle_Proc
@@tt2:
        add esi,SIZE IDLE_PROC_REC
        jmp @@tt1
@@ex:

        retn

        retn
        endp

.DATA?
Idle_Proc_Query_Begin  dd  ?
Idle_Proc_Query_End    dd  ?
Idle_Proc_Query db MAX_IDLE_PROC_NUM*SIZE IDLE_PROC_REC dup (?)

.DATA
include ev_scrp.inc

Params1:


Cur_Part_Record         dd  offset Parts
Cur_Param_Proc_Record   dd  offset Params1
Next_Param_Proc_Time    dd  PARAM_TIME0
Cur_Param_Proc          dd  offset Empty

.DATA?
Cur_Part                dd  ?
Cur_Init_Part           dd  ?
Next_Part_Time          dd  ?
