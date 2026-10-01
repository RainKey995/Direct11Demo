#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#include "D3DClass.h"
#include "InputClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
#include "FontClass.h"
#include "FontShaderClass.h"
#include "TextClass.h"
#include "ModelListClass.h"
#include "TimerClass.h"
#include "PositionClass.h"
#include "FrustumClass.h"
//#include "ShaderManagerClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass {
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass& other);
	~ApplicationClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame(InputClass* input);

private:
	bool Render();
	bool UpdateRenderCountString(int renderCount);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightClass* m_Light;
	LightShaderClass* m_LightShader;
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	TextClass* m_RenderCountString;
	ModelListClass* m_ModelList;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FrustumClass* m_Frustum;
	XMMATRIX m_baseViewMatrix;
};

#endif // !_APPLICATIONCLASS_H_
