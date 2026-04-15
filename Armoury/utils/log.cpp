#include "pch.h"
#include "log.h"
#include <time.h>

wchar_t eLog::path[MAX_PATH];
FILE* eLog::log = nullptr;

const char* eLog::szLogLevelNames[(int)ELogLevel::Count] =
{
	"INFO",
	"WARNING",
	"ERROR"
};

void eLog::Initialize()
{
	GetModuleFileNameW(NULL, path, MAX_PATH);

	wchar_t* end = wcsrchr(path, L'\\');
	if (end)
		end[1] = L'\0';
	wcscat(path, LOGNAME);

	log = _wfopen(path, L"w");

	time_t ttime = time(0);
	fprintf(log, "Logging Started - %s", ctime(&ttime));
	fflush(log);
}

void eLog::Message(ELogLevel logLevel, const char* format, ...)
{
	char msg[2048];

	va_list args;
	va_start(args, format);
	vsnprintf(msg, sizeof(msg), format, args);
	va_end(args);

	if (log)
	{
		fprintf(log, "%s:%s | %s\n", PROJ_NAME, szLogLevelNames[(int)logLevel], msg);
		fflush(log);
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	bool bFailed = false;
	bool bShouldColourChange = false;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		WORD consoleColour;
		switch (logLevel) // set console colour accordingly
		{
			case ELogLevel::Warning:
				consoleColour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				bShouldColourChange = true;
				break;
			case ELogLevel::Error:
				consoleColour = FOREGROUND_RED | FOREGROUND_INTENSITY;
				bShouldColourChange = true;
				break;
			default:
				break;
		}

		if (bShouldColourChange)
			SetConsoleTextAttribute(hConsole, consoleColour);
	}
	else
		bFailed = true;

	printf("%s:%s | %s\n", PROJ_NAME, szLogLevelNames[(int)logLevel], msg);

	if (!bFailed && bShouldColourChange)
		SetConsoleTextAttribute(hConsole, csbi.wAttributes);
}

void eLog::Shutdown()
{
	if (log)
	{
		fclose(log);
		log = nullptr;
	}
}
