/********************************************************************
	Created:	2015/09/20   15:27:27
	Filename: 	GameConsole2.1\Tetris\TetrisBoard.cpp

	Author:		SlavaGrasiuk

	purpose:	Tetris board class
*********************************************************************/


#include "TetrisBoard.hpp"
#include <avr/eeprom.h>

TetrisBoard::TetrisBoard(const Coord_t x_pos, const Coord_t y_pos) : TetrisBase(x_pos, y_pos) {
	initDone = false;

	for (unsigned char i = 0; i < 21; i++) {
		for (unsigned char j = 0; j < 10; j++) {
			board[i][j] = BOARD_EMPTY_BLOCK_COLOR;
		}
	}
	redraw();
	initDone = true;
}

void TetrisBoard::redraw() {
	lcd::FillRect(x, y, 1, lcd::Height() - y - 1, BOARD_BORDER_COLOR);
	lcd::FillRect(x + 1, lcd::Height() - y - 1, 84, 1, BOARD_BORDER_COLOR);
	lcd::FillRect(x + 84, y, 1, lcd::Height() - y - 1, BOARD_BORDER_COLOR);
	DrawStack();
	if (initDone) {	//��� �������� ����� ����� �� ������ � ������ ������� ������ ��������� �������, ������� �������� �� �������
		UpdateFig();	//������� �� ����� ���� �� ���������� � ������, ���� �� �����, ������� ������ � ��������
	}
}

void TetrisBoard::DrawStack() {
	for (unsigned char i = 0; i < 21; i++) {
		for (unsigned char j = 0; j < 10; j++) {
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(i, 1), board[i][j]);
		}
	}
}

void TetrisBoard::GameOverEffect() {
	int const n = 21;
	int const m = 10;

	int i = 0, j, k = 0;

	while (i < n*m) {	//��� ������� �����
		k++;
		for (j = k - 1; j < m - k + 1; j++, i++) {		// ���������� �������� ��������������� �������
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(k - 1, 1), 0xFF);
			OS::sleep(20);
		}

		for (j = k; j < n - k + 1; j++, i++) {			// ���������� ������� ������������� �������
			DrawBlock(GetScreenCoordX(m - k), GetScreenCoordY(j, 1), 0xFF);
			OS::sleep(20);
		}

		for (j = m - k - 1; j >= k - 1; j--, i++) {		// ���������� ������� ��������������� �������
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(n - k, 1), 0xFF);
			OS::sleep(20);
		}

		for (j = n - k - 1; j >= k; j--, i++) {			// ���������� ������ ������������� �������
			DrawBlock(GetScreenCoordX(k - 1), GetScreenCoordY(j, 1), 0xFF);
			OS::sleep(20);
		}
	}
}

void TetrisBoard::MoveRight() {
	if (CanPlace(figure_x + 1, figure_y, figure_orient)) {
		EraseFig();
		figure_x++;
		UpdateFig();
	}
}

void TetrisBoard::MoveLeft() {
	if (CanPlace(figure_x - 1, figure_y, figure_orient)) {
		EraseFig();
		figure_x--;
		UpdateFig();
	}
}

void TetrisBoard::Rotate() {
	unsigned char tmp_orient = figure_orient;

	if (++tmp_orient == 4) {		//4 ��������� ����������
		tmp_orient = 0;
	}
	if (CanPlace(figure_x, figure_y, tmp_orient)) {
		EraseFig();
		figure_orient = tmp_orient;
		GetFigure(cur_figure, figure, figure_orient);
		UpdateFig();
	}
}

unsigned char TetrisBoard::FindAndDeleteLines() {
	unsigned char j, linesCnt = 0;
	for (signed char i = 20; i >= 0; i--) {
		for (j = 0; j < 10; j++) {
			if (board[i][j] == BOARD_EMPTY_BLOCK_COLOR) {
				break;
			}
		}
		if (j < 10) {
			continue;
		}
		// ������ � i ������ ������, ������� ����� �������
		for (j = i; j > 0; j--) {
			for (unsigned char k = 0; k < 10; k++) {
				board[j][k] = board[j - 1][k];
			}
		}
		for (unsigned char k = 0; k < 10; k++) {
			board[0][k] = BOARD_EMPTY_BLOCK_COLOR;
		}
		linesCnt++;
		i++;	//������, ������������ ������, ����� ����� ���� ���������, �������� ���� ��������� � ����
	}
	if (linesCnt) {
		DrawStack();
	}
	return linesCnt;
}

