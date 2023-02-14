#pragma once
#pragma warning (disable :4005)
#define DIRECTINPUT_VERSION 0x0800

// DirectX
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <dinput.h>
#include "TMath.h"
#include "WICTextureLoader.h"
#include "ScreenGrab.h"
#include "DirectXTex.h"
#include <dxgidebug.h>

// General
#include <iostream>
#include <windows.h>
#include <wincodec.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <string>
#include <assert.h>
#include <tchar.h>
#include <memory>
#include <wrl.h>
#include <codecvt>
#include "Constant.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace TBASIS_EX;

#pragma comment		(lib, "dxguid.lib")
#pragma comment		(lib, "dxgi.lib")
#pragma comment		(lib, "d3d11.lib")
#pragma comment		(lib, "d3dcompiler.lib")
#pragma comment		(lib, "d2d1.lib" )
#pragma comment		(lib, "dwrite.lib" )
#pragma comment		(lib, "winmm.lib")
#pragma comment     (lib, "dinput8.lib")

#pragma comment		(lib, "DirectXTK.lib")
#pragma comment		(lib, "DirectXTex.lib")
#pragma comment		(lib, "Core.lib")

//--------------------------------------------------------------------------------------
// Define
//--------------------------------------------------------------------------------------
#define EPSILON						0.001f
#define PI							3.141592654f
#define DegreeToRadian(degree)		((degree) * ((PI) / (180.0f)))
#define RadianToDegree(radian)		((radian) * ((180.0f) / (PI)))
#define SAMPLE_BUFFER_SIZE			16 
#define KEYSTATECOUNT				256
#define randstep(min, max)			(min + ((float)max - (float)min) * (rand() / (float)RAND_MAX))
#define NEAR_DISTANCE				1.0f
#define FAR_DISTANCE				10000.0f

//--------------------------------------------------------------------------------------
// Typedef
//--------------------------------------------------------------------------------------
typedef basic_string<TCHAR> T_STR;
static TCHAR g_szDataPath[] = L"../data/";

// 멀티바이트 문자집합 사용경우
	// ABC홍길동 = 9
	// [A][B][C] [0]
	// 유니코드바이트 문자집합 사용경우
	// ABC홍길동 = 12
	// [A][0][B][0][C][0] [][][][][][]
static std::wstring mtw(std::string str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(str);
}

static std::string wtm(std::wstring str)
{
	return  std::string(str.begin(), str.end());
}

//--------------------------------------------------------------------------------------
// 전역변수
//--------------------------------------------------------------------------------------
extern ID3D11Device*	g_pd3dDevice;
extern HWND				g_hWnd;
extern float			g_fSecPerFrame;
extern UINT				g_iWindowWidth;
extern UINT				g_iWindowHeight;
extern float			g_fElapseTime;
extern bool				g_bActiveApp;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct PNCT_VERTEX
{
	TVector3 p; // point
	TVector3 n; // normal
	TVector4 c; // color
	TVector2 t; // texture

	PNCT_VERTEX() {}
	PNCT_VERTEX(TVector3 v0, TVector3 n0, TVector4 c0, TVector2 t0)
	{
		p = v0;
		n = n0;
		c = c0;
		t = t0;
	}

	// == != 재정의
	bool operator ==(const PNCT_VERTEX& vertex)
	{
		if (p == vertex.p && n == vertex.n && c == vertex.c && t == vertex.t)
		{
			return true;
		}
		return false;
	}
};

struct PNCT3_VERTEX
{
	TVector3 p; // point
	TVector3 n; // normal
	TVector4 c; // color
	TVector2 t; // texture
	float    i[4];
	float	 w[4];
	//TVector4 i; // I0, I1, I2, I3
	//TVector4 w; // W0, fW1, fW2, fNumWeight

	PNCT3_VERTEX() {}
	/*PNCT3_VERTEX(TVector3 v0, TVector3 n0, TVector4 c0, TVector2 t0, int* i0, float* w0)
	{
		p = v0;
		n = n0;
		c = c0;
		t = t0;
		i = i0;
		w = w0;
	}*/

	// == != 재정의
	bool operator ==(const PNCT3_VERTEX& vertex)
	{
		if (p == vertex.p && n == vertex.n && c == vertex.c && t == vertex.t && i == vertex.i && w == vertex.w)
		{
			return true;
		}
		return false;
	}
};

struct VSCBMatrix
{
	TMatrix matWorld;		// c0 ~ c3
	TMatrix matView;		// c4 ~ c7
	TMatrix matProj;		// c8 ~ c11
	TMatrix matNormal;		// c12 ~ c15
	TVector4 vColor;		// c16
};

struct cbShadowData
{
	TMatrix g_matShadow1; // Lw * Lv * Lp	
};

struct PC_VERTEX
{
	TVector3 p;
	TVector4 c;
	bool operator ==(const PC_VERTEX& vertex)
	{
		if (p == vertex.p && c == vertex.c)
			return true;
		return false;
	}
	PC_VERTEX() {
		p = TVector3();
		c = TVector4();
	};
	PC_VERTEX(TVector3 vp, TVector4 vc)
	{
		p = vp; c = vc;
	}
};

struct RT_F :public D2D_RECT_F
{
	/*FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;*/
	RT_F() {}
	RT_F(FLOAT l, FLOAT t, FLOAT r, FLOAT b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
};

struct CollisionBox
{
	// Common
	TVector3		vCenter;
	TVector3		vPos[8];
	TVector4		vColor;
	// AABB
	TVector3		vMax;
	TVector3		vMin;
	// OBB
	TVector3		vAxis[3];
	float			fLength[3];
};

struct CollisionSphere
{
	TVector3		vCenter;
	float			fRadius;
};

// CreatePlane 함수 Frustum.cpp 정의
struct CollisionPlane
{
	float fA, fB, fC, fD;
	bool	CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2);
	bool	CreatePlane(TVector3 vNormal, TVector3 v0);
	void	Normalize()
	{
		float fMag = sqrt(fA*fA + fB*fB + fC*fC);
		fA = fA / fMag;
		fB = fB / fMag;
		fC = fC / fMag;
		fD = fD / fMag;
	}
};

#ifdef _DEBUG
static void DXCheckMemoryLeak()
{
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

	IDXGIDebug* debug;

	GetDebugInterface(IID_PPV_ARGS(&debug));

	OutputDebugStringW(L"=================                 DirectX 메모리 누수 체크                 =================\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
	OutputDebugStringW(L"================= 위의 내용은 반환되지 않은 IUnknown 객체가 있을 경우 출력 =================\r\n");

	debug->Release();
}
#endif

//--------------------------------------------------------------------------------------
// WinMain 매크로
//--------------------------------------------------------------------------------------
#define BASIS_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){ Sample gSample;
#define BASIS_WIN(x) if( !gSample.SetWindow(hInstance, L#x) )	{   return 0;	}	ShowCursor ( TRUE ); gSample.Run(); return 1; }
#define BASIS_RUN(x) BASIS_START; BASIS_WIN(x);



//--------------------------------------------------------------------------------------
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//--------------------------------------------------------------------------------------
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

//--------------------------------------------------------------------------------------
// 싱글톤
//--------------------------------------------------------------------------------------
template<class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
	static T* GetInstancePtr()
	{
		static T theSingleInstance;
		return &theSingleInstance;
	}
};

//--------------------------------------------------------------------------------------
// STL Delete 함수
//--------------------------------------------------------------------------------------
template<class T>
void DeleteVector(vector<T>& delData)
{
	vector<T> vClear;
	delData.swap(vClear);

	vClear.clear();
	delData.clear();
}