#ifndef _PB_JOURNEY_HPP_
#define _PB_JOURNEY_HPP_

#include <stack>
#include "pb_path.h"


class PB_Journey {
public:
	PB_Journey();
	~PB_Journey();
	
	// returns true if there are more paths to go
	bool continues() { return (!pathList.empty()); }
	
	// returns the next path of the journey
	PB_Path* getNextPath();
	
	// copies back the path to the original in mapgraph
	void savePathData();
	
	// cancels the journey (continues() will return false)
	void cancel();
	
	// DATA:
	std::deque<int>	pathList; // need access from mapGraph
	
private:
	PB_Path *currentOriginal;
	PB_Path currentCopy;
};

#endif // _PB_JOURNEY_HPP_
