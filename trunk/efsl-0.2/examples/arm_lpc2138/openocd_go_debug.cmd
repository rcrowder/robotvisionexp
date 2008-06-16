@echo off

call openocd_env_info.cmd

start %OOCD_EXE% %OOCD_DBG% -f %OOCD_CFG_DBG%

rem pause

