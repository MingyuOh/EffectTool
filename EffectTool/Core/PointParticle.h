#pragma once
#include "Shape.h"
#include "Camera.h"

#define MAX_PARTICLES 30000

struct PARTICLE_VERTEX
{
	TVector3 vPos;
	TVector3 vVel;
	float fTimer;
	UINT Type;
};

struct PARTICLE_CONSTANT_BUFFER
{
	TVector4	m_vFrameGravity;

	float		m_fGlobalTime;
	float		m_fElapsedTime;
	float		m_fSecondsPerFirework;
	float		m_fNumEmber1s;
	float		m_fMaxEmber2s;
};

struct Instance
{
	TMatrix	 matWorld;
	TVector4  m_vColor;
};

class PointParticle : public Shape
{
public:
	ComPtr<ID3D11Buffer>				m_pParticleStreamTo;		// Stream Out 출력 버텍스 버퍼
	ComPtr<ID3D11Buffer>				m_pParticleDrawFrom;		// Stream Out 렌더링 버텍스 버퍼
	ComPtr<ID3D11Buffer>				m_pConstantPointBuffer;		// 상수버퍼
	ComPtr<ID3D11Texture1D>				m_pRandomTexture;			// 텍스처
	ComPtr<ID3D11GeometryShader>		m_pGSAdvanceParticlesMain;
	ComPtr<ID3D11GeometryShader>	    m_pGSStreamOutput;			// Stream Out Geometry 쉐이더 
	ComPtr<ID3D11VertexShader>			m_pVSPassThroughMain;
	ComPtr<ID3D11ShaderResourceView>	m_pRandomTexRV;
	
	Instance*							m_pInstData;
	PARTICLE_CONSTANT_BUFFER			m_cbParticleData;


	Camera*								m_pCamera;
	bool                                m_bFirst;
public:
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateConstantBuffer();
	HRESULT			LoadShaderAndInputLayout(T_STR vertexShaderName = _T(""), 
											 T_STR PixelShaderName = _T(""), T_STR geometryShaderName = _T(""));
	bool			Init();
	bool			Frame(ID3D11DeviceContext* pContext, float fElapsedTime, float fSecPerFrame);
	bool			Render(ID3D11DeviceContext* pContext);
	HRESULT			CreateResource();

	HRESULT			CreateRandomTexture();
	bool			AdvanceParticles(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime, TVector3 vGravity);
	void			SetCamera(Camera* pCamera);
public:
	PointParticle();
	virtual ~PointParticle();
};

