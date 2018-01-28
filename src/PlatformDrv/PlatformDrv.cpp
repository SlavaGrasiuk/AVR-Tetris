/********************************************************************
	Created:	2015/08/04  18:21:54
	Filename: 	GameConsole2.1\PlatformDrv\PlatformDrv.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Definition of os processes and keyboard driver
*********************************************************************/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "PlatformDrv.hpp"

EncController EncCtrl;
KeybController KeybCtrl;
TUserProc UserProc;

//Канал для передачи сигналов, ёмкость - 64 сигнала
static OS::channel<signal, 64> SignalQueue;

//Флаг нажатия клавиши для подсветки дисплея
OS::TEventFlag KeyEventFlag;

//Таблица выходов КА для обработки событий энкодера
//4 возможных состояния и 4 возможных входных сигнала
static const signal EncTransitTable[4][4] PROGMEM = {
	{signal::none, signal::EncRightStep, signal::EncLeftStep, signal::none},
	
	{signal::EncLeftStep, signal::none, signal::none, signal::EncRightStep},
	
	{signal::EncRightStep, signal::none, signal::none, signal::EncLeftStep},
	
	{signal::none, signal::EncLeftStep, signal::EncRightStep, signal::none}
};

//Таблица масок строк матричной клавиатуры
//#define _BV(x)	(1<<x) 
static const unsigned char MKeyMasks[4] PROGMEM = {
	_BV(MKEY_X1), _BV(MKEY_X2), _BV(MKEY_X3), _BV(MKEY_X4)
};

//Таблица масок столбцов матричной клавиатуры
static const unsigned char MKeyMasks_Y[3] PROGMEM = {
	_BV(MKEY_Y1), _BV(MKEY_Y2), _BV(MKEY_Y3)
};

//Таблица выходных сигналов отпускания клавиш
static const signal MKeySignalsUp[3][4] PROGMEM = {
	{signal::Btn1Up, signal::Btn4Up, signal::Btn7Up, signal::BtnDelUp},
	
	{signal::Btn2Up, signal::Btn5Up, signal::Btn8Up, signal::Btn0Up},
	
	{signal::Btn3Up, signal::Btn6Up, signal::Btn9Up, signal::BtnEntUp}
};

//Таблица выходных сигналов нажатия клавиш
static const signal MKeySignalsDown[3][4] PROGMEM = {
	{signal::Btn1Down, signal::Btn4Down, signal::Btn7Down, signal::BtnDelDown},
	
	{signal::Btn2Down, signal::Btn5Down, signal::Btn8Down, signal::Btn0Down},
	
	{signal::Btn3Down, signal::Btn6Down, signal::Btn9Down, signal::BtnEntDown}
};

void OS::idle_process_user_hook() {
	__asm("sleep");
}

int main(void) {
	ACSR |= _BV(ACD);		//Отключаем аналоговый компаратор для экономии энергии
	MCUCR |= _BV(SE);		//Настраиваем спящий режим. Выбираем режим idle - отключение тактирования CPU и flash
	TIMER2_OC_REG = 0xF9;	//Значение сравнения для системного таймера.
	TIMER2_CS_REG = _BV(WGM21)|_BV(CS21)|_BV(CS20);		//Режим CTC и деление входной частоты на 32
	TIMER2_IE_REG |= _BV(OCIE2);		//Разрешаем прерывание по совпадению системного таймера. Интервал срабатывания - 0.0005 сек.
	OS::run();		//Запуск операционной системы
	return 0;		//Сюда никогда не вернёмся
}

signal GetSignal() {
	signal ret;
	if(!SignalQueue.get_count()) {
		return signal::none;
	}
	SignalQueue.pop(ret);
	return ret;
}

signal WaitSignal(timeout_t timeout) {
	signal ret;
	SignalQueue.pop(ret, timeout);		//При чтении из пустого канала вызвавший процесс ставится на ожидание до прихода данных.
	return ret;
}

void BreakGuiLoop() {
	SignalQueue.push(signal::BreakGuiLoop);
}

void PushSignal(const signal sig) {
	SignalQueue.push(sig);
}

extern SoftTimerController STCtrl;

