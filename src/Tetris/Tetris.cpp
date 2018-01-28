/********************************************************************
	Created:	2015/09/18   15:32:39
	Filename: 	GameConsole2.1\Tetris\Tetris.cpp

	Author:		SlavaGrasiuk

	purpose:	Tetris game
*********************************************************************/

#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <PlatformDrv.hpp>
#include "TetrisBoard.hpp"
#include "logo.h"
#include "FigInd.hpp"
#include <stdlib.h>

#define MENU_BG_COLOR		static_cast<color8>(0xDB)		//Серый

static const char vBtnStartNoLoadText[]	PROGMEM = "Старт";
static const char vBtnStartLoadText[]	PROGMEM = "Загрузить";
static const char vBtnSettingsText[]	PROGMEM = "Настройки";
static const char PauseText[]			PROGMEM = "Пауза";
static const char YesText[]				PROGMEM = "Да";
static const char NoText[]				PROGMEM = "Нет";
static const char ExitText[]			PROGMEM = "Выйти?";
static const char OverWriteText[]		PROGMEM = "Перезаписать?";
static const char DeleteSave[]			PROGMEM = "Очистить?";
static const char LoadQ[]				PROGMEM = "Загрузить?";

static const char high[]	PROGMEM = "HIGH:";
static const char score[]	PROGMEM = "SCORE:";
static const char next[]	PROGMEM = "NEXT:";

static unsigned int EEMEM HighScore = 0;		//Рекорд хранится в eeprom
static unsigned char EEMEM HaveSavedGame = 0;	//Есть ли сохранённая игра

static unsigned int EEMEM ee_cur_score = 0;
static unsigned char EEMEM ee_cur_figure = 0;
static unsigned char EEMEM ee_next_figure = 0;

unsigned char EEMEM BacklightTimeoutEnabled = 0xDA;
unsigned char EEMEM Brigitness = 180;
unsigned char EEMEM BacklightTimeout = 30;

static void Game(bool needLoad);
static void Settings();
static void pause();
static bool YesNoDialog(const char * const text);
static void GetTextScore(char *str, unsigned int num);

static button *vBtnStartLoadPtr;

void RTOSMain() {
	lcd::SetColorMode(mode_8);

	layer menu_lay(MENU_BG_COLOR);

	picture tetris_logo(0, 0, logo);
	button vBtnSettings(15, 145, lcd::Width() - 30, 23);
	button vBtnStartLoad(15, 117, lcd::Width() - 30, 23);
	button vBtnStartNoLoad(10, 67, lcd::Width() - 20, 40);

	vBtnStartNoLoad.SetText(vBtnStartNoLoadText, fnt::font8x14);
	vBtnStartNoLoad.SetHandler([] { Game(false); });

	if (eeprom_read_byte(&HaveSavedGame) != 0xDA) {
		vBtnStartLoad.disable();
	}
	vBtnStartLoad.SetText(vBtnStartLoadText, fnt::font8x14);
	vBtnStartLoad.SetHandler([] { Game(true); });
	vBtnStartLoadPtr = &vBtnStartLoad;

	vBtnSettings.disable();	// Временно
	vBtnSettings.SetText(vBtnSettingsText, fnt::font8x14);
	vBtnSettings.SetHandler(Settings);

	GuiEventLoop();
}

