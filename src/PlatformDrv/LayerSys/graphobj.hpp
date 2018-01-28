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
	//�������� ��������� �� ����������� ������ � ������ �������� ������ �������� ����.
	//Type - ����� �� ������ ��������� ����� �����.
	GraphObj(const bool type);

	//����������� �������. ����� ���������� ������ ��� ��� �������� ���� ��� ���.
	virtual void redraw() = 0;

	//��������� ������ �����
	virtual void SetFocus() = 0;

	//������ ������ �����	
	virtual void RemoveFocus() = 0;

	//���������� ������
	virtual void ProcSignal(const signal sig) = 0;

	friend void GuiEventLoop();
	friend void GuiEventLoop_noblock(VoidFuncPtr userHook);
};

extern "C"[[noreturn]] void __cxa_pure_virtual();

#endif // GRAPHOBJ_H
