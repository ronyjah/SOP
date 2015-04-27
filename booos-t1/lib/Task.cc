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
Queue Task::__ready;
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
	Task::__ready.insert(this);
	}	
	return;
	
}



void Task::pass_to(Task * t, State s){
//cout << "Valor de  s: " << s << endl;
	//sai a main e scheduler vai ser processado. mas nao muda o estado
	if(t->_state == Task::SCHEDULER){
//		cout << "ERRO.  s:" << s << endl;
		Task::__running = t;
		this->_state = s;
		if(this->_state == Task::READY){
//			cout << "ERRO1.  s:" << s << endl;	
			Task::__ready.insert(this);
			swapcontext(&this->_context,&t->_context);
			return;
		}else if(this->_state == Task::FINISHING){
	//		cout << "ERRO2.  s:" << s << endl;
			swapcontext(&this->_context,&t->_context);
			return;
		}
		
		//scheduler deixara de ser processado. scheduler nao é incluso na lista
	}else if(this->_state == Task::SCHEDULER){
//		cout << "ERRO4.  s:" << s << endl;
		Task::__running = t;
		t->_state = RUNNING;
		swapcontext(&this->_context,&t->_context);
		return;
	}else{
//		 cout << "ERRO5.  s:" << s << endl;
		//nao se trata de scheduler
		this->_state = s;
		if(this->_state == Task::READY){
			Task::__ready.insert(this);
			Task::__running = t; 
			t->_state = RUNNING;
			swapcontext(&this->_context,&t->_context);
			return;
		}else if(this->_state == Task::FINISHING){
			Task::__running = t; 
			t->_state = RUNNING;
			swapcontext(&this->_context,&t->_context);
			return;
//		cout << "finisnh de  s: " << s << endl;
		}else{
			
			}
		
		}
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
//cout << "ERRO11" << endl;	
	this->pass_to((Task*)__main, aux);
//cout << "ERRO12" << endl;		
}

void Task::nice(int n){
	if(n > -21 and n < 21){
		rank(n);
	}else{
		cerr << "Erro. Prioridade: "<< n << endl;	
	}
}

void Task::yield(){
	//cout << "----=====EXECUTANDO=====----" << endl;
	//cout << "Tid da tarefa em execução>: "<< this->_tid << endl;	
	
	this->pass_to(Scheduler::self());
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
