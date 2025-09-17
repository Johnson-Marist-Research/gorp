#pragma once

#include "WorldState.h"

#include <iostream>
#include <limits>
#include <cmath>
#include <string>
#include <memory>
#include <array>

class Element {
public:
	int key;
	//std::shared_ptr<WorldState> value;
	std::optional<WorldState> value;
};

class PriorityQueue {
public:
	// Needs a base value, so I'm initializing it as 100
	std::array<Element, 100> heap;

	// For keeping track of if the array is empty
	int heapSize = 0;

	// Default Constructor
	PriorityQueue();

	//PriorityQueue(std::string k, std::shared_ptr<WorldState> v);
	// Observers
	int size();
	bool is_empty();
	Element front();

	// Auxiliary Observers
	int left(int i);
	int parent(int i);
	int right(int i);

	// Mutators
	// Inserts an new element into the queue while preserving the heap property
	// "int x" was originally "Variable", but since the "value" in Element is an string, I changed it to string for now.
	//void insert(std::shared_ptr<WorldState> x, int priority);
	void insert(std::optional<WorldState> x, int priority);
	// Removes and returns the element of the queue with the highest priority
	// Originally returned "Variant", but "value" is now a string, so I'm setting it as that for now.
	//std::shared_ptr<WorldState> extract();
	std::optional<WorldState> extract();

	// Auxiliary Mutators
	// Maintains the heap property by floating the root down to its proper position
	void heapify(int i);
	// Updates the value of an element's key without violating the heap property
	void update_key(int i, Element elem);

};
