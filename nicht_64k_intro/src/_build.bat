del vesatest.obj
del vesatest.exe

tasmx vesatest.as2 /m >f
wlink system zrdx file vesatest,F:\DEMO_SB\PLAYER\mxmplay,d:\nicht\player\incs\font\font4_.obj >file
wlink system zrdx file vesatest,F:\DEMO_SB\PLAYER\mxmplay >file
vesatest.exe
