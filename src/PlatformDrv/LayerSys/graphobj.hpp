#ifndef GRAPHOBJ_H
#define GRAPHOBJ_H

#include "layer.hpp"
#include "../KeyDrv.hpp"

class GraphObj {
	GraphObj *next;
	friend class layer;
	bool CanReciveFocus;

protected:
	bool Enabled;

public:
	//Помещает указатель на графический объект в массив объектов самого верхнего слоя.
	//Type - может ли объект принимать фокус ввода.
	GraphObj(const bool type);

	//Перерисовка объекта. Будет вызываться каждый раз при удалении слоя над ним.
	virtual void redraw() = 0;

	//Установка фокуса ввода
	virtual void SetFocus() = 0;

	//Снятие фокуса ввода	
	virtual void RemoveFocus() = 0;

	//Обработать сигнал
	virtual void ProcSignal(const signal sig) = 0;

	friend void GuiEventLoop();
	friend void GuiEventLoop_noblock(VoidFuncPtr userHook);
};

extern "C"[[noreturn]] void __cxa_pure_virtual();

#endif // GRAPHOBJ_H
