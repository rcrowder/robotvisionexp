@echo off
call openocd_setup.cmd

rem start %OOCD_EXE% %OOCD_OPT% -f %OOCD_FLASH_CONFIG%
%OOCD_EXE% %OOCD_OPT% -f %OOCD_FLASH_CONFIG%

rem pause

