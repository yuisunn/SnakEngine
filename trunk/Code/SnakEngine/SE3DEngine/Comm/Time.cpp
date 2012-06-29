#include <SE3DEngine/Comm/Time.h>

Timer::Timer(void):m_fTimer(0.0f),m_eState(Timer::STOPPED)
{
	Restart();
}
Timer::~Timer()
{

}
float Timer::Start()
{
	m_eState = Timer::RUNNING;
	QueryPerformanceCounter(&m_PerformanceCount);
	return m_fTimer;
}

void Timer::Restart()
{
	m_eState = Timer::RUNNING;
	m_fTimer = 0;
    QueryPerformanceCounter(&m_PerformanceCount);//返是返回定时器当前计数值

}
float Timer::Stop()
{

	m_eState = Timer::STOPPED;

	LARGE_INTEGER pcount;
	QueryPerformanceCounter(&pcount);//返是返回定时器当前计数值。。

	LARGE_INTEGER counter_frequency;
	QueryPerformanceCounter(&counter_frequency);//返回硬件支持的高精度计数器的频率。


    LARGE_INTEGER count_diff;
	count_diff.QuadPart = pcount.QuadPart - m_PerformanceCount.QuadPart;
	float Timer_diff = (float)count_diff.QuadPart / (float)counter_frequency.QuadPart;
    
	m_fTimer += Timer_diff;
    return m_fTimer;
}

