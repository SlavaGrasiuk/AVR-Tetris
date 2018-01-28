/********************************************************************
	Created:	2015/10/16   0:21:11
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Picture.cpp

	Author:		SlavaGrasiuk

	purpose:	Picture class
*********************************************************************/


#include "Picture.hpp"
#include <avr/pgmspace.h>

picture::picture(const Coord_t x_pos, const Coord_t y_pos, const image &img) : x(x_pos), y(y_pos), bitmap(&const_cast<image&>(img)) {
	redraw();
}

void picture::redraw() {
	lcd::SetWindow(x, y, bitmap->w, bitmap->h);
	lcd::SetColorMode(mode_16);
	lcd::SetDatMode();
	for (unsigned int n = 0; n < static_cast<unsigned int>(bitmap->h * bitmap->w); n++) {
		lcd::send16(pgm_read_word(&bitmap->img[n]));
	}
	lcd::SetColorMode(mode_8);
}

void picture::SetBitMap(const image &img) {
	bitmap = &const_cast<image&>(img);
	redraw();
}
