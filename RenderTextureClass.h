#pragma once

#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class RenderTextureClass
{
public:
    RenderTextureClass();
    RenderTextureClass(const RenderTextureClass& other);
    ~RenderTextureClass();

    bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear, int format);
    void Shutdown();

    void SetRenderTarget(ID3D11DeviceContext* deviceContext);
    void ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha);
    ID3D11ShaderResourceView* GetShaderResourceView();

    void GetProjectionMatrix(XMMATRIX& projectionMatrix);
    void GetOrthoMatrix(XMMATRIX& orthoMatrix);

    int GetTextureWidth();
    int GetTextureHeight();

private:
    int m_textureWidth, m_textureHeight;
    ID3D11Texture2D* m_renderTargetTexture;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11ShaderResourceView* m_shaderResourceView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilView* m_depthStencilView;
    D3D11_VIEWPORT m_viewport;
    XMMATRIX m_projectionMatrix;
    XMMATRIX m_orthoMatrix;
};

#endif // !_RENDERTEXTURECLASS_H_
