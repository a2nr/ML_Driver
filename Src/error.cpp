/*
 * error.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: mori3rti
 */

#include "error.h"

void dummyFnc(void);
void (*fnc) (void);

error::error(){
	// TODO Auto-generated constructor stub
	 fnc = &dummyFnc;
}

error::~error() {
	// TODO Auto-generated destructor stub
}

void error::raiseError(){
	fnc();
}

void error::attachError(void (*someFnc)(void)){
	fnc = someFnc;
}

void dummyFnc (void){
	//nothing
	while(1);
}
