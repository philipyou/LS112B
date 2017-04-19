@set enc=1
@set enckey=0000000000000000
@echo off
setlocal enabledelayedexpansion
del /S /Q output
mkdir output
copy   .\module.dat + .\chipsea.dat + .\revd.dat .\sched.rom

goto genromrevc

:genromrevc
@echo Start to generate EEPROM code

.\geneep ramcode.rom fwram.rom .\sched.rom 0 %enckey% c

echo eeprom.dat Generated........

perl eeprom2hciimage.pl

echo bt_patch.bin Generated........

move bt_patch.bin .\output
move eeprom.dat .\output
move otp.dat .\output

del /Q sched.rom

echo off

:gen_eep_end
echo **********************************
echo EEPROM Generated.
echo The Device is %device_option%. 
echo Use command "e ep" to download EEPROM.
echo **********************************
goto end


:end