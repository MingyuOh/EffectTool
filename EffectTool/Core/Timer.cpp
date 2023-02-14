#include "Timer.h"
float g_fSecPerFrame = 0.0f;
float g_fElapseTime = 0.0f;

Timer::Timer()
{
	m_iFramePerSecond	= 0;
	m_iFPSElapse		= 0;
	m_fSecPerFrame		= 0;
}


Timer::~Timer()
{
}

bool Timer::Init()
{
	// 시스템의 주파수 변동폭을 얻어 온다. 이는 시간의 표준을 잡기 위해서 사용한다.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	QueryPerformanceCounter(&m_Frame);

	if (m_Frame.QuadPart == 0)
		return false;

	m_FPS				= m_Frame;
	m_iFramePerSecond	= 0;
	m_iFPSElapse		= 0;
	return true;
}

bool Timer::Frame()
{
	// 1프레임의 시간 체크(현재 시간을 찾음)
	QueryPerformanceCounter(&m_Current); // 시간이 누적되어 계속 넘어오는 함수
	m_fSecPerFrame = static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart) / static_cast<float> (m_Frequency.QuadPart);
	g_fSecPerFrame = m_fSecPerFrame;
	g_fElapseTime += m_fSecPerFrame;
	if (((m_Current.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{
		m_iFramePerSecond	= m_iFPSElapse;
		m_FPS				= m_Current;
		m_iFPSElapse = 0;
	}
	m_iFPSElapse++;

	m_Frame = m_Current;
	return true;
}

bool Timer::Render()
{
	return true;
}

bool Timer::Release()
{
	return true;
}

//--------------------------------------------------------------------------------------
// 경과한 시간에 대한 지원 함수
//--------------------------------------------------------------------------------------
void Timer::Reset()
{

}

void Timer::Start()
{

}

void Timer::Stop()
{

}

bool Timer::IsStarted() const
{
	return true;
}

//--------------------------------------------------------------------------------------
// Get함수
//--------------------------------------------------------------------------------------
float	Timer::GetElapsedTime()
{
	return g_fElapseTime;
}

int	Timer::GetFPS()
{
	return m_iFramePerSecond; 
}

float Timer::GetSPF()
{
	return m_fSecPerFrame;
}