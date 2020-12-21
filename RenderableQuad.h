#pragma once
#include "Instance.h"
class RenderableQuad :
    public Instance
{
    MeshRenderer* MR = nullptr;
    Transform* TRS = nullptr;

    UATexture3D* QuadTex = nullptr;

    ComputeObject CO;

public:
    RenderableQuad();
    ~RenderableQuad();

    virtual void Init() override;
    virtual void Update(float Delta) override;
    virtual void Render(float Delta) override;
    virtual void Release() override;
};

