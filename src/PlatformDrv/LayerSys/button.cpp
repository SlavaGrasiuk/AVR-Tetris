/********************************************************************
	Created:	2015/09/30   9:22:31
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\button.cpp

	Author:		SlavaGrasiuk

	purpose:	Button class
*********************************************************************/

#include <avr/pgmspace.h>
#include "button.hpp"

button::button(const Coord_t x, const Coord_t y, const Size_t w, const Size_t h) :
	x_pos(x), y_pos(y), w_val(w), h_val(h),
	handler(nullptr), text(nullptr), have_focus(false) {
	ButColor = 0xDB;
	redraw();
}

void button::DrawText() {
	unsigned char i;
	Coord_t X, Y;
	for (i = 0; pgm_read_byte(&text[i]); i++);		//Определение длины строки
	if (Enabled) {
		X = x_pos + (w_val >> 1) - ((i*fnt->char_w) >> 1);
		Y = y_pos + (h_val >> 1) - (fnt->char_h >> 1);
		lcd::PutStr(X, Y, text, *fnt, (color8)0x00, ButColor);
	} else {
		X = x_pos + (w_val >> 1) - ((i*fnt->char_w) >> 1) + 1;
		Y = y_pos + (h_val >> 1) - (fnt->char_h >> 1) + 1;
		lcd::PutStr(X, Y, text, *fnt, (color8)0xFF, ButColor);
		X = x_pos + (w_val >> 1) - ((i*fnt->char_w) >> 1);
		Y = y_pos + (h_val >> 1) - (fnt->char_h >> 1);
		lcd::PutStr(X, Y, text, *fnt, (color8)0x92);
	}
}

void button::redraw() {
	Coord_t X = x_pos + 1, Y = y_pos + 1;
	Size_t W = w_val - 2, H = h_val - 2;
	lcd::FillRect(X, Y, W, H, ButColor);
	X = x_pos; Y = y_pos; W = w_val; H = 1;
	lcd::FillRect(X, Y, W, H, (color8)0xFF);
	Y = y_pos + 1; W = 1; H = h_val - 1;
	lcd::FillRect(X, Y, W, H, (color8)0xFF);
	X = x_pos + w_val - 1; Y = y_pos + 1;
	lcd::FillRect(X, Y, W, H, (color8)0x92);
	X = x_pos + 1; Y = y_pos + h_val - 1; W = w_val - 2; H = 1;
	lcd::FillRect(X, Y, W, H, (color8)0x92);
	X = x_pos + w_val; Y = y_pos; W = 1; H = h_val + 1;
	lcd::FillRect(X, Y, W, H, (color8)0x49);
	X = x_pos; Y = y_pos + h_val; W = w_val; H = 1;
	lcd::FillRect(X, Y, W, H, (color8)0x49);
	if (text) {
		DrawText();
	}
	if (have_focus) {
		SetFocus();
	}
}

void button::SetText(const char * const string, const font &TextFnt) {
	text = string;
	fnt = &TextFnt;
	DrawText();
}

void button::SetFocus() {
	Coord_t x = x_pos + 4, y = y_pos + 4;
	Size_t w = w_val - 7, h = h_val - 7;
	lcd::SetWindow(x, y, w, 1);
	lcd::SetDatMode();
	for (unsigned char i = 0; i < w; i++) {
		lcd::send8(ButColor);
		lcd::send8(0x49);
	}
	lcd::SetWindow(x, y, 1, h);
	lcd::SetDatMode();
	for (unsigned char i = 0; i < h; i++) {
		lcd::send8(ButColor);
		lcd::send8(0x49);
	}
	lcd::SetWindow(x + w - 1, y, 1, h);
	lcd::SetDatMode();
	for (unsigned char i = 0; i < h; i++) {
		lcd::send8(ButColor);
		lcd::send8(0x49);
	}
	lcd::SetWindow(x, y + h - 1, w, 1);
	lcd::SetDatMode();
	for (unsigned char i = 0; i < w; i++) {
		lcd::send8(ButColor);
		lcd::send8(0x49);
	}
	lcd::SetPixel(x, y, (color8)0x49);
	lcd::SetPixel(x + w - 1, y, (color8)0x49);
	lcd::SetPixel(x, y + h - 1, (color8)0x49);
	lcd::SetPixel(x + w - 1, y + h - 1, (color8)0x49);

	have_focus = true;
}

