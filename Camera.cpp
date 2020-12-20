#include"stdafx.h"
#include"Camera.h"

void Camera::Translation(XMVECTOR Vector, bool isLocal)
{
	auto Origin = GetView();

	Origin *= XMMatrixTranspose(XMMatrixTranslationFromVector(Vector));

	

	XMStoreFloat4x4(&TRS, Origin);
}

//void Camera::Rotate(XMVECTOR Vector)
//{}

void Camera::SetScale(XMVECTOR Vector)
{}

void Camera::SetScale(float x, float y, float z)
{}

void Camera::SetPosition(float x, float y, float z)
{}

void Camera::SetPosition(XMVECTOR Vector)
{}

void Camera::Init()
{
	D3D11_BUFFER_DESC Desc{};
	D3D11_SUBRESOURCE_DATA Sub{};

	auto Device = D3DHardware::GetInstance().GetDevice();

	auto V = XMMatrixLookAtLH(XMVectorSet(-40.0f, 0.0f, 0.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	auto P = XMMatrixPerspectiveFovLH(XMConvertToRadians(50.0f), 1.333f, 0.01f, 1000.0f);

	XMStoreFloat4(&Position, XMVectorSet(-40.0f, 0.0f, 0.0f, 1.0f));

	SetView(XMMatrixTranspose(V));
	SetProjection(XMMatrixTranspose(P));

	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.ByteWidth = sizeof(CameraInfo);
	Desc.Usage = D3D11_USAGE_DEFAULT;

	CameraInfo Info;
	
	XMStoreFloat4x4(&Info.View, XMMatrixTranspose(V));
	XMStoreFloat4x4(&Info.Projection, XMMatrixTranspose(P));
	XMStoreFloat4(&Info.ViewPosition, GetViewPosition());

	Sub.pSysMem = &Info;

	auto Result = Device->CreateBuffer(&Desc, &Sub, GetBuffer().GetAddressOf());
	ResultLog(Result, "Creating camera buffer");



	auto RootScene = Root->GetScene();
	RootScene->SetMainCamera(this);

}

void Camera::Update()
{
	static auto Context = D3DHardware::GetInstance().GetContext();
	static CameraInfo Info;

	XMStoreFloat4(&Position, GetViewPosition());

	Info.View = TRS;
	Info.ViewPosition = Position;

	

	XMStoreFloat4x4(&Info.Projection, GetProjection());
	XMStoreFloat4(&Info.ViewPosition, GetViewPosition());

	Context->UpdateSubresource(CameraBuffer.Get(), 0, nullptr, &Info, 0, 0);

}

void Camera::Release()
{}
