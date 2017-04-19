@set FPGA_PATH=fpgajic\fpga
@set ROM_PATH=.
@set MV_PATCH=mv\src\yichip
@set YC_PATCH_FILE=yc_patch_yc1021.h
@set enc=1
@set enckey=0000000000000000
@echo off
setlocal enabledelayedexpansion

copy patch\patch.prog output\bt_program23.meta
copy rom\rom.format  + rom\label.format + patch\patch.format + rom\command.format output\bt_format.meta
perl util/mergepatch.pl output/bt_program23.meta
perl util/memalloc.pl output/bt_format.meta
cd output
osiuasm bt_program23 -O-W
copy   ..\sched\module.dat + ..\sched\chipsea.dat + ..\sched\revd.dat ..\output\sched.rom


if "%1" equ "eep" (
	goto genromrevc
)else (
  goto downloadram
)



:genromrevc
@echo Start to generate EEPROM code

..\util\geneep ramcode.rom fwram.rom sched.rom 0 %enckey% c
cd ..\output

echo eeprom.dat Generated........

:perl eeprom2hciimage.pl

:echo bt_patch.bin Generated........

echo off

:gen_eep_end
cd ..
echo **********************************
echo EEPROM Generated.
echo The Device is %device_option%. 
echo Use command "e ep" to download EEPROM.
echo **********************************
goto end



:downloadram
echo on
@echo Start to download ram code
e ku
e hu
e su sched.rom
@echo **********************************
@echo RAM CODE has been downloaded.
@echo The Device is %device_option%. 
@echo **********************************
cd ..


:end
copy .\output\memmap.format ..\MCU51\output
copy .\output\ramcode.rom ..\MCU51\output
copy .\output\sched.rom ..\MCU51\output


