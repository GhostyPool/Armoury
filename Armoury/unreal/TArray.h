#pragma once
#include "../plugin/PatternSolver.h"
#include <iostream>

template <class T> 
class TArray {
public:
	T* Data;
	int Count;
	int Max;

	TArray();
	T& Get(int id) { return this->Data[id]; };
	void Add(const T& data);

	void ResizeTo(int newMax);
};

template<class T>
inline TArray<T>::TArray()
{
	Data = nullptr;
	Count = 0;
	Max = 0;
}

template<class T>
inline void TArray<T>::Add(const T& InputData)
{
	Data[Count++] = InputData;
}

template<typename T>
void TArray<T>::ResizeTo(int newMax)
{
	if (newMax)
	{
		static uintptr_t patQuantizeSize = _pattern(PATID_FMemory_QuantizeSize);
		size_t quantizedSize = ((size_t(*)(size_t, unsigned int))patQuantizeSize)(sizeof(T) * newMax, 0);
		int elementCount = quantizedSize / sizeof(T);

		newMax = newMax > elementCount ? INT_MAX : elementCount;
	}
	if (newMax != Max)
	{
		Max = newMax;
		if (Data || newMax)
		{
			static uintptr_t patRealloc = _pattern(PATID_FMemory_Realloc);
			Data = ((T* (*)(T*, size_t, unsigned int))patRealloc)(Data, sizeof(T) * newMax, 0);
		}
	}
}