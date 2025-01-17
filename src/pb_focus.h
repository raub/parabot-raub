#ifndef _PB_FOCUS_HPP_
#define _PB_FOCUS_HPP_

#include "pb_sectors.h"


class PB_Focus : PB_Sectors {
public:
	PB_Focus();
	
	void addDir(Vector dir);
	float forDir(Vector dir);
	int cellsForDir(Vector dir);
	
	bool load(FILE *fp);
	bool save(FILE *fp);
	
private:
	short numCellsInSector[NUM_SECTORS];
	float focusValue[NUM_SECTORS];
};

#endif // _PB_FOCUS_HPP_