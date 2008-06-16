
rem set OOCD_INSTALLDIR=C:\Programme\openocd-2006re100\bin
rem set OOCD_EXE=%OOCD_INSTALLDIR%\openocd-ftd2xx.exe
rem set OOCD_EXE=%OOCD_INSTALLDIR%\openocd-pp.exe

set OOCD_INSTALLDIR=C:\WinARM\utils\OpenOCD
set OOCD_EXE=%OOCD_INSTALLDIR%\openocd.exe

rem The used interface either FTDI(=WinARM-JTAG, JTAGKEY etc.) or PP(="Wiggler")
rem set OOCD_INTERFACE=ftdi
set OOCD_INTERFACE=pp

set OOCD_TARGET=lpc2378
rem set OOCD_OPT=-d 2
set OOCD_OPT=

set OOCD_DBG_CONFIG=openocd_dbg_%OOCD_TARGET%_%OOCD_INTERFACE%.cfg
set OOCD_FLASH_CONFIG=openocd_flash_%OOCD_TARGET%_%OOCD_INTERFACE%.cfg
