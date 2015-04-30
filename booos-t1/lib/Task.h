/*
 * Task.h
 *
 *  Created on: Aug 15, 2014
 *      Author: arliones
 */

#ifndef TASK_H_
#define TASK_H_

#include <Queue.h>
#include <ucontext.h>
//#include <BOOOS.h>
//#include "Scheduler.h"

static const int STACK_SIZE = 32768;

namespace BOOOS {

class Task : public Queue::Element {


public:

	enum State {
		READY,
		WAITING,
		RUNNING,
		FINISHING,
		SCHEDULER
	};
	static int count(){return _count;} 
	Task(void (*entry_point)(void*), int nargs, void * arg);
	virtual ~Task();
	int tid_counter(){return __tid_counter;}
	int tid() { return _tid; }
	State state() { return _state; }
	void  yield(); 
	void pass_to(Task * t, State s = READY);
	void exit(int code);
	void nice(int n);
	static Task * self() { return (Task*) __running; }
	static void init();
	int join();
	void wait(Task * t);
	
protected:	

	Task();
	void insert_in_ready(Task*);
	State _state;
	static Queue __ready, _waiting;
	int _tid;
	static int _count;
	ucontext_t _context;
	static Task * __main;

private:

	char * _stack;
	int _exit_code;
	static int __tid_counter;
	static volatile Task * __running;

/*

+ public 
- private 
# protected 
	
	*/
};
} /* namespace BOOOS */

#endif /* TASK_H_ */
