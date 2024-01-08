#define NOMINMAX

#include <fstream>
#include <sstream>

#include "json.hpp"
#include "pb_configuration.h"
#include "pb_global.h"
#include "bot.h"

using nlohmann::json;

extern int mod_id;
extern char mod_name[32];


PB_Configuration::PB_Configuration() {
	// number of bots
	myNumBots = 5;
	myMinBots = 4;
	myMaxBots = 6;
	
	// skills
	minAimSkill = 1;
	maxAimSkill = 10;
	
	// game modes:
	peaceMode = false;
	restrictedWeaponMode = false;
	serverMode = false;
	myStayTime = 20.0 * 60;
	
	// personalities:
	maxPers = 0;
}


PB_Personality PB_Configuration::personality(int index) {
	/* char dbgBuffer[256];
	sprintf(dbgBuffer, "  %i: %s  ", index, character[index].name);
	debugFile(dbgBuffer); */
	return character[index];
}


int PB_Configuration::clampInt(const char *str, int min, int max) {
	int val = atoi(str);
	
	if (val < min) {
		val = min;
	} else if (val > max) {
		val = max;
	}
	
	return val;
}


bool PB_Configuration::varSet(const char *srcName, FILE *file, const char *varName, bool &var) {
	char buffer[1024];
	
	if (_stricmp(srcName, varName) == 0) {
		fscanf(file, " = %s ", buffer);
		
		if (_stricmp(buffer, "ON") == 0) {
			var = true;
		} else if (_stricmp(buffer, "OFF") == 0) {
			var = false;
		}
		
		return true;
	} else {
		return false;
	}
}


bool PB_Configuration::varSet(const char *srcName, const char *srcValue, const char *varName, bool &var) {
	if (_stricmp(srcName, varName)) {
		return false;
	}
	
	if (srcValue == 0) {
		if (var) infoMsg(varName, " is on.\n");
		else infoMsg(varName, " is off.\n");
	} else {
		if (_stricmp(srcValue, "ON") == 0) {
			var = true;
			infoMsg(varName, " activated.\n");
		} else if (_stricmp(srcValue, "OFF") == 0) {
			var = false;
			infoMsg(varName, " deactivated.\n");
		} else {
			infoMsg("Usage: ", varName, " on/off\n"	);
		}
	}
	return true;
}


bool PB_Configuration::varSet(const char *srcName, int srcValue, const char *varName, int &var) {
	char buffer[64];
	if (_stricmp(srcName, varName)) {
		return false;
	}
	
	var = srcValue;
	sprintf(buffer, "%s set to %i\n", varName, srcValue);
	infoMsg(buffer);
	return true;
}


bool PB_Configuration::setBoolVar(const char *name, const char *value) {
	return (
		varSet(name, value, "PeaceMode", peaceMode) ||
		varSet(name, value, "RestrictedWeapons", restrictedWeaponMode) ||
		varSet(name, value, "ServerMode", serverMode)
	);
}


bool PB_Configuration::setIntVar(const char *name, int value, int min, int max) {
	if (value < min) {
		value = min;
	}
	if (value > max) {
		value = max;
	}
	
	return (
		varSet(name, value, "MinBots", myMinBots) ||
		varSet(name, value, "MaxBots", myMaxBots) ||
		varSet(name, value, "NumBots", myNumBots) ||
//		varSet(name, value, "StayTime", myStayTime) ||
		varSet(name, value, "MinAimSkill", minAimSkill) ||
		varSet(name, value, "MaxAimSkill", maxAimSkill)
	);
}

void to_json(json &j, const PresonJson &p) {
	j = json {
		{ "name", p.name },
		{ "model", p.model },
		{ "aim", p.aim },
		{ "aggression", p.aggression },
		{ "perception", p.perception },
	};
}

void from_json(const json &j, PresonJson &p) {
	printf("\n\nfrom_json PresonJson 1\n");
	fflush(stdout);
	
	if (j.contains("name")) {
		j.at("name").get_to(p.name);
	}
	
	if (j.contains("model")) {
		j.at("model").get_to(p.model);
	}
	
	if (j.contains("aim")) {
		j.at("aim").get_to(p.aim);
	}
	
	if (j.contains("aggression")) {
		j.at("aggression").get_to(p.aggression);
	}
	
	if (j.contains("perception")) {
		j.at("perception").get_to(p.perception);
	}
	
	printf("\n\nfrom_json PresonJson 2\n");
	fflush(stdout);
}

void to_json(json &j, const ConfigJson &p) {
	j = json {
		{ "numBots", p.numBots },
		{ "minBots", p.minBots },
		{ "maxBots", p.maxBots },
		{ "stayTime", p.stayTime },
		{ "minAimSkill", p.minAimSkill },
		{ "maxAimSkill", p.maxAimSkill },
		{ "peaceMode", p.peaceMode },
		{ "restrictedWeapons", p.restrictedWeapons },
		{ "serverMode", p.serverMode },
		{ "characters", p.characters },
	};
}

