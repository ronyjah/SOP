#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "Scheduler.h"


using namespace std;
namespace BOOOS
{
Scheduler * Scheduler::__dispatcher;

Scheduler::Scheduler() : Task(dispatcher, 1,(void*)"SCHEDULER"){
this->_state = SCHEDULER;
this->_tid=1;
//cout << "Endereço Scheduler" << this << endl;
}

Scheduler::~Scheduler() {

}

void Scheduler::init() {
	if(__dispatcher != 0) {
				//cout << "Erro criação da pilha" << endl;
		return;
	}else{
	__dispatcher =  new Scheduler();
	}
}

Task * Scheduler::choose_next(){//tenho que dizer qual subclasse quer acessar o membro protected. Como vou acessar um membro protected sem dizer quem quer acessar.
	
		//cout << "Tid da tarefa a entrar>: "<< __ready.remove() << endl;	
//cout << "ERRO7" << endl;
		return (Task*)__ready.remove();
		//cout << "Tid da tarefa a entrar>: "<< t << endl;	

}

void Scheduler::dispatcher(void*d){

	while(Task::_count > 2){
		Task * t = __dispatcher->choose_next(); //c	
		__dispatcher->pass_to(t);
	}	
}
}
