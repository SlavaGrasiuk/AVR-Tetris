/********************************************************************
	Created:	2015/10/16   0:22:09
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Label.hpp

	Author:		SlavaGrasiuk

	purpose:	Text label class
*********************************************************************/


#ifndef __LABEL_H__
#define __LABEL_H__

#include "simplegraphobj.hpp"
#include "policy.hpp"

template<class TextStoragePolicy>
class label : public SimpleGraphObj, public TextStoragePolicy {
	Coord_t x, y;
	const char *text;
	font *fnt;
	color8 text_col, text_bg_col;

public:
	label(const Coord_t x_pos, const Coord_t y_pos) : x(x_pos), y(y_pos), text(NULL), fnt(NULL) {}
	void redraw();
	void SetText(const char * const txt, const font &text_fnt, const color8 col, const color8 bgcol);
}; //Label

template<class TextStoragePolicy>
void label<TextStoragePolicy>::redraw() {
	if (text) {
		TextStoragePolicy::print(x, y, text, *fnt, text_col, text_bg_col);
	}
}

template<class TextStoragePolicy>
void label<TextStoragePolicy>::SetText(const char * const txt, const font &text_fnt, const color8 col, const color8 bgcol) {
	text = const_cast<char*>(txt);
	fnt = const_cast<font*>(&text_fnt);
	text_col = col;
	text_bg_col = bgcol;
	redraw();
}

#endif //__LABEL_H__