template<> void EncController::exec() {
	signal res;
	unsigned char EncNewSignal, EncState = 0, div4 = 0;

	while(1) {
		//Извлекаем биты, к которым подключен энкодер и помещаем в младшие разряды по порядку		
		__asm volatile(	
			"in  __tmp_reg__, %[port]"  "; Читаем порт, к которому подлючён энкодер во временный регистр"			"\n\t"
			"clr %0"					"; Обнуляем регистр, в который поместим результат"							"\n\t"
			"bst __tmp_reg__, %[enc_a]"	"; Сохраняем бит, к которому подключён контакт А энкодера во флаг T"		"\n\t"
			"bld %0, 1"					"; Копируем флаг T в первый бит регистра результата"						"\n\t"
			"bst __tmp_reg__, %[enc_b]"	"; Сохраняем бит, к которому подключён контакт B энкодера во флаг T"		"\n\t"
			"bld %0, 0"					"; Копируем флаг T в нулевой бит регистра результата"						"\n\t"
			: "=r" (EncNewSignal)
			:[port] "I" (_SFR_IO_ADDR(ENC_PIN)), [enc_a] "I" (ENC_A), [enc_b] "I" (ENC_B)
		);

		//То же самое на C	   
		//EncNewSignal = ((ENC_PIN_REG&(1<<ENC_B))>>ENC_B)|((ENC_PIN_REG&(1<<ENC_A))>>(ENC_A-1));
		
		//КА, обрабатывающий данные от энкодера.
		//Выбираем выходной сигнал из таблицы.
		res = static_cast<signal>(pgm_read_byte(&(EncTransitTable[EncNewSignal][EncState])));
		if(res != signal::none) {
			EncState = EncNewSignal;		//Переходим в новое состояние.
			if(++div4 == 4) {				//Сохраняем только каждое четвёртое событие.
				div4 = 0;
				if (!KeyEventFlag.is_signaled()) {
					KeyEventFlag.signal();	//Сигналим подсветке дисплея
				}
				SignalQueue.push(res);	
			}
		}
		
		sleep(2);
	}
}

template<> void KeybController::exec() {
	unsigned char OldRead = _BV(ENC_BUT), NewRead, tmpX, tmpY;
	char MKeyOldRead[3] = {_BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4), _BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4), _BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4) };

	while(1) {
		sleep(17);			//Каждая группа опрашивается около 30 раз в сек., 4 группы - клавиша энкодера и 3 столбца матрицы клавиш
		NewRead = ENC_PIN & _BV(ENC_BUT);
		
		//Если предыдущее состояние порта было нулевым, а текущее - единичное,
		//то произошло отпускание кнопки
		if( !OldRead && NewRead ) {
			if (!KeyEventFlag.is_signaled()) {
				KeyEventFlag.signal();
			}
			SignalQueue.push(signal::BtnEncUp);
			OldRead = NewRead;
		}
		//Если наоборот - нажатие
		if( OldRead && !NewRead ) {
			if (!KeyEventFlag.is_signaled()) {
				KeyEventFlag.signal();
			}
			SignalQueue.push(signal::BtnEncDown);
			OldRead = NewRead;
		}
	
		//Для каждого столбца матричной клавиатуры
		for(unsigned char mYi = 0; mYi < 3; mYi++) {
			tmpY = pgm_read_byte(&MKeyMasks_Y[mYi]);
			MKEY_PORT &= ~tmpY;			//Выбираем очередной столбец
			sleep(17);				//Задержка здесь, т.к. защитному диоду необходимо время для открытия
			NewRead = MKEY_PIN;
			//И для каждой клавиши на столбце выполняем такую же проверку
			for(unsigned char mXi = 0; mXi < 4; mXi++) {
				tmpX = pgm_read_byte(&MKeyMasks[mXi]);
				//Перепад с низкого уровня на высокий соответствует отпусканию
				if( !(MKeyOldRead[mYi] & tmpX) && (NewRead & tmpX) ) {	
					if (!KeyEventFlag.is_signaled()) {
						KeyEventFlag.signal();
					}
					SignalQueue.push(static_cast<signal>(pgm_read_byte(&MKeySignalsUp[mYi][mXi])));
					MKeyOldRead[mYi] |= tmpX;
					break;
				}
				if( (MKeyOldRead[mYi] & tmpX) && !(NewRead & tmpX) ) {
					if (!KeyEventFlag.is_signaled()) {
						KeyEventFlag.signal();
					}
					SignalQueue.push(static_cast<signal>(pgm_read_byte(&MKeySignalsDown[mYi][mXi])));
					MKeyOldRead[mYi] &= ~tmpX;
				}
			}
			MKEY_PORT |= tmpY;  //Снимаем выделение со столбца
		}
	}
}

template<> void TUserProc::exec() {
	RTOSMain();
	sleep();		//Если всё же вышли из пользовательской функции, отправляем пользовательский процесс в бесконечный сон, чтобы не поломать систему.
}

//Инициализация клавиатуры
//Вызывается автоматически до входа в main()
[[using gnu: naked, section(".init8")]] void KeyInit() {
	ENC_PORT |= _BV(ENC_A)|_BV(ENC_B)|_BV(ENC_BUT);
	ENC_DDR &= ~(_BV(ENC_A)|_BV(ENC_B)|_BV(ENC_BUT));

	MKEY_DDR &= ~(_BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4));		//Строки - входы
	MKEY_DDR |= _BV(MKEY_Y1)|_BV(MKEY_Y2)|_BV(MKEY_Y3);						//Столбцы - выходы
	MKEY_PORT |= _BV(MKEY_Y1)|_BV(MKEY_Y2)|_BV(MKEY_Y3);					//Снимаем выделение со всех столбцов
	MKEY_PORT |= _BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4);		//Pull-down для строк
}
