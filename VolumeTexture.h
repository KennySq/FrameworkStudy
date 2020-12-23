#pragma once
#include "Instance.h"
class VolumeTexture :
    public Instance
{
    MeshRenderer* MR = nullptr;
    Transform* TRS = nullptr;

    UATexture3D* QuadTex = nullptr;

    ComputeObject* CO;

public:
    VolumeTexture();
    ~VolumeTexture();

    virtual void Init() override;
    virtual void Update(float Delta) override;
    virtual void Render(float Delta) override;
    virtual void Release() override;
};

