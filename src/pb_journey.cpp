#include "pb_journey.h"
#include "pb_global.h"



PB_Journey::PB_Journey() {
	currentOriginal = 0;
}


PB_Journey::~PB_Journey() {
	pathList.clear();
}


// returns the next path of the journey
PB_Path* PB_Journey::getNextPath() {
	int pathId = pathList.back();
	pathList.pop_back();
//	return getPath(pathId);
	currentOriginal = getPath(pathId);
	currentCopy = *currentOriginal;
	return &currentCopy; 
}


// copies back the path to the original in mapgraph
void PB_Journey::savePathData() {
	assert(currentOriginal != 0);
	*currentOriginal = currentCopy;
}


// cancels the journey (continues() will return false) { 
void PB_Journey::cancel() {
	pathList.clear();
	currentOriginal = 0;
}