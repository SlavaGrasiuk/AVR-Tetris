/********************************************************************
	Created:	2015/09/20   15:27:27
	Filename: 	GameConsole2.1\Tetris\TetrisBoard.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris board class
*********************************************************************/

#ifndef __TETRISBOARD_H__
#define __TETRISBOARD_H__

#include <PlatformDrv.hpp>
#include "TetrisBase.hpp"

#define BOARD_BORDER_COLOR	static_cast<color8>(0x00)	//׸����

class TetrisBoard: public SimpleGraphObj, public TetrisBase {
	color8 board[21][10], cur_figure_color;
	unsigned char figure, figure_orient, figure_x, figure_y;
	signed char cur_figure[3][2];
	bool initDone;
	
public:
	TetrisBoard(const Coord_t x_pos, const Coord_t y_pos);
	TetrisBoard(const TetrisBoard&) = delete;	//���� ������ ������ ����������
	void operator=(const TetrisBoard&) = delete;	//� ���������
	void redraw();
	void DrawStack();
	void GameOverEffect();
	void MoveRight();	//����� ������� ������� �� ���� ������ ������, ���� ��� ��������
	void MoveLeft();	//����� ������� ������� �� ���� ������ �����, ���� ��� ��������
	void Rotate();	//������� �������, ���� ��� ��������
	unsigned char FindAndDeleteLines();		// ���� � ������� ����������� ������, ���������� ����������� �������� �����
	bool insert(const unsigned char new_figure);	//��������� ������� �� �����. ���������� true, ���� ������� �� ���� ���������
	bool CanStepDown();		//���������� true, ���� ����� ���� ����� ����������.
	void StepDown();	//����� ������� ���� �� ���� ������.
	void Save();
	void Load();
	
private:
	//����� �� ���������� ������� ������� � ������ ������������ � �����������
	bool CanPlace(const signed char fig_x, const unsigned char fig_y, const unsigned char fig_or) const;
	/*	!TODO	������� ����� �� ��� �������, � ������ �� �����, ������� �� ������������� ����� ��������,
	����� ����� ��������.	*/
	void EraseFig();	//������� ������� ������� � ������
	void UpdateFig();	//������������ ������� �� ������
}; //TetrisBoard

/*	������������ ��� ���������� ����. ������ ��������� ��� ���� ��������� TetrisBoard � EEPROM,
�.�. ��������� ����� ������������ �����, ����� ����� �� ������ ����������, � ������� TetrisBoard 
��� ������������ ������, �.�. ��� ����������� ������.	*/
struct GameSaveStruct {
	color8 board[21][10], cur_figure_color;
	unsigned char figure, figure_orient, figure_x, figure_y;
	signed char cur_figure[3][2];
	bool initDone;
};

#endif //__TETRISBOARD_H__
