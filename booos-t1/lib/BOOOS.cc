/*
* BOOOS.h
*
*  Created on: Aug 14, 2014
*      Author: arliones
*/

#include <BOOOS.h>
#include <Task.h>
#include <iostream>
#include <Scheduler.h>

namespace BOOOS {

	SchedulerType BOOOS::SCHED_POLICY;
	bool BOOOS::SCHED_PREEMPT;
	bool BOOOS::SCHED_AGING;
	

BOOOS::BOOOS(bool verbose) : _verbose(verbose) {
	if(_verbose) std::cout << "Welcome to BOOOS - Basic Object Oriented Operating System!" << std::endl;

	Task::init();
	Scheduler::init();
	
}

BOOOS::~BOOOS() {
	// Call finish routines of other components (if any)


	if(_verbose) std::cout << "BOOOS ended... Bye!" << std::endl;
}

void BOOOS::panic() {
	std::cerr << "BOOOSta! Panic!" << std::endl;
	while(true);
}

} /* namespace BOOOS */
