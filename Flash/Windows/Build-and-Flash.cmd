@echo off
setlocal EnableExtensions DisableDelayedExpansion
call "%~dp0..\..\FreeEEG32.cmd" build-flash %*
set "FREEEEG32_RESULT=%ERRORLEVEL%"
if "%~1"=="" pause
exit /b %FREEEEG32_RESULT%
