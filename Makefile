BUILD_TYPE ?= debug

PREFIX = i686-w64-mingw32-
WINDRES = $(PREFIX)windres
CXX = $(PREFIX)g++

OPT = -g -Og

CXXSTD += -std=gnu++14

CFLAGS += -Wall -Werror -Wfatal-errors
CFLAGS += -Wno-unused-but-set-variable
CXXFLAGS += -fno-exceptions

CFLAGS += -municode

WOPT += -municode

FXC ?= C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Utilities/bin/x86/fxc.exe

INCLUDE = \
	-I./include

%.fxo: %.fx
	@echo fxc.exe @"shader_$(BUILD_TYPE).rsp" /T fx_4_0 /nologo /Fo $@ $<
	@wine "$(FXC)" @"shader_$(BUILD_TYPE).rsp" /T fx_4_0 /nologo /Fo $@ $<

SHADERS = \
	main.fxo \
	font.fxo \
	volume.fxo \
	bloom.fxo \
	static.fxo

$(BUILD_TYPE)/%.res: %.rc $(SHADERS)
	$(WINDRES) -O coff -o $@ $<

$(BUILD_TYPE)/%.obj: src/%.cpp
	$(CXX) $(CXXSTD) $(CFLAGS) $(CXXFLAGS) $(WOPT) $(OPT) -o $@ $(INCLUDE) -c $<

OBJS = \
	$(BUILD_TYPE)/robot_player.obj \
	$(BUILD_TYPE)/cube.obj \
	$(BUILD_TYPE)/main.obj \
	$(BUILD_TYPE)/print.obj \
	$(BUILD_TYPE)/render_state.obj \
	$(BUILD_TYPE)/main.res

$(BUILD_TYPE)/d3d10.exe: $(OBJS)
	$(CXX) $(LDFLAGS) $(WOPT) -o $@ $(OBJS) -ld3dx10 -ld3d10
