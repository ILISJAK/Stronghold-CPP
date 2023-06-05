@echo off
start cmd /k "cd /D D:\dev\repos\projekt\StrongholdCPP\build\release && StrongholdCPP"
set API_PID=%PID%
start cmd /k "cd /D D:\dev\repos\projekt\StrongholdCPP && python APItest.py"
set TEST_PID=%PID%

:CHECK
tasklist /FI "PID eq %API_PID%" | find ":" > NUL
if errorlevel 1 goto :END
tasklist /FI "PID eq %TEST_PID%" | find ":" > NUL
if errorlevel 1 goto :END
timeout /t 1
goto :CHECK

:END
taskkill /PID %API_PID% /F
taskkill /PID %TEST_PID% /F
