/********************************************************************
	Created:	2015/10/16   0:13:00
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Layer.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Layer -	something like window in Windows or X11,
						but extremely simplified
*********************************************************************/

#include "layer.hpp"
#include "graphobj.hpp"

StaticStack<layer> layer::layer_stack;


layer::layer(): head(nullptr), Filled(false), FillColorMode(mode_8) {
	layer_stack.push(this);
}

layer::layer(const color8 color): head(nullptr), Filled(true), FillColorMode(mode_8) {
	layer_stack.push(this);
	FillColor.FillColor8Bit = color;
	Coord_t x = 0, y = 0;
	Size_t w = lcd::Width(), h = lcd::Height();
	lcd::FillRect(x, y, w, h, color);
}

layer::layer(const color16 color): head(nullptr), Filled(true), FillColorMode(mode_16) {
	layer_stack.push(this);
	FillColor.FillColor16Bit = color;
	Coord_t x = 0, y = 0;
	Size_t w = lcd::Width(), h = lcd::Height();
	lcd::FillRect(x, y, w, h, color);
}

layer::~layer() {
	layer_stack.pop();	//������� ������� ����
	layer *prev_lay = layer_stack.pick();		//�������� ��������� �� ���������� ����
	if(prev_lay != nullptr) {
		prev_lay->refrash();
	}
}

void layer::refrash() const {
	if(Filled) {		//�������� ���� ������, ���� ���������
		Coord_t x = 0, y = 0;
		Size_t w = lcd::Width(), h = lcd::Height();
		if(FillColorMode == mode_8) {
			lcd::FillRect(x, y, w, h, FillColor.FillColor8Bit);
		} else {
			lcd::FillRect(x, y, w, h, FillColor.FillColor16Bit);
		}
	}
	/*	TODO: ���� ��� ����� ��� ������� ��� ������, � ����� ����� ��� ���� ����, ��
	���������� ������� ������������ ����, ������� ��������� ��� ����� ��� �������, � ���� �������. */
	
	//�������������� ��� �������, ����������� �� ���� ����
	for(GraphObj *obj = head; obj != nullptr; obj = obj->next) {
		obj->redraw();
	}
}
