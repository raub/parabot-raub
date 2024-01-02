#ifndef _HL_CLASSES_HPP_
#define _HL_CLASSES_HPP_

#include "pb_global.h"


#define CSUITPLAYLIST 4
#define CSUITNOREPEAT 32
#define TEAM_NAME_LENGTH 16
#define CBTEXTURENAMEMAX 13 // only load first n chars of name


class CMultiManager : public CBaseToggle {
public:
	int m_cTargets; // the total number of targets in this manager's fire list.
	int m_index; // Current target
	float m_startTime; // Time we started firing
	
	// list if indexes into global string array
	int m_iTargetName[MAX_MULTI_TARGETS];
	
	// delay (in seconds) from time of manager fire to target fire
	float m_flTargetDelay[MAX_MULTI_TARGETS];
};

#endif // _HL_CLASSES_HPP_
