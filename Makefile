LIBNAME = parabot_mm.dll
OUT_DIR_OBJ = build
OUT_DIR_LIB = lib-x32

# ------- Debug
CFLAGS += /Zi /DDEBUG /D_DEBUG /MDd
LDFLAGS += /DEBUG:FULL
# ------- Optimized
# CFLAGS += /O2 /Oy /GL /GF /Gm- /EHa-s-c- /MD /GS /Gy /GR- /Gd
# LDFLAGS += /DEBUG:NONE /LTCG /OPT:NOREF

# Cleaner w/o unnecessary stuff - when `make -d`
.SUFFIXES:
MAKEFLAGS += --no-builtin-rules

INCLUDES := \
	/Isrc \
	/Isrc/vendored

SRC := $(wildcard src/*.cpp) $(wildcard src/vendored/*.cpp)
OBJ := $(SRC:src/%.cpp=$(OUT_DIR_OBJ)/%.obj)

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	mkdir -p $(OUT_DIR_LIB)
	link /MACHINE:X86 $(LDFLAGS) /OUT:$(OUT_DIR_LIB)/$(LIBNAME) /DLL $(OBJ)

$(OUT_DIR_OBJ)/%.obj: src/%.cpp
	mkdir -p $(dir $@)
	cl $(CFLAGS) /c $(CPPFLAGS) $(INCLUDES) /Fo$@ $<

.PHONY: all clean

clean:
	rm -rf $(OUT_DIR_OBJ)
	rm -rf $(OUT_DIR_LIB)
