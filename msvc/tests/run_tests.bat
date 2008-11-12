@echo off

rem First build the debug tests in Visual Studio, then run this script from
rem this directory to execute all tests.
rem
rem TODO: disabled tests:
rem TODO: kmlbase_date_time_test

if not exist Debug\libkmlbase_attributes_test.exe (
  echo No debug tests exist
  goto end
)

call exec_tests Debug

rem If the release tests have been built, run them too.
if not exist Release\libkmlbase_attributes_test.exe (
  echo No release tests exist
  goto end
)
call exec_tests Release

:end

