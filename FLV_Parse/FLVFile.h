#pragma once
#include "FLVFileHeader.h"
#include "FLVFileBody.h"

class CFLVFile
{
public:
	CFLVFile();
	~CFLVFile();

public:

	// �����ļ�,��ȡ����;
	bool LoadFile(char* strFile);

	// ��ʼdStart~dEnd������Ƶ�����ļ�;
	// Ҫ��0~1, dEnd > dStart ���򷵻� false;
	bool SaveFile(char* strFile, double dStart, double dEnd);

	// ��ʾ;
	void Display();

protected:
	// ��������;
	bool ReadData(byte_ptr pData, int iSize);
	// �������;
	bool WriteData(byte_ptr pData, int &iSize, double dStart, double dEnd);

private:
	CFLVFileHeader		m_flvHeader;
	CFLVFileBody		m_flvBody;
	int					m_iSize;
};