/*
 * SimpleGraphObj.h
 *
 * Created: 26.05.2016 9:11:01
 *  Author: user
 */ 


#ifndef SIMPLEGRAPHOBJ_H_
#define SIMPLEGRAPHOBJ_H_

#include "graphobj.hpp"

//	Базовый класс графических объектов, которые НЕ могут принимать фокус ввода
class SimpleGraphObj: public GraphObj {
public:
	SimpleGraphObj(): GraphObj(false) {}
	virtual void redraw() = 0;

private:
	//Достались по наследству, но не будут использованы наследниками
	void SetFocus() final {}
	void RemoveFocus() final {}
	virtual void ProcSignal(const signal) final {}
};


#endif /* SIMPLEGRAPHOBJ_H_ */