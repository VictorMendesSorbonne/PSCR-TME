#pragma once

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <cstring>

namespace pr {

// MT safe version of the Queue, non blocking.
template <typename T>
class Queue {
	T ** tab;
	const size_t allocsize;
	size_t begin;
	size_t sz;
	bool isBlocking;
	mutable std::mutex m;
	std::condition_variable cv;

	// fonctions private, sans protection mutex
	bool empty() const {
		return sz == 0;
	}
	bool full() const {
		return sz == allocsize;
	}
public:
	Queue(size_t size) :allocsize(size), begin(0), sz(0), isBlocking(false){
		tab = new T*[size];
		memset(tab, 0, size * sizeof(T*));
	}
	size_t size() const {
		std::unique_lock<std::mutex> lg(m);
		return sz;
	}
	T* pop() {
		std::unique_lock<std::mutex> lg(m);
		while(empty() && isBlocking){
			cv.wait(lg);
		}
		if (empty()) {
			return nullptr;
		}
		if(full()){
			cv.notify_all();
		}
		auto ret = tab[begin];
		tab[begin] = nullptr;
		sz--;
		begin = (begin + 1) % allocsize;
		return ret;
	}
	bool push(T* elt) {
		std::unique_lock<std::mutex> lg(m);
		while(full() && isBlocking){
			cv.wait(lg);
		}
		if (full()) {
			return false;
		}
		if(empty()){
			cv.notify_all();
		}
		tab[(begin + sz) % allocsize] = elt;
		sz++;
		return true;
	}
	void setBlocking(bool isBlocking){
		std::unique_lock<std::mutex> lg(m);
		this->isBlocking =isBlocking;
		cv.notify_all();
	}
	~Queue() {
		// ?? lock a priori inutile, ne pas detruire si on travaille encore avec
		for (size_t i = 0; i < sz; i++) {
			auto ind = (begin + i) % allocsize;
			delete tab[ind];
		}
		delete[] tab;
	}
};

}

