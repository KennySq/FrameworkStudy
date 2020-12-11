#include"stdafx.h"
#include"InputManager.h"

bool InputManager::ReadKeyboard()
{
	HRESULT Result;

	Result = Keyboard->GetDeviceState(sizeof(KeyboardStates), (void*)KeyboardStates);

#ifdef DEBUG
	

	
	//for (unsigned char i = 0; i < 256; i++)
	//{
	//	if(IsPressed(i))
	//		DebugLog(L_NORMAL, to_string(IsPressedDebug(i)));

	//}

#endif

	if (Result != S_OK)
	{
		if ((Result == DIERR_INPUTLOST) || (Result == DIERR_NOTACQUIRED))
		{
			Keyboard->Acquire();
		}
		else
		{
			
			return false;
		}
	}


	return true;
}

bool InputManager::ReadMouse()
{
	HRESULT Result;

	Result = Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&MouseState);
	if (Result != S_OK)
	{
		if ((Result == DIERR_INPUTLOST) || (Result == DIERR_NOTACQUIRED))
		{
			Mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputManager::ProcessInput()
{
	MouseX += MouseState.lX;
	MouseY += MouseState.lY;

	if (MouseX < 0) { MouseX = 0; }
	if (MouseY < 0) { MouseY = 0; }

	if (MouseX > Width) { MouseX = Width; }
	if (MouseY > Height) { MouseY = Height; }

	for (auto f : KeyboardTasks)
	{
		if(IsPressed(f.first))
			f.second();
	}

	for (auto f : MouseTasks)
	{
		if (IsButtonPressed(f.first))
			f.second(MouseState.lX, MouseState.lY);
	}


}

bool InputManager::IsButtonPressed(int Index)
{
	return MouseState.rgbButtons[Index] & 0x80;
}

bool InputManager::IsPressed(unsigned char Key)
{
	return KeyboardStates[Key] & 0x80;
}

#ifdef DEBUG

unsigned char InputManager::IsPressedDebug(unsigned char Key)
{
	if (KeyboardStates[Key] & 0x80)
		return Key;

	return NULL;
}

#endif

InputManager::InputManager()
{
	Dinput = nullptr;
	Keyboard = nullptr;
	Mouse = nullptr;
}

InputManager::~InputManager()
{}

bool InputManager::Init(HINSTANCE hInstance, HWND hWnd, unsigned int Width_, unsigned int Height_)
{
	HRESULT Result;

	Width = Width_;
	Height = Height_;

	MouseX = 0;
	MouseY = 0;

	Result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)Dinput.GetAddressOf(), NULL);
	if (Result != S_OK)
	{
		AssertCritical("Failed to create DirectInput8.", Result);
		return false;
	}

	Result = Dinput->CreateDevice(GUID_SysKeyboard, Keyboard.GetAddressOf(), nullptr);
	if (Result != S_OK)
	{
		AssertCritical("Failed to create virtual keyboard.", Result);
		return false;
	}

	Result = Keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (Result != S_OK)
	{
		AssertCritical("Failed to set keyboard data format.", Result);
		return false;
	}

	Result = Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (Result != S_OK)
	{
		AssertCritical("Failed to set the cooperative level.", Result);
		return false;
	}
	
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	Result = Keyboard->Acquire();
	if (Result != S_OK)
	{
		AssertCritical("Failed to acquire the keyboard.", Result);
		return false;
	}

	Result = Dinput->CreateDevice(GUID_SysMouse, Mouse.GetAddressOf(), nullptr);
	if (Result != S_OK)
	{
		AssertCritical("Failed to create virtual mouse.", Result);
		return false;
	}

	Result = Mouse->SetDataFormat(&c_dfDIMouse);
	if (Result != S_OK)
	{
		AssertCritical("Failed to set mouse data format.", Result);
		return false;
	}

	Result = Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (Result != S_OK)
	{
		AssertCritical("Failed to set the cooperative level.", Result);
		return false;
	}

	Result = Mouse->Acquire();
	if (Result != S_OK)
	{
		AssertCritical("Failed to acquire the mouse.", Result);
		return false;
	}

	return true;
}

void InputManager::Release()
{
	if(Dinput)
		Dinput->Release();

	if (Keyboard)
	{
		Keyboard->Unacquire();
		Keyboard->Release();
	}
	if (Mouse)
	{
		Mouse->Unacquire();
		Mouse->Release();
	}

	return;
}

bool InputManager::Frame()
{
	bool Result;

	Result = ReadKeyboard();
	if (!Result)
	{
		DebugLog(L_ERROR, "Failed to read keyboard, check InputManager.cpp");
		return false;
	}

	Result = ReadMouse();
	if (!Result)
	{
		DebugLog(L_ERROR, "Faield to read mouse, check InputManager.cpp");
		return false;
	}

	ProcessInput();


	return true;
}

void InputManager::GetMousePosition(int &MX, int &MY)
{
	MX = MouseX;
	MY = MouseY;

	return;
}
