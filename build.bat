cl.exe @"compile.rsp" "main.cpp"

link.exe @"link.rsp" /OUT:"Debug\d3d10.exe" /PDB:"Debug\d3d10.pdb" ".\Debug\main.obj" /NOLOGO /ERRORREPORT:PROMPT

fxc /Od /Zi /T fx_4_0 /nologo /Fo main.fxo main.fx

@if %errorlevel% neq 0 exit /b %errorlevel%

Debug\d3d10.exe
