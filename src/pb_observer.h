#ifndef _PB_OBSERVER_HPP_
#define _PB_OBSERVER_HPP_

#include <queue>

#include "pb_global.h"
#include "marker.h"


// max. observed players
#define MAX_OBS 32
// max. bots
#define MAX_BOTS 32
// max. stored waypoints for following
#define MAX_WPTS 128


class PB_Observer {
public:
	// calls init()
	PB_Observer();
	~PB_Observer();
	
	// initializes the observer before mapstart
	void init();
	
	// registers all human clients currently connected and valid
	void registerClients();
	
	// observes all human clients currently registered
	void observeAll();
	
	// returns the observer id of player
	int playerId(edict_t *player);
	
	// links botId with observerId
	void reportPartner(int botId, int observerId);
	
	// adds a waypoint for followers
	void addWaypoint(int observerId, Vector pos, int action = 0, int col = 1);
	
	// returns the next waypoint to follow player for bot with id
	PB_Path_Waypoint getNextWaypoint(int botId);
	
	void reportWaypointReached(int botId);
	
	// returns true if bot should move towards partner
	bool shouldFollow(int botId, edict_t *bot);
	
	// if bot isn't succesful in following frees waypoints and returns true
	bool canNotFollow(int botId);
	
	// returns true if bots registered partner is involved in a combat
	bool partnerInCombat(int botId);
	
	bool partnerValid(int botId);
	
private:
	typedef struct {
		EHANDLE player; // pointer to observed players
		bool active; // true if observation runs
		int leadWaypoint; // latest waypoint and plat of observed
		int lastPlatId; // player, indices in waypoint-table
		edict_t *platform;
		PB_Navpoint *lastReachedNav;
		Vector lastWpPos; // position where last waypoint was stored
		float lastWpTime;
		float lastWpYaw;
		Vector lastFramePos; // position (for teleporters)
		Vector lastFrameVel; // velocity (for teleporters)
		float health;
		int frags;
		bool jumpPressed;
		bool usePressed;
		bool inCombat;
		float fallTime;
		short currentCell;
		short lastCell;
		float lastCellTime;
	} tObserverData;

	tObserverData obs[MAX_OBS];
	
	PB_Path_Waypoint waypoint[MAX_OBS][MAX_WPTS]; // waypoint lists to follow players
	PB_Path_Platform platInfo[MAX_OBS][MAX_WPTS];
	
	int partner[MAX_BOTS]; // stores botpartner
	int currentWaypoint[MAX_BOTS]; // index in waypoint-table
	
	CMarker trail[MAX_OBS];
	std::queue<int>	markerId[MAX_OBS];
	
	// registers the player for observation, observerId is returned
	int registerPlayer(edict_t *player);
	
	// clears all lists for observer oId
	void clear(int oId);
	
	void startObservation(int oId);
	
	// returns WP_ON_LADDER and WP_ON_PLATFORM if necessary
	int checkGround(int oId, edict_t **plat);
	
	bool shouldObservePlayer(int oId);
	
	void checkForJump(int oId, Vector &pos);
	
	void checkForUse(int oId, Vector &pos);
	
	void checkForMove(int oId, Vector &pos);
	
	void checkForCamping(int oId, Vector &pos);
	
	void checkForTripmines(int oId, Vector &pos);
	
	void checkForButtonShot(int oId, Vector &pos);
	
	void checkPlayerHealth(int oId);
	
	int getStartIndex(int oId, PB_Navpoint *endNav);
	
	void newNavpointReached(int oId, Vector &pos, PB_Navpoint *endNav);
	
	void updateCellInfo(int i);
};

#endif // _PB_OBSERVER_HPP_
