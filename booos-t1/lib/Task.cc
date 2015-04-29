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
Queue Task::__ready, _waiting;
int Task::_count;


Task::Task(){
	}

void Task::init() {
	
	Task::__main = new Task();
	getcontext(&Task::__main->_context);
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
		 //(ucontext*)
	}else{
		cout << "Erro para criar uma pilha! Endereço stack" << _stack << endl;
		//return 0;
	}
	makecontext(&_context, (void (*)())entry_point, nargs, arg);
	this->_state = Task::READY;
	this->_tid = Task::__tid_counter;
	Task::__tid_counter++;
	Task::_count++;
	
	cout << "SCHE_POLICY NA TASK: " << SCHED_POLICY << endl;
	//cout << "valor de zulu " << zulu << endl;
	if(this->_tid != 1){
		if(1){
			Task::__ready.insert_ordered(this);	
			//SCHED_POLICY == SCHED_FCFS
		//	cout << "politica FCFS" << endl;	
			//Task::__ready.insert(this);
		}else{//(SCHED_POLICY == SCHED_PRIORITY){
			cout << " politica PRIO" << endl;
			
		//}else{
		//	cout << "politica nao definida" << endl;
		}	
		//}	
	}	
	
	
	//return;
}



void Task::pass_to(Task * t, State s){
		
		if(this->_state != SCHEDULER) this->_state = s;
		if(this->_state == Task::READY){
			Task::__ready.insert(this); 
			
		}else if(this->_state == Task::FINISHING){ 
			//cout << "Main End2\n";
		}
		if(t->_state != SCHEDULER) t->_state = RUNNING;
		Task::__running = t;
		swapcontext(&this->_context,&t->_context);		

	//return;
}

void Task::exit(int code){
	//cout << _count;
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
		//pass_to(self(), Task::WAITING);
	}
	return 0;
}

/*
MENSAGENS PARA DEPURAÇÃO
  cout << "----=====EXECUTANDO=====----" << endl;
  cout << "Task::init __tid_counter: " <<__tid_counter << endl;
  cout << "Task::init  _count: " <<_count << endl;
  cout << "Task::init __tid_counter: " <<__tid_counter << endl;
  cout << "Tid da tarefa em execução>: "<< this->_tid << endl;
  cout << "Tid da tarefa a entrar>: "<< tExec->_tid << endl; 
   
*/
} /* namespace BOOOS */
