/********************************************************************
	Created:	2015/09/29   21:38:10
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\button.hpp

	Author:		SlavaGrasiuk

	purpose:	Button class
*********************************************************************/

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "activegraphobj.hpp"
#include "../LCD/fonts.hpp"

class button : public ActiveGraphObj {
	Coord_t x_pos, y_pos;
	Size_t w_val, h_val;
	void(*handler)();
	const char *text;
	const font *fnt;
	bool Pressed, have_focus;
	color8 ButColor;

public:
	void redraw() override;
	void SetFocus() override;
	void RemoveFocus() override;
	void ProcSignal(const signal sig) override;

	void Enable();
	void Disable();

	button(const Coord_t x, const Coord_t y, const Size_t w, const Size_t h);

	Size_t width() const {
		return w_val;
	}

	Size_t height() const {
		return h_val;
	}

	void SetText(const char * const string, const font &TextFnt);

	void SetHandler(void(*HandlerPtr)()) {
		handler = HandlerPtr;
	}

private:
	void DrawText();
}; //button

#endif //__BUTTON_H__
