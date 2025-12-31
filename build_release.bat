fxc /Od /Zi /T fx_4_0 /nologo /Fo main.fxo main.fx

@if %errorlevel% neq 0 exit /b %errorlevel%

rem build main resource
rc.exe /d "_UNICODE" /d "UNICODE" /fo"Release\main.res" ".\main.rc"

@if %errorlevel% neq 0 exit /b %errorlevel%

rem compile
cl.exe @"compile_release.rsp" "main.cpp"

@if %errorlevel% neq 0 exit /b %errorlevel%

rem link
link.exe @"link_release.rsp" /NOLOGO /ERRORREPORT:PROMPT

@if %errorlevel% neq 0 exit /b %errorlevel%

mt.exe -manifest d3d10.exe.release.manifest -outputresource:Release\d3d10.exe;1

@if %errorlevel% neq 0 exit /b %errorlevel%

Release\d3d10.exe
