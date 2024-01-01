LIBNAME = parabot_mm.dll

# ------- Debug
CFLAGS += /Zi /DDEBUG /D_DEBUG
LDFLAGS += /DEBUG:FULL
# ------- Optimized
# CFLAGS += /O2 /Oy /GL /GF /Gm- /EHa-s-c- /MT /GS /Gy /GR- /Gd
# LDFLAGS += /DEBUG:NONE /LTCG /OPT:NOREF


INCLUDES = \
	/Isrc/bot \
	/Isrc/utils \
	/Isrc/common \
	/Isrc/dlls \
	/Isrc/engine \
	/Isrc/pm_shared \
	/Isrc/metamod

CPP_MODULES = \
	src/bot/bot.cpp \
	src/bot/bot_client.cpp \
	src/dlls/commands.cpp \
	src/dlls/dll.cpp \
	src/dlls/engine.cpp \
	src/dlls/h_export.cpp \
	src/dlls/linkfunc.cpp \
	src/dlls/startframe.cpp \
	src/dlls/util.cpp \
	src/bot/marker.cpp \
	src/bot/parabot.cpp \
	src/bot/pb_action.cpp \
	src/bot/pb_cell.cpp \
	src/bot/pb_chat.cpp \
	src/bot/pb_combat.cpp \
	src/bot/pb_combatgoals.cpp \
	src/bot/pb_configuration.cpp \
	src/bot/pb_focus.cpp \
	src/bot/pb_global.cpp \
	src/bot/pb_goalfinder.cpp \
	src/bot/pb_goals.cpp \
	src/bot/pb_journey.cpp \
	src/bot/pb_kills.cpp \
	src/bot/pb_mapcells.cpp \
	src/bot/pb_mapgraph.cpp \
	src/bot/pb_mapimport.cpp \
	src/bot/pb_navpoint.cpp \
	src/bot/pb_needs.cpp \
	src/bot/pb_observer.cpp \
	src/bot/pb_path.cpp \
	src/bot/pb_perception.cpp \
	src/bot/pb_roaming.cpp \
	src/bot/pb_sectors.cpp \
	src/bot/pb_vistable.cpp \
	src/bot/pb_weapon.cpp \
	src/bot/pb_weaponhandling.cpp \
	src/bot/pbt_dynarray.cpp \
	src/bot/pbt_priorityqueue.cpp \
	src/bot/sounds.cpp \
	src/bot/utilityfuncs.cpp \
	src/metamod/meta_api.cpp

SRC := $(CPP_MODULES:%.cpp=src/%.cpp)
OBJ := $(CPP_MODULES:%.cpp=build/%.obj)


all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	link /MACHINE:X86 $(LDFLAGS) /OUT:lib-x32/$(LIBNAME) /DLL $(OBJ)

%.obj: %.cpp
	cl $(CFLAGS) /c $(CPPFLAGS) $(INCLUDES) /Fo$@ $<

.PHONY: all clean

clean:
	rm -rf build
	rm -rf lib-x32
