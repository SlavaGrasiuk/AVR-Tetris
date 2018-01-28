/********************************************************************
	Created:	2014/12/27   13:10:37
	Filename: 	GameConsole2.1\PlatformDrv\KeyDrv.h

	Author:		SlavaGrasiuk
	
	purpose:	Keyboard connection options
*********************************************************************/

#ifndef KEYDRV_H_
#define KEYDRV_H_

/**************************************************************************\
|**				НАСТРОЙКА ПОДКЛЮЧЕНИЯ КЛАВИАТУРЫ И ЭНКОДЕРА				 **|
\**************************************************************************/

#define MKEY_PORT_NAME			C			//Буква порта
#define MKEY_X1					0			//Контакт строки 1
#define MKEY_X2					2			//Контакт строки 2
#define MKEY_X3					5			//Контакт строки 3
#define MKEY_X4					4			//Контакт строки 4
#define MKEY_Y1					1			//Контакт столбца 1
#define MKEY_Y2					3			//Контакт столбца 2
#define MKEY_Y3					6			//Контакт столбца 3

#define ENC_PORT_NAME			D			//Буква порта
#define ENC_BUT					6			//Кнопка энкодера
#define ENC_A					3			//Первый контакт энкодера
#define ENC_B					1			//Второй контакт энкодера

/**************************************************************************/

#define PRECOMP2(x, y)			y ## x			
#define PRECOMP(x, y)			PRECOMP2(x, y)

#define ENC_PORT				PRECOMP(ENC_PORT_NAME, PORT)
#define ENC_DDR					PRECOMP(ENC_PORT_NAME, DDR)
#define ENC_PIN					PRECOMP(ENC_PORT_NAME, PIN)

#define MKEY_PORT				PRECOMP(MKEY_PORT_NAME, PORT)
#define MKEY_DDR				PRECOMP(MKEY_PORT_NAME, DDR)
#define MKEY_PIN				PRECOMP(MKEY_PORT_NAME, PIN)

enum class signal: unsigned char {
	none,				//Отсутствие сигналов
	EncRightStep,		//Сигналы энкодера
	EncLeftStep,
	
	BtnEncDown,			//Нажатие кнопки энкодера
	Btn1Down,			//Нажатие клавиш на матричной клавиатуре
	Btn2Down,
	Btn3Down,
	Btn4Down,
	Btn5Down,
	Btn6Down,
	Btn7Down,
	Btn8Down,
	Btn9Down,
	Btn0Down,
	BtnDelDown,
	BtnEntDown,
	
	BtnEncUp,			//Отпускание кнопки энкодера
	Btn1Up,				//Отпускание клавиш матричной клавиатуре
	Btn2Up,
	Btn3Up,
	Btn4Up,
	Btn5Up,
	Btn6Up,
	Btn7Up,
	Btn8Up,
	Btn9Up,
	Btn0Up,
	BtnDelUp,
	BtnEntUp,
	
	BreakGuiLoop,		//Особый сигнал для остановки цикла обработки событий gui
	
	//Сигналы для использования програмными таймерами
	UsrDef1,
	UsrDef2,
	UsrDef3,
	UsrDef4,
	UsrDef5,
	UsrDef6,
	UsrDef7,
	UsrDef8,
	UsrDef9,
	UsrDef10,
	UsrDef11,
	UsrDef12,
	UsrDef13,
	UsrDef14,
	UsrDef15,
	UsrDef16
};

#endif /* KEYDRV_H_ */