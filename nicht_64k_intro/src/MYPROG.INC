        finit

        call Init_sin2
        call Calc_Fade_Tab

        call  Do_Objects
        call  Do_Textures
        call  Do_Tables
        call  Load_Fonts	

        call Exec_Part
        mov Time,0

        mov alpha1,250;50

        call Player_Play

        call Clear_Idle_Proc_Query

mainloop:
        call Exec_Part

        call Player_Get_Time

        call [Cur_Part]

        Draw_Virt_Screen

        in  al,60h
        cmp al,1
        jnz mainloop
Abort:
