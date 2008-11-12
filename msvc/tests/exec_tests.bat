@echo off

rem This file is called by run_tests.bat. Do not run it manually.

set OLDPATH=%PATH%
set PATH=..\..\third_party\expat.win32;%PATH%

for %%i in (%1\*.exe) do (
  echo running %%i
  %%i
  if errorlevel 1 goto failures
)

echo ALL TESTS PASSED
goto end

:failures
echo TEST FAILED

:end
set PATH=%OLDPATH%

