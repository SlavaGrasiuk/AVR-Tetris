/********************************************************************
	Created:	2015/10/16   0:20:28
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Picture.h

	Author:		SlavaGrasiuk
	
	purpose:	Picture class
*********************************************************************/


#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "simplegraphobj.hpp"

struct image{
	Size_t w;
	Size_t h;
	color16 *img;
};

class picture: public SimpleGraphObj {
	Coord_t x, y;
	image *bitmap;
	
public:
	picture(const Coord_t x_pos, const Coord_t y_pos, const image &img);
	void redraw();
	void SetBitMap(const image &img);
}; //Picture

#endif //__PICTURE_H__
