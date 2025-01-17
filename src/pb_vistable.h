#ifndef _PB_VISTABLE_HPP_
#define _PB_VISTABLE_HPP_

#include <stdio.h>

#define MAX_CELLS 8192


class PB_VisTable {
public:
	PB_VisTable();
	virtual ~PB_VisTable();
	
	void clear();
	
	void load(FILE *fp);
	
	void save(FILE *fp);
	
	void addCell();
	
	// returns true and two cells that still have to be traced or false
	bool needTrace(int &cell1, int &cell2);
	
	// sets the result for the next two cells (as returned by needTrace)
	void addTrace(bool visible);
	
	bool isVisible(int cell1, int cell2);
	
private:
	int *visTable[MAX_CELLS];
	int bitMask[32]; // stores the bitmask for fast anding
	int numCells;
	int traceCell, traceBit;
	
	int* getMem(int numBits);
	
	void setVisibility(int cell1, int cell2, bool visible);
};

#endif /* _PB_VISTABLE_HPP_ */
