#pragma once

#include "WorldState.h"

#include <string>
#include <memory>

class PriorityQueue {
public:
	// var heap:Array
	// Needs a base value, so I'm initializing it as 100
	std::shared_ptr<PriorityQueue> heap[100];

	// Original is just "var key" and "var value", so I'm guessing at the data type
	std::string key;
	std::shared_ptr<WorldState> value;

	// For keeping track of if the array is empty
	// Setting heapEnd as -1; this is due to the fact that the first element in the array will be at position 0
	// Therefore, if the heap is empty, then the end will be less than one
	int heapStart = 0;
	int heapEnd = -1;
	
	PriorityQueue(std::string k, std::shared_ptr<WorldState> v);
	// Observers
	bool is_empty();
	std::shared_ptr<PriorityQueue> front();

	// Auxiliary Observers
	int left(int i);
	int parent(int i);
	int right(int i);

	// Mutators
	// Inserts an new element into the queue while preserving the heap property
	// "int x" was originally "Variable", but since the "value" in Element is an string, I changed it to string for now.
	void insert(std::shared_ptr<WorldState> x, int priority);
	// Removes and returns the element of the queue with the highest priority
	// Originally returned "Variant", but "value" is now a string, so I'm setting it as that for now. 
	// DEBUG: Have extract() return WorldState or WorldState*
	std::shared_ptr<WorldState> extract();

	// Auxiliary Mutators
	// Maintains the heap property by floating the root down to its proper position
	void heapify(int i);
	// Updates the value of an element's key without violating the heap property
	void update_key(int i, std::shared_ptr<PriorityQueue> nonPointerElem);
};

// Change to std::pair (using Element = std::pair<int, WorldState>)
/*class Element {
public:
	// Original is just "var key" and "var value", so I'm guessing at the data type
	int key;
	// DEBUG: Change to WorldState or WorldState*
	std::shared_ptr<WorldState> value;
	// Replace all _inits with a constructor
	void _init(int k, std::shared_ptr<WorldState> v) {
		key = k;
		value = v;
	}
};*/