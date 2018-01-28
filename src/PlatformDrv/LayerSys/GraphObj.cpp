/********************************************************************
	Created:	2015/10/16   0:04:58
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\GraphObj.cpp

	Author:		SlavaGrasiuk

	purpose:	Base class for all graphic objects
*********************************************************************/

#include "graphobj.hpp"

//��� AVR ��� ����������� ���������� C++ � ABI
extern "C" void __cxa_pure_virtual() {
	//������: ����� ����� ����������� �������
	while (1);
}

GraphObj::GraphObj(const bool type) : CanReciveFocus(type), Enabled(true) {
	if (!layer::layer_stack.IsEmpty()) {	//���� �� ����, ������ ���� �� ���� ����.
		layer *top_lay = layer::layer_stack.pick();
		next = top_lay->head;
		top_lay->head = this;
	} else {
		//������: ������ ������ �� ����, ��� ��� ������ ������ ����
	}
}
