BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE),debug)
OPT = -g -Og
LDOPT =
else
OPT = -O2
LDOPT = -flto
endif

PREFIX = i686-w64-mingw32-
WINDRES = $(PREFIX)windres
CXX = $(PREFIX)g++

CXXSTD += -std=gnu++14

CFLAGS += -march=core2
CFLAGS += -Wall -Werror -Wfatal-errors
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -municode
CFLAGS += -I./include
LDFLAGS += -municode
LIBS += -ld3d10

CXXFLAGS += -fno-exceptions

all: $(BUILD_TYPE)/d3d10.exe

$(BUILD_TYPE)/%.fxo: src/%.fx
	@mkdir -p $(@D)
ifeq ($(OS),Windows_NT)
	fxc.exe @"shader_$(BUILD_TYPE).rsp" /T fx_4_0 /nologo /Fo $@ $<
else
	@echo fxc.exe @"shader_$(BUILD_TYPE).rsp" /T fx_4_0 /nologo /Fo $@ $<
	@wine fxc.exe @"shader_$(BUILD_TYPE).rsp" /T fx_4_0 /nologo /Fo $@ $<
endif

SHADERS = \
	$(BUILD_TYPE)/effect/main.fxo \
	$(BUILD_TYPE)/effect/font.fxo \
	$(BUILD_TYPE)/effect/volume.fxo \
	$(BUILD_TYPE)/effect/bloom.fxo \
	$(BUILD_TYPE)/effect/static.fxo

$(BUILD_TYPE)/%.res: %.rc $(SHADERS)
	@mkdir -p $(@D)
	$(WINDRES) -O coff -I$(BUILD_TYPE)/effect -o $@ $<

$(BUILD_TYPE)/%.obj: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXSTD) $(CFLAGS) $(CXXFLAGS) $(WOPT) $(OPT) -o $@ -c $<

OBJS = \
	$(BUILD_TYPE)/robot_player.obj \
	$(BUILD_TYPE)/cube.obj \
	$(BUILD_TYPE)/main.obj \
	$(BUILD_TYPE)/print.obj \
	$(BUILD_TYPE)/render_state.obj \
	$(BUILD_TYPE)/main.res

$(BUILD_TYPE)/d3d10.exe: $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(LDOPT) -o $@ $(OBJS) $(LIBS)
