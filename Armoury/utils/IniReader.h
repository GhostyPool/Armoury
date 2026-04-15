#ifndef INIREADER_H
#define INIREADER_H

class CIniReader
{
public:
	CIniReader(const char* szFileName);
	CIniReader(const wchar_t* szFileName);
	int ReadInteger(const char* szSection, const char* szKey, int iDefaultValue);
	float ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue);
	bool ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue);
	const char* ReadString(const char* szSection, const char* szKey, const char* szDefaultValue);
	const wchar_t* ReadWideString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szDefaultValue);
	void WriteInteger(const char* szSection, const char* szKey, int iValue);
	void WriteFloat(const char* szSection, const char* szKey, float fltValue);
	void WriteBoolean(const char* szSection, const char* szKey, bool bolValue);
	void WriteString(const char* szSection, const char* szKey, const char* szValue);
	void WriteWideString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szValue);

	bool HasInited() const;
private:
	char m_szFileName[260];
	wchar_t m_szFileNameW[260];
	bool m_bInited = false;
};

#endif//INIREADER_H