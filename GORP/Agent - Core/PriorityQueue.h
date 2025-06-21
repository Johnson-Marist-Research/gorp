#pragma once

#include <string>

class PriorityQueue {
public:
	// var heap:Array
	// Needs a base value, so I'm initializing it as 100
	Element* heap[100];

	// For keeping track of if the array is empty
	// Setting heapEnd as -1; this is due to the fact that the first element in the array will be at position 0
	// Therefore, if the heap is empty, then the end will be less than one
	int heapStart = 0;
	int heapEnd = -1;
	
	//void _init(f: Callable);
	// Observers
	bool is_empty();
	Element* front();

	// Auxiliary Observers
	int left(int i);
	int parent(int i);
	int right(int i);

	// Mutators
	// Inserts an new element into the queue while preserving the heap property
	// "int x" was originally "Variable", but since the "value" in Element is an string, I changed it to string for now.
	void insert(std::string x, int priority);
	// Removes and returns the element of the queue with the highest priority
	// Originally returned "Variant", but "value" is now a string, so I'm setting it as that for now. 
	Element* extract();

	// Auxiliary Mutators
	// Maintains the heap property by floating the root down to its proper position
	void heapify(int i);
	// Updates the value of an element's key without violating the heap property
	void update_key(int i, Element nonPointerElem);

	// returning values
	std::string return_extremum(std::string extremum);
	void* return_null();

};

class Element {
public:
	// Original is just "var key" and "var value", so I'm guessing at the data type
	int key;
	std::string value;
	void _init(int k, std::string v) {
		key = k;
		value = v;
	}
};