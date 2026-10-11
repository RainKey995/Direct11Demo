#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#include "D3DClass.h"
#include "InputClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "RenderTextureClass.h"
#include "DisplayPlaneClass.h"

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
    bool RenderSceneToTexture(float rotation);

private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    ModelClass* m_Model;
    TextureShaderClass* m_TextureShader;
    RenderTextureClass* m_RenderTexture;
    DisplayPlaneClass* m_DisplayPlane;
};

#endif // !_APPLICATIONCLASS_H_
