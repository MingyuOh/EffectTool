#pragma once
#include "Sprite.h"
#include "Particle.h"
#include "Camera.h"
#include "BoundingBoxShape.h"

#define MAX_PARTICLE_COUNT 1000
#define EFFECT_EXTINCTION_DISTANCE 10.0f 

enum class EFFECT_TYPE : int 
{ 
	Straight = 0, 
	Rotate,
	Circle
};

enum class COORDINATE_TYPE : int
{ 
	RotationX = 0, 
	RotationY = 1, 
	RotationZ = 2 
};

struct Instance
{
	TMatrix	  matWorld;
	TVector4  m_vColor;
};

class ParticleSystem
{
public:
	Sprite							m_Effect;						// Real Effect(this object have plane)
	vector<Particle>				m_Particle;						// Particle Elements Infomation
	Camera*							m_pCamera;						// Camera
	TMatrix							m_matWorld;						// World Matrix
	ComPtr<ID3D11BlendState>		m_pAlphaBlend;					// Alpha Blend Device
	ComPtr<ID3D11BlendState>		m_pAlphaTesting;				// Alpha Testing Device
	BOOL							m_bAlphaBlending;				// Alpha Blend apply to Effect
	BOOL							m_bAlphaTesting;				// Alpha Testing apply to Effect

	ComPtr<ID3D11Buffer>			m_pInstanceBuffer;				// Instancing Buffer
	ComPtr<ID3D11InputLayout>		m_pLayout;						// Instancing Layout
	Instance*						m_pInstData;					// Instancing Data

	int								m_iEffectCount;					// Effect Count
	TCHAR*							m_szEffectName;					// Effect Name
	TCHAR*							m_szParticleName;				// ParticleName	(Using ParticleSystemManager)
	EFFECT_TYPE						m_EffectType;					// Effect Type
	COORDINATE_TYPE					m_CoordinateType;				// If Effect type is circle exercise, this type determine how to rotate circle effect
	BoundingBoxShape				m_BoundingBoxShape;				// Bounding Box Shape
	CollisionPlane					m_BoundingPlane[6];				// Colllision plane
	float							m_fLifeTime;					// LifeTime
	int								m_iRow;							// Matrix(Row) - Use Sprites Texture
	int								m_iCol;							// Matrix(Col) - Use Sprites Texture
	int								m_iIndex;						// Particle System Manager Index
	bool							m_bBillboard;					// Billboard Matrix apply to Effect
	bool							m_bPerspective;					// Perspective Projection apply to Effect
	bool							m_bOrthogonal;					// Orthogonal Projection Matrix apply to Effect
	bool							m_bRepeatEffect;				// Variables that distinguish repetitive effects
public:
	//--------------------------------------------------------------------------------------
	// Create Instancing Buffer Function
	//--------------------------------------------------------------------------------------
	bool			CreateInstanceBuffer();
	//--------------------------------------------------------------------------------------
	// Create Layout and Shader Function
	//--------------------------------------------------------------------------------------
	bool			LoadShaderAndInputLayout(T_STR vertexShaderName = _T(""),
											 T_STR PixelShaderName = _T(""), T_STR geometryShaderName = _T(""));
	//--------------------------------------------------------------------------------------
	// Device / Camera Set Function
	//--------------------------------------------------------------------------------------
	bool			Set(Camera* pMainCamera);
	//--------------------------------------------------------------------------------------
	// Particle Update Function
	//--------------------------------------------------------------------------------------
	void			ParticleUpdate(ID3D11DeviceContext* pContext);
	//--------------------------------------------------------------------------------------
	// Create Instancing Struct And Resize Particle
	//--------------------------------------------------------------------------------------
	bool			Create();
	//--------------------------------------------------------------------------------------
	// Create Bounding Box
	//--------------------------------------------------------------------------------------
	bool			CreateBoundingBox(TVector3 vBoundingBoxMin, TVector3 vBoundingBoxMax);
	//--------------------------------------------------------------------------------------
	// Rotate function
	//--------------------------------------------------------------------------------------
	void			Rotate(TMatrix& matRotate, float fAngle);
	//--------------------------------------------------------------------------------------
	// Circle Exercise Function
	//--------------------------------------------------------------------------------------
	TVector3		Circle(float fRadius, float fRadian);
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ڽ� ���� �Ѿ�� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			CheckEffectRangeFromBox();
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ð� üũ �Լ�
	//--------------------------------------------------------------------------------------
	void			EffectLifeTimeCheck();
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ٿ���ڽ� ���� ���� �Լ�
	//--------------------------------------------------------------------------------------
	bool			ClassifyPoint(TVector3* pv);
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ٿ���ڽ� ���� ���� ���� �� ���� �Լ�
	//--------------------------------------------------------------------------------------
	bool			ClassifyPointInInterpolate(TVector3* pv);
	//--------------------------------------------------------------------------------------
	// System Function
	//--------------------------------------------------------------------------------------
	bool			Init();
	bool			Frame(ID3D11DeviceContext* pContext);
	bool			Render(ID3D11DeviceContext* pContext);
	bool			Release();
public:
	ParticleSystem();
	~ParticleSystem();
};

