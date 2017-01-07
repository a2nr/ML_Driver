/*
 * error.h
 *
 *  Created on: Jan 7, 2017
 *      Author: mori3rti
 */

#ifndef ERROR_H_
#define ERROR_H_


class error {
private:
public:
	error();
	virtual ~error();
	void attachError( void (*someFnc) (void));
	void raiseError();
};

static error Error;

#endif /* ERROR_H_ */
