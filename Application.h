#pragma once
class Application
{
private:
	static shared_ptr<Application> Inst;

	D3DHardware* Hardware = nullptr;
	ImmediateRenderer* Renderer = nullptr;

	HWND WindowHandle;

	UINT Width;
	UINT Height;

	Scene* SelectedScene = nullptr;

private:


public:

	static inline Application& GetInstance(HWND hWnd = nullptr)
	{
		if (!Inst)
			Inst = make_shared<Application>(hWnd);

		return *Inst;
	}

	Application(HWND hWnd) { Width = 800; Height = 600; WindowHandle = hWnd; }
	~Application() {}

	UINT GetWidth() const { return Width; }
	UINT GetHeight() const { return Height; }

	bool Init();
	void Update();
	void Render();
	void Release();


};

