#pragma once
#include "AVFile.h"

/*      PACKET_TYPE_...                0x00 */
#define PACKET_TYPE_CHUNK_SIZE         0x01
/*      PACKET_TYPE_...                0x02 */
#define PACKET_TYPE_BYTES_READ_REPORT  0x03
#define PACKET_TYPE_CONTROL            0x04
#define PACKET_TYPE_SERVER_BW          0x05
#define PACKET_TYPE_CLIENT_BW          0x06
/*      PACKET_TYPE_...                0x07 */
#define PACKET_TYPE_AUDIO              0x08
#define PACKET_TYPE_VIDEO              0x09
/*      PACKET_TYPE_...                0x0A */
/*      PACKET_TYPE_...                0x0B */
/*      PACKET_TYPE_...                0x0C */
/*      PACKET_TYPE_...                0x0D */
/*      PACKET_TYPE_...                0x0E */
#define PACKET_TYPE_FLEX_STREAM_SEND   0x0F
#define PACKET_TYPE_FLEX_SHARED_OBJECT 0x10
#define PACKET_TYPE_FLEX_MESSAGE       0x11
#define PACKET_TYPE_SCRIPT             0x12
#define PACKET_TYPE_SHARED_OBJECT      0x13
#define PACKET_TYPE_INVOKE             0x14
/*      PACKET_TYPE_...                0x15 */
#define PACKET_TYPE_FLASH_VIDEO        0x16

/************************************************************************************************************
header���ּ�¼��flv�����͡��汾����Ϣ����flv�Ŀ�ͷ��һ�㶼��࣬ռ9bytes.

�����ʽ����;
�ļ�����	3 bytes		"FLV";
�汾		1 byte		һ��Ϊ0x01;
����Ϣ		1 byte		������һλ��1��ʾ����Ƶ����������λ��1��ʾ����Ƶ�������ڶ�����λ����Ϊ0;
header����	4 bytes		����header�ĳ��ȣ�һ��Ϊ9������9��ʾ���滹����չ��Ϣ;
************************************************************************************************************/
struct FlvHeader
{
	byte Type[3];
	byte Version;
	byte StreamInfo;
	byte HeaderSize[4];
};

/************************************************************************************************************
ÿ��Tag��Ҳ������������ɵģ�Tag Header��Tag Data;
Tag Header���ŵ��ǵ�ǰTag�����͡���������Tag Data�����ȵ���Ϣ;
��������;

����			����					����;
Tag����			1 bytes					8����Ƶ \ 9����Ƶ \ 18���ű� \  ����������;
����������		3 bytes					���������ĳ���;
ʱ���			3 bytes					��������λ�Ǻ��� ���ڽű��͵�tag����0;
ʱ�����չ		1 bytes					��ʱ�����չΪ4bytes,�����8λ �����õ�;
StreamsID		3 bytes					����0;
������(data)	�����������Ⱦ���		����ʵ��;
************************************************************************************************************/
struct TagHeader
{
	byte TagType;
	byte DataSize[3];
	byte Timestamp[3];
	byte TimeExtend;
	byte StreamsID[3];
};

class CFLVFile: public CAVFile
{
public:
	CFLVFile();
	virtual ~CFLVFile();

	virtual bool ParseFile();

protected:
	bool ParseFLVHeader();
	bool ParseFLVBody();

	bool ParseAudioData(int iDataSize);
	bool ParseVideoData(int iDataSize);
	bool ParseScriptData(int iDataSize);
};