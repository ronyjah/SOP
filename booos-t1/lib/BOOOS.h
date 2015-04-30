/*
 * BOOOS.h
 *
 *  Created on: Aug 14, 2014
 *      Author: arliones
 */

#ifndef BOOOS_H_
#define BOOOS_H_

#include <Scheduler.h>

namespace BOOOS {
	enum SchedulerType {

		SCHED_FCFS,
		SCHED_PRIORITY
	};

/* Configuration Space */

class BOOOS {
public:

	static SchedulerType SCHED_POLICY;
	static bool SCHED_PREEMPT;
	static bool SCHED_AGING;
	

	BOOOS(bool verbose = true);
	~BOOOS();

	static const int BOOOS_VERSION = 0;
	//SchedulerType sched() {return SCHED_POLICY;}
		   
	void panic();

private:
	bool _verbose;
};

} /* namespace BOOOS */

#endif /* BOOOS_H_ */
