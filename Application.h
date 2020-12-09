#pragma once
class Application
{
private:
	static shared_ptr<Application> Inst;

	D3DHardware* Hardware = nullptr;
	ImmediateRenderer* Renderer = nullptr;
	InputManager* Input = nullptr;

	HWND WindowHandle;
	HINSTANCE WindowInstance;

	UINT Width;
	UINT Height;

	Scene* SelectedScene = nullptr;

private:


public:

	static inline Application& GetInstance(HWND hWnd = nullptr, HINSTANCE hInst = nullptr)
	{
		if (!Inst)
			Inst = make_shared<Application>(hWnd, hInst);

		return *Inst;
	}

	Application(HWND hWnd, HINSTANCE hInst) { Width = 800; Height = 600; WindowHandle = hWnd; WindowInstance = hInst; }
	~Application() {}

	UINT GetWidth() const { return Width; }
	UINT GetHeight() const { return Height; }

	void InitImGui();

	bool Init();
	void Update();
	void Render();
	void Release();


};

