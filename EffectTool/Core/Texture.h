#pragma once
#include "CoreSTD.h"

class Texture
{
public:
	//--------------------------------------------------------------------------------------
	// �ؽ��� �ε� �� ���ҽ����� ����
	//--------------------------------------------------------------------------------------
	vector<ID3D11ShaderResourceView*>	m_pTextureSRV;
	//--------------------------------------------------------------------------------------
	// �ؽ��� �̸�
	//--------------------------------------------------------------------------------------
	vector<T_STR>						m_pTextureNameList;
	ID3D11SamplerState*					m_pSampler;
public:
	UINT        m_iCurrentIndex = 0;
	UINT        m_iMaxIndex = 1;
	UINT        m_iIndices = 0;
	bool		LoadTexture(T_STR szFilePath, T_STR szExtension);
	bool		LoadDDS(T_STR szFilePath);
	bool		LoadTGA(T_STR szFilePath);
	bool		LoadWIC(T_STR szFilePath);
public:
	bool		Apply(ID3D11DeviceContext* pImmediateContext);
	bool		Release();
public:
	Texture();
	~Texture();
};

