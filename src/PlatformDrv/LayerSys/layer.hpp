#ifndef LAYER_H
#define LAYER_H

#include "staticstack.hpp"
#include "..\LCD\LcdDrv.hpp"

typedef void (*VoidFuncPtr)();

class GraphObj;

void GuiEventLoop();
void GuiEventLoop_noblock(VoidFuncPtr userHook = nullptr);

class layer {
	mutable layer *prev;			//��������� �� ���������� ����
	GraphObj *head;					//��������� �� ������� ����������� �������� ����� ����
	const bool Filled;					//����� �� ���� ������?
	union {
		color8 FillColor8Bit;
		color16 FillColor16Bit;
	} FillColor;						//���� �������
	const ColorMode FillColorMode;		//����� �������� ����� ������������ ��� �������
	static StaticStack<layer> layer_stack;

	friend class GraphObj;
	friend class StaticStack<layer>;

public:
	layer();
	layer(const color8 color);
	layer(const color16 color);
	~layer();
	
	friend void GuiEventLoop();
	friend void GuiEventLoop_noblock(VoidFuncPtr userHook);
	
private:
	void refrash() const;
};

#endif // LAYER_H
