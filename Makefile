BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE),debug)
OPT = -g -Og
CFLAGS += -D_DEBUG
LDFLAGS += -mconsole
LDOPT =
else
OPT = -O2
LDOPT = -flto
LDFLAGS += -mwindows
endif

PREFIX = i686-w64-mingw32-
WINDRES = $(PREFIX)windres
CXX = $(PREFIX)g++

CXXSTD += -std=gnu++14

CFLAGS += -march=core2
CFLAGS += -Wall -Werror -Wfatal-errors
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -I./include
CFLAGS += -municode
LDFLAGS += -municode
LDFLAGS += -static-libgcc

CXXFLAGS += -fno-exceptions

LIBS = -ld3d10

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
	$(BUILD_TYPE)/effect/static.fxo \
	$(BUILD_TYPE)/effect/collada.fxo \
	$(BUILD_TYPE)/effect/collada_scene.fxo

SCENES = \
	src/scenes/curve_interpolation/curve_interpolation.cpp

include curve_interpolation.mk

RC_FILES = $(wildcard rc/*.rc)

$(BUILD_TYPE)/main.rc: $(RC_FILES)
	cat $^ > $@

$(BUILD_TYPE)/%.res: $(BUILD_TYPE)/%.rc $(SHADERS) $(SCENES) $(IMAGES)
	@mkdir -p $(@D)
	$(WINDRES) -O coff -I$(BUILD_TYPE)/effect -o $@ $<

$(BUILD_TYPE)/%.obj: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXSTD) $(CFLAGS) $(CXXFLAGS) $(WOPT) $(OPT) -o $@ -c $<

COLLADA_PY_SOURCE = \
	$(wildcard collada/*.py)

include/scenes/%.hpp: $(COLLADA_PY_SOURCE)
	@mkdir -p $(@D)
	PYTHONPATH=. python -m collada.main $@

src/scenes/%.cpp: scenes/%.DAE include/scenes/%.hpp
	@mkdir -p $(@D)
	PYTHONPATH=. python -m collada.main $< $@ $(<:.DAE=.vtx) $(<:.DAE=.idx) rc/$(notdir $(<:.DAE=.rc)) $(notdir $(<:.DAE=.mk))

#	$(BUILD_TYPE)/curve_interpolation.res \

OBJS = \
	$(BUILD_TYPE)/main.res \
	$(BUILD_TYPE)/robot_player.obj \
	$(BUILD_TYPE)/cube.obj \
	$(BUILD_TYPE)/main.obj \
	$(BUILD_TYPE)/print.obj \
	$(BUILD_TYPE)/render_state.obj \
	$(BUILD_TYPE)/input.obj \
	$(BUILD_TYPE)/dds_validate.obj \
	$(BUILD_TYPE)/collada.obj \
	$(BUILD_TYPE)/collada_scene.obj \
	$(BUILD_TYPE)/scenes/curve_interpolation/curve_interpolation.obj

$(BUILD_TYPE)/d3d10.exe: $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(LDOPT) -o $@ $(OBJS) $(LIBS)

.SUFFIXES:
.INTERMEDIATE:
.SECONDARY:
.PHONY: all clean

%: RCS/%,v
%: RCS/%
%: %,v
%: s.%
%: SCCS/s.%
