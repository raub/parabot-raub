#ifndef _PBT_PRIORITYQUEUE_HPP_
#define _PBT_PRIORITYQUEUE_HPP_

// maximal number of elements that queue can handle
#define MAX_QUEUE_ELEMENTS 8191

#define EMPTY_KEY MAX_QUEUE_ELEMENTS
#define NOT_IN_HEAP -1


class PBT_PriorityQueue {
public:
	PBT_PriorityQueue();
	void init();
	
	bool empty() { return (numElements == 0); }
	int size() { return numElements; }
	bool neverContained(short index) { return (heapPos[index] == NOT_IN_HEAP); }
	float getWeight(short index) { return weight[index]; }
	float getValue (short index) { return value[index]; }
	
	short getFirst();
	void addOrUpdate(short index, float newWeight, float newValue=0.0);
	
private:
	short heap[MAX_QUEUE_ELEMENTS]; // heap with node indices
	float weight[MAX_QUEUE_ELEMENTS + 1]; // cost array
	float value[MAX_QUEUE_ELEMENTS + 1]; // e.g. costs without heuristics
	short heapPos[MAX_QUEUE_ELEMENTS]; // heap indices
	int numElements; // number of elements in heap
	short freePos[MAX_QUEUE_ELEMENTS]; // list with free heap positions
	int numFree; // number of elements in freePos
};

#endif /* _PBT_PRIORITYQUEUE_HPP_ */