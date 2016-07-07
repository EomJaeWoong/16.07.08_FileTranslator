#include <winsock2.h>
#include <windows.h>
#include <atlsocket.h>
#include <stdlib.h>
#include <stdio.h>
#include "FileTranslator.h"

#define BUF_SIZE 1000

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

BOOL DomainToIP(WCHAR *szDomain, IN_ADDR *pAddr)
{
	ADDRINFOW	*pAddrInfo;
	SOCKADDR_IN *pSockAddr;

	if (GetAddrInfo(szDomain, L"0", NULL, &pAddrInfo) != 0)
	{
		return FALSE;
	}

	pSockAddr = (SOCKADDR_IN *)pAddrInfo->ai_addr;
	*pAddr = pSockAddr->sin_addr;
	FreeAddrInfo(pAddrInfo);

	return TRUE;
}

int main()
{
	WSADATA wsa;
	int retval;
	int fSize;

	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		return -1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)		err_display("socket()");

	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(19010);
	DomainToIP(L"procademyserver.iptime.org", &serveraddr.sin_addr);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)		err_display("connect()");

	FILE* fp;
	fopen_s(&fp, "image.jpg", "rb");
	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	rewind(fp);

	//사이즈 체크
	st_PACKET_HEADER stPacketHeader;
	stPacketHeader.dwPacketType = 0x11223344;
	stPacketHeader.iFileSize = fSize;
	wcscpy_s(stPacketHeader.szName, L"엄재웅");
	wcscpy_s(stPacketHeader.szFileName, L"image.jpg");

	retval = send(sock, (char*)&stPacketHeader, sizeof(stPacketHeader), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send header error");
		return -1;
	}

	char buf[BUF_SIZE];
	int iread, iReadTotal = 0;
	int isEnd;

	while (1)
	{
		iread = fread(buf, 1, 1000, fp);
		if (iread > 0)
		{
			retval = send(sock, buf, BUF_SIZE, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("image send error");
				break;
			}
			iReadTotal += iread;
		}
		else if (iread == 0 && iReadTotal == fSize){
			break;
		}
		else{
			err_display("file error");
		}
	}
	recv(sock, (char*)&isEnd, 10, 0);
	printf("%x", isEnd);

	closesocket(sock);
	WSACleanup();

	return 0;
}