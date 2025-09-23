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
	WorldState value;
};

class PriorityQueue {
public:
	// Needs a base value, so I'm initializing it as 100
	std::array<std::optional<Element>, 100> heap;

	// For keeping track of if the array is empty
	int heapSize = 0;

	// Default Constructor
	PriorityQueue();

	// Observers
	int size();
	bool is_empty();
	std::optional<Element> front() const;

	// Auxiliary Observers
	int left(int i);
	int parent(int i);
	int right(int i);

	// Mutators
	// Inserts an new element into the queue while preserving the heap property
	// "int x" was originally "Variable", but since the "value" in Element is an string, I changed it to string for now.
	void insert(WorldState const& x, int priority);
	// Removes and returns the element of the queue with the highest priority
	// Originally returned "Variant", but "value" is now a string, so I'm setting it as that for now.
	std::optional<WorldState> extract();

	// Auxiliary Mutators
	// Maintains the heap property by floating the root down to its proper position
	void heapify(int i);
	// Updates the value of an element's key without violating the heap property
	void update_key(int i, Element elem);

};
