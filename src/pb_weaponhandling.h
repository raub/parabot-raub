#ifndef _PB_WEAPON_HANDLING_HPP_
#define _PB_WEAPON_HANDLING_HPP_

#include "pb_weapon.h"

#define CHANGE_WEAPON_DELAY 1.0	// time needed to switch between weapons


class PB_WeaponHandling {
public:
	PB_WeaponHandling();
	
	// has to be called with the botSlot before all other methods
	void init(int slot, edict_t *ent, PB_Action *action);

	// call every frame
	void initCurrentWeapon();

	// returns the WeaponID of the currently handled weapon
	int currentWeapon();

	// returns true if bot can use weapon wId
	bool available(int wId);

	// attacks in best mode at best time the given position when accuracy is reached
	bool attack(Vector target, float accuracy, Vector relVel = Vector(0, 0, 0));

	void checkForForcedAttack();

	// returns the weapon-ID of best weapon for given situation
	int getBestWeapon(float distance, float hitProb, int flags);

	// returns true if getBestWeapon could deliver a score > 0
	bool bestWeaponUsable() { return weaponUsable; }

	// arms the best weapon for given situation, returns true if already armed before
	bool armBestWeapon(float distance, float hitProb, int flags);
	
	// sets the weapon that will be armed by armBestWeapon during the next 0.5 seconds
	void setPreferredWeapon(int wId, int mode = 1);

	float getWeaponScore(int wId, float distance, float hitProb, int flags, bool checkAmmo);

	float bestDistance(int wId);

	float currentHighAimProb();

private:
	int botSlot; // slot the bot is using
	edict_t *botEnt;
	PB_Action *botAction; // pointer to action-instace the bot is using
	PB_Weapon weapon;
	int defaultWeapon;
	int armedWeapon;
	int preferredWeapon;
	int preferredMode;
	float preferredWeaponTimeOut;
	float lastModeSwitch; // worldTime last change (zoom in/out) had ocurred
	bool weaponUsable;
	int minModWeapon, maxModWeapon;
	
	void switchToWeapon(int wId);
};

#endif /* _PB_WEAPON_HANDLING_HPP_ */