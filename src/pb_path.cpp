#pragma warning(disable: 4530) // warning for disabled exceptions

#include <queue>

#include "pb_path.h"
#include "pb_global.h"
#include "pb_action.h"
#include "marker.h"

#define MAX_PLAT_WAIT 2.0 // maximal time to wait if platform is not moving

extern CMarker glMarker;
extern bool pb_pause;

int journeyMode = JOURNEY_FAST;

// sets global journey mode
void setJourneyMode(int mode) {
	journeyMode = mode;
}


PB_Path_Waypoint::PB_Path_Waypoint() {
	data.act = WP_NOT_INITIALIZED;
}


PB_Path_Waypoint::PB_Path_Waypoint(Vector pos, int action, float arrival) {
	data.pos = pos;
	data.act = action;
	data.arrival = arrival;
}


// returns true if waypoint is reached from pos
bool PB_Path_Waypoint::reached(edict_t *ent) {
	if (data.act & WP_NOT_REACHABLE) {
		return false;
	}
	
	Vector wpDir = pos(ent) - ent->v.origin;
	float dist = (wpDir).Length();
	
	if (dist < 16.0) {
		return true;
	}
	if (dist > 40.0) {
		return false;
	}
	
	Vector vDir = ent->v.velocity;
	wpDir = wpDir / dist;
	float dres = DotProduct(vDir.Normalize(), wpDir);
	
	return dres < 0;
	/*
	float requiredDist = 40;
	// approach jumps and platforms up to 30 units
	if (action() == BOT_JUMP || isOnPlatform()) requiredDist = 30;
	// approach ladder waypoints up to 20 units
	if (data.act & WP_ON_LADDER) requiredDist = 20;
	if (dist < requiredDist) return true;
	else return false;
	*/
}


// adjusts positions if ent is on ladder
Vector PB_Path_Waypoint::pos(edict_t *ent) {
	assert(ent!=0);
	if (ent->v.movetype != MOVETYPE_FLY) {
		return data.pos;
	} else {
		return (data.pos + Vector(0,0,20));
	}
}


PB_Path::PB_Path() {
	data.startId = 0;
	data.endId = 0;
	waypoint = 0;
	hiddenAttack = 0;
	platformPos = 0;
	readyToDelete = false;
}


PB_Path::~PB_Path() {
/*	don't delete anything because:
 *  1) if path is member of mapgraph the clear function is called there
 *  2) if path is temporal it might contain waypoint/hiddenAttack-pointers to
 *	  paths in mapgraph which must not be deleted
 */
}


// returns the start navpoint
PB_Navpoint& PB_Path::startNav() {
	return getNavpoint(data.startId);
}


// returns the end navpoint
PB_Navpoint& PB_Path::endNav() {
	return getNavpoint(data.endId);
}


// start recording a path from given point at given time
void PB_Path::startRecord(int startPnt, float worldTime) {
	data.privateId = -1;
	data.dataId = -1; // needs to be initialized in PB_MapGraph::addPath
	data.startId = startPnt;
	data.endId = -1; // = recording
	startTime = worldTime;
	data.attempts = 0;
	data.lastAttempt = getTotalAttempts(); // init as new
	data.successful = 0;
	data.lostHealth = 0;
	data.enemyEncounters = 0;
	readyToDelete = false;
	specialId = -1;
	waypoint = new WaypointList;
	hiddenAttack = new AttackList;
	platformPos = new PlatformList;
}


// adds the waypoint to the path
void PB_Path::addWaypoint(PB_Path_Waypoint &wp) {
	assert(waypoint != 0);
	waypoint->push_back(wp);
}


// adds the waypoint to the path
void PB_Path::addWaypoint(Vector &pos, int action, float arrival) {
	PB_Path_Waypoint wp(pos, action, arrival - startTime);
	//wp.data.pos = pos;
	//wp.data.act = action;
	//wp.data.arrival = arrival - startTime;
	assert(waypoint != 0);
	waypoint->push_back(wp);
}


// adds platform info
void PB_Path::addPlatformInfo(int navId, Vector &pos) {
	PB_Path_Platform pf(navId, pos);
	assert(platformPos != 0);
	platformPos->push_back(pf);
}


// stops recording
void PB_Path::stopRecord(int endPnt, float worldTime, int mode) {
	data.endId = endPnt;
	data.scheduledTime = worldTime - startTime;
	data.passTime = data.scheduledTime;
	data.specials = mode;
}


