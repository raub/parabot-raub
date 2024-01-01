# Makefile for parabot
# Copyright © nekonomicon 2016

# ------- Debug
CFLAGS += /Zi
LDFLAGS += /DEBUG:FULL
# ------- Optimized
# CFLAGS += /O2 /Oy /GL /GF /Gm- /EHa-s-c- /MT /GS /Gy /GR- /Gd
# LDFLAGS += /DEBUG:NONE /LTCG /OPT:NOREF

OSLIB = dll

INCLUDES = -I. \
		-I../bot \
		-I../utils \
		-I../common \
		-I../dlls \
		-I../engine \
		-I../pm_shared \
		-I../metamod

SRC = ../bot/bot.cpp \
		../bot/bot_client.cpp \
		commands.cpp \
		dll.cpp \
		engine.cpp \
		h_export.cpp \
		linkfunc.cpp \
		startframe.cpp \
		util.cpp \
		../bot/marker.cpp \
		../bot/parabot.cpp \
		../bot/pb_action.cpp \
		../bot/pb_cell.cpp \
		../bot/pb_chat.cpp \
		../bot/pb_combat.cpp \
		../bot/pb_combatgoals.cpp \
		../bot/pb_configuration.cpp \
		../bot/pb_focus.cpp \
		../bot/pb_global.cpp \
		../bot/pb_goalfinder.cpp \
		../bot/pb_goals.cpp \
		../bot/pb_journey.cpp \
		../bot/pb_kills.cpp \
		../bot/pb_mapcells.cpp \
		../bot/pb_mapgraph.cpp \
		../bot/pb_mapimport.cpp \
		../bot/pb_navpoint.cpp \
		../bot/pb_needs.cpp \
		../bot/pb_observer.cpp \
		../bot/pb_path.cpp \
		../bot/pb_perception.cpp \
		../bot/pb_roaming.cpp \
		../bot/pb_sectors.cpp \
		../bot/pb_vistable.cpp \
		../bot/pb_weapon.cpp \
		../bot/pb_weaponhandling.cpp \
		../bot/pbt_dynarray.cpp \
		../bot/pbt_priorityqueue.cpp \
		../bot/sounds.cpp \
		../bot/utilityfuncs.cpp \
		../metamod/meta_api.cpp

LIBNAME = parabot_mm.dll

OBJ := $(SRC:.cpp=.obj)

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	link /MACHINE:X86 $(LDFLAGS) /OUT:$(LIBNAME) /DLL $(OBJ)

%.obj: %.cpp
	cl $(CFLAGS) /c $(CPPFLAGS) $(INCLUDES) /Fo$@ $<

.PHONY: all clean

clean:
	rm -rf $(OBJ)
	rm $(LIBNAME)
