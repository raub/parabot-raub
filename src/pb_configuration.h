#ifndef _PB_CONFIGURATION_HPP_
#define _PB_CONFIGURATION_HPP_

#include <stdio.h>
#include "json_fwd.hpp"


#define MAX_PERS 128 // different personalities

struct PB_Personality {
	char name[32];
	char model[32];
	int aimSkill;
	int aggression;
	int sensitivity;
	int communication;
	bool inUse;
};

struct PresonJson {
	std::string name;
	std::string model;
	int aim;
	int aggression;
	int perception;
};

struct ConfigJson {
	int numBots;
	int minBots;
	int maxBots;
	int stayTime;
	int minAimSkill;
	int maxAimSkill;
	bool peaceMode;
	bool restrictedWeapons;
	bool serverMode;
	std::vector<PresonJson> characters;
};


class PB_Configuration {
public:
	PB_Configuration();
	
	bool initConfiguration(const char *configPath);
	bool createConfiguration(const char *configFile);
	
	bool setBoolVar(const char *name, const char *value);
	bool setIntVar(const char *name, int value, int min, int max);
	PB_Personality personality(int index);
	char* getColor(int persNr, int modulo);
	void personalityJoins(int index, float time) { character[index].inUse = true; }
	void personalityLeaves(int index, float time) { character[index].inUse = false; }
	int numberOfPersonalities() { return maxPers; }
	int minSkill() { return minAimSkill; }
	int maxSkill() { return maxAimSkill; }
	int numBots() { return myNumBots; }
	int minBots() { return myMinBots; }
	int maxBots() { return myMaxBots; }
	float stayTime() { return myStayTime; }
	char* menuActivation() { return menuKey; }
	bool onServerMode() { return serverMode; }
	bool onPeaceMode() { return peaceMode; }
	bool onRestrictedWeaponMode() { return restrictedWeaponMode; }
	
protected:
	int clampInt(const char *str, int min, int max);
	bool varSet(const char *srcName, const char *srcValue, const char *varName, bool &var);
	bool varSet(const char *srcName, int srcValue, const char *varName, int &var);
	bool varSet(const char *srcName, FILE *file, const char *varName, bool &var);
	
private:
	int myNumBots, myMinBots, myMaxBots;
	float myStayTime;
	int minAimSkill, maxAimSkill;
	char menuKey[16];
	bool peaceMode, restrictedWeaponMode, serverMode;
	
	int maxPers; // max. personlities
	PB_Personality character[MAX_PERS]; // stores different bot personalities
};

#endif // _PB_CONFIGURATION_HPP_
