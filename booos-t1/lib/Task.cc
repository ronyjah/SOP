#include <iostream>
#include "BOOOS.h"
#include "Task.h"
#include <stdlib.h>
#include "Scheduler.h"

using namespace std;
namespace BOOOS
{

volatile Task * Task::__running;
int Task::__tid_counter;
Task * Task::__main = 0 ;
Queue Task::__ready;
Queue Task::_waiting;
int Task::_count;

Task::Task(){
	}

void Task::init() {
	
	Task::__main = new Task();
	getcontext(&__main->_context);
	Task::__running = Task::__main;
	Task::__main->_state = RUNNING;
	Task::_count = 0;
	Task::__tid_counter  = 0;
	Task::__main->_tid = __tid_counter;
	Task::__tid_counter++;
	Task::_count++;
}

Task::~Task(){
	free(this->_context.uc_stack.ss_sp);
}

Task::Task(void (*entry_point)(void*), int nargs, void * arg){
	getcontext(&_context);
	char * _stack = (char*) malloc(STACK_SIZE);
	if(_stack){
		 _context.uc_stack.ss_sp = _stack;
		 _context.uc_stack.ss_size = STACK_SIZE;
		 _context.uc_stack.ss_flags = 0;
		 _context.uc_link = 0;
		 
	}else{
		cout << "Erro para criar uma pilha! Endereço stack" << _stack << endl;
		//return 0;
	}
	makecontext(&_context, (void (*)())entry_point, nargs, arg);
	this->_state = Task::READY;
	this->_tid = Task::__tid_counter;
	Task::__tid_counter++;
	Task::_count++;

	if(this->_tid != 1){
		if(BOOOS::SCHED_POLICY == SCHED_FCFS){
			Task::__ready.insert(this);
		}if(BOOOS::SCHED_POLICY == SCHED_PRIORITY){
			Task::__ready.insert_ordered(this);
		}	
	}	
}

void Task::pass_to(Task * t, State s){
		if(this->_state != SCHEDULER) this->_state = s;
		if(this->_state == Task::READY){
			if(BOOOS::SCHED_POLICY == SCHED_FCFS){
			Task::__ready.insert(this);
			}if(BOOOS::SCHED_POLICY == SCHED_PRIORITY){
				Task::__ready.insert_ordered(this);
			}	
		}else if(this->_state == Task::FINISHING){ 
		}
		if(t->_state != SCHEDULER) t->_state = RUNNING;
		Task::__running = t;
		swapcontext(&this->_context,&t->_context);		
}

void Task::exit(int code){
	State aux=FINISHING;
	Task::_count--;
	this->pass_to(Scheduler::self(), aux);
}

void Task::nice(int n){
	if(n > -21 and n < 21){
		rank(n);
	}else{
		cerr << "Erro. Prioridade: "<< n << endl;	
	}
}

void Task::yield(){
	this->pass_to(Scheduler::self());
}

void Task::wait(Task * t) {
	_waiting.insert(t);
	return;
}

int Task::join() {
	if (this == 0) {
		return -1;
	}
	if (this->_state != Task::FINISHING) {
		this->wait(this);
		this->_state = WAITING;
		this->yield();
	}
	return 0;
}

} /* namespace BOOOS */
