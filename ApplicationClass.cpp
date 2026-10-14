#include "applicationclass.h"
#include <stdio.h>

ApplicationClass::ApplicationClass()
{
    m_Direct3D = 0;
    m_Camera = 0;
    m_Model = 0;
    m_FogShader = 0;
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
    m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
    m_Camera->Render();

    strcpy_s(modelFilename, "../Engine/data/cube.txt");
    strcpy_s(textureFilename, "../Engine/data/stone01.tga");

    m_Model = new ModelClass;

    result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    m_FogShader = new FogShaderClass;

    result = m_FogShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the fog shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void ApplicationClass::Shutdown()
{
    if (m_FogShader)
    {
        m_FogShader->Shutdown();
        delete m_FogShader;
        m_FogShader = 0;
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
    static float rotation = 0.0f;
    bool result;

    if (input->IsEscapePressed())
    {
        return false;
    }

    rotation -= 0.0174532925f * 0.25f;
    if (rotation < 0.0f)
    {
        rotation += 360.0f;
    }

    static float distance = 0.0f;
    distance += 0.01f;
    float z = -10.0f * abs(sin(distance));

    m_Camera->SetPosition(0.0f, 0.0f, z);
    m_Camera->Render();

    result = Render(rotation);
    if (!result)
    {
        return false;
    }

    return true;
}

bool ApplicationClass::Render(float rotation)
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    float fogColor, fogStart, fogEnd;
    bool result;

    fogColor = 0.5f;
    fogStart = 0.0f;
    fogEnd = 10.0f;

    m_Direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    worldMatrix = XMMatrixRotationY(rotation);

    m_Model->Render(m_Direct3D->GetDeviceContext());

    result = m_FogShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), fogStart, fogEnd);
    if (!result)
    {
        return false;
    }

    m_Direct3D->EndScene();

    return true;
}