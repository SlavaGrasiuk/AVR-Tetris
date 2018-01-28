/* 
* SoftTimer.cpp
*
* Created: 27.05.2016 8:21:32
* Author: user
*/


#include "SoftTimer.hpp"

SoftTimer *SoftTimer::head = nullptr;

void PushSignal(const signal sig);
extern SoftTimerController STCtrl;

SoftTimer::SoftTimer(const timeout_t interval, const signal alarm, const bool AutoRestart): Interval(interval), CurTimerValue(0),
Alarm(alarm), AutoRestartEnabled(AutoRestart), Running(false) {
	if( head == nullptr ) {		//Первый созданный програмный таймер запустит контроллер
		STCtrl.start();
	}
	TCritSect cs;
	next = head;
	head = this;
}

SoftTimer::~SoftTimer() {			//Первый разрушенный таймер остановит контроллер
	if( next == nullptr ) {
		STCtrl.terminate();
	}
	TCritSect cs;
	head = next;
}

void SoftTimer::run() {
	TCritSect cs;
	Running = true;
}

void SoftTimer::stop() {
	TCritSect cs;
	Running = false;
	CurTimerValue = 0;
}

void SoftTimer::pause() {
	TCritSect cs;
	Running = false;
}

void SoftTimer::SetInterval(const timeout_t interval) {
	TCritSect cs;
	if(Interval != interval) {		//Предотвращаем сброс таймера при записи такого же значения
		CurTimerValue = 0;
		Interval = interval;
	}
}

void SoftTimer::SetAlarmSignal(const signal alarm) {
	TCritSect cs;
	Alarm = alarm;
}

void SoftTimer::DisableAutoRestart() {
	TCritSect cs;
	AutoRestartEnabled = false;
}

void SoftTimer::EnableAutoRestart() {
	TCritSect cs;
	AutoRestartEnabled = true;
}

void SoftTimer::tick() {
	if(Running) {
		if(++CurTimerValue == Interval) {
			CurTimerValue = 0;
			PushSignal(Alarm);
			if(!AutoRestartEnabled) {
				Running = false;
			}
		}
	}
}
