/********************************************************************
	Created:	2015/08/15 23:57:32
	Filename: 	GameConsole2.1\PlatformDrv\PlatformDrv.h

	Author:		SlavaGrasiuk
	
	purpose:	PlatformDrv main include file
*********************************************************************/


#ifndef PLATFORMDRV_H_
#define PLATFORMDRV_H_

#include "scmRTOS/core/scmRTOS.h"
#include "LayerSys/layer.hpp"
#include "LayerSys/activegraphobj.hpp"
#include "LayerSys/simplegraphobj.hpp"
#include "LayerSys/Picture.hpp"
#include "LayerSys/button.hpp"
#include "LayerSys/policy.hpp"
#include "LayerSys/Label.hpp"
#include "SoftTimer/softtimer.hpp"
#include "LCD/LcdDrv.hpp"
#include "LCD/fonts.hpp"
#include "KeyDrv.hpp"

using EncController =  OS::process<OS::pr0, 40>;		//Контроллер энкодера
using KeybController = OS::process<OS::pr1, 50>;		//Контроллер клавиатуры
using SoftTimerController = OS::process<OS::pr2, 50>;	//Контроллер програмных таймеров
using BacklightController = OS::process<OS::pr3, 45>;	//Контроллер подсветки дисплея
using TUserProc = OS::process<OS::pr4, 680>;			//Процесс, внутри которого вызывается пользовательскя функция

namespace OS {
	template<> void EncController::exec();
	template<> void KeybController::exec();
	template<> void SoftTimerController::exec();
	template<> void BacklightController::exec();
	template<> void TUserProc::exec();
}

//главная функция пользовательского процесса
void RTOSMain();

//Неблокирующая функция получения сигналов
//Если сигналов нет - возвращает сигнал none
signal GetSignal();

//Блокирующая функция получения сигналов
signal WaitSignal(timeout_t timeout = 0);

void BreakGuiLoop();

#endif /* PLATFORMDRV_H_ */