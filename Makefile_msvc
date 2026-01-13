BUILD_TYPE ?= debug

all: $(BUILD_TYPE)/d3d10.exe
	$(BUILD_TYPE)/d3d10.exe

%.fxo: %.fx
	fxc.exe @"shader_$(BUILD_TYPE).rsp" /T fx_4_0 /nologo /Fo $@ $<

SHADERS = \
	main.fxo \
	font.fxo \
	volume.fxo \
	bloom.fxo \
	static.fxo

$(BUILD_TYPE)/%.res: %.rc $(SHADERS)
	rc.exe /d "_UNICODE" /d "UNICODE" /fo $@ $<

$(BUILD_TYPE)/%.obj: src/%.cpp
	cl.exe /Fo"$@" /Fd"$(BUILD_TYPE)\vc80.pdb" @"compile_$(BUILD_TYPE).rsp" $<

OBJS = \
	$(BUILD_TYPE)/robot_player.obj \
	$(BUILD_TYPE)/cube.obj \
	$(BUILD_TYPE)/main.obj \
	$(BUILD_TYPE)/print.obj \
	$(BUILD_TYPE)/render_state.obj \
	$(BUILD_TYPE)/main.res

$(BUILD_TYPE)/d3d10.exe: $(OBJS)
	link.exe /OUT:"$(BUILD_TYPE)\d3d10.exe" /PDB:"$(BUILD_TYPE)\d3d10.pdb" @"link_$(BUILD_TYPE).rsp" $(OBJS) /NOLOGO /ERRORREPORT:PROMPT
	mt.exe -manifest d3d10.exe.${BUILD_TYPE}.manifest -outputresource:$(BUILD_TYPE)\d3d10.exe;1 -nologo
