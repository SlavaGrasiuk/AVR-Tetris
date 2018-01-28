/********************************************************************
	Created:	2015/09/09   19:43:36
	Filename: 	GameConsole2.1\PlatformDrv\LCD\fonts.cpp

	Author:		SlavaGrasiuk

	purpose:	Simply font engine for lcd
*********************************************************************/

#include "fonts.hpp"
#include <string.h>
#include <avr/pgmspace.h>

void lcd::PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color8 color, const color8 bgcolor) {
	unsigned char i, x0;
	char c;
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void lcd::PutStr(const Coord_t y, const char *str, const font &fnt, const color8 color, const color8 bgcolor) {
	unsigned char i, x, x0;
	char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (lcd::Width() >> 1) - ((i*fnt.char_w) >> 1);
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void lcd::PutStrRAM(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color8 color, const color8 bgcolor) {
	unsigned char i, x0;
	char c;
	for (i = 0, x0 = 0, c = str[i++]; c; fnt.print8(x + x0, y, c, color, bgcolor), c = str[i++], x0 += fnt.char_w);
}

void lcd::PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color8 color) {
	unsigned char i, x0;
	char c;
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void lcd::PutStr(const Coord_t y, const char *str, const font &fnt, const color8 color) {
	unsigned char i, x, x0;
	char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (lcd::Width() >> 1) - ((i*fnt.char_w) >> 1);
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void lcd::PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color16 color, const color16 bgcolor) {
	unsigned char i, x0;
	char c;
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void lcd::PutStr(const Coord_t y, const char *str, const font &fnt, const color16 color, const color16 bgcolor) {
	unsigned char i, x, x0;
	char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (Width() >> 1) - ((i*fnt.char_w) >> 1);
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void lcd::PutStr(const Coord_t y, const char *str, const font &fnt, const color16 color) {
	unsigned char i, x, x0;
	char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (Width() >> 1) - ((i*fnt.char_w) >> 1);
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void lcd::PutStr(const Coord_t x, const Coord_t y, const char *str, const font &fnt, const color16 color) {
	unsigned char i, x0;
	char c;
	for (i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}
