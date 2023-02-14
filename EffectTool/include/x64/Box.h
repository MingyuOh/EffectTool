#pragma once
#include "Shape.h"

class Box :public Shape
{
public:
	//--------------------------------------------------------------------------------------
	// 정점 버퍼 생성
	//--------------------------------------------------------------------------------------
	bool			CreateVertexData() override;
	HRESULT			CreateVertexBuffer() override;
	//--------------------------------------------------------------------------------------
	// 인덱스 버퍼 생성
	//--------------------------------------------------------------------------------------
	bool			CreateIndexData() override;
	HRESULT			CreateIndexBuffer() override;
public:
	Box();
	virtual ~Box();
};

