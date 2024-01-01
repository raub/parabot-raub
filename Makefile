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
	/Isrc/bot \
	/Isrc/utils \
	/Isrc/common \
	/Isrc/dlls \
	/Isrc/engine \
	/Isrc/pm_shared \
	/Isrc/metamod

CPP_MODULES := \
	bot/bot.cpp \
	bot/bot_client.cpp \
	dlls/commands.cpp \
	dlls/dll.cpp \
	dlls/engine.cpp \
	dlls/h_export.cpp \
	dlls/linkfunc.cpp \
	dlls/startframe.cpp \
	dlls/util.cpp \
	bot/marker.cpp \
	bot/parabot.cpp \
	bot/pb_action.cpp \
	bot/pb_cell.cpp \
	bot/pb_chat.cpp \
	bot/pb_combat.cpp \
	bot/pb_combatgoals.cpp \
	bot/pb_configuration.cpp \
	bot/pb_focus.cpp \
	bot/pb_global.cpp \
	bot/pb_goalfinder.cpp \
	bot/pb_goals.cpp \
	bot/pb_journey.cpp \
	bot/pb_kills.cpp \
	bot/pb_mapcells.cpp \
	bot/pb_mapgraph.cpp \
	bot/pb_mapimport.cpp \
	bot/pb_navpoint.cpp \
	bot/pb_needs.cpp \
	bot/pb_observer.cpp \
	bot/pb_path.cpp \
	bot/pb_perception.cpp \
	bot/pb_roaming.cpp \
	bot/pb_sectors.cpp \
	bot/pb_vistable.cpp \
	bot/pb_weapon.cpp \
	bot/pb_weaponhandling.cpp \
	bot/pbt_dynarray.cpp \
	bot/pbt_priorityqueue.cpp \
	bot/sounds.cpp \
	bot/utilityfuncs.cpp \
	metamod/meta_api.cpp

SRC := $(CPP_MODULES:%.cpp=src/%.cpp)
OBJ := $(CPP_MODULES:%.cpp=$(OUT_DIR_OBJ)/%.obj)

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
