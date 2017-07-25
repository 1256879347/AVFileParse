#pragma once
#include "bytes.h"
#include <windows.h>

struct RIFF_HEADER
{
	byte	szRiffID[4];		// 'R','I','F','F'
	DWORD	dwRiffSize;
	byte	szRiffFormat[4];	// 'W','A','V','E'
};

struct WAVE_FORMAT
{
	WORD	wFormatTag;
	WORD	wChannels;
	DWORD	dwSamplesPerSec;
	DWORD	dwAvgBytesPerSec;
	WORD	wBlockAlign;
	WORD	wBitsPerSample;
};

struct FMT_BLOCK
{
	byte		szFmtID[4];		// 'f','m','t',' '
	DWORD		dwFmtSize;
	WAVE_FORMAT	wavFormat;
};

struct DATA_HEADER
{
	byte		szDataID[4];		// 'd','a','t','a '
	DWORD		dwDataSize;
};

class CWAVFile
{
public:
	CWAVFile();
	~CWAVFile();

public:
	// �����ļ�,��ȡ����;
	bool LoadFile(const char* strFile);

	bool SaveFile(const char* strFile, RECT rt = { 0 });

	// ��ʾ;
	void Display();

protected:
	// ��������;
	bool ReadData(byte_ptr pData, int iSize);
	// �������;
	bool WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd);

public:
	RIFF_HEADER			m_RiffHeader;
	FMT_BLOCK			m_FmtBlock;
	DATA_HEADER			m_DataHeader;
	byte*				m_pData;
	int					m_iSize;
};