BUILD_TYPE ?= debug

all: $(BUILD_TYPE)/d3d10.exe

%.fxo: %.fx
	fxc /Od /Zi /T fx_4_0 /nologo /Fo $@ $<

$(BUILD_TYPE)/%.res: %.rc main.fxo
	rc.exe /d "_UNICODE" /d "UNICODE" /fo $@ $<

$(BUILD_TYPE)/%.obj: %.cpp
	cl.exe /Fo"$@" /Fd"$(BUILD_TYPE)\vc80.pdb" @"compile_$(BUILD_TYPE).rsp" $<

#$(BUILD_TYPE)/cesium_man.obj
OBJS = \
	$(BUILD_TYPE)/robot_player.obj \
	$(BUILD_TYPE)/main.obj \
	$(BUILD_TYPE)/main.res

$(BUILD_TYPE)/d3d10.exe: $(OBJS)
	link.exe /OUT:"$(BUILD_TYPE)\d3d10.exe" /PDB:"$(BUILD_TYPE)\d3d10.pdb" @"link_$(BUILD_TYPE).rsp" $(OBJS) /NOLOGO /ERRORREPORT:PROMPT
	mt.exe -manifest d3d10.exe.${BUILD_TYPE}.manifest -outputresource:$(BUILD_TYPE)\d3d10.exe;1
