/*
 * SoftTimerController.cpp
 *
 * Created: 27.05.2016 9:45:06
 *  Author: user
 */ 

#include "softtimer.hpp"

SoftTimerController STCtrl;

template<> void SoftTimerController::exec() {
	while(1) {
		for(SoftTimer *CurTimer = SoftTimer::head; CurTimer != nullptr; CurTimer = CurTimer->next) {
			CurTimer->tick();
		}
		sleep(50);
	}
}