void button::RemoveFocus() {
	Coord_t X = x_pos + 4, Y = y_pos + 4;
	Size_t W = w_val - 7, H = h_val - 7;
	lcd::Rect(X, Y, W, H, ButColor);
	have_focus = false;
	if (Pressed) {		//Если кнопка была нажата, и с неё сняли фокус ввода, необходимо эмулировать отпускание кнопки
		ProcSignal(signal::BtnEncUp);
	}
}

void button::ProcSignal(const signal sig) {
	Coord_t X, Y;
	Size_t W, H;

	switch (sig) {
		default:
			break;

		case signal::BtnEncUp:
		case signal::BtnEntUp:
			if (Pressed) {
				X = x_pos; Y = y_pos; W = w_val; H = 1;
				lcd::FillRect(X, Y, W, H, (color8)0xFF);
				Y = y_pos + 1; W = 1; H = h_val - 1;
				lcd::FillRect(X, Y, W, H, (color8)0xFF);
				X = x_pos + w_val - 1; Y = y_pos + 1;
				lcd::FillRect(X, Y, W, H, (color8)0x92);
				X = x_pos + 1; Y = y_pos + h_val - 1; W = w_val - 2; H = 1;
				lcd::FillRect(X, Y, W, H, (color8)0x92);
				X = x_pos + 1; Y = y_pos + 1; W = 1; H = h_val - 1;
				lcd::FillRect(X, Y, W, H, ButColor);
				X = x_pos + 2; Y = y_pos + 1; W = w_val - 2; H = 1;
				lcd::FillRect(X, Y, W, H, ButColor);
				X = x_pos + w_val; Y = y_pos; W = 1; H = h_val + 1;
				lcd::FillRect(X, Y, W, H, (color8)0x49);
				X = x_pos; Y = y_pos + h_val; W = w_val; H = 1;
				lcd::FillRect(X, Y, W, H, (color8)0x49);
				if (handler != nullptr && have_focus) {
					handler();
				}
				Pressed = false;
			}
			break;

		case signal::BtnEncDown:
		case signal::BtnEntDown:
			X = x_pos; Y = y_pos; W = w_val; H = 1;
			lcd::FillRect(X, Y, W, H, (color8)0x49);
			Y = y_pos + 1; W = 1; H = h_val - 1;
			lcd::FillRect(X, Y, W, H, (color8)0x49);
			X = x_pos + w_val - 1; Y = y_pos + 1;
			lcd::FillRect(X, Y, W, H, ButColor);
			X = x_pos + 1; Y = y_pos + h_val - 1; W = w_val - 2; H = 1;
			lcd::FillRect(X, Y, W, H, ButColor);
			X = x_pos + 1; Y = y_pos + 1; W = 1; H = h_val - 1;
			lcd::FillRect(X, Y, W, H, (color8)0x92);
			X = x_pos + 2; Y = y_pos + 1; W = w_val - 2; H = 1;
			lcd::FillRect(X, Y, W, H, (color8)0x92);
			X = x_pos + w_val; Y = y_pos; W = 1; H = h_val + 1;
			lcd::FillRect(X, Y, W, H, (color8)0xFF);
			X = x_pos; Y = y_pos + h_val; W = w_val; H = 1;
			lcd::FillRect(X, Y, W, H, (color8)0xFF);
			Pressed = true;
			break;
	}
}

void button::Enable() {
	if (text != nullptr) {
		DrawText();
	}
}

void button::Disable() {
	if (text != nullptr) {
		DrawText();
	}
}
