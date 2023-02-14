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
	// �ý����� ���ļ� �������� ��� �´�. �̴� �ð��� ǥ���� ��� ���ؼ� ����Ѵ�.
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
	// 1�������� �ð� üũ(���� �ð��� ã��)
	QueryPerformanceCounter(&m_Current); // �ð��� �����Ǿ� ��� �Ѿ���� �Լ�
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
// ����� �ð��� ���� ���� �Լ�
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
// Get�Լ�
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