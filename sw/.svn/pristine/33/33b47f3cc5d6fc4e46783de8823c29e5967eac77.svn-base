@echo off
setlocal enabledelayedexpansion
del /S /Q genfile
mkdir genfile
cd output
copy ..\sched\module.dat ..\genfile\
copy ..\sched\chipsea.dat ..\genfile\
copy ..\sched\revd.dat ..\genfile\
copy ..\util\geneep.exe ..\genfile\
copy .\eeprom2hciimage.pl ..\genfile\
copy .\ramcode.rom ..\genfile\
copy .\memmap.format ..\genfile\
copy ..\util\genfile.bat ..\genfile\
pause