void from_json(const json &j, ConfigJson &p) {
	printf("\n\nfrom_json ConfigJson 1\n");
	fflush(stdout);
	
	if (j.contains("numBots")) {
		j.at("numBots").get_to(p.numBots);
	}
	
	if (j.contains("minBots")) {
		j.at("minBots").get_to(p.minBots);
	}
	
	if (j.contains("maxBots")) {
		j.at("maxBots").get_to(p.maxBots);
	}
	
	if (j.contains("stayTime")) {
		j.at("stayTime").get_to(p.stayTime);
	}
	
	if (j.contains("minAimSkill")) {
		j.at("minAimSkill").get_to(p.minAimSkill);
	}
	
	if (j.contains("maxAimSkill")) {
		j.at("maxAimSkill").get_to(p.maxAimSkill);
	}
	
	if (j.contains("peaceMode")) {
		j.at("peaceMode").get_to(p.peaceMode);
	}
	
	if (j.contains("restrictedWeapons")) {
		j.at("restrictedWeapons").get_to(p.restrictedWeapons);
	}
	
	if (j.contains("serverMode")) {
		j.at("serverMode").get_to(p.serverMode);
	}
	
	if (j.contains("characters")) {
		j.at("characters").get_to(p.characters);
	}
	
	printf("\n\nfrom_json ConfigJson 2\n");
	fflush(stdout);
}

static inline std::string _readFileContent(const char *pathToFile) {
	printf("\n\n_readFileContent %s\n", pathToFile);
	fflush(stdout);
	
	std::ifstream inFile;
	inFile.open(pathToFile);
	if (!inFile.is_open()) {
		printf("\n_readFileContent empty\n");
		fflush(stdout);
		
		return std::string();
	}
	
	std::stringstream strStream;
	strStream << inFile.rdbuf();
	
	printf("\n_readFileContent completed\n");
	fflush(stdout);
	
	return strStream.str();
}

bool PB_Configuration::initConfiguration(const char *configPath) {
	printf("\ninitConfiguration %s\n", configPath);
	fflush(stdout);
	
	char str[256];
	strcpy(str, configPath);
	strcat(str, "parabot.json");
	
	std::string configText = _readFileContent(str);
	
	printf("\n\ninitConfiguration len %d\n", configText.length());
	fflush(stdout);
	
	if (configText.length() < 3) {
		infoMsg("Missing ", str, "\n");
		
		CreateDirectory(configPath, NULL);
		if (!createConfiguration(str))
			return false;
		configText = _readFileContent(str);
	}
	
	printf("\n\ninitConfiguration parse pre\n", configText.length());
	fflush(stdout);
	
	json parsed = json::parse(configText, nullptr, false);
	if (parsed.is_discarded()) {
		printf("\n\ninitConfiguration parse fail\n");
		fflush(stdout);
		return false;
	}
	
	printf("\n\ninitConfiguration parse post %s\n", parsed.dump().c_str());
	fflush(stdout);
	
	ConfigJson config = parsed.template get<ConfigJson>();
	
	printf("\n\ninitConfiguration template\n", configText.length());
	fflush(stdout);
	
	myNumBots = std::clamp(config.numBots, 0, 32);
	myMinBots = std::clamp(config.minBots, 0, 32);
	myMaxBots = std::clamp(config.maxBots, myMinBots, 32);
	myStayTime = 60.0 * static_cast<float>(std::clamp(config.stayTime, 2, 180));
	minAimSkill = std::clamp(config.minAimSkill, 1, 10);
	maxAimSkill = std::clamp(config.maxAimSkill, minAimSkill, 10);
	peaceMode = config.peaceMode;
	restrictedWeaponMode = config.restrictedWeapons;
	serverMode = config.serverMode;
	
	printf("\n\ninitConfiguration assign\n", configText.length());
	fflush(stdout);
	
	for (int i = 0; i < MAX_PERS; i++) {
		character[i].inUse = false;
	}
	
	printf("\n\ninitConfiguration char pre\n", configText.length());
	fflush(stdout);
	
	int nCharacters = std::min(static_cast<size_t>(MAX_PERS), config.characters.size());
	for (int i = 0; i < nCharacters; i++) {
		character[i].inUse = true;
		
		strcpy_s(character[i].name, 31, config.characters[i].name.c_str());
		strcpy_s(character[i].model, 31, config.characters[i].model.c_str());
		
		character[i].aimSkill = config.characters[i].aim;
		character[i].aggression = config.characters[i].aggression;
		character[i].sensitivity = config.characters[i].perception;
	}
	
	printf("\n\ninitConfiguration char post\n", configText.length());
	fflush(stdout);
	
	return true;
}

bool PB_Configuration::createConfiguration(const char *configFile) {
	FILE *file;
	
	infoMsg("Creating ", configFile, "... ");
	file = fopen(configFile, "wt");
	
	if (!file) {
		infoMsg("failed!\n");
		return false;
	}
	
	fprintf(file, R"(
{
    "serverMode": true,
    "minBots": 2,
    "maxBots": 3,
    "averageStay": 8,
    "numBots": 6,
    "peaceMode": false,
    "restrictedWeapons": false,
    "minAimSkill": 2,
    "maxAimSkill": 5,
    "characters": [
        { "name": "pwner", "model": "gman", "aim": 2, "aggression": 9, "perception": 7 }
    ]
}
)");
	
	fclose(file);
	
	infoMsg("OK!\n");
	return true;
}

char* PB_Configuration::getColor(int persNr, int modulo) {
	static char color[4];
	int code = 0;
	int nameLen = strlen(character[persNr].name);
	
	for (int i = 0; i < nameLen; i++) {
		code += ((int)character[persNr].name[i] * (729 + i)) % modulo;
	}
	
	code = (code % 255) + 1;
	sprintf(color, "%i", code);
	
	return color;
}
