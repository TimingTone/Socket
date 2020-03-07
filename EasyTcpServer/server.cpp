#define WIN32_LEAN_AND_MEAN //�����ȥ�������ڵĻ������ͻ��һЩ�⣬�������������������
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
//#pragma warning(disable:4996)  //error4996:inet_addr: Use inet_pton() or InetPton() instead of define



//��Socket API�������׵�TCP������� 

		
int main() {

	//����windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;

	//����winSocket����������
	//ver  ��WORD�İ汾��
	//&dat ��WSADATA�����ݽṹ
	WSAStartup(ver, &dat);

	//��Socket API�������׵�TCP�����

	// 1. ����һ��socket�׽���
	//socket(ipv4/ipv6,  type of Internet,  InternetProtocol)
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 2. �󶨽��ܿͻ������ӵĶ˿� bind
	sockaddr_in _sin = { } ;
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);  //host to net unsigned short(htons)
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//Use "INADDR_ANY" if you don't know the specific addr
	//InetPton(AF_INET, _T("127.0.0.1"), &_sin.sin_addr.S_un.S_addr);
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR) {
		printf("Error, failed to bind Internet Port using to receive client connecting\n");
	}
	else {
		printf("Succeed in binding port\n");
	}

	// 3. ��������˿� listen  
	if (listen(_sock, 5 == SOCKET_ERROR)) {
		printf("Error, failed in listenning Internet Port\n");
	}
	else {
		printf("Succeed in listenning Internet Port\n");
	}
	// 4. �ȴ����տͻ������� accept
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	char msqBuf[] = "Hello , I'm server.";

	while (true) {
		_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
		if (INVALID_SOCKET == _cSock) {
			printf("failed to receive the socket of client,,,,");
		}
		printf("A new client has joined...: IP = %s \n", inet_ntoa(clientAddr.sin_addr));
		// 5. ��ͻ��˷���һ������ send
		send(_cSock, msqBuf, strlen(msqBuf) + 1, 0);
	}
	
	// 6. �ر�socket    closesocket
	closesocket(_sock);

	//���Windows Socket����
	WSACleanup();
	return 0;
}