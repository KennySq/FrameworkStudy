#pragma once
#include "Instance.h"
class RenderableQuad :
    public Instance
{
    MeshRenderer* MR;

    RTTexture2D** GBufferRef = nullptr;
    UINT GBufferCount;
public:

    RenderableQuad();
    ~RenderableQuad();
    virtual void Init() override;
    virtual void Update(float Delta) override;
};

