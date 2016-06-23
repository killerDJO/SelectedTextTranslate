ECHO OFF
ECHO Copy DLLs...

SET OutputDir=%1
SET ProjectDir=%2
SET Configuration=%3

REM Validate output folder
IF "%OutputDir%"=="" GOTO NO_OUTPUT_SPECIFIED
IF NOT EXIST "%OutputDir%" GOTO OUTPUT_INVALID

REM Validate project folder
IF "%ProjectDir%"=="" GOTO NO_PROJECT_SPECIFIED
IF NOT EXIST "%ProjectDir%" GOTO PROJECT_INVALID

XCOPY /y "%ProjectDir%libs\libcurl\lib\release\curllib.dll" %OutputDir%
XCOPY /y "%ProjectDir%libs\jsoncpp\build\vs71\%Configuration%\lib_json\*.dll" %OutputDir%
XCOPY /y "%ProjectDir%Bin\*.dll" %OutputDir%

GOTO END

:NO_OUTPUT_SPECIFIED
	ECHO No output directory specified.
	GOTO END

:OUTPUT_INVALID
	ECHO The output directory specified is invalid.  Output must be an existing, fully qualified folder.
	GOTO END

:NO_PROJECT_SPECIFIED
	ECHO No output directory specified.
	GOTO END

:PROJECT_INVALID
	ECHO The output directory specified is invalid.  Output must be an existing, fully qualified folder.
	GOTO END

:END