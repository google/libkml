@echo off

rem This file is called by run_tests.bat. Do not run it manually.

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