bool TetrisBoard::insert(const unsigned char new_figure) {
	figure = new_figure;
	figure_orient = 0;
	figure_x = 4;	//��������� ���������� ��� ���� �������
	figure_y = 0;	//
	cur_figure_color = GetColor(figure);
	GetFigure(cur_figure, figure, figure_orient);

	if (!CanPlace(figure_x, figure_y, figure_orient)) {
		return true;	//�������� ��������
	}
	UpdateFig();
	return false;	//��������� ���
}

bool TetrisBoard::CanStepDown() {
	if (CanPlace(figure_x, figure_y + 1, figure_orient)) {
		return true;	//������ ��� �� �����
	} else {
		board[figure_y][figure_x] = cur_figure_color;
		for (unsigned char i = 0; i < 3; i++) {		//������ ������� ���������� ����������� ���������� �������
			board[figure_y + cur_figure[i][1]][figure_x + cur_figure[i][0]] = cur_figure_color;
		}
		return false;	//������ ��� �����
	}
}

void TetrisBoard::StepDown() {
	//�������� �� �����, ����� ������� ���� ������� ����� ��������, ��� ����� ���������� ����
	EraseFig();
	figure_y++;
	UpdateFig();
}

bool TetrisBoard::CanPlace(const signed char fig_x, const unsigned char fig_y, const unsigned char fig_or) const {
	signed char tmp_fig_vect[3][2];
	//���������, �� ������ �� �� ������� ����� ��� ������� �������� �������
	if (fig_x < 0 || fig_x > 9 || fig_y > 20) {
		return false;	//fig_x ����� ��������� ������������� ��� ������, ���� �������� �������� ����� �������, ������� � ��� �� ����� ����� ����
	}
	GetFigure(tmp_fig_vect, figure, fig_or);	//������� ������ �������
	for (unsigned char i = 0; i < 3; i++) {
		//������ �������� �������, ���� �����-���� � ���� ����� �� ������� �������
		if ((fig_x + tmp_fig_vect[i][0]) < 0 || (fig_x + tmp_fig_vect[i][0]) > 9) {
			return false;
		}
		if ((fig_y + tmp_fig_vect[i][1]) < 0 || (fig_y + tmp_fig_vect[i][1]) > 20) {
			return false;
		}
		//Collision detect
		if (board[fig_y + tmp_fig_vect[i][1]][fig_x + tmp_fig_vect[i][0]] != BOARD_EMPTY_BLOCK_COLOR) {
			return false;
		}
	}
	if (board[fig_y][fig_x] != BOARD_EMPTY_BLOCK_COLOR) {	//������ ��� ������� ����� ����������� ��������, ��� ��� � �������
		return false;
	}
	return true;
}

//����� ����� ��� �������, �������� ���� � �� ��? ���� � ���, ��� ����� ������� EraseFig() ���������
void TetrisBoard::EraseFig() {
	//	TODO	������ ��������
	DrawBlock(GetScreenCoordX(figure_x), GetScreenCoordY(figure_y, 1), BOARD_EMPTY_BLOCK_COLOR);
	for (unsigned char i = 0; i < 3; i++) {
		DrawBlock(GetScreenCoordX(figure_x + cur_figure[i][0]), GetScreenCoordY(figure_y + cur_figure[i][1], 1), BOARD_EMPTY_BLOCK_COLOR);
	}
}

void TetrisBoard::UpdateFig() {
	DrawBlock(GetScreenCoordX(figure_x), GetScreenCoordY(figure_y, 1), cur_figure_color);
	for (unsigned char i = 0; i < 3; i++) {
		DrawBlock(GetScreenCoordX(figure_x + cur_figure[i][0]), GetScreenCoordY(figure_y + cur_figure[i][1], 1), cur_figure_color);
	}
}

GameSaveStruct EEMEM SaveSlot1;	//����������� ����� � EEPROM ��� ��������� ����

void TetrisBoard::Save() {
	eeprom_write_block(this, &SaveSlot1, sizeof(TetrisBoard));
}

void TetrisBoard::Load() {
	eeprom_read_block(this, &SaveSlot1, sizeof(TetrisBoard));
}