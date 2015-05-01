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
	getcontext(&this->_context);
	this->_state = RUNNING;
	this->_tid = __tid_counter++;
	Task::_count++;

	}

void Task::init() {
	
	Task::__tid_counter  = 0;
	Task::_count = 0;
	Task::__main = new Task();
	Task::__running = Task::__main;
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
		 _context.uc_link = &__main->_context;
		 
	}else{
		cerr << "Erro para criar uma pilha! Endereço stack" << _stack << endl;
		//return 0;
	}
	
	makecontext(&_context, (void (*)())entry_point, nargs, arg);
	
	//alteração dos atributos da tarefa criada.
	this->_state = Task::READY;
	this->_tid = Task::__tid_counter;

	if(this->_tid != 1){
		if(BOOOS::SCHED_POLICY == BOOOS::SCHED_FCFS){
			Task::__ready.insert(this);
		}if(BOOOS::SCHED_POLICY == BOOOS::SCHED_PRIORITY){
			Task::__ready.insert_ordered(this);
		}	
	}
	//alteração das variaveis globais.
	Task::__tid_counter++;
	Task::_count++;	
}

void Task::insert_in_ready(Task *t) {
	if(BOOOS::SCHED_POLICY == BOOOS::SCHED_FCFS){                                     
		Task::__ready.insert(this);                                                       
    }if(BOOOS::SCHED_POLICY == BOOOS::SCHED_PRIORITY){                                
		Task::__ready.insert_ordered(this);                                       
    }                                                                                
}

void Task::pass_to(Task * t, State s){
		
		//tarefa corrente diferente de scheduler. Ex: __main
		if(this->_state != SCHEDULER) this->_state = s;
		
		if(this->_state == Task::READY){
			if(BOOOS::SCHED_POLICY == BOOOS::SCHED_FCFS){
				Task::__ready.insert(this);
			}if(BOOOS::SCHED_POLICY == BOOOS::SCHED_PRIORITY){
				Task::__ready.insert_ordered(this);
			}	
		}
		
		//nova tarefa muda o estado se nao for dispatcher
		if(t->_state != SCHEDULER) t->_state = RUNNING;
		
		//atualização da running com a nova tarefa.
		Task::__running = t;
		swapcontext(&this->_context,&t->_context);		
}

void Task::exit(int code){
	Task::_count--;
	this->_exit_code = code;
	while(_waiting.length()) {
		insert_in_ready((Task*)_waiting.remove());
	}
	this->pass_to(Scheduler::self(), FINISHING);
}

void Task::nice(int n){
	if(n > -21 and n < 21){
		this->rank(n);
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
	if (this->_state != Task::FINISHING) {
		this->wait(self());
		this->pass_to(Scheduler::self(), WAITING);
	}
	return this->_exit_code;
}

} /* namespace BOOOS */
