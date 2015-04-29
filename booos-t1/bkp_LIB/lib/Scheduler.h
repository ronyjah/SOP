/*
 * Scheduler.h
 *
 *  Created on: Mar 21, 2014
 *      Author: arliones
 */
 
#ifndef SCHEDULER_H_
#define SCHEDULER_H_
 
#include <Task.h>
#include <Queue.h>
 
namespace BOOOS {
 
class Scheduler : public Task {
	friend class Task;
 
protected:
	Scheduler();
 
public:
	enum SchedulerType {

		SCHED_FCFS,
		SCHED_PRIORITY
	};
	
//	static SchedulerType SCHED_POLICY;
//	static bool SCHED_PREEMPT;
//	static bool SCHED_AGING;
	
	/*
		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_FCFS;
		BOOOS::BOOOS::SCHED_PREEMPT = false;
		BOOOS::BOOOS::SCHED_AGING = false;
		BOOOS::BOOOS booos(false);
	*/
 
	virtual ~Scheduler();
 
	static void init();
 
	static void dispatcher(void*);
 
	static Scheduler * self() { return __dispatcher; }
 
protected:
	virtual Task * choose_next();
 
	static Scheduler * __dispatcher;
};
 
} /* namespace BOOOS */
 
#endif /* SCHEDULER_H_ */