// deletes all path-data, cancels recording
void PB_Path::clear() {
	if (waypoint) {
		waypoint->clear();
		delete waypoint;
		waypoint = 0;
	}
	
	if (hiddenAttack) {
		hiddenAttack->clear();
		delete hiddenAttack;
		hiddenAttack = 0;
	}
	
	if (platformPos) {
		platformPos->clear();
		delete platformPos;
		platformPos = 0;
	}
	
	data.endId = 0; // not recording anything
	readyToDelete = true; // just in case...
}


void PB_Path::print() {
	debugMsg("path from ");
	startNav().print();
	debugMsg(" to ");
	endNav().print();
}

// mark all waypoints
void PB_Path::mark() {
	WaypointList::iterator wpi = waypoint->begin();
	PB_Path_Waypoint wp;
	
	while (wpi != waypoint->end()) {
		glMarker.newMarker(wpi->pos(), 1);
		wpi++;
	}
	
	if (currentWaypoint != waypoint->end()) {
		glMarker.newMarker(Vector(0,0,10)+currentWaypoint->pos(), 1);
	} else {
		debugMsg("Current wp = end\n");
	}
	
	if (lastReachedWaypoint != waypoint->end()) {
		glMarker.newMarker(Vector(0,0,10)+lastReachedWaypoint->pos(), 2);
	} else {
		debugMsg("Last reached wp = end\n");
	}
}


// init path from file
void PB_Path::load(FILE *fp) {
	readyToDelete = false;
	
	// read data
	int i;
	fread(&data, sizeof(TSaveData), 1, fp);
	
	if (data.dataId == data.privateId) { // read lists if stored here
		// read waypoints
		waypoint = new WaypointList;
		int numWpts;
		fread(&numWpts, sizeof(int), 1, fp);
		PB_Path_Waypoint wp;
		assert(waypoint != 0);
		for (i = 0; i < numWpts; i++) {
			//printf("w");
			fread(&wp.data, sizeof(PB_Path_Waypoint::TSaveData), 1, fp);
			waypoint->push_back(wp);
		}
		
		// read attacks
		hiddenAttack = new AttackList;
		int numAtts;
		fread(&numAtts, sizeof(int), 1, fp);
		PB_Path_Attack att;
		assert(hiddenAttack != 0);
		for (i = 0; i < numAtts; i++) {
			//printf("a");
			fread(&att.data, sizeof(PB_Path_Attack::TSaveData), 1, fp);
			hiddenAttack->push_back(att);
		}
		
		// read platform info
		platformPos = new PlatformList;
		int numPlats;
		fread(&numPlats, sizeof(int), 1, fp);
		PB_Path_Platform pf;
		assert(platformPos != 0);
		float pos[3];
		for (i = 0; i < numPlats; i++) {
			fread(&(pf.data.navId), sizeof(int), 1, fp);
			fread(&pos, 3*sizeof(float), 1, fp);
			pf.data.pos.x = pos[0];pf.data.pos.y = pos[1];pf.data.pos.z = pos[2];
			if (pf.data.navId < 0) {
				readyToDelete = true;
				debugMsg("Deleted 1 path because of incorrect platform info\n");
			}
			platformPos->push_back(pf);
		}
	} else { // must be initialized later on when all paths are loaded...
		waypoint = 0;
		hiddenAttack = 0;
		platformPos = 0;
	}
}


// save path to file if not readyToDelete
void PB_Path::save(FILE *fp) {
	if (readyToDelete) {
		return;
	}
	
	// save data
	//printf("P");
	int i;
	fwrite(&data, sizeof(TSaveData), 1, fp);
	
	if (data.dataId == data.privateId) { // save lists if stored here
		// save waypoints
		assert(waypoint != 0);
		int numWpts = waypoint->size();
		fwrite(&numWpts, sizeof(int), 1, fp);
		WaypointList::iterator wpi = waypoint->begin();
		PB_Path_Waypoint wp;
		for (i = 0; i < numWpts; i++) {
			//printf("W");
			fwrite(&(wpi->data), sizeof(PB_Path_Waypoint::TSaveData), 1, fp);
			wpi++;
		}
		
		// save attacks
		assert(hiddenAttack != 0);
		int numAtts = hiddenAttack->size();
		fwrite(&numAtts, sizeof(int), 1, fp);
		AttackList::iterator atti = hiddenAttack->begin();
		//PB_Path_Attack att;
		for (i = 0; i < numAtts; i++) {
			//printf("A");
			fwrite(&(atti->data), sizeof(PB_Path_Attack::TSaveData), 1, fp);
			atti++;
		}
		
		// save platform pos
		assert(platformPos != 0);
		int numPlats = platformPos->size();
		fwrite(&numPlats, sizeof(int), 1, fp);
		PlatformList::iterator pfi = platformPos->begin();
		float pos[3];
		for (i = 0; i < numPlats; i++) {
			pfi->data.pos.CopyToArray(pos);
			assert(pfi->data.navId < 1000);
			fwrite(&(pfi->data.navId), sizeof(int), 1, fp);
			fwrite(pos, 3*sizeof(float), 1, fp);
			pfi++;
		}
	}
}


