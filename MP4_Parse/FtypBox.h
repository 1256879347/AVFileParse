#pragma once
#include "BaseBox.h"

#define FTYP_MAJORBRAND_SIZE		4
#define FTYP_MINORVERSION_SIZE		4
#define FTYP_COMPBRAND_SIZE			4

/************************************************************************************************************
				File Type Box (ftyp);
��box����ֻ��1��������ֻ�ܱ��������ļ��㣬�����ܱ�����box����;
��boxӦ�ñ������ļ����ʼ��ָʾ��MP4�ļ�Ӧ�õ������Ϣ;

major_brand			4�ֽ�		0x69736f6d:		"isom"��ASCII��;
minor_version		4�ֽ�		0x00000200:		 ismo�İ汾��;
compatible_brands	N��4�ֽ�	˵�����ļ����ismo, iso2, mp41����Э��;
************************************************************************************************************/

class FtypBox : public BaseBox
{
public:
	FtypBox();
	virtual ~FtypBox();

	virtual int ParseAttrs(byteptr &pData);

	static BaseBox* CreateObject();

public:
	string				m_strMajorBrand;
	int					m_iMinorVersion;
	vector<string>		m_vecCompBrands;
};