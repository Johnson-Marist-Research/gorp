#include "PriorityQueue.h"

#include <iostream>
#include <limits>

// Default Constructor
PriorityQueue::PriorityQueue() {};

// Initializes the key and value variables
/*PriorityQueue::PriorityQueue(std::string k, std::shared_ptr<WorldState> v) {
	key = k;
	value = v;
}*/

int PriorityQueue::size() {
	return heapSize;
}

// Checks if the heap is empty
bool PriorityQueue::is_empty() {
	//std::cout << "Running PriorityQueue.is_empty()" << std::endl;
	if (heapSize == 0) {
		return true;
	}
	else {
		return false;
	}
};

// Returns the first position in the heap
Element PriorityQueue::front() {
	//std::cout << "Running PriorityQueue.front()" << std::endl;
	return heap[0];
};

// Returns the leftmost position of the variables in the heap
int PriorityQueue::left(int i) {
	//std::cout << "Running PriorityQueue.left()" << std::endl;
	return 2 * (i + 1) - 1;
};

int PriorityQueue::parent(int i) {
	//std::cout << "Running PriorityQueue.parent()" << std::endl;
	return floor((i - 1) / 2); // floor is redundant here, but retained for clarity of purpose
};

// Returns the rightmost position of the variables in the heap
int PriorityQueue::right(int i) {
	//std::cout << "Running PriorityQueue.right()" << std::endl;
	return 2 * (i + 1);
};

// Inserts a value into heap
void PriorityQueue::insert(std::shared_ptr<WorldState> x, int priority) {
	//std::cout << "Running PriorityQueue.insert()" << std::endl;
	if (heapSize == heap.size()) {
		std::cerr << "No more room in the heap!" << std::endl;
		// Kill the program for now
		exit(0);
		//return;
	}
	// If this was a simple PriorityQueue* pointer, we would only need PriorityQueue* newElement = new PriorityQueue(priority, x);
	// However, since it is a shared pointer, we need the std::make_shared
	//std::shared_ptr<PriorityQueue> newElement = std::make_shared<PriorityQueue>(std::to_string(priority), x);
	// std::pair<int, WorldState>
	Element newElement;
	newElement.key = priority;
	newElement.value = x;
	//std::shared_ptr<PriorityQueue> newElement = std::make_shared<PriorityQueue>(Element.first, Element.second);
	//update_key(sizeof(heap) - 1, newElement);
	update_key(heapSize, newElement);
	heapSize++;
};

// Takes a value out of heap
std::shared_ptr<WorldState> PriorityQueue::extract() {
	//std::cout << "Running PriorityQueue.extract()" << std::endl;
	if (heapSize < 1){
		return nullptr; // error “heap underflow”
	}
	std::shared_ptr<WorldState> extremum = heap[0].value;
	heap[0] = heap[heapSize - 1];
	heapSize--;
	heapify(0);
	return extremum;
};

// Trims the heap down?
void PriorityQueue::heapify(int i) {
	//std::cout << "Running PriorityQueue.heapify()" << std::endl;
	int l = left(i);
	int r = right(i);
	int extremum;
	if ((l < heapSize) && (heap[l].key < heap[i].key)) {
		extremum = l;
	}
	else {
		extremum = i;
	}
	if ((r < heapSize) && (heap[r].key < heap[extremum].key)) {
		extremum = r;
	}
	if (extremum != i) {
		Element tmp = heap[i];
		heap[i] = heap[extremum];
		heap[extremum] = tmp;
		heapify(extremum);
	}
	//std::cout << "Extremum: " << extremum << std::endl;
};

// Updates the value of a key in heap
void PriorityQueue::update_key(int i, Element elem) {
	//std::cout << "Running PriorityQueue.update_key()" << std::endl;
	// var prev_key = heap[i].key if heap[i] else INF
	int prev_key;
	if (0 <= i && i < heapSize) {
		prev_key = heap[i].key;
	}
	else {
		// This should be the C++ code corresponding to infinity...
		prev_key = std::numeric_limits<int>::max();
	}
	heap[i] = elem;
	if (prev_key < elem.key) {
		heapify(i);
		return;
	}
	int j = parent(i);
	while ((i > 0) && (heap[i].key < heap[j].key)) {
		Element tmp = heap[i];
		heap[i] = heap[j];
		heap[j] = tmp;
		i = j;
		j = parent(i);
	}
};