void PB_Path::initReturnOf(PB_Path &path) {
	data.privateId = -1;
	data.dataId = path.data.privateId;
	data.startId = path.endId();
	data.endId = path.startId();
	data.scheduledTime = path.data.scheduledTime;
	data.passTime = data.scheduledTime;
	data.specials = 0;
	data.attempts = 0;
	data.lastAttempt = 0;
	data.successful = 0;
	data.lostHealth = 0;
	data.enemyEncounters = 0;
	readyToDelete = false;
	specialId = path.specialId;
	waypoint = path.waypoint; // use same data 
	hiddenAttack = path.hiddenAttack;
	platformPos = path.platformPos;
}


// used by graph algo, returns weight of path depending of journeyMode
float PB_Path::weight() {
	float certainty = 0.5; // base certainty for new paths
	float encounterProb = 0.1;
	
	switch(journeyMode) {
	// return fastest journey
	case JOURNEY_FAST:
		return data.passTime;
	// return most reliable journey
	case JOURNEY_RELIABLE:
		if (data.attempts > 0) {
			certainty = ((float)data.successful) / ((float)data.attempts);
		}
		return (1.0001 - certainty);
	// return journey with most enemy encounters
	case JOURNEY_CROWDED:
		if (data.attempts > 0) {
			encounterProb = ((float)data.enemyEncounters) / ((float)data.attempts);
			if (encounterProb > 9.9) {
				encounterProb = 9.9;
			}
		}
		return (10 - encounterProb); // encounterProb may be >1
	case JOURNEY_LONELY:
		if (data.attempts > 0) 
			encounterProb = ((float)data.enemyEncounters) / ((float)data.attempts);
		return encounterProb;
	}
	
	debugMsg("Unknown journeymode!\n");
	return 0;
}


bool PB_Path::valid(int mode) {
	if (readyToDelete) {
		return false; // don't accept deleted paths
	}
	
	int dif = mode ^ data.specials; // differences between searchmode and path-specials
	int neg = mode ^ (-1); // specials not allowed in searchmode
	return !(dif & neg); // only valid if AND = 0
}


// start the path at given time
void PB_Path::startAttempt(float worldTime) {
	assert(waypoint != 0);
	assert(platformPos != 0);
	
	if (waypoint->size() > 1) {
		WaypointList::iterator wpForw = waypoint->begin();
		float distForw = (wpForw->pos() - startNav().pos()).Length();
		WaypointList::iterator wpBackw = waypoint->end();wpBackw--;
		float distBackw = (wpBackw->pos() - startNav().pos()).Length();
		// path in forward direction ?
		if (distForw <= distBackw) {
			forwardPass = true;
			currentWaypoint = wpForw;
			currentPlat = platformPos->begin();
		} else {
			forwardPass = false;
			currentWaypoint = wpBackw;
			currentPlat = platformPos->end();
			if (platformPos->size() > 1) currentPlat--;
		}
	} else { // no waypoints stored: simulate forward
		forwardPass = true;
		currentWaypoint = waypoint->begin();
		currentPlat = platformPos->begin();
	}
	
	lastReachedWaypoint = waypoint->end(); // nothing reached yet
	lastReachedPlat = platformPos->end();
	startTime = worldTime;
	data.attempts++;
	incTotalAttempts();
	data.lastAttempt = getTotalAttempts();
	lastWaitingPlat = 0;
	ignoredPlat = 0;
}


// cancels the pass, no reportTarget needs to be called
void PB_Path::cancelAttempt() {
	if (data.attempts > 0) {
		data.attempts--;
	}
}


