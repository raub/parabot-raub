#ifndef _PB_SECTORS_HPP_
#define _PB_SECTORS_HPP_

#include "pb_global.h"

#define NUM_SECTORS 4

class PB_Sectors {
protected:
	int getSector(Vector dir);
};

#endif // _PB_SECTORS_HPP_