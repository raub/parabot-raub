#ifndef _PB_GOALFINDER_HPP_
#define _PB_GOALFINDER_HPP_

#pragma warning(disable : 4786) // disable stl warnings

#include <list>
#include <map>

#include "pb_global.h"
#include "pb_perception.h"


class CParabot;
typedef void (*tGoalFunc)(CParabot*, PB_Percept*);
typedef float (*tWeightFunc)(CParabot*, PB_Percept*);

typedef struct {
	int type; // goal class (parallism!)
	tGoalFunc goal;
	tWeightFunc weight;
} tGoal;
typedef std::multimap<int, tGoal> tGoalList;

#define GOAL_UNCONDITIONAL 0

// goal classes (flags!):
#define G_ACTION 0 // only impact on action
#define G_MOVE 1 // impact on movement 
#define G_VIEW 2 // impact on body-/viewangle

#define MAX_GOALS 4 // combinations!


class PB_GoalFinder {
public:
	PB_GoalFinder() {};
	~PB_GoalFinder();
	
	// initializes all necessary variables
	void init(CParabot *pb);
	void addGoal(int goalClass, int triggerId, tGoalFunc gf, tWeightFunc wf);
	void analyze(PB_Perception &senses);
	void analyzeUnconditionalGoals();
	void check();
	
	// deletes goals that can't be executed at the same time
	void synchronize();
	
	// returns a pointer to the best goal function
	tGoalFunc bestGoal(int nr) { return bestGoalFunction[nr]; }
	
	// returns a pointer to the perception that caused the selection
	PB_Percept* trigger(int nr) { return responsiblePercept[nr]; }
	
private:
	tGoalList knownGoals;
	CParabot *bot;
	tGoalFunc bestGoalFunction[MAX_GOALS]; // for each class
	PB_Percept *responsiblePercept[MAX_GOALS];
	float bestWeight[MAX_GOALS];
};

#endif
