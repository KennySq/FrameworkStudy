#include "stdafx.h"
#include "Instance.h"


void Instance::Init()
{


}

void Instance::Update(float Delta)
{

}

void Instance::Render(float Delta)
{}

void Instance::Release()
{}

Instance::Instance()
{
	SetName("GameObject");

	D3D11_BUFFER_DESC Desc{};
	auto Device = D3DHardware::GetInstance().GetDevice();
	auto Context = D3DHardware::GetInstance().GetContext();

	Desc.ByteWidth = sizeof(InstanceData);
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	auto Result = Device->CreateBuffer(&Desc, nullptr, InstanceBuffer.GetAddressOf());
	ResultLog(Result, "Creating instance buffer.");

}


Instance::~Instance()
{}