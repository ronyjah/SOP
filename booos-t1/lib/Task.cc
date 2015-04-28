#include <iostream>
#include <sstream>
#include "BOOOS.h"
#include "Task.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <Scheduler.h>

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
		 _context.uc_link = (ucontext*)1;
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
		if(SCHED_POLICY == SCHED_FCFS){	
			Task::__ready.insert(this);
		}else if(SCHED_POLICY == SCHED_PRIORITY){	
			Task::__ready.insert_ordered(this);
		}else{
			cout << "politica nao definida" << endl;
			
		}	
	}	
	
	
	return;
}



void Task::pass_to(Task * t, State s){

	//sai a main e scheduler vai ser processado. mas nao muda o estado
	if(t->_state == Task::SCHEDULER){
		if(this->_state != Task::WAITING){
			this->_state = s;
			if(this->_state == Task::READY){
				Task::__ready.insert(this);
			
			}else if(this->_state == Task::FINISHING){
				cout << "State da tarefa main é finish" << endl;
		}	
	
	}
		//scheduler deixara de ser processado. scheduler nao é incluso na lista
	}else if(this->_state == Task::SCHEDULER){
		t->_state = RUNNING;
		
	}else{
		this->_state = s;
		if(this->_state == Task::READY){
			Task::__ready.insert(this); 
			
		}else if(this->_state == Task::FINISHING){ 
			
		}else{
			cout << "Status S desconhecido.  s:" << s << endl;
			}
		t->_state = RUNNING;
	}
		Task::__running = t;
		swapcontext(&this->_context,&t->_context);		
		
/*	
	if(this->_state != Task::SCHEDULER){
		
		this->_state = s;
	 	tExec->_state = RUNNING;					//atualização da tarefa em execução	
	}
	if(this->_state == Task::FINISHING){
		
	}
	
	if(this->_state == Task::READY){
		Task::__ready.insert(this);
	}
	
	Task::__running = tExec; 						//atualização da tarefa em execução
*/
	return;
}

void Task::exit(int code){
	State aux=FINISHING;
	Task::_count--;

	this->pass_to((Task*)__main, aux);
	
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
