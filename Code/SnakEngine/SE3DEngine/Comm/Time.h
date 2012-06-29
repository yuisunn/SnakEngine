#ifndef _Timer_H_
#define _Timer_H_

#include <windows.h>

class Timer
{
public:
	enum TimerState
	{
		RUNNING,
		STOPPED,
	};
private:
	float m_fTimer;
	TimerState m_eState;
	LARGE_INTEGER m_PerformanceCount;//���ڱ�ʾһ64λ�з�������ֵ

public:
	Timer();
		~Timer();
		float Stop();
		void Restart();
		float Start();
};


#endif