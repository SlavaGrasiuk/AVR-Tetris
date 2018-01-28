/********************************************************************
	Created:	2015/09/03   16:43:26
	Filename: 	GameConsole2.1\PlatformDrv\LCD\LcdDrv.hpp

	Author:		SlavaGrasiuk

	purpose:	LS020 dispaly low-level driver header
*********************************************************************/

#ifndef LCDDRV_H_
#define LCDDRV_H_

/************************************************************************/
/****						Подключение LCD							 ****/
/************************************************************************/
#define LCD_PORT_NAME	B
#define	LCD_RS			1
#define	LCD_RESET		2
#define	LCD_CS			4
/************************************************************************/

#include "../KeyDrv.hpp"						//Для PRECOMP()
#include "../scmRTOS/port/avr/gcc/os_target.h"	//Для INLINE

#define LCD_PORT				PRECOMP(LCD_PORT_NAME, PORT)
#define LCD_DDR					PRECOMP(LCD_PORT_NAME, DDR)

using Coord_t = unsigned char;		//Тип для экранных координат
using Size_t = unsigned char;
typedef unsigned char color8;		//8-ми битный цвет 3-3-2 R-G-B
typedef unsigned int color16;		//16-ти битный цвет 5-6-5 R-G-B

enum orient {
	or_vrt,
	or_hrz
};

enum ColorMode {
	mode_8,
	mode_16
};

namespace lcd {
	NOINLINE void send8(const unsigned char data);	//Отправляет 8 бит данных дисплею
	NOINLINE void send16(const unsigned int data);	//Отправляет 16 бит данных дисплею

	/*	При отправке комманд или данных нет необходимости тратить время на переключение сигналов RS и CS,
	это можно сделать всего один раз и получить прирост скорости. */

	void SetCmdMode();			//Установливает режим отправки комманд
	INLINE void SetDatMode() {	//Устанавливает режим отправки данных
		while (!(SPSR & _BV(SPIF)));
		LCD_PORT &= ~_BV(LCD_RS);
	}

	Size_t Width();
	Size_t Height();
	orient Orient();
	ColorMode ClrMode();

	void SetBLTimeout(const unsigned char time);
	void SetBLLevel(const unsigned char level);
	void DisableBLTimeout();
	void EnableBLTimeout();

	void SetColorMode(const ColorMode clm);		//Установливает цветовой режим	
	void SetOrient(const orient orientation);	//Установливает ориентацию дисплея

	/*Устанавливает окно вывода с координатами верхнего левого угла x, y и размерами w x h*/
	void SetWindow(const Coord_t x, const Coord_t y, const Size_t w = 1, const Size_t h = 1);

	/*Рисует НЕзакрашенный прямоугольник
		с координатами левого верхнего угла  x, y и размерами w x h цветом color*/
	void Rect(const Coord_t x, const Coord_t y, const Size_t w, const Size_t h, const color8 color);
	void Rect(const Coord_t x, const Coord_t y, const Size_t w, const Size_t h, const color16 color);

	/*Рисует закрашенный прямоугольник
		с координатами левого верхнего угла  x, y и размерами w x h цветом color*/
	void FillRect(const Coord_t x, const Coord_t y, const Size_t w, const Size_t h, const color8 color);
	void FillRect(const Coord_t x, const Coord_t y, const Size_t w, const Size_t h, const color16 color);

	/*закрашивает пиксель с координатами x, y цветом color*/
	void SetPixel(const Coord_t x, const Coord_t y, const color8 color);
	void SetPixel(const Coord_t x, const Coord_t y, const color16 color);

	/*Отрезок с координатами концов x1, y1 и x2, y2*/
	void Line(Coord_t x1, Coord_t y1, const Coord_t x2, const Coord_t y2, const color8 color);
	void Line(Coord_t x1, Coord_t y1, const Coord_t x2, const Coord_t y2, const color16 color);

	/*Окружность с координатами xcenter, ycenter и радиусом rad*/
	void Circle(const Coord_t xcenter, const Coord_t ycenter, const Size_t rad, const color8 color);
	void Circle(const Coord_t xcenter, const Coord_t ycenter, const Size_t rad, const color16 color);
}

#endif /* LCDDRV_H_ */