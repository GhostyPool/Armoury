#include "pch.h"
#include "Armoury.h"

static const wchar_t* wszBadUrls[] =
{
	L"/matches/matchmaking/versus-casual/request",
	L"/matches/matchmaking/versus-ranked/request",
	L"/matches/matchmaking/koth-casual/request"
};

bool Armoury::ProcessRequest_IsDomainAllowed_Hook(void* a1, const wchar_t** URL)
{
	for (int i = 0; i < _countof(wszBadUrls); ++i)
	{
		if (wcsstr(*URL, wszBadUrls[i]))
			return false;
	}

	return oProcessRequest_IsDomainAllowed(a1, URL);
}