TIME0  equ  013Fh
TIME_1  equ 043Fh

TIME_3 equ   63Fh
TIME_4 equ   083Fh
TIME_5 equ   0A3Fh
TIME_6 equ   0D3Ah
TIME_7 equ   0F3Ah

TIME_8 equ   0113Ah


CRED_TIME1  equ  23fh SHL 8
CRED_TIME2  equ  31fh SHL 8
CRED_TIME3  equ  33fh SHL 8
CRED_TIME4  equ  41fh SHL 8

CRED2_TIME  equ 73eh shl 8
GREET_FLASH_TIME5_2  equ  5 SHL 8
GREET_FLASH_TIME5_3  equ  9 SHL 8
GREET_FLASH_TIME5_4  equ  11 SHL 8
GREET_FLASH_TIME5_6  equ  16 SHL 8
GREET_FLASH_TIME5_7  equ  21 SHL 8
GREET_FLASH_TIME5_8  equ  25 SHL 8
GREET_FLASH_TIME5_9  equ  27 SHL 8
GREET_FLASH_TIME5_10  equ  31 SHL 8
GREET_FLASH_TIME5_11  equ  37 SHL 8
GREET_FLASH_TIME5_12  equ  41 SHL 8
GREET_FLASH_TIME5_14  equ  43 SHL 8
GREET_FLASH_TIME5_15  equ  47 SHL 8
GREET_FLASH_TIME5_16  equ  53 SHL 8
GREET_FLASH_TIME5_17  equ  57 SHL 8
GREET_FLASH_TIME5_18  equ  59 SHL 8

FLARE_TIME6_1  equ   0B3Fh SHL 8
FLARE_TIME6_2  equ   0C3Fh SHL 8

Parts:
              PROC_RECORD  <offset Init_Part0,offset Part0,TIME0>
              PROC_RECORD  <offset Init_Part1,offset Part1,TIME_1>
              PROC_RECORD  <offset Init_Part3,offset Part3,TIME_3>
              PROC_RECORD  <offset Init_Part5,offset Part5,TIME_4>
              PROC_RECORD  <offset Init_Part4,offset Part4,TIME_5>
              PROC_RECORD  <offset Init_Part6,offset Part6,TIME_6>
              PROC_RECORD  <offset Init_Part7,offset Part7,TIME_7>
              PROC_RECORD  <offset Init_Part8,offset Part8,TIME_8>
              dd NULL

Times_Tab1        TIME_TAB <Times_Tab1_,Times_Tab1_,0,0>
Times_Tab1_:
                  TIME_TAB_REC <CRED_TIME1,0,offset Ev_Proc1_1>
                  TIME_TAB_REC <CRED_TIME2,0,offset Ev_Proc1_2>
                  TIME_TAB_REC <CRED_TIME3,0,offset Ev_Proc1_3>
                  TIME_TAB_REC <CRED_TIME4,0,offset Ev_Proc1_4>
                  dd  NULL

Times_Tab5        TIME_TAB <Times_Tab5_0,Times_Tab5_0,0,0>
Times_Tab5_0:
                  TIME_TAB_REC <CRED2_TIME,0,offset Ev_Proc5_0>
                  dd  NULL

Times_Tab5_2        TIME_TAB <Times_Tab5_,Times_Tab5_,800h SHL 8,1>
Times_Tab5_1        TIME_TAB <Times_Tab5_,Times_Tab5_,700h SHL 8,1>
Times_Tab5_:
                  TIME_TAB_REC <GREET_FLASH_TIME5_2,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_3,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_4,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_6,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_7,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_8,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_9,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_10,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_11,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_12,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_14,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_15,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_16,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_17,0,offset Ev_Proc5_1>
                  TIME_TAB_REC <GREET_FLASH_TIME5_18,0,offset Ev_Proc5_1>
                  dd  NULL


Times_Tab6_1        TIME_TAB <Times_Tab5_,Times_Tab5_,0C00h SHL 8,1>
Times_Tab6_2        TIME_TAB <Times_Tab5_,Times_Tab5_,0D00h SHL 8,1>

Times_Tab6        TIME_TAB <Times_Tab6_,Times_Tab6_,0,0>
Times_Tab6_:
                  TIME_TAB_REC <FLARE_TIME6_1,0,offset Ev_Proc6_1>
                  TIME_TAB_REC <FLARE_TIME6_2,0,offset Ev_Proc6_2>
                  dd  NULL

Times_Tab7_1        TIME_TAB <Times_Tab5_,Times_Tab5_,0F00h SHL 8,1>