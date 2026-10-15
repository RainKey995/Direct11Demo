#include "applicationclass.h"
#include <stdio.h>

ApplicationClass::ApplicationClass()
{
    m_Direct3D = 0;
    m_Camera = 0;
    m_Model = 0;
    m_TranslateShader = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    char modelFilename[128], textureFilename[128];
    bool result;

    m_Direct3D = new D3DClass;
    if (!m_Direct3D)
    {
        return false;
    }

    result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    m_Camera = new CameraClass;
    m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
    m_Camera->Render();

    strcpy_s(modelFilename, "../Engine/data/square.txt");
    strcpy_s(textureFilename, "../Engine/data/stone01.tga");

    m_Model = new ModelClass;

    result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    m_TranslateShader = new TranslateShaderClass;

    result = m_TranslateShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the translate shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void ApplicationClass::Shutdown()
{
    if (m_TranslateShader)
    {
        m_TranslateShader->Shutdown();
        delete m_TranslateShader;
        m_TranslateShader = 0;
    }

    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }

    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = 0;
    }

    if (m_Direct3D)
    {
        m_Direct3D->Shutdown();
        delete m_Direct3D;
        m_Direct3D = 0;
    }

    return;
}

bool ApplicationClass::Frame(InputClass* input)
{
    static float textureTranslation = 0.0f;
    bool result;

    if (input->IsEscapePressed())
    {
        return false;
    }

    textureTranslation += 0.01f;
    if (textureTranslation > 1.0f)
    {
        textureTranslation -= 1.0f;
    }

    result = Render(textureTranslation);
    if (!result)
    {
        return false;
    }

    return true;
}

bool ApplicationClass::Render(float textureTranslation)
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;

    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    m_Model->Render(m_Direct3D->GetDeviceContext());

    result = m_TranslateShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), textureTranslation);
    if (!result)
    {
        return false;
    }

    m_Direct3D->EndScene();

    return true;
}