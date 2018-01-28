/********************************************************************
	Created:	2015/09/09   15:22:31
	Filename: 	GameConsole2.1\PlatformDrv\LCD\fonts.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Simply font engine for lcd
*********************************************************************/


#ifndef FONTS_H_
#define FONTS_H_

#include "LcdDrv.hpp"

typedef void (*CharDrawFunc8)(const Coord_t x, const Coord_t y, const char c, const color8 color, const color8 bgcolor);
typedef void (*CharDrawFunc16)(const Coord_t x, const Coord_t y, const char c, const color16 color, const color16 bgcolor);
typedef void (*CharDrawFunc8BGL)(const Coord_t x, const Coord_t y, const char c, const color8 color);
typedef void (*CharDrawFunc16BGL)(const Coord_t x, const Coord_t y, const char c, const color16 color);
	
struct font {
	Size_t char_w, char_h;
	CharDrawFunc8 print8;
	CharDrawFunc8BGL print8BGL;
	CharDrawFunc16 print16;
	CharDrawFunc16BGL print16BGL;
};

namespace lcd {
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color и фоном bgcolor
	void PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color8 color, const color8 bgcolor);
	
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color без фона
	void PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color8 color);
	
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color и фоном bgcolor по центру экрана
	void PutStr(const Coord_t y, const char *str, const font &fnt, const color8 color, const color8 bgcolor);
	
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color без фона по центру экрана
	void PutStr(const Coord_t y, const char *str , const font &fnt, const color8 color);

	// Те же функции для 16-ти битного цвета ------------------------------------------------------------------
	void PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color16 color);
	void PutStr(const Coord_t y, const char *str, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStr(const Coord_t y, const char *str, const font &fnt, const color16 color);
	//---------------------------------------------------------------------------------------------------------
	
	// Те же функции для печати строки из RAM -----------------------------------------------------------------
	void PutStrRAM(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color8 color, const color8 bgcolor);
	void PutStrRAM(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color8 color);
	void PutStrRAM(const Coord_t y, const char *str, const font &fnt, const color8 color, const color8 bgcolor);
	void PutStrRAM(const Coord_t y, const char *str, const font &fnt, const color8 color);
	
	void PutStrRAM(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStrRAM(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color16 color);
	void PutStrRAM(const Coord_t y, const char *str, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStrRAM(const Coord_t y, const char *str, const font &fnt, const color16 color);
	//---------------------------------------------------------------------------------------------------------
}

namespace fnt {
	//Объявления доступных шрифтов
	extern const font font8x14;
	extern const font font6x8;
}

#endif /* FONTS_H_ */