#pragma once
#include <corecrt_wstdio.h>

enum class ELogLevel
{
	Info,
	Warning,
	Error,
	Count
};

class eLog {
public:
	static void Initialize();
	static void Shutdown();

	static void Message(ELogLevel logLevel, const char* format, ...);

private:
	static wchar_t path[MAX_PATH];
	static FILE* log;
	static const char* szLogLevelNames[(int)ELogLevel::Count];
};