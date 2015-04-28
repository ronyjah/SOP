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
//                        void rank(int r) { _rank = r; }


// Queue constructor: must initialize queue's attributes
Queue::Queue(){
    //this->_head->_prev = 0;
this->_length=0;
this->_head.rank(0);
//this->Element::rank(0);
}

// Queue destructor: must finalize queue's attributes
Queue::~Queue() {

    
}

// insert method: must insert the Element at the end of the queue
// Error messages:
//   01: invalid Element (if elem == 0)
//   02: duplicated Element (if elem already is on the queue)

void Queue::insert_ordered(Element* elem){
	int i;
	
	if(this->length() == 0){
        this->head()->next(elem);
        this->head()->prev(elem);
        elem->prev(elem); //talvez desnecessario.
        elem->next(elem);
        elem->rank(this->_length);
    }else{
	//	Element kct = this->head()->next();
		
		//for(
		
		
		
		i = elem->rank();
	    this->head()->prev()->next(elem);

        elem->prev(this->head()->prev());
        elem->next(this->head()->next());

        this->head()->prev(elem);        
        this->head()->next()->prev(elem);
        i = i + 1;
        elem->rank(i);
	}
	
	}


void Queue::insert(Queue::Element * elem) {
  
     if(elem == 0){
//        cerr << " error null!!!" << endl;       
        return;
    }
    
    //sendo o primeiro elemento da fila
    if(this->length() == 0){
        this->head()->next(elem);
        this->head()->prev(elem);
        elem->prev(elem); //talvez desnecessario.
        elem->next(elem);
        //cout << "valor do primeiro elemento inserido: "<< elem << endl;
   //     elem->rank(this->_length);
    }else{
		
        this->head()->prev()->next(elem);

        elem->prev(this->head()->prev());
        elem->next(this->head()->next());

        this->head()->prev(elem);        
        this->head()->next()->prev(elem);   
       // cout << "valor do elemento inserido mais>: "<< elem << endl;	     
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
   //     cout << "valor do elemento removido quando mais de um>: "<< e<< endl;	
        return e;
    }else{ // apenas um elemento na fila
        e = this->head()->next();
        this->head()->next(0);
        this->head()->prev(0);
	//cout << "valor do elemento removido ultimo> : "<< e<< endl;	
    }    
    
    _length = _length - 1;

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
