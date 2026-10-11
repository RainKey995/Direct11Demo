#pragma once

#ifndef _DISPLAYPLANECLASS_H_
#define _DISPLAYPLANECLASS_H_

#include "D3DClass.h"

class DisplayPlaneClass
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    DisplayPlaneClass();
    DisplayPlaneClass(const DisplayPlaneClass& other);
    ~DisplayPlaneClass();

    bool Initialize(ID3D11Device* device, float width, float height);
    void Shutdown();
    void Render(ID3D11DeviceContext* deviceContext);

    int GetIndexCount();

private:
    bool InitializeBuffers(ID3D11Device* device, float width, float height);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount;
};

#endif // !_DISPLAYPLANECLASS_H_
