#include "stdafx.h"
#include "Main.h"

bool Main::Init()
{
	return true;
}

bool Main::Frame()
{
	if (m_ParticleSystem.size() != 0)
	{
		for (int iObj = 0; iObj < m_ParticleSystem.size(); iObj++)
		{
			m_ParticleSystem[iObj].Frame(m_pImmediateContext);
		}
	}
	return true;
}

bool Main::Render()
{
	if (m_ParticleSystem.size() != NULL)
	{
		// 샘플러 스테이트 적용
		DxState::ApplySS(m_pImmediateContext, DxState::g_pSSLinear, 1);

		DxState::ApplyDSS(m_pImmediateContext, DxState::g_pDSSDisable);
		for (int iObj = 0; iObj < m_ParticleSystem.size(); iObj++)
		{
			if (m_ParticleSystem[iObj].m_bAlphaBlending == TRUE)
			{
				m_pImmediateContext->OMSetBlendState(m_ParticleSystem[iObj].m_pAlphaBlend.Get(), 0, -1);
			}
			if (m_ParticleSystem[iObj].m_bAlphaTesting == TRUE)
			{
				m_pImmediateContext->OMSetBlendState(m_ParticleSystem[iObj].m_pAlphaTesting.Get(), 0, -1);
			}

			m_ParticleSystem[iObj].Render(m_pImmediateContext);
		}
		m_pImmediateContext->OMSetBlendState(0, 0, -1);
	}

	return true;
}

bool Main::Release()
{
	if (m_ParticleSystem.size() != 0)
	{
		for (int iDel = 0; iDel < m_ParticleSystem.size(); iDel++)
		{
			m_ParticleSystem[iDel].Release();
		}
		DeleteVector(m_ParticleSystem);
	}

	return true;
}

Main::Main()
{
	m_iParticleSystemCount	= 0;
}


Main::~Main()
{
}
