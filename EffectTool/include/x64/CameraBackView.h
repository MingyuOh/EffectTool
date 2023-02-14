#pragma once
#include "Camera.h"

class CameraBackView :public Camera
{
	bool m_bStartCameraPocus;
public:
	bool			Frame();
	bool			Update();
	bool			UpdateQuaternion();
public:
	CameraBackView();
	virtual ~CameraBackView();
};

