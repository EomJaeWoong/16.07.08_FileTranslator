#ifndef __FILETRANSLATOR__H__
#define __FILETRANSLATOR__H__

struct st_PACKET_HEADER
{
	DWORD	dwPacketType;		// 0x11223344	우리의 패킷확인 고정값

	WCHAR	szName[32];		// 본인이름, 유니코드 NULL 문자 끝
	WCHAR	szFileName[128];	// 파일이름, 유니코드 NULL 문자 끝
	int	iFileSize;
};

#endif