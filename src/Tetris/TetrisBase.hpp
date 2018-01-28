/********************************************************************
	Created:	2015/11/06   13:51:23
	Filename: 	GameConsole2.1\Tetris\TetrisBase.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Base operations for tetris objects
*********************************************************************/


#ifndef __TETRISBASE_H__
#define __TETRISBASE_H__

#include <PlatformDrv.hpp>

#define GAME_BG_COLOR		static_cast<color8>(0x1B)		//Светло-голубой
#define BOARD_EMPTY_BLOCK_COLOR					0x1A	//Слегка темнее фона игры

class TetrisBase {
protected:
	Coord_t x, y;
	
	//Создавать объекты этого класса нельзя
	TetrisBase(const Coord_t x_pos, const Coord_t y_pos): x(x_pos), y(y_pos) {}
	
	//Рисует блок, принимая экранные координаты
	void DrawBlock(const Coord_t x, const Coord_t y, const color8 col) const;

	//Возвращают экранные координаты
	Coord_t GetScreenCoordX(const unsigned char block_x) const;
	Coord_t GetScreenCoordY(const unsigned char block_y, const unsigned char s = 0) const;
	
	//Загружает в buf фигуру figure в ориентации figure_orient
	void GetFigure(signed char buf[3][2], const unsigned char figure, const unsigned char figure_orient) const;
	
	//Возвращает цвет фигуры figure
	color8 GetColor(const unsigned char figure) const;
}; //TetrisBase

#endif //__TETRISBASE_H__
