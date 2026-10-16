#include "applicationclass.h"
#include <stdio.h>

ApplicationClass::ApplicationClass()
{
    m_Direct3D = 0;
    m_Camera = 0;
    m_Model1 = 0;
    m_Model2 = 0;
    m_TextureShader = 0;
    m_TransparentShader = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    char modelFilename[128], textureFilename1[128], textureFilename2[128];
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
    strcpy_s(textureFilename1, "../Engine/data/dirt01.tga");
    strcpy_s(textureFilename2, "../Engine/data/stone01.tga");

    m_Model1 = new ModelClass;
    result = m_Model1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename1);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    m_Model2 = new ModelClass;
    result = m_Model2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename2);
    if (!result)
    {
        return false;
    }

    m_TextureShader = new TextureShaderClass;
    result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
        return false;
    }

    m_TransparentShader = new TransparentShaderClass;
    result = m_TransparentShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void ApplicationClass::Shutdown()
{
    if (m_TransparentShader)
    {
        m_TransparentShader->Shutdown();
        delete m_TransparentShader;
        m_TransparentShader = 0;
    }

    if (m_Model2)
    {
        m_Model2->Shutdown();
        delete m_Model2;
        m_Model2 = 0;
    }

    if (m_Model1)
    {
        m_Model1->Shutdown();
        delete m_Model1;
        m_Model1 = 0;
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
    bool result;

    if (input->IsEscapePressed())
    {
        return false;
    }

    result = Render();
    if (!result)
    {
        return false;
    }

    return true;
}

bool ApplicationClass::Render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    float blendAmount;
    bool result;

    blendAmount = 0.5f;

    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    static float z1 = 0.0f;
    z1 -= 0.02f;
    worldMatrix = XMMatrixTranslation(-0.25f, 0.0f, sin(z1));

    m_Model1->Render(m_Direct3D->GetDeviceContext());

    result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model1->GetTexture());
    if (!result)
    {
        return false;
    }

    static float z2 = 0.0f;
    z2 += 0.02f;
    worldMatrix = XMMatrixTranslation(0.25f, 0.0f, sin(z2));

    m_Direct3D->EnableAlphaBlending();

    m_Model2->Render(m_Direct3D->GetDeviceContext());

    result = m_TransparentShader->Render(m_Direct3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model2->GetTexture(), blendAmount);
    if (!result)
    {
        return false;
    }

    m_Direct3D->DisableAlphaBlending();

    m_Direct3D->EndScene();

    return true;
}