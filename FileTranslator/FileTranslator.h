#ifndef __FILETRANSLATOR__H__
#define __FILETRANSLATOR__H__

struct st_PACKET_HEADER
{
	DWORD	dwPacketType;		// 0x11223344	�츮�� ��ŶȮ�� ������

	WCHAR	szName[32];		// �����̸�, �����ڵ� NULL ���� ��
	WCHAR	szFileName[128];	// �����̸�, �����ڵ� NULL ���� ��
	int	iFileSize;
};

#endif