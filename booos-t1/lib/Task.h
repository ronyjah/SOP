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
	static int count(){return _count;} //definir o que deve ser retornado
	Task(void (*entry_point)(void*), int nargs, void * arg);
	virtual ~Task();
	int tid_counter(){return __tid_counter;}
	int tid() { return _tid; }
	State state() { return _state; }
	//Task::count() == 6, 9);
	//ASSERT(t4->state() == Task::READY, 8);
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
	State _state;
//	static Queue * __ready;
	static Queue __ready;
	int _tid;
	static int _count;

private:

	ucontext_t _context, uct_main;
	char * _stack;
	static int __tid_counter;
	static volatile Task * __running;
	static Task * __main;

/*

+ public 
- private 
# protected 
	
	*/
};
} /* namespace BOOOS */

#endif /* TASK_H_ */
