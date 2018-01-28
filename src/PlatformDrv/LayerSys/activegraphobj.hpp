/*
 * ActiveGraphObj.h
 *
 * Created: 26.05.2016 9:39:47
 *  Author: user
 */


#ifndef ACTIVEGRAPHOBJ_H_
#define ACTIVEGRAPHOBJ_H_

#include "graphobj.hpp"

 //	Базовый класс графических объектов, которые могут принимать фокус ввода
class ActiveGraphObj : public GraphObj {
public:
	ActiveGraphObj() : GraphObj(true) {}

	virtual void redraw() = 0;

	virtual void SetFocus() = 0;
	virtual void RemoveFocus() = 0;
	virtual void ProcSignal(const signal sig) = 0;

	void enable(bool Update = true) { //Включает объект
		Enabled = true;
		if (Update) {
			Enable();
		}
	}

	void disable(bool Update = true) { //Выключает объект
		Enabled = false;
		if (Update) {
			Disable();
		}
	}

protected:
	//Производят перерисовку при включении/отключении объекта
	virtual void Enable() = 0;
	virtual void Disable() = 0;
};

#endif /* ACTIVEGRAPHOBJ_H_ */