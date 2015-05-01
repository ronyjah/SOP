/*
 * BOOOS.h
 *
 *  Created on: Aug 14, 2014
 *      Author: arliones
 */
//#include <iostream>
#include "Queue.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
namespace BOOOS {
// Queue constructor: must initialize queue's attributes
Queue::Queue(){
	this->_length=0;
	this->_head.rank(0);
}

// Queue destructor: must finalize queue's attributes
Queue::~Queue() {    
}

// insert method: must insert the Element at the end of the queue
// Error messages:
//   01: invalid Element (if elem == 0)
//   02: duplicated Element (if elem already is on the queue)

void Queue::insert_ordered(Element* elem){
    Queue::Element * ant;
	Queue::Element * prox;

	//caso de erro 01.
	if (elem == 0) {
		cerr << "01: invalid Element" << endl;
		return;
	}

	//quando adicionar um elemento com a lista vazia.
	if (this->length() == 0) {
		return insert(elem);
	}
	for (ant = this->head()->next(); ant->next() != this->head()->next(); ant =
			ant->next()) {
		if (ant == elem) {
			cerr << "02: duplicated Element" << endl;
			return;
		}
	}

	if (this->length() == 1) {
		if (elem->rank() < this->_head.next()->rank()) {
			elem->next(this->_head.next());
			elem->prev(this->_head.next());
			this->head()->next()->next(elem);
			this->head()->next()->prev(elem);
			this->head()->next(elem);
			this->head()->prev(this->head()->next()->prev());
			this->_length++;
			return;
		} else {
			return insert(elem);
		}
	}
	int c;
	for (prox = this->_head.next(), c = 0; prox != this->_head.next() || c == 0;
			prox = prox->next(), c++) {
		if (elem->rank() < prox->rank()) {
			if (c == 0) {
				this->_head.next(elem);
			}
			elem->next(prox);
			prox->prev()->next(elem);
			elem->prev(prox->prev());
			prox->prev(elem);
			this->_length++;
			return;
		}
	}
	return insert(elem);
}

void Queue::insert(Queue::Element * elem) {
       if(elem == 0){     
        return;
    }
    if(this->length() == 0){
        this->head()->next(elem);
        this->head()->prev(elem);
        elem->prev(elem); //talvez desnecessario.
        elem->next(elem);

    }else{		
        this->head()->prev()->next(elem);
        elem->prev(this->head()->prev());
        elem->next(this->head()->next());
        this->head()->prev(elem);        
        this->head()->next()->prev(elem);    
    }
    this->_length = this->_length + 1;
}
// remove: must search and remove the element from the queue
// Error messages:
//   01: queue is empty
Queue::Element * Queue::remove() {
    Element * e;
    
    if((this->length()) == 0){
        return 0; //queue empty
    }else if((this->length()) != 1){
        
        e = this->head()->next();
        this->head()->next(this->head()->next()->next());
        this->head()->prev()->next(this->head()->next());
        this->head()->next()->prev(this->head()->prev());
		_length = _length - 1;
        return e;
    }else{ // apenas um elemento na fila
        e = this->head()->next();
        this->head()->next(0);
        this->head()->prev(0);
		_length = _length - 1;    
        return e;
    }    
}

Queue::Element * Queue::search(Element * e) {
    Element * vassoura;
    vassoura = this->head()->next();
    do{
        if(vassoura == e) return vassoura;
        vassoura = vassoura->next();
    }while(vassoura != this->head()->prev());

}
}
