#define WIN32_LEAN_AND_MEAN //引入宏去避免早期的会引起冲突的一些库，减少其他依赖库的引用
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")


//用Socket API建立简易的TCP客户端


		
int main() {

	//启动windows socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;

	//调用winSocket的启动函数
	//ver  ：WORD的版本号
	//&dat ：WSADATA的数据结构
	WSAStartup(ver, &dat);
	//用Socket API建立简易的TCP客户端
	// 1. 建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock) {
		printf("Failed to establish socket client...");
	}
	else {
		printf("Succeed to establish the client...");
	}
	// 2. 连接服务器 connect
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
	// 3. 接收服务器信息recv
	char recvBuf[256] = {};
	int nlen = recv(_sock, recvBuf, 256, 0);
	if (nlen > 0) {
		printf("Have received Data: %s \n", recvBuf);
	}
	// 4. 关闭socket  closesocket
	closesocket(_sock);
	WSACleanup();
	getchar();
	return 0;
}


