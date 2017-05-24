# Software synthesizer sources #

#### see English description below ####

### Что это? ###

Исходники синтезатора сэмплов by SunmaN//THROB. 

*/bin - скомпилированный синт с GUI интерфейсом
*/presets - примеры сэмплов
*/src/gui - исходники GUI интерфейса синта (Delphi)
*/src/synth - исходники синта (TASM+MASM), компилится в .dll, которую юзает интерфейс. Из main.asm видно как синт ставить в интру и что при этом можно выкинуть.
*/bin/util/syn_conv.exe - конвертер текстового скрипта во внутренний формат для использования в интро берет скрипт из 1.scr и конвертит его в 1.ins знаю, что неудобно, но делалась конвертилка на скорую руку.
*/history.sorry_only_russian_language.txt - история версий
*/manual.sorry_only_russian_language.txt - документация

### Copyright ###
* (c) Сергей Лебедев, 2002, программа распространяется на условиях лицензии Apache 2.0
* Cвязаться cо мною можно через:
 * https://habrahabr.ru/users/sunman/
 * http://stackoverflow.com/users/7135046/sergeylebedev
 * https://www.facebook.com/sergei.lebedev.5891 

## English description ##

### Softsynth sources ###
(c) 2002 Sergey Lebedev, licensed under the Apache License, Version 2.0

/src/gui - synth GUI sources (Delphi)
/src/synth - synth sources (TASM+MASM), compiles to DLL used by GUI
/bin - synth compiled binary
*/bin/util/syn_conv.exe - converts text script to inner script for use in 64k intro, reads text script from 1.scr and writes the inner script to 1.ins
/presets - samples examples

*/history.sorry_only_russian_language.txt - changes list, sorry only in Russian language
*/manual.sorry_only_russian_language.txt - manual, sorry only in Russian language


********************************************
here goes original description from 2002 AD
********************************************

Sources of softsynth by SunmaN//THROB. 
Licensed for non-commercial purposes.
Contact the author at: sunman_pfc@mail.ru

Sorry for only Russian comments, but I hope, variables' names can replace english comments

SunmaN//THROB
26.05.2002