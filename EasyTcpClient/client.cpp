#define WIN32_LEAN_AND_MEAN //�����ȥ�������ڵĻ������ͻ��һЩ�⣬�������������������
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")


//��Socket API�������׵�TCP�ͻ���


		
int main() {

	//����windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;

	//����winSocket����������
	//ver  ��WORD�İ汾��
	//&dat ��WSADATA�����ݽṹ
	WSAStartup(ver, &dat);
	//��Socket API�������׵�TCP�ͻ���
	// 1. ����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock) {
		printf("Failed to establish socket client...");
	}
	else {
		printf("Succeed to establish the client...");
	}
	// 2. ���ӷ����� connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret) {
		printf("Failed to establish socket client...");
	}
	else {
		printf("Succeed to establish the client...");
	}
	// 3. ���շ�������Ϣrecv
	char recvBuf[256] = {};
	int nlen = recv(_sock, recvBuf, 256, 0);
	if (nlen > 0) {
		printf("Have received Data: %s \n", recvBuf);
	}
	// 4. �ر�socket  closesocket
	closesocket(_sock);
	WSACleanup();
	getchar();
	return 0;
}


