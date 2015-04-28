#include <iostream>
#include <queue>
#include <sstream>
#include <BOOOS.h>
#include <Scheduler.h>
 
#define ASSERT(x,y) if(!(x)) return y;
 
using namespace std;
using namespace BOOOS;
 
Task *pang, *peng, *ping, *pong, *pung;
 
void function(void * arg) {
	int i;
 
	Task::self()->nice(2*Task::self()->tid());
 
	for(i=0; i<10; i++) {
		cout << (char*)arg << " " << i << endl;
	//	Timer::delay_ticks(25);
	}
	cout << (char*)arg << " End" << endl;
	Task::self()->exit(2*Task::self()->tid());
}
 
int main() {
	BOOOS::SCHED_POLICY == SCHED_FCFS;
	BOOOS::SCHED_PREEMPT = true;
	BOOOS::SCHED_AGING = true;
	BOOOS::BOOOS booos(false);
 
	cout << "Main Start" << endl;
 
	pang = new Task(function, 1, (char*)"\tPang");
	peng = new Task(function, 1, (char*)"\t\tPeng");
	ping = new Task(function, 1, (char*)"\t\t\tPing");
	pong = new Task(function, 1, (char*)"\t\t\t\tPong");
	pung = new Task(function, 1, (char*)"\t\t\t\t\tPung");
 
	while(Task::count() > 2) {
		Task::self()->nice(20);
		Task::self()->yield();
	}
 
	int result = 0;
 
	cout << "Main wait pang... ";
	result += pang->join();
	cout << "Result: " << result << endl;
 
	cout << "Main wait peng... ";
	result += peng->join();
	cout << "Result: " << result << endl;
 
	cout << "Main wait ping... ";
	result += ping->join();
	cout << "Result: " << result << endl;
 
	cout << "Main wait pong... ";
	result += pong->join();
	cout << "Result: " << result << endl;
 
	cout << "Main wait pung... ";
	result += pung->join();
	cout << "Result: " << result << endl;
 
 
	cout << "Main End" << endl;
	Task::self()->exit(0);
 
	return 0;
}
