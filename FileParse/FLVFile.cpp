#include "FLVFile.h"
#include "amf.h"

CFLVFile::CFLVFile()
{
}

CFLVFile::~CFLVFile()
{
}

bool CFLVFile::ParseFile()
{
	if (!ParseFLVHeader())
	{
		return false;
	}

	if (!ParseFLVBody())
	{
		return false;
	}

	return true;
}

bool CFLVFile::ParseFLVHeader()
{
	printf("******************************FLV Header******************************\n");
	// FLV Header;
	{
		FlvHeader flvHeader;
		int iSize = sizeof(FlvHeader);
		if (iSize != fread(&flvHeader, 1, iSize, m_pFile))
		{
			printf("Read FLV Header is Error. \n");
			return false;
		}

		printf("\t File Type     \t : %c %c %c \n", flvHeader.Type[0], flvHeader.Type[1], flvHeader.Type[2]);
		printf("\t Version       \t : %d \n", flvHeader.Version);
		printf("\t Stream Info   \t : %d \n", flvHeader.StreamInfo);
		printf("\t Header Length \t : %d \n\n", ByteToInt(flvHeader.HeaderSize, sizeof(flvHeader.HeaderSize)));
	}

	return true;
}

bool CFLVFile::ParseFLVBody()
{
	printf("******************************FLV Body******************************\n");

	bool bStop = false;
	while (!bStop)
	{
		printf("\n      ************************Tag Header************************      \n");

		_getw(m_pFile);

		TagHeader tagHeader;
		int iTagHeaderSize = sizeof(TagHeader);
		if (iTagHeaderSize != fread(&tagHeader, 1, iTagHeaderSize, m_pFile))
		{
			// ������;
			printf("\t Read File Finished. \n");
			bStop = true;
		}

		printf("\t Tag Type   \t : %d \n", tagHeader.TagType);
		printf("\t DataSize   \t : %d \n", ByteToInt(tagHeader.DataSize, sizeof(tagHeader.DataSize)));
		printf("\t Timestamp  \t : %d \n", ByteToInt(tagHeader.Timestamp, sizeof(tagHeader.Timestamp)));
		printf("\t TimeExtend \t : %d \n", tagHeader.TimeExtend);
		printf("\t StreamsID  \t : %d \n", ByteToInt(tagHeader.StreamsID, sizeof(tagHeader.StreamsID)));

		printf("      ************************Tag Body************************      \n");
		const int iDataSize = ByteToInt(tagHeader.DataSize, sizeof(tagHeader.DataSize));

		switch (tagHeader.TagType)
		{
		case PACKET_TYPE_AUDIO:
			{
				// ��Ƶ����;
				ParseAudioData(iDataSize);
			}
			break;
		case PACKET_TYPE_VIDEO:
			{
				// ��Ƶ����;
				ParseVideoData(iDataSize);
			}
			break;
		case PACKET_TYPE_SCRIPT:
			{
				// ��������;
				ParseScriptData(iDataSize);
			}
			break;
		default:
			{
				printf("\t Read Tag Body %d bytes. \n\n", iDataSize);
				fseek(m_pFile, iDataSize, SEEK_CUR);
			}
			break;
		}
	}

	return true;
}

