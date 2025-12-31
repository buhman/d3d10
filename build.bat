fxc /Od /Zi /T fx_4_0 /nologo /Fo main.fxo main.fx

rem build main resource
rc.exe /d "_UNICODE" /d "UNICODE" /fo"Debug\main.res" ".\main.rc"

rem compile
cl.exe @"compile.rsp" "main.cpp"

rem link
link.exe @"link.rsp" /NOLOGO /ERRORREPORT:PROMPT

mt.exe -manifest d3d10.exe.manifest -outputresource:Debug\d3d10.exe;1

@if %errorlevel% neq 0 exit /b %errorlevel%

Debug\d3d10.exe
