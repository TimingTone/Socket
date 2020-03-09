#define WIN32_LEAN_AND_MEAN //�����ȥ�������ڵĻ������ͻ��һЩ�⣬�������������������
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")


//��Socket API�������׵�TCP�ͻ���



int main() {

	//����windows socket 2.x����
	WORD wVersionRequested = MAKEWORD(2, 2);
	//The WSADATA structure is used to save the Windows Sockets
	// initialization information returned by the function WSAStartup
	WSADATA wsadata;

	//����winSocket����������WSAStartup
	//wVersionRequested  ��WORD�İ汾��
	//&wsadata ��WSADATA�����ݽṹ(struct)
	WSAStartup(wVersionRequested, &wsadata);
	//��Socket API�������׵�TCP�ͻ���
	// 1. ����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);  //_sock�Ƿ����socket
	if (INVALID_SOCKET == _sock) {
		printf("Failed to establish socket client...\n");
	}
	else {
		printf("Succeed to establish the client...\n");
	}
	// 2. ���ӷ����� connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret) {
		printf("Failed to establish socket client...\n");
	}
	else {
		printf("Succeed to establish the client...\n");
	}

	// 3, Input request command
	while (true) {
		char cmdBuf[128] = {};
		scanf("%s", cmdBuf);
		// 4, Deal with request command
		if (0 == strcmp(cmdBuf, "exit")) {
			printf("Received quit command!");
			break;
		}
		else {
			// 5, send request command to server.
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		// 6. ���շ�������Ϣrecv
		char recvBuf[128] = {};
		int nlen = recv(_sock, recvBuf, 128, 0);
		if (nlen > 0) {
			printf("Have received Data: %s  \n", recvBuf);
		}
	}
	
	
	// 7. �ر�socket  closesocket
	closesocket(_sock);
	WSACleanup();
	printf("quit!, mission completed.");
	getchar();
	return 0;
}