#pragma once
#include "bytes.h"
#include "TagType.h"

class CFLVTagBody
{
public:
	CFLVTagBody();
	~CFLVTagBody();

	// ��������,���ؽ����ֽ���;
	int ReadData(byte_ptr &pData, int iTagSize,int iTagType);
	// �������;
	int WriteData(byte_ptr &pData);

	// ��ʾ;
	void Display();

	bool IsKeyFrame();

private:
	CTagType*	m_pTag;
};