/************************************************************************************************************
	��һ���ֽ�����Ƶ��Ϣ��ʽ,��ʽ����;

	����			����					����;
	
	��Ƶ��ʽ		4 bits					
											0 = Linear PCM, platform endian
											1 = ADPCM
											2 = MP3
											3 = Linear PCM, little endian
											4 = Nellymoser 16-kHz mono
											5 = Nellymoser 8-kHz mono
											6 = Nellymoser
											7 = G.711 A-law logarithmic PCM
											8 = G.711 mu-law logarithmic PCM
											9 = reserved
											10 = AAC
											11 = Speex
											14 = MP3 8-Khz
											15 = Device-specific sound

	������			2 bits					(����AAC����3)
											0 = 5.5-kHz
											1 = 11-kHz
											2 = 22-kHz
											3 = 44-kHz

	�����ĳ���		1 bit					(ѹ��������Ƶ����16bit)
											0 = snd8Bit
											1 = snd16Bit

	��Ƶ����		1 bit					(����AAC����1)
											0 = sndMono
											1 = sndStereo

	�ڶ����ֽڿ�ʼ������Ƶ��������;
************************************************************************************************************/
bool CFLVFile::ParseAudioData(int iDataSize)
{
	// ������Ƶ��ʽ;
	byte audioHeader;
	fread(&audioHeader, 1, sizeof(audioHeader), m_pFile);

	char strAudioInfo[100] = { 0 };
	{
		byte audioFormat = audioHeader >> 4;
		switch (audioFormat)
		{
		case 0:strcat(strAudioInfo, "Linear PCM, platform endian"); break;
		case 1:strcat(strAudioInfo, "ADPCM"); break;
		case 2:strcat(strAudioInfo, "MP3"); break;
		case 3:strcat(strAudioInfo, "Linear PCM, little endian"); break;
		case 4:strcat(strAudioInfo, "Nellymoser 16-kHz mono"); break;
		case 5:strcat(strAudioInfo, "Nellymoser 8-kHz mono"); break;
		case 6:strcat(strAudioInfo, "Nellymoser"); break;
		case 7:strcat(strAudioInfo, "G.711 A-law logarithmic PCM"); break;
		case 8:strcat(strAudioInfo, "G.711 mu-law logarithmic PCM"); break;
		case 9:strcat(strAudioInfo, "reserved"); break;
		case 10:strcat(strAudioInfo, "AAC"); break;
		case 11:strcat(strAudioInfo, "Speex"); break;
		case 14:strcat(strAudioInfo, "MP3 8-Khz"); break;
		case 15:strcat(strAudioInfo, "Device-specific sound"); break;
		default:strcat(strAudioInfo, "UNKNOWN"); break;
		}
		strcat(strAudioInfo, "| ");
	}
	
	{
		byte sampBits = audioHeader << 4;
		sampBits = sampBits >> 6;
		switch (sampBits)
		{
		case 0:strcat(strAudioInfo, "5.5-kHz"); break;
		case 1:strcat(strAudioInfo, "1-kHz"); break;
		case 2:strcat(strAudioInfo, "22-kHz"); break;
		case 3:strcat(strAudioInfo, "44-kHz"); break;
		default:strcat(strAudioInfo, "UNKNOWN"); break;
		}
		strcat(strAudioInfo, "| ");
	}

	{
		byte sampLen = audioHeader << 6;
		sampLen = sampLen >> 7;
		switch (sampLen)
		{
		case 0:strcat(strAudioInfo, "8Bit"); break;
		case 1:strcat(strAudioInfo, "16Bit"); break;
		default:strcat(strAudioInfo, "UNKNOWN"); break;
		}
		strcat(strAudioInfo, "| ");
	}

	{
		byte audioType = audioHeader << 7;
		audioType = audioType >> 7;
		switch (audioType)
		{
		case 0:strcat(strAudioInfo, "Mono"); break;
		case 1:strcat(strAudioInfo, "Stereo"); break;
		default:strcat(strAudioInfo, "UNKNOWN"); break;
		}
		strcat(strAudioInfo, "| ");
	}

	printf("\t %s audio data: %d bytes \n", strAudioInfo, iDataSize - 1);
	fseek(m_pFile, iDataSize - 1, SEEK_CUR);

	return true;
}

/************************************************************************************************************
��һ���ֽ�����Ƶ��Ϣ��ʽ,��ʽ����;

	����			����					����;

	֡����			4 bits 
											1: keyframe(for AVC, a seekable frame)
											2 : inter frame(for AVC, a non - seekable frame)
											3 : disposable inter frame(H.263 only)
											4 : generated keyframe(reserved for server use only)
											5 : video info / command frame

	����ID			4 bits					
											1 : JPEG(currently unused)
											2 : Sorenson H.263
											3 : Screen video
											4 : On2 VP6
											5 : On2 VP6 with alpha channel
											6 : Screen video version 2
											7 : AVC

�ڶ����ֽڿ�ʼ������Ƶ��������;
************************************************************************************************************/
bool CFLVFile::ParseVideoData(int iDataSize)
{
	// ������Ƶ��ʽ;
	byte videoHeader;
	fread(&videoHeader, 1, sizeof(videoHeader), m_pFile);

	char strVideoInfo[100] = { 0 };
	{
		byte frameType = videoHeader >> 4;
		switch (frameType)
		{
		case 1:strcat(strVideoInfo, "key frame  "); break;
		case 2:strcat(strVideoInfo, "inter frame"); break;
		case 3:strcat(strVideoInfo, "disposable inter frame"); break;
		case 4:strcat(strVideoInfo, "generated keyframe"); break;
		case 5:strcat(strVideoInfo, "video info/command frame"); break;
		default:strcat(strVideoInfo, "UNKNOWN"); break;
		}
		strcat(strVideoInfo, "| ");
	}

	{
		byte sampBits = videoHeader << 4;
		sampBits = sampBits >> 4;
		switch (sampBits)
		{
		case 1:strcat(strVideoInfo, "JPEG (currently unused)"); break;
		case 2:strcat(strVideoInfo, "Sorenson H.263"); break;
		case 3:strcat(strVideoInfo, "Screen video"); break;
		case 4:strcat(strVideoInfo, "On2 VP6"); break;
		case 5:strcat(strVideoInfo, "On2 VP6 with alpha channel"); break;
		case 6:strcat(strVideoInfo, "Screen video version 2"); break;
		case 7:strcat(strVideoInfo, "AVC"); break;
		default:strcat(strVideoInfo, "UNKNOWN"); break;
		}
		strcat(strVideoInfo, "| ");
	}

	printf("\t %s audio data: %d bytes \n", strVideoInfo, iDataSize - 1);
	fseek(m_pFile, iDataSize - 1, SEEK_CUR);

	return true;
}

