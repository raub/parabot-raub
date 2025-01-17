#include <ctime>

#include "extdll.h"
#include "enginecallback.h"
#include "dllapi.h"
#include "meta_api.h"
#include "entity_state.h"
#include "bot.h"
#include "engine.h"
#include "pb_global.h"
#include "pb_configuration.h"

extern int mod_id;
extern bool g_meta_init;
PB_Configuration pbConfig;
HINSTANCE h_Library = NULL;
char mod_name[32];
enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;
GETENTITYAPI other_GetEntityAPI;
GIVEFNPTRSTODLL other_GiveFnptrsToDll;
GETNEWDLLFUNCTIONS other_GetNewDLLFunctions; 

float sineTable[256]; // sine table for e.g. look-arounds

void initSineTable() {
	for (int i = 0; i < 256; i++) {
		float f = (float) i;
		f *= 2*3.1415927 / 256;
		sineTable[i] = sin(f);
	}
}

#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

void initPath() {
	// char path[MAX_PATH];
	// char pathFile[MAX_PATH];
	// HMODULE hm = NULL;
	
	// if (GetModuleHandleEx(
	// 	GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
	// 	(LPCSTR) &DllMain,
	// 	&hm
	// ) == 0) {
	// 	int ret = GetLastError();
	// 	printf("GetModuleHandle failed, error = %d\n", ret);
	// 	// Return or however you want to handle an error.
	// }
	
	// if (GetModuleFileName(hm, path, sizeof(path)) == 0) {
	// 	int ret = GetLastError();
	// 	printf("GetModuleFileName failed, error = %d\n", ret);
	// 	// Return or however you want to handle an error.
	// }
	// // The path variable should now contain the full filepath for this DLL.
	
	// std::time_t t = std::time(0); // t is an integer type
	// sprintf(pathFile, "%s/log/%lld.txt", path, t);
	// freopen(pathFile, "w", stdout);
	// char pathFile[MAX_PATH];
	// sprintf(pathFile, "%s/log/%lld.txt", path, t);
	freopen("stdout.txt", "w", stdout);
}



// Required DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	printf("\n\nDllMain %d\n", DEBUG);
	fflush(stdout);
	
	if (fdwReason == DLL_PROCESS_ATTACH) {
		// nop
		initPath();
	} else if (fdwReason == DLL_PROCESS_DETACH) {
		if (!g_meta_init) {
			if (h_Library)
				FreeLibrary(h_Library);
		}
	}
	return TRUE;
}
#endif


