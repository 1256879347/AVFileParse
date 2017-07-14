#pragma once
#include "BaseBox.h"

#define STSD_VERSION_SIZE				1
#define STSD_FLAGS_SIZE					3
#define STSD_ENTRYCOUNT_SIZE			4

/************************************************************************************************************
**					sample description box (stsd);
**   
**	"stsd"�ز�����,�����ٰ���һ����Ŀ ��box������data reference box����sample���ݼ�������Ϣ;
**	û�С�stsd�����޷�����media sample�Ĵ洢λ��. "stsd"�����˱������Ϣ����洢����Ϣ��ý�����Ͳ�ͬ����ͬ;
************************************************************************************************************/
class StsdBox : public BaseBox
{
public:
	StsdBox();
	~StsdBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int				m_iVersion;
	int				m_iFlags;
	int				m_iEntryCount;
};