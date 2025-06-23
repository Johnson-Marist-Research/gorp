#include "PriorityQueue.h"

#include <limits>

PriorityQueue::PriorityQueue(std::string k, std::shared_ptr<WorldState> v) {
	key = k;
	value = v;
}

bool PriorityQueue::is_empty() {
	if ((heapStart == 0) && (heapEnd == -1)) {
		return true;
	}
	else {
		return false;
	}
};

std::shared_ptr<PriorityQueue> PriorityQueue::front() {
	return heap[0];
};

int PriorityQueue::left(int i) {
	return 2 * (i + 1) - 1;
};

int PriorityQueue::parent(int i) {
	return floor((i - 1) / 2); // floor is redundant here, but retained for clarity of purpose
};

int PriorityQueue::right(int i) {
	return 2 * (i + 1);
};

void PriorityQueue::insert(std::shared_ptr<WorldState> x, int priority) {
	// If this was a simple PriorityQueue* pointer, we would only need PriorityQueue* newElement = new PriorityQueue(priority, x);
	// However, since it is a shared pointer, we need the std::make_shared
	std::shared_ptr<PriorityQueue> newElement = std::make_shared<PriorityQueue>(priority, x);
	PriorityQueue newElement(std::to_string(priority), x);
	update_key(sizeof(heap) - 1, newElement);
	heapEnd++;
};

/*
/*
func insert(x:Variant, priority:int):
	heap.push_back(null)
	update_key(heap.size() - 1, Element.new(priority, x))
*/

std::shared_ptr<WorldState> PriorityQueue::extract() {
	if (sizeof(heap) < 1){
		return nullptr; // error “heap underflow”
	}
	std::shared_ptr<WorldState> extremum = heap[0]->value;
	heap[0] = heap[heapEnd];
	heapEnd--;
	heapify(0);
	return extremum;
};

void PriorityQueue::heapify(int i) {
	int l = left(i);
	int r = right(i);
	int extremum;
	if ((l < sizeof(heap)) && (heap[l]->key < heap[i]->key)) {
		extremum = l;
	}
	else {
		extremum = i;
	}
	if ((r < sizeof(heap)) && (heap[r]->key < heap[extremum]->key)) {
		extremum = r;
	}
	if (extremum != i) {
		std::shared_ptr<PriorityQueue> tmp = heap[i];
		heap[i] = heap[extremum];
		heap[extremum] = tmp;
		heapify(extremum);
	}
};

void PriorityQueue::update_key(int i, std::shared_ptr<PriorityQueue> elem) {
	// var prev_key = heap[i].key if heap[i] else INF
	std::string prev_key;
	if (heapEnd > 0) {
		prev_key = heap[i]->key;
	}
	else {
		// This should be the C++ code corresponding to infinity...
		prev_key = std::numeric_limits<int>::max();
	}
	heap[i] = elem;
	if (prev_key < elem->key) {
		heapify(i);
		return;
	}
	int j = parent(i);
	while ((i > 0) && (heap[i]->key < heap[j]->key)) {
		std::shared_ptr<PriorityQueue> tmp = heap[i];
		heap[i] = heap[j];
		heap[j] = tmp;
		i = j;
		j = parent(i);
	}
};