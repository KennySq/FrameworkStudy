#pragma once

using namespace std;
using namespace Microsoft::WRL;

class InputManager
{
	ComPtr<IDirectInput8W> Dinput;
	ComPtr<IDirectInputDevice8W> Keyboard;
	ComPtr<IDirectInputDevice8W> Mouse;

	unsigned char KeyboardStates[256];
	unsigned char KeyBuffer;

	DIMOUSESTATE MouseState;
	int MouseX, MouseY;

	
	int Width, Height;

	unordered_map<unsigned char,function<void()>> KeyboardTasks;
	vector<pair<int, function<void(int DeltaX, int DeltaY)>>> MouseTasks;
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	bool IsButtonPressed(int Index);
	bool IsPressed(unsigned char Key);

#ifdef DEBUG
	unsigned char IsPressedDebug(unsigned char Key);
#endif

public:
	InputManager();
	~InputManager();
	
	bool Init(HINSTANCE hInstance, HWND hWnd, unsigned int Width, unsigned int Height);
	void Release();
	bool Frame();
	
	void GetMousePosition(int&, int&);

	inline void AddTask(unsigned char Key, function<void()> Task)
	{	
		if (Task == nullptr)
		{
			DebugLog(L_ERROR, "Failed to add task.");
			return;
		}

		KeyboardTasks.insert_or_assign(Key, Task);

		return;
	}

	inline void AddMouseTask(unsigned char Button, function<void(int DX, int DY)> Task)
	{
		if (Task == nullptr)
		{
			DebugLog(L_ERROR, "Failed to add task.");
			return;
		}

		MouseTasks.emplace_back(Button, Task);

		return;
	}
};