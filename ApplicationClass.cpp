#include "applicationclass.h"
#include <stdio.h>

ApplicationClass::ApplicationClass()
{
    m_Direct3D = 0;
    m_Camera = 0;
    m_Model = 0;
    m_Light = 0;
    m_LightShader = 0;
    m_Font = 0;
    m_FontShader = 0;
    m_RenderCountString = 0;
    m_ModelList = 0;
    m_Timer = 0;
    m_Position = 0;
    m_Frustum = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    char modelFilename[128], textureFilename1[128], renderString[32];
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
    m_Camera->GetViewMatrix(m_baseViewMatrix);

    strcpy_s(modelFilename, "../Engine/data/model.txt");
    strcpy_s(textureFilename1, "../Engine/data/stone01.tga");

    m_Model = new ModelClass;

    result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename1);
    if (!result)
    {
        return false;
    }

    m_Light = new LightClass;
    m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light->SetDirection(0.0f, 0.0f, 1.0f);

    m_LightShader = new LightShaderClass;

    result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result)
    {
        return false;
    }

    m_Font = new FontClass;

    result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
    if (!result)
    {
        return false;
    }

    m_FontShader = new FontShaderClass;

    result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
        return false;
    }

    strcpy_s(renderString, "Render Count : 0");

    m_RenderCountString = new TextClass;

    result = m_RenderCountString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, renderString, 10, 10, 1.0f, 1.0f, 1.0f);
    if (!result)
    {
        return false;
    }

    m_ModelList = new ModelListClass;
    m_ModelList->Initialize(25);

    m_Timer = new TimerClass;

    result = m_Timer->Initialize();
    if (!result)
    {
        return false;
    }

    m_Position = new PositionClass;

    m_Frustum = new FrustumClass;

    return true;
}

void ApplicationClass::Shutdown()
{
    if (m_Frustum)
    {
        delete m_Frustum;
        m_Frustum = 0;
    }

    if (m_Position)
    {
        delete m_Position;
        m_Position = 0;
    }

    if (m_Timer)
    {
        delete m_Timer;
        m_Timer = 0;
    }

    if (m_ModelList)
    {
        m_ModelList->Shutdown();
        delete m_ModelList;
        m_ModelList = 0;
    }

    if (m_RenderCountString)
    {
        m_RenderCountString->Shutdown();
        delete m_RenderCountString;
        m_RenderCountString = 0;
    }

    if (m_Font)
    {
        m_Font->Shutdown();
        delete m_Font;
        m_Font = 0;
    }

    if (m_FontShader)
    {
        m_FontShader->Shutdown();
        delete m_FontShader;
        m_FontShader = 0;
    }

    if (m_Light)
    {
        delete m_Light;
        m_Light = 0;
    }

    if (m_LightShader)
    {
        m_LightShader->Shutdown();
        delete m_LightShader;
        m_LightShader = 0;
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
    static float rotationY;
    bool result, keyDown;

    m_Timer->Frame();

    if (input->IsEscapePressed())
    {
        return false;
    }

    m_Position->SetFrameTime(m_Timer->GetTime());

    keyDown = input->IsLeftArrowPressed();
    m_Position->TurnLeft(keyDown);

    keyDown = input->IsRightArrowPressed();
    m_Position->TurnRight(keyDown);

    m_Position->GetRotation(rotationY);

    m_Camera->SetRotation(0.0f, rotationY, 0.0f);
    m_Camera->Render();

    result = Render();
    if (!result)
    {
        return false;
    }

    return true;
}

bool ApplicationClass::Render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
    float positionX, positionY, positionZ, radius;
    int modelCount, renderCount, i;
    bool renderModel, result;

    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);
    m_Direct3D->GetOrthoMatrix(orthoMatrix);

    m_Frustum->ConstructFrustum(viewMatrix, projectionMatrix, SCREEN_DEPTH);

    modelCount = m_ModelList->GetModelCount();

    renderCount = 0;

    for (i = 0; i < modelCount; i++)
    {
        m_ModelList->GetData(i, positionX, positionY, positionZ);

        radius = 1.0f;

        renderModel = m_Frustum->CheckCube(positionX, positionY, positionZ, radius);

        if (renderModel)
        {
            worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

            m_Model->Render(m_Direct3D->GetDeviceContext());

            result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
            if (!result)
            {
                return false;
            }

            renderCount++;
        }
    }

    result = UpdateRenderCountString(renderCount);
    if (!result)
    {
        return false;
    }

    // draw gui...
    m_Direct3D->TurnZBufferOff();
    m_Direct3D->EnableAlphaBlending();

    m_Direct3D->GetWorldMatrix(worldMatrix);

    m_RenderCountString->Render(m_Direct3D->GetDeviceContext());

    result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_RenderCountString->GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), m_RenderCountString->GetPixelColor());
    if (!result)
    {
        return false;
    }

    m_Direct3D->TurnZBufferOn();
    m_Direct3D->DisableAlphaBlending();

    m_Direct3D->EndScene();

    return true;
}

bool ApplicationClass::UpdateRenderCountString(int renderCount)
{
    char tempString[16], finalString[32];
    bool result;

    sprintf_s(tempString, "%d", renderCount);

    strcpy_s(finalString, "Render Count : ");
    strcat_s(finalString, tempString);

    result = m_RenderCountString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 10, 1.0f, 1.0f, 1.0f);
    if (!result)
    {
        return false;
    }

    return true;
}