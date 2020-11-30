#pragma once

using namespace std;

template<typename _Ty>
inline size_t make_hash(_Ty Data)
{
	std::hash<_Ty> h;
	return h(Data);
}


// Debug info

enum DEBUG_FLAG
{
	L_NORMAL,
	L_WARNING,
	L_ERROR,
};

inline void AssertCritical(string Message, HRESULT Result)
{
	USES_CONVERSION;

	MessageBox(nullptr, A2W((Message + string(" => " + Result)).c_str()) , 0, 0);

	exit(-1);
}

inline void ResultLog(HRESULT Result, string Message)
{
#ifdef DEBUG

	int Color = 0;

	switch (Result)
	{
		case S_OK:
			Color = 0 + 2 * 16; // WHITE BACK, BLACK FORE
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
			cout << "S_OK => " << Message << endl;
			break;

		case E_INVALIDARG:
			Color = 8 + 14 * 16; // YELLOW BACK, LIGHTGRAY FORE
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
			cout << "E_INVALIDARG => " << Message << endl;
			break;

		case E_FAIL:
			Color = 0 + 4 * 16; // RED BACK, WHITE FORE
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
			cout << "E_FAIL => " << Message << endl;
			break;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 + 0 * 16);

#endif
}

inline void DebugLog(DEBUG_FLAG Flag, string Message)
{
#ifdef DEBUG

	int Color = 0;

	switch (Flag)
	{
		case DEBUG_FLAG::L_NORMAL:
			Color = 0+ 2 * 16; // WHITE BACK, BLACK FORE
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
			cout << "L_NORMAL => " << Message << endl;
			break;

		case DEBUG_FLAG::L_WARNING:
			Color = 8 + 14 * 16; // YELLOW BACK, LIGHTGRAY FORE
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
			cout << "L_WARNING => "<<Message << endl;
			break;

		case DEBUG_FLAG::L_ERROR:
			Color = 0 + 4 * 16; // RED BACK, WHITE FORE
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
			cout << "L_ERROR => " << Message << endl;
			break;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 + 0 * 16);


#endif
}