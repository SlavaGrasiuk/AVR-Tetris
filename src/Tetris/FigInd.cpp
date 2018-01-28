/********************************************************************
	Created:	2015/11/06   12:31:06
	Filename: 	GameConsole2.1\Tetris\FigInd.cpp

	Author:		SlavaGrasiuk

	purpose:	Tetris figure indicator
*********************************************************************/


#include "FigInd.hpp"

FigInd::FigInd(const Coord_t x_pos, const Coord_t y_pos) : TetrisBase(x_pos, y_pos) {
	ClearBuf();
	redraw();
}

void FigInd::redraw() {
	lcd::Rect(x, y, 37, 37, BOARD_BORDER_COLOR);
	DrawInd();
}

void FigInd::SetFigure(const unsigned char figno) {
	fig_no = figno;
	ClearBuf();
	GetFigure(fig_vect, fig_no, 0);
	color8 fig_col = GetColor(fig_no);
	ind[FIGURE_Y][FIGURE_X] = fig_col;
	for (unsigned char i = 0; i < 3; i++) {
		ind[FIGURE_Y + fig_vect[i][1]][FIGURE_X + fig_vect[i][0]] = fig_col;
	}
	DrawInd();
}

void FigInd::DrawInd() const {
	for (unsigned char i = 0; i < 4; i++) {
		for (unsigned char j = 0; j < 4; j++) {
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(i), ind[i][j]);
		}
	}
}

void FigInd::ClearBuf() {
	for (unsigned char i = 0; i < 4; i++) {
		for (unsigned char j = 0; j < 4; j++) {
			ind[i][j] = BOARD_EMPTY_BLOCK_COLOR;
		}
	}
}