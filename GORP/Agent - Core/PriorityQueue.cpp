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

// Checks if the heap is empty
bool PriorityQueue::is_empty() {
	std::cout << "Running PriorityQueue.is_empty()" << std::endl;
	if ((heapStart == 0) && (heapEnd == -1)) {
		return true;
	}
	else {
		return false;
	}
};

// Returns the first position in the heap
Element PriorityQueue::front() {
	std::cout << "Running PriorityQueue.front()" << std::endl;
	return heap[0];
};

// Returns the leftmost position of the variables in the heap
int PriorityQueue::left(int i) {
	std::cout << "Running PriorityQueue.left()" << std::endl;
	return 2 * (i + 1) - 1;
};

int PriorityQueue::parent(int i) {
	std::cout << "Running PriorityQueue.parent()" << std::endl;
	return floor((i - 1) / 2); // floor is redundant here, but retained for clarity of purpose
};

// Returns the rightmost position of the variables in the heap
int PriorityQueue::right(int i) {
	std::cout << "Running PriorityQueue.right()" << std::endl;
	return 2 * (i + 1);
};

// Inserts a value into heap
void PriorityQueue::insert(std::shared_ptr<WorldState> x, int priority) {
	std::cout << "Running PriorityQueue.insert()" << std::endl;
	// If this was a simple PriorityQueue* pointer, we would only need PriorityQueue* newElement = new PriorityQueue(priority, x);
	// However, since it is a shared pointer, we need the std::make_shared
	//std::shared_ptr<PriorityQueue> newElement = std::make_shared<PriorityQueue>(std::to_string(priority), x);
	// std::pair<int, WorldState>
	Element newElement;
	newElement.key = priority;
	newElement.value = x;
	//std::shared_ptr<PriorityQueue> newElement = std::make_shared<PriorityQueue>(Element.first, Element.second);
	//update_key(sizeof(heap) - 1, newElement);
	update_key(heap.size() - 1, newElement);
	heapEnd++;
};

// Takes a value out of heap
std::shared_ptr<WorldState> PriorityQueue::extract() {
	std::cout << "Running PriorityQueue.extract()" << std::endl;
	if (heap.size() < 1){
		return nullptr; // error “heap underflow”
	}
	std::shared_ptr<WorldState> extremum = heap[0].value;
	heap[0] = heap[heapEnd];
	heapEnd--;
	heapify(0);
	return extremum;
};

// Trims the heap down?
void PriorityQueue::heapify(int i) {
	std::cout << "Running PriorityQueue.heapify()" << std::endl;
	int l = left(i);
	int r = right(i);
	int extremum;
	if ((l < sizeof(heap)) && (heap[l].key < heap[i].key)) {
		extremum = l;
	}
	else {
		extremum = i;
	}
	if ((r < sizeof(heap)) && (heap[r].key < heap[extremum].key)) {
		extremum = r;
	}
	if (extremum != i) {
		Element tmp = heap[i];
		heap[i] = heap[extremum];
		heap[extremum] = tmp;
		heapify(extremum);
	}
	std::cout << "Extremum: " << extremum << std::endl;
};

// Updates the value of a key in heap
void PriorityQueue::update_key(int i, Element elem) {
	std::cout << "Running PriorityQueue.update_key()" << std::endl;
	// var prev_key = heap[i].key if heap[i] else INF
	int prev_key;
	if (heapEnd > 0) {
		prev_key = heap[i].key;
	}
	else {
		// This should be the C++ code corresponding to infinity...
		prev_key = std::numeric_limits<int>::max();
	}
	std::cout << i << std::endl;
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