// confirms path as finished, internally set new success-variables
void PB_Path::reportTargetReached(edict_t *traveller, float worldTime) {
	float allTime = (data.passTime  * ((float)data.successful)) + (worldTime - startTime);
	
	data.successful++;
	if (data.successful == 0) {
		errorMsg("reportTargetReached!");
		return;
	}
	
	data.passTime = allTime / ((float)data.successful); // set new passTime
	endNav().reportVisit(traveller, worldTime); //now in observer
	
	if (ignoredPlat) { // we ignored a plat but arrived nevertheless?
		WaypointList::iterator wpi = waypoint->begin();
		PlatformList::iterator pfi = platformPos->begin();
		while (wpi!=waypoint->end()) {
			if (wpi->isOnPlatform()) {
				edict_t *plat = getNavpoint(pfi->data.navId).entity();
				// delete the platform flag for these waypoints
				if (plat == ignoredPlat) wpi->data.act &= (~WP_ON_PLATFORM);
				pfi++;
			}
			wpi++;
		}
	}
}


// confirms path as failed, internally set new success-variables
void PB_Path::reportTargetFailed() {
	if (data.attempts == 0) {
		return; // must be...
	}
	
	float certainty = ((float)data.successful) / ((float)data.attempts);
	// check new certainty
	if (certainty < 0.5) {
		markForDelete();
		debugMsg("Auto-deleted ");  print();  debugMsg("\n");
		//debugMsg("%i attempts, ", data.attempts);  
		//debugMsg("%i succesful\n", data.successful);
		//mark();
		//pb_pause = true;
	}
	
	debugMsg("%i attempts, ", data.attempts);  
	debugMsg("%i succesful\n", data.successful);
	//mark();
	//pb_pause = true;
}


// stores the position of attack (inclusive actual position in path) {
void PB_Path::addAttack(Vector &origin) {
	assert(hiddenAttack != 0);
	
	PB_Path_Attack att;
	att.data.pos = origin;
	att.data.time = currentWaypoint->data.arrival;
	
	hiddenAttack->push_back(att);
}


// Priority values return 0 (nextWaypoint) or 2 (button) { 
Vector PB_Path::getViewPos(edict_t *traveller, int &prior) {
	assert(waypoint != 0);
	if (lastReachedWaypoint != waypoint->end()) { // watch at buttons for pressing them
		if (lastReachedWaypoint->action()==BOT_USE) {
			assert(traveller != 0);
			Vector t = traveller->v.origin;
			Vector p = startNav().pos(); //lastReachedWaypoint->pos();
			prior = 2; // UNDONE: ButtonUse uses AimDir, not ViewDir!!!
			return p;
		}
	}
	
	prior = 0;
	return (getNextWaypoint().pos(traveller) + traveller->v.view_ofs);
}


// returns true if the next waypoint could not be reached in time
bool PB_Path::timeOut(float worldTime) {
	float plan = data.scheduledTime; // default = approaching path end
	
	assert(waypoint != 0);
	if (currentWaypoint != waypoint->end()) { // approaching another waypoint
		if (forwardPass) { // path in forward direction ?
			plan = currentWaypoint->data.arrival;
		} else {
			// backward passing
			plan = data.scheduledTime - currentWaypoint->data.arrival;
		}
	}
	
	return worldTime > (startTime + plan + 3);
}


// returns true if the path has to be canceled
bool PB_Path::cannotBeContinued(edict_t *ent) {
	PB_Path_Waypoint nextWP = getNextWaypoint();
	float dist = (nextWP.pos(ent) - ent->v.origin).Length();
	
	if (dist > 100) {
		// suppose bot has fallen...
		if (nextWP.isOnPlatform()) {
			return true;
		}
	}
	
	return false;
}


// returns true if all waypoints have been confirmed and end-navpoint is reached by traveller
bool PB_Path::finished(edict_t *traveller) {
	// UNDONE for teleporter hintpoints!
//	if (currentWaypoint == waypoint->end()) { // no more waypoints to go...
	if (endNav().reached(traveller)) {
		return true;
	}
// }
	return false;
}


// returns the action to perform at the next waypoint
int PB_Path::getNextAction() {
	if (!hasData()) {
		return 0; // don't do anything on return path
	}
	
	assert(waypoint != 0);
	if (currentWaypoint != waypoint->end()) { // approaching another waypoint
		return currentWaypoint->action();
	}
	
	return 0;
}



