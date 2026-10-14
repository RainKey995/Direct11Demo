#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#include "D3DClass.h"
#include "InputClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "FogShaderClass.h"

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
    bool Render(float rotation);

private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    ModelClass* m_Model;
    FogShaderClass* m_FogShader;
};

#endif // !_APPLICATIONCLASS_H_
