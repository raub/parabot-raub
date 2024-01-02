#ifndef _PB_COMBAT_HPP_
#define _PB_COMBAT_HPP_

#include "pb_global.h"
#include "pb_roaming.h"
#include "pb_weaponhandling.h"
#include "pb_perception.h"


// combat reports
#define CR_NO_ENEMY 1
#define CR_ENGAGING 2
#define CR_RETREATING 3
#define CR_ENEMY_TOO_STRONG 4

#define CHECK_WEAPON_COMBAT 0.5	// intervall in which to check for best weapon
#define CHECK_WEAPON_IDLE 3	// the same for idle


class PB_Combat {
public:
	// initializes all necessary variables
	void init(int slot, edict_t *ent, PB_Action *act, PB_Roaming *pFinder);
	
	// rates importance of enemy
	float getRating(PB_Percept &perceipt);
	
	// picks best place to shoot at and fires
	bool shootAtEnemy(Vector enemyOrigin, float accuracy);
	
	// picks best place to shoot at and fires
	bool shootAtEnemy(edict_t *enemy, float accuracy);
	
	void closeCombatMovement(PB_Percept &perceipt);
	
	Vector evade(PB_Percept &perceipt);
	
	// flees from the enemy
	void retreat(edict_t *enemy);
	
	// manages weapon actions when no enemy is around
	void idleActions();
	
	bool hasWeapon(int wId) { return weapon.available(wId); }
	
	float nextWeaponCheck; // worldTime next armBestWeapon should be called
	PB_WeaponHandling weapon;
	
private:
	edict_t *botEnt;
	PB_Action *action;
	PB_Roaming *pathfinder;
	float enemyContact; // worldtime enemy has been registered
	int strafeState;
	float nextStrafe, nextJump;
	bool closeUp, gainDistance;
	
	void classifyDistance(float dist);
};

#endif // _PB_COMBAT_HPP_