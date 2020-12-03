#pragma once

struct DirectionalLight
{
	XMFLOAT3 Direction;
	float Intensity;
};

struct SpotLight
{
	XMFLOAT3 Position;
	float Radius;
};