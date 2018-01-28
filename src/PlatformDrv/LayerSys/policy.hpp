/********************************************************************
	Created:	2015/10/16   16:35:20
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\policy.h

	Author:		SlavaGrasiuk
	
	purpose:	policy library for graphic objects
*********************************************************************/


#ifndef POLICY_H_
#define POLICY_H_


#include "../LCD/fonts.hpp"

struct TextInFlash {
	static void print(const Coord_t x, const Coord_t y, const char * const txt, const font &fnt, const color8 col, const color8 bgcol) {
		lcd::PutStr(x, y, txt, fnt, col, bgcol);
	}
};

struct TextInRAM {
	static void print(const Coord_t x, const Coord_t y, const char * const txt, const font &fnt, const color8 col, const color8 bgcol) {
		lcd::PutStrRAM(x, y, txt, fnt, col, bgcol);
	}
};


#endif /* POLICY_H_ */