// return true if bot has to wait for a platform before continuing to the next waypoint
bool PB_Path::waitForPlatform() {
	bool wait = false;
	assert(platformPos != 0);
	if (platformPos->size() == 0) {
		return false; // no platform on path
	}
	
	if ((currentWaypoint != waypoint->end()) && currentWaypoint->isOnPlatform()) {
		if (currentPlat != platformPos->end()) {
			edict_t *plat = getNavpoint(currentPlat->data.navId).entity();
			assert (plat != 0);
			
			if (plat != lastWaitingPlat) {
				lastWaitingPlat = plat;
				waitPlatEndTime = worldTime() + MAX_PLAT_WAIT;
			}
			
			Vector tDir = currentPlat->data.pos - plat->v.absmin;
			float tLen = tDir.Length();
			Vector vDir = plat->v.velocity;
			float vLen = vDir.Length();
			if (vLen > 0) {
				waitPlatEndTime = worldTime() + MAX_PLAT_WAIT;
			}
			
			if (tLen > 16) {
				wait = true;
			}
			
			if (worldTime() > waitPlatEndTime) {
				ignoredPlat = plat;
				wait = false;
			}
			
			/* if (getNavpoint(currentPlat->data.navId).type()==NAV_F_PLAT) {
				// for platforms only wait if they are moving (touch lift problem!)
				if (tLen > 16 && vLen > 0) wait = true;
			}
			else {
				// always wait for trains and doors
				if (tLen > 16) wait = true;
			}
			*/

			if (!wait && vLen > 0) {
				float dres = DotProduct((vDir/vLen), (tDir/tLen));
				if (dres > 0) {
					wait = true; // platform still moving in dir
				}
			}
		}
	}
	
	return wait;
}



// returns the next waypoint on a platform or (0, 0, 0) if no platform on path
Vector PB_Path::nextPlatformPos() {
	Vector wpPlatPos;
	assert(platformPos != 0);
	if (platformPos->size() == 0) {
		return Vector(0, 0, 0); // no platform on path
	}
	
	if ((currentWaypoint != waypoint->end()) && currentWaypoint->isOnPlatform()) {
		wpPlatPos = currentWaypoint->pos();
		return wpPlatPos;
	}
	
	WaypointList::iterator storeWP = currentWaypoint;
	PlatformList::iterator storePF = currentPlat;
	// simulate next WP
	reportWaypointReached();
	if ((currentWaypoint != waypoint->end()) && currentWaypoint->isOnPlatform()) {
		wpPlatPos = currentWaypoint->pos();
	} else { // alright this if-else is bad style :-(
		// simulate next WP
		reportWaypointReached();
		if ((currentWaypoint != waypoint->end()) && currentWaypoint->isOnPlatform()) {
			wpPlatPos = currentWaypoint->pos();
		}
	}
	
	// restore values
	currentWaypoint = storeWP;
	currentPlat = storePF;
	
	return wpPlatPos;
}



// returns the next waypoint
PB_Path_Waypoint PB_Path::getNextWaypoint() {
	assert(waypoint != 0);
	if (currentWaypoint != waypoint->end()) { // approaching another waypoint
		return (*currentWaypoint);
	}
	
	// approaching the target navpoint
	//PB_Navpoint n = getNavpoint(data.endId);
	return PB_Path_Waypoint(endNav().pos(), WP_IS_NAVPOINT, 0);
}


// returns the position of the last waypoint reached
Vector PB_Path::getLastWaypointPos(edict_t *playerEnt) {
	assert(waypoint != 0);
	if (lastReachedWaypoint != waypoint->end()) { // at least one waypoint has been reached
		return lastReachedWaypoint->pos(playerEnt);
	}
	
	// nothing reached yet
	PB_Navpoint n = getNavpoint(data.startId);
	return n.pos(playerEnt);
}


// confirms the waypoint as reached, internally choose next waypoint
void PB_Path::reportWaypointReached() {
	assert(waypoint != 0);
	assert(platformPos != 0);
	if (currentWaypoint == waypoint->end()) {
		return; // nothing more to reach...
	}
	
	lastReachedWaypoint = currentWaypoint;
	lastReachedPlat = currentPlat;
	if (forwardPass) { // path in forward direction ?
		if (currentWaypoint->isOnPlatform()) {
			currentPlat++;
		}
		currentWaypoint++;
	} else {
		if (currentWaypoint->isOnPlatform()) {
			if (currentPlat != platformPos->begin()) {
				currentPlat--;
			} else {
				currentPlat = platformPos->end();
			}
		}
		if (currentWaypoint != waypoint->begin()) { // approaching another waypoint
			currentWaypoint--;
		} else {
			currentWaypoint = waypoint->end(); // manually set to end (backward!)			
		}
	}
}


// message that the waypoint has not been reached in time, roll back possible changes
void PB_Path::reportWaypointFailed() {
	//startTime += 1.5;
}