extern "C" DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals) {
	char game_dir[256], filePath[100];
	int pos = 0;
	
	// get the engine functions from the engine...
	memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
	gpGlobals = pGlobals;
	
	// find the directory name of the currently running MOD...
	(*g_engfuncs.pfnGetGameDir)(game_dir);
	
	if (strstr(game_dir,"/")) {
		pos = strlen(game_dir) - 1;
		
		// scan backwards till first directory separator...
		while ((pos > 0) && (game_dir[pos] != '/')) {
			pos--;
		}
		if (pos == 0) {
			errorMsg("Error determining MOD directory name!");
		}
		
		pos++;
	}
	strcpy(mod_name, &game_dir[pos]);
	
	if (!strcmp(mod_name, "ag")) {
		mod_id = AG_DLL;
	} else if (!strcmp(mod_name, "valve") || !strcmp(mod_name, "hldm")) {
		mod_id = VALVE_DLL;
	} else if (!strcmp(mod_name, "Hunger")) {
		mod_id = HUNGER_DLL;
	} else if (!strcmp(mod_name, "holywars")) {
		mod_id = HOLYWARS_DLL;
	} else if (!strcmp(mod_name, "dmc")) {
		mod_id = DMC_DLL;
	} else if (!strcmp(mod_name, "gearbox")) {
		mod_id = GEARBOX_DLL;
	}
	
	sprintf(filePath, "%s/addons/parabot/config/", mod_name);
	
	strcat(filePath, mod_name);
	strcat(filePath, "/");
	pbConfig.initConfiguration(filePath);
	
	pos = strlen(mod_name);
	filePath[pos] = '\0';
	strcat(filePath, "/addons/parabot/log");
	CreateDirectory(filePath, NULL);
	
	initSineTable();
	
	if (!g_meta_init) {
		filePath[pos]= '\0';
		
		switch(mod_id)
		{
			case AG_DLL:
				strcat(filePath, "/dlls/ag" ARCH_SUFFIX "." OS_LIB_EXT);
				break;
			case VALVE_DLL:
				strcat(filePath, "/dlls/hl." OS_LIB_EXT);
				break;
			case DMC_DLL:
				strcat(filePath, "/dlls/dmc." OS_LIB_EXT);
				break;
			case GEARBOX_DLL:
				strcat(filePath, "/dlls/opfor." OS_LIB_EXT);
				break;
			case HOLYWARS_DLL:
				strcat(filePath, "/dlls/holywars" ARCH_SUFFIX "." OS_LIB_EXT);
				break;
			case HUNGER_DLL:
				strcat(filePath, "/dlls/einar" ARCH_SUFFIX "." OS_LIB_EXT);
				break;
			default:
				break;
		}
		
		h_Library = LoadLibrary(filePath);
		
		if (h_Library == NULL) { // Directory error or Unsupported MOD!
			errorMsg("MOD Dll not found (or unsupported MOD)!");
			debugFile("Library = 0\n");
		}
		
		other_GetEntityAPI = (GETENTITYAPI)GetProcAddress(h_Library, "GetEntityAPI");
		if (other_GetEntityAPI == NULL)	errorMsg("Can't get MOD's GetEntityAPI!\n");
		
		if (mod_id == TFC_DLL) { // NewDLLFunctions only necessary for TFC
			other_GetNewDLLFunctions = (GETNEWDLLFUNCTIONS)GetProcAddress(h_Library, "GetNewDLLFunctions"); 
			if (other_GetNewDLLFunctions == NULL) errorMsg("Can't get TFC GetNewDLLFunctions!\n");
		}
		
		other_GiveFnptrsToDll = (GIVEFNPTRSTODLL)GetProcAddress(h_Library, "GiveFnptrsToDll"); 
		if (other_GiveFnptrsToDll == NULL) {
			errorMsg("Can't get MOD's GiveFnptrsToDll!\n");
		}
		pengfuncsFromEngine->pfnCmd_Args = Cmd_Args;
		pengfuncsFromEngine->pfnCmd_Argv = Cmd_Argv;
		pengfuncsFromEngine->pfnCmd_Argc = Cmd_Argc;

		pengfuncsFromEngine->pfnPrecacheModel = pfnPrecacheModel;
		pengfuncsFromEngine->pfnPrecacheSound = pfnPrecacheSound;
		pengfuncsFromEngine->pfnSetModel = pfnSetModel;
		pengfuncsFromEngine->pfnModelIndex = pfnModelIndex;
		pengfuncsFromEngine->pfnModelFrames = pfnModelFrames;
		pengfuncsFromEngine->pfnSetSize = pfnSetSize;
		pengfuncsFromEngine->pfnChangeLevel = pfnChangeLevel;
		pengfuncsFromEngine->pfnGetSpawnParms = pfnGetSpawnParms;
		pengfuncsFromEngine->pfnSaveSpawnParms = pfnSaveSpawnParms;
		pengfuncsFromEngine->pfnVecToYaw = pfnVecToYaw;
		pengfuncsFromEngine->pfnVecToAngles = pfnVecToAngles;
		pengfuncsFromEngine->pfnMoveToOrigin = pfnMoveToOrigin;
		pengfuncsFromEngine->pfnChangeYaw = pfnChangeYaw;
		pengfuncsFromEngine->pfnChangePitch = pfnChangePitch;
		pengfuncsFromEngine->pfnFindEntityByString = pfnFindEntityByString;
		pengfuncsFromEngine->pfnGetEntityIllum = pfnGetEntityIllum;
		pengfuncsFromEngine->pfnFindEntityInSphere = pfnFindEntityInSphere;
		pengfuncsFromEngine->pfnFindClientInPVS = pfnFindClientInPVS;
		pengfuncsFromEngine->pfnEntitiesInPVS = pfnEntitiesInPVS;
		pengfuncsFromEngine->pfnMakeVectors = pfnMakeVectors;
		pengfuncsFromEngine->pfnAngleVectors = pfnAngleVectors;
		pengfuncsFromEngine->pfnCreateEntity = pfnCreateEntity;
		pengfuncsFromEngine->pfnRemoveEntity = pfnRemoveEntity;
		pengfuncsFromEngine->pfnCreateNamedEntity = pfnCreateNamedEntity;
		pengfuncsFromEngine->pfnMakeStatic = pfnMakeStatic;
		pengfuncsFromEngine->pfnEntIsOnFloor = pfnEntIsOnFloor;
		pengfuncsFromEngine->pfnDropToFloor = pfnDropToFloor;
		pengfuncsFromEngine->pfnWalkMove = pfnWalkMove;
		pengfuncsFromEngine->pfnSetOrigin = pfnSetOrigin;
		pengfuncsFromEngine->pfnEmitSound = pfnEmitSound;
		pengfuncsFromEngine->pfnEmitAmbientSound = pfnEmitAmbientSound;
		pengfuncsFromEngine->pfnTraceLine = pfnTraceLine;
		pengfuncsFromEngine->pfnTraceToss = pfnTraceToss;
		pengfuncsFromEngine->pfnTraceMonsterHull = pfnTraceMonsterHull;
		pengfuncsFromEngine->pfnTraceHull = pfnTraceHull;
		pengfuncsFromEngine->pfnTraceModel = pfnTraceModel;
		pengfuncsFromEngine->pfnTraceTexture = pfnTraceTexture;
		pengfuncsFromEngine->pfnTraceSphere = pfnTraceSphere;
		pengfuncsFromEngine->pfnGetAimVector = pfnGetAimVector;
		pengfuncsFromEngine->pfnServerCommand = pfnServerCommand;
		pengfuncsFromEngine->pfnServerExecute = pfnServerExecute;
		
		pengfuncsFromEngine->pfnClientCommand = pfnClientCommand;
		
		pengfuncsFromEngine->pfnParticleEffect = pfnParticleEffect;
		pengfuncsFromEngine->pfnLightStyle = pfnLightStyle;
		pengfuncsFromEngine->pfnDecalIndex = pfnDecalIndex;
		pengfuncsFromEngine->pfnPointContents = pfnPointContents;
		pengfuncsFromEngine->pfnMessageBegin = pfnMessageBegin;
		pengfuncsFromEngine->pfnMessageEnd = pfnMessageEnd;
		pengfuncsFromEngine->pfnWriteByte = pfnWriteByte;
		pengfuncsFromEngine->pfnWriteChar = pfnWriteChar;
		pengfuncsFromEngine->pfnWriteShort = pfnWriteShort;
		pengfuncsFromEngine->pfnWriteLong = pfnWriteLong;
		pengfuncsFromEngine->pfnWriteAngle = pfnWriteAngle;
		pengfuncsFromEngine->pfnWriteCoord = pfnWriteCoord;
		pengfuncsFromEngine->pfnWriteString = pfnWriteString;
		pengfuncsFromEngine->pfnWriteEntity = pfnWriteEntity;
		pengfuncsFromEngine->pfnCVarRegister = pfnCVarRegister;
		pengfuncsFromEngine->pfnCVarGetFloat = pfnCVarGetFloat;
		pengfuncsFromEngine->pfnCVarGetString = pfnCVarGetString;
		pengfuncsFromEngine->pfnCVarSetFloat = pfnCVarSetFloat;
		pengfuncsFromEngine->pfnCVarSetString = pfnCVarSetString;
		pengfuncsFromEngine->pfnPvAllocEntPrivateData = pfnPvAllocEntPrivateData;
		pengfuncsFromEngine->pfnPvEntPrivateData = pfnPvEntPrivateData;
		pengfuncsFromEngine->pfnFreeEntPrivateData = pfnFreeEntPrivateData;
		pengfuncsFromEngine->pfnSzFromIndex = pfnSzFromIndex;
		pengfuncsFromEngine->pfnAllocString = pfnAllocString;
		pengfuncsFromEngine->pfnGetVarsOfEnt = pfnGetVarsOfEnt;
		pengfuncsFromEngine->pfnPEntityOfEntOffset = pfnPEntityOfEntOffset;
		pengfuncsFromEngine->pfnEntOffsetOfPEntity = pfnEntOffsetOfPEntity;
		pengfuncsFromEngine->pfnIndexOfEdict = pfnIndexOfEdict;
		pengfuncsFromEngine->pfnPEntityOfEntIndex = pfnPEntityOfEntIndex;
		pengfuncsFromEngine->pfnFindEntityByVars = pfnFindEntityByVars;
		pengfuncsFromEngine->pfnGetModelPtr = pfnGetModelPtr;
		pengfuncsFromEngine->pfnRegUserMsg = pfnRegUserMsg;
		pengfuncsFromEngine->pfnAnimationAutomove = pfnAnimationAutomove;
		pengfuncsFromEngine->pfnGetBonePosition = pfnGetBonePosition;
		pengfuncsFromEngine->pfnFunctionFromName = pfnFunctionFromName;
		pengfuncsFromEngine->pfnNameForFunction = pfnNameForFunction;
		pengfuncsFromEngine->pfnClientPrintf = pfnClientPrintf;
		pengfuncsFromEngine->pfnServerPrint = pfnServerPrint;
		pengfuncsFromEngine->pfnGetAttachment = pfnGetAttachment;
		pengfuncsFromEngine->pfnCRC32_Init = pfnCRC32_Init;
		pengfuncsFromEngine->pfnCRC32_ProcessBuffer = pfnCRC32_ProcessBuffer;
		pengfuncsFromEngine->pfnCRC32_ProcessByte = pfnCRC32_ProcessByte;
		pengfuncsFromEngine->pfnCRC32_Final = pfnCRC32_Final;
		pengfuncsFromEngine->pfnRandomLong = pfnRandomLong;
		pengfuncsFromEngine->pfnRandomFloat = pfnRandomFloat;
		pengfuncsFromEngine->pfnSetView = pfnSetView;
		pengfuncsFromEngine->pfnTime = pfnTime;
		pengfuncsFromEngine->pfnCrosshairAngle = pfnCrosshairAngle;
		pengfuncsFromEngine->pfnLoadFileForMe = pfnLoadFileForMe;
		pengfuncsFromEngine->pfnFreeFile = pfnFreeFile;
		pengfuncsFromEngine->pfnEndSection = pfnEndSection;
		pengfuncsFromEngine->pfnCompareFileTime = pfnCompareFileTime;
		pengfuncsFromEngine->pfnGetGameDir = pfnGetGameDir;
		pengfuncsFromEngine->pfnCvar_RegisterVariable = pfnCvar_RegisterVariable;
		pengfuncsFromEngine->pfnFadeClientVolume = pfnFadeClientVolume;
		pengfuncsFromEngine->pfnSetClientMaxspeed = pfnSetClientMaxspeed;
		pengfuncsFromEngine->pfnCreateFakeClient = pfnCreateFakeClient;
		pengfuncsFromEngine->pfnRunPlayerMove = pfnRunPlayerMove;
		pengfuncsFromEngine->pfnNumberOfEntities = pfnNumberOfEntities;
		pengfuncsFromEngine->pfnGetInfoKeyBuffer = pfnGetInfoKeyBuffer;
		pengfuncsFromEngine->pfnInfoKeyValue = pfnInfoKeyValue;
		pengfuncsFromEngine->pfnSetKeyValue = pfnSetKeyValue;
		pengfuncsFromEngine->pfnSetClientKeyValue = pfnSetClientKeyValue;
		pengfuncsFromEngine->pfnIsMapValid = pfnIsMapValid;
		pengfuncsFromEngine->pfnStaticDecal = pfnStaticDecal;
		pengfuncsFromEngine->pfnPrecacheGeneric = pfnPrecacheGeneric;
		pengfuncsFromEngine->pfnGetPlayerUserId = pfnGetPlayerUserId;
		pengfuncsFromEngine->pfnBuildSoundMsg = pfnBuildSoundMsg;
		pengfuncsFromEngine->pfnIsDedicatedServer = pfnIsDedicatedServer;
		pengfuncsFromEngine->pfnCVarGetPointer = pfnCVarGetPointer;
		pengfuncsFromEngine->pfnGetPlayerWONId = pfnGetPlayerWONId;
		
		// give the engine functions to the other DLL...
		(*other_GiveFnptrsToDll)(pengfuncsFromEngine, pGlobals);
	}	
}