/************************************************************************************************************
	�ű�Tagһ��ֻ��һ������flv�ĵ�һ��Tag;
	���ڴ��flv����Ϣ������duration��audiodatarate��creator��width��;

	���Ƚ����½ű�����������;
	�������ݶ�������������+(���ݳ���)+���ݵĸ�ʽ���ֵģ���������ռ1byte�����ݳ��ȿ����������Ƿ���ڣ������������;

	AMF���ݸ�ʽ����;
************************************************************************************************************/
bool CFLVFile::ParseScriptData(int iDataSize)
{
	// ����������Ϣ;
	byte* pData = new byte[iDataSize];
	byte* pDataEnd = pData + iDataSize;
	fread(pData, 1, iDataSize, m_pFile);

	while (pDataEnd - pData > 0)
	{
		switch (pData[0])
		{
		case AMF_NUMBER:		// ����(double);
			{
				//double dVal = AMF_DecodeNumber(pFileFlv);
			}
			break;
		case AMF_BOOLEAN:		// ����;
			{

			}
			break;
		case AMF_STRING:		// �ַ���;
			{
				pData = pData + 1;

				AVal valName;
				AMF_DecodeString((char*)pData, &valName);
				pData = pData + (valName.av_len + 2);
			}
			break;
		case AMF_OBJECT:		// ����;
			{

			}
			break;
		case AMF_MOVIECLIP:		// ����,δʹ��;
			break;
			// AMF_NULL,				// null;
			// AMF_UNDEFINED,			// δ����;
			// AMF_REFERENCE,			// ����;
		case AMF_ECMA_ARRAY:			// ����;
			{
				pData = pData + 1;

				// ����Ԫ�ظ���;
				int iArrLen = ByteToInt(pData, 4);
				pData = pData + 4;

				AMFObject obj;
				int iSize = pDataEnd - pData;
				int nRes = AMF_DecodeArray(&obj, (char*)pData, iSize, iArrLen, TRUE);
				if (nRes == -1)
				{
					return false;
				}

				for (int i = 0; i < iArrLen; i++)
				{
					std::string strName(obj.o_props[i].p_name.av_val, obj.o_props[i].p_name.av_len);
					switch (obj.o_props[i].p_type)
					{
					case AMF_NUMBER:			// ����(double);
						{
							printf("\t %s : %.0f\n", strName.c_str(), obj.o_props[i].p_vu.p_number);
						}
						break;
					case AMF_STRING:			// �ַ���; 
						{
							std::string strValue(obj.o_props[i].p_vu.p_aval.av_val, obj.o_props[i].p_vu.p_aval.av_len);
							printf("\t %s : %s\n", strName.c_str(), strValue.c_str());
						}
						break;
					default:
						break;
					}
				}

				pData = pData + iSize;
			}
			break;
			// AMF_OBJECT_END,			// �������(0x09);
			// AMF_STRICT_ARRAY,		// �ϸ������;
			// AMF_DATE,				// ����;
			// AMF_LONG_STRING,			// ���ַ���;
			// AMF_UNSUPPORTED,			// δ֧��;
			// AMF_RECORDSET,			// ����,δʹ��;
			// AMF_XML_DOC,				// xml�ĵ�;
			// AMF_TYPED_OBJECT,		// �����͵Ķ���;
			// AMF_AVMPLUS,				// ��Ҫ��չ��AMF3;
			// AMF_INVALID = 0xff		// ��Ч��;
		default:
			break;
		}
	}
	

	return true;
}
