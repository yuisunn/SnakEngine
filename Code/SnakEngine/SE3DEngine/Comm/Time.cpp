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
    QueryPerformanceCounter(&m_PerformanceCount);//���Ƿ��ض�ʱ����ǰ����ֵ

}
float Timer::Stop()
{

	m_eState = Timer::STOPPED;

	LARGE_INTEGER pcount;
	QueryPerformanceCounter(&pcount);//���Ƿ��ض�ʱ����ǰ����ֵ����

	LARGE_INTEGER counter_frequency;
	QueryPerformanceCounter(&counter_frequency);//����Ӳ��֧�ֵĸ߾��ȼ�������Ƶ�ʡ�


    LARGE_INTEGER count_diff;
	count_diff.QuadPart = pcount.QuadPart - m_PerformanceCount.QuadPart;
	float Timer_diff = (float)count_diff.QuadPart / (float)counter_frequency.QuadPart;
    
	m_fTimer += Timer_diff;
    return m_fTimer;
}