static void Game(bool needLoad) {
	unsigned int cur_score = 0, high_score = eeprom_read_word(&HighScore);
	char high_score_buf[] = "000000", cur_score_buf[] = "000000";
	unsigned char cur_figure, next_figure, cur_lines;
	bool wasLoaded, FallingCycle = true;
	const signal FigureStepDownSignal = signal::UsrDef1;

	srand(TCNT0);		//Используем счётчик таймера управления подсветкой и несогласованность тактового генератора с действиями человека как неплохой источник энтропии

	SoftTimer FigureStepDownTimer(20, FigureStepDownSignal, true);

	layer game_lay(GAME_BG_COLOR);

	label<TextInFlash> high_lab(lcd::Width() - 40, 25);
	label<TextInFlash> score_lab(lcd::Width() - 40, 3);

	label<TextInRAM> high_score_lab(lcd::Width() - 41, 36);
	label<TextInRAM> cur_score_lab(lcd::Width() - 41, 14);

	label<TextInFlash> next_lab(lcd::Width() - 38, 68);

	TetrisBoard game_board(1, 1);

	FigInd next_fig_ind(90, 80);

	high_lab.SetText(high, fnt::font6x8, 0xFF, GAME_BG_COLOR);
	score_lab.SetText(score, fnt::font6x8, 0xFF, GAME_BG_COLOR);
	next_lab.SetText(next, fnt::font6x8, 0xFF, GAME_BG_COLOR);

	GetTextScore(high_score_buf, high_score);
	high_score_lab.SetText(high_score_buf, fnt::font6x8, 0xFF, GAME_BG_COLOR);

	if (needLoad) {
		game_board.Load();
		game_board.redraw();
		next_figure = eeprom_read_byte(&ee_next_figure);
		next_fig_ind.SetFigure(next_figure);
		next_fig_ind.redraw();
		cur_figure = eeprom_read_byte(&ee_cur_figure);
		cur_score = eeprom_read_word(&ee_cur_score);
		wasLoaded = true;
	} else {
		wasLoaded = false;
		next_figure = rand() % 7;
	}

	GetTextScore(cur_score_buf, cur_score);
	cur_score_lab.SetText(cur_score_buf, fnt::font6x8, 0xFF, GAME_BG_COLOR);

	FigureStepDownTimer.run();

	//Главный цикл игры
	while (1) {
		if (!wasLoaded) {
			cur_figure = next_figure;	//Получаем номер новой фигурки
			next_figure = rand() % 7;	//Генерируем номер следующей фигурки
			next_fig_ind.SetFigure(next_figure);	//Загружаем следующую фигурку в индикатор
			if (game_board.insert(cur_figure)) {		//Добавяем на игровую доску новую фигурку
				break;	//Если нельзя добавить - конец игры
			}
		} else {
			wasLoaded = false;
		}

		//Не нужен бытрый спуск для новой фигурки
		FigureStepDownTimer.SetInterval(20);

		//Цикл падения фигурки
		while (FallingCycle) {
			switch (WaitSignal()) {			//До прихода сигнала процесс блокируется
				case FigureStepDownSignal:
					if (game_board.CanStepDown()) {
						game_board.StepDown();
					} else {
						FallingCycle = false;
					}
					break;

				case signal::EncRightStep:
				case signal::Btn4Down:
					game_board.MoveLeft();
					break;

				case signal::EncLeftStep:
				case signal::Btn6Down:
					game_board.MoveRight();
					break;

				case signal::BtnEncUp:
				case signal::Btn5Up:
					game_board.Rotate();
					break;

				case signal::Btn8Down:
					FigureStepDownTimer.SetInterval(2);
					break;

				case signal::Btn8Up:
					FigureStepDownTimer.SetInterval(20);
					break;

				case signal::Btn3Up:
					FigureStepDownTimer.pause();
					pause();
					FigureStepDownTimer.SetInterval(20);		//Если вдруг вызвали паузу во время быстрого падения
					FigureStepDownTimer.run();
					break;

				case signal::BtnDelUp:
					FigureStepDownTimer.pause();
					if (eeprom_read_byte(&HaveSavedGame) && YesNoDialog(DeleteSave)) {
						eeprom_write_byte(&HaveSavedGame, 0);
						vBtnStartLoadPtr->disable(false);
					}
					FigureStepDownTimer.SetInterval(20);
					FigureStepDownTimer.run();
					break;

				case signal::Btn1Up:
					FigureStepDownTimer.pause();
					if (YesNoDialog(ExitText)) {
						return;
					}
					//На всякий случай, лучше после закрытия диалогового окна ещё раз нажать вниз
					FigureStepDownTimer.SetInterval(20);
					FigureStepDownTimer.run();
					break;

				case signal::Btn2Up:
					FigureStepDownTimer.pause();
					if (eeprom_read_byte(&HaveSavedGame) == 0xDA) {
						if (YesNoDialog(OverWriteText)) {
							game_board.Save();
							eeprom_write_word(&ee_cur_score, cur_score);
							eeprom_write_byte(&ee_cur_figure, cur_figure);
							eeprom_write_byte(&ee_next_figure, next_figure);
						}
					} else {
						game_board.Save();
						eeprom_write_byte(&HaveSavedGame, 0xDA);	//Помечаем, что есть сохранение
						eeprom_write_word(&ee_cur_score, cur_score);
						eeprom_write_byte(&ee_cur_figure, cur_figure);
						eeprom_write_byte(&ee_next_figure, next_figure);
						vBtnStartLoadPtr->enable(false);
					}
					FigureStepDownTimer.SetInterval(20);
					FigureStepDownTimer.run();
					break;

				default:
					break;
			}
		}
		cur_lines = game_board.FindAndDeleteLines();
		cur_score += 1 + cur_lines * 5;	//5 очков за каждый заполненный ряд, 1 очко за поставленную фигурку
		if (cur_score > high_score) {
			high_score = cur_score;
			eeprom_write_word(&HighScore, high_score);
			GetTextScore(high_score_buf, high_score);
			high_score_lab.SetText(high_score_buf, fnt::font6x8, 0xFF, GAME_BG_COLOR);
		}
		GetTextScore(cur_score_buf, cur_score);
		cur_score_lab.SetText(cur_score_buf, fnt::font6x8, 0xFF, GAME_BG_COLOR);
		FallingCycle = true;
	}
	FigureStepDownTimer.stop();
	game_board.GameOverEffect();
}

static void Settings() {
	layer settings_lay(MENU_BG_COLOR);
	//Это меню пока недоступно
	WaitSignal();
}

static void GetTextScore(char *str, unsigned int num) {
	for (unsigned char i = 5; i > 0; i--) {
		str[i] = 0x30 + num % 10;
		num /= 10;
	}
}

static void pause() {
	layer pause_lay;
	lcd::FillRect(20, (lcd::Height() >> 1) - 15, lcd::Width() - 40, 30, static_cast<color8>(0b00000011));
	label<TextInFlash> pause_lab((lcd::Width() >> 1) - 20, (lcd::Height() >> 1) - 7);

	pause_lab.SetText(PauseText, fnt::font8x14, 0xFF, 0b00000011);
	while (1) {
		if (WaitSignal() == signal::Btn3Up) {
			break;
		}
	}
}

static bool ret;		//Захватить лямбдой внутри YesNoDialog выёдет слишком дорого

static bool YesNoDialog(const char * const text) {
	layer dialog_lay;
	lcd::FillRect(10, (lcd::Height() >> 1) - 30, lcd::Width() - 20, 60, MENU_BG_COLOR);
	button yes_but(20, (lcd::Height() >> 1), 40, 23);
	button no_but(72, (lcd::Height() >> 1), 40, 23);	//Фокус ввода устанавливается на последней созданной кнопке

	yes_but.SetText(YesText, fnt::font8x14);
	yes_but.SetHandler([] { ret = true; BreakGuiLoop(); });
	no_but.SetText(NoText, fnt::font8x14);
	no_but.SetHandler([] { ret = false; BreakGuiLoop(); });

	lcd::PutStr((lcd::Height() >> 1) - 22, text, fnt::font8x14, 0x00, MENU_BG_COLOR);

	GuiEventLoop();

	return ret;
}
