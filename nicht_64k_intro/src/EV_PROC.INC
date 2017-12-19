.CODE

Ev_Proc1_1  proc

mov Cred1_Flg,1

        retn
endp

Ev_Proc1_2  proc

mov Cred2_Flg,1

        retn
endp


Ev_Proc1_3  proc

mov Cred1_Flg,0
mov Cred2_Flg,0
mov Cred3_Flg,1

        retn
endp

Ev_Proc1_4  proc

mov Cred4_Flg,1

        retn
endp

Ev_Proc6_1  proc

mov dword ptr Scene6_Patch,offset Part_Object6

mov Cam_Path_Flg6,1
;mov Man_Angle_Y_Coef,600000

        retn
endp

Ev_Proc5_1  proc
            inc Highlight_Greet_Num
            cmp Highlight_Greet_Num,5
            jna @@Ok
            mov Highlight_Greet_Num,1
@@Ok:

xor Man_Flg,1
        mov  eax,Time2
        mov  PULSE_BASE_TIME,eax


        retn
endp

Ev_Proc5_0:
            mov Highlight_Greet_Num,1
        call Clear_Idle_Proc_Query
mov Greets1_Flg,1


        retn
endp

Ev_Proc6_2  proc
mov Man2_Flg,1
        call Clear_Idle_Proc_Query
        retn
endp
