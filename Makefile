LIBNAME = parabot_mm.dll

# Cleaner w/o unnecessary stuff - when `make -d`
.SUFFIXES:
MAKEFLAGS += --no-builtin-rules

INCLUDES := /Isrc /Isrc/vendored
SRC := $(wildcard src/*.cpp) $(wildcard src/vendored/*.cpp)

# ------- Debug
CFLAGS := /Zi /DDEBUG /D_DEBUG /MDd
LDFLAGS := /DEBUG:FULL
OUT_DIR_LIB := lib-x32
OUT_DIR_OBJ := build
OBJ := $(SRC:src/%.cpp=$(OUT_DIR_OBJ)/%.obj)

# Compile CPP into OBJ
$(OUT_DIR_OBJ)/%.obj: src/%.cpp
	mkdir -p $(dir $@)
	cl $(CFLAGS) /c $(INCLUDES) /Fo$@ $<

# Link OBJ into DLL/SO
debug: $(OBJ)
	mkdir -p $(OUT_DIR_LIB)
	link /MACHINE:X86 $(LDFLAGS) /OUT:$(OUT_DIR_LIB)/$(LIBNAME) /DLL $(OBJ)

# ------- Release
release: CFLAGS := /O2 /Oy /GL /GF /Gm- /EHa-s-c- /MD /GS /Gy /GR- /Gd /DNDEBUG
release: LDFLAGS := /DEBUG:NONE /LTCG /OPT:NOREF
release: OUT_DIR_LIB := lib-x32/release
OUT_DIR_OBJ_REL := build/release
OBJ_REL := $(SRC:src/%.cpp=$(OUT_DIR_OBJ_REL)/%.obj)

# Compile CPP into OBJ
$(OUT_DIR_OBJ_REL)/%.obj: src/%.cpp
	mkdir -p $(dir $@)
	cl $(CFLAGS) /c $(INCLUDES) /Fo$@ $<

# Link OBJ into DLL/SO
release: $(OBJ_REL)
	mkdir -p $(OUT_DIR_LIB)
	link /MACHINE:X86 $(LDFLAGS) /OUT:$(OUT_DIR_LIB)/$(LIBNAME) /DLL $(OBJ_REL)

# -------

all: debug

clean:
	rm -rf $(OUT_DIR_OBJ)
	rm -rf $(OUT_DIR_LIB)

.PHONY: all clean debug release
