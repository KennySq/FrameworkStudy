#pragma once

struct Timer
{
private:

	double Begin = 0.0, End = 0.0;
	double Diff= 0.0;

	

public:
	Timer()
	{
		Begin = clock();
		End = clock();
		Diff = End - Begin;
	}

	inline void Start() { Begin = clock(); }
	inline void Stop() { End = clock(); Diff = End - Begin; Begin = End; }

	inline double GetDiff() { return Diff; }
	inline double GetDelta() { return min(Diff, (1 / 144.0f)); }
};