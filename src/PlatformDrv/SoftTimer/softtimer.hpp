/* 
* SoftTimer.h
*
* Created: 27.05.2016 8:21:33
* Author: user
*/


#ifndef __SOFTTIMER_H__
#define __SOFTTIMER_H__

#include "../scmRTOS/core/scmRTOS.h"
#include "../KeyDrv.hpp"
#include "../PlatformDrv.hpp"

class SoftTimer {
	timeout_t Interval, CurTimerValue;
	signal Alarm;
	bool AutoRestartEnabled, Running;
	SoftTimer *next;
	static SoftTimer *head;

public:
	SoftTimer(const timeout_t interval, const signal alarm, const bool AutoRestart = false);
	~SoftTimer();
	void run();
	void stop();
	void pause();
	void SetInterval(const timeout_t interval);
	void SetAlarmSignal(const signal alarm);
	void DisableAutoRestart();
	void EnableAutoRestart();
	
private:
	void tick();
	
	friend class OS::process<OS::pr2, 50>;
}; //SoftTimer

#endif //__SOFTTIMER_H__
