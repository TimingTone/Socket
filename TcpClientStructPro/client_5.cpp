#define WIN32_LEAN_AND_MEAN //引入宏去避免早期的会引起冲突的一些库，减少其他依赖库的引用
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")


//用Socket API建立简易的TCP客户端

enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT,
	CMD_LOGINOUT_RESULT,
	CMD_ERROR

};
struct DataHeader {
	short dataLength;
	short cmd;
};
//DataPackage
struct Login : public DataHeader {
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};

struct LoginResult : public DataHeader {
	LoginResult() {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};

struct LoginOut : public DataHeader {
	LoginOut() {
		dataLength = sizeof(LoginOut);
		cmd = CMD_LOGINOUT;
	}
	char userName[32];
};

struct LoginOutResult :public DataHeader {
	LoginOutResult() {
		dataLength = sizeof(LoginOutResult);
		cmd = CMD_LOGINOUT_RESULT;
		result = 0;
	}
	int result;
};

int main() {

	//启动windows socket 2.x环境
	WORD wVersionRequested = MAKEWORD(2, 2);
	//The WSADATA structure is used to save the Windows Sockets
	// initialization information returned by the function WSAStartup
	WSADATA wsadata;

	//调用winSocket的启动函数WSAStartup
	//wVersionRequested  ：WORD的版本号
	//&wsadata ：WSADATA的数据结构(struct)
	WSAStartup(wVersionRequested, &wsadata);
	//用Socket API建立简易的TCP客户端
	// 1. 建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);  //_sock是服务端socket
	if (INVALID_SOCKET == _sock) {
		printf("Failed to establish socket client...\n");
	}
	else {
		printf("Succeed to establish the client...\n");
	}
	// 2. 连接服务器 connect
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
		else if (0 == strcmp(cmdBuf, "login")) {
			Login login; 
			strcpy(login.userName, "lyd");
			strcpy(login.passWord, "lydmima");
			// 5, send request command to server.
			send(_sock, (const char*)&login, sizeof(login), 0);
			//  receive the data returned form server
			LoginResult loginRet = {};
			recv(_sock, (char*)&loginRet, sizeof(loginRet), 0);
			printf("LoginResult:  %d \n", loginRet.result);
		}
		else if (0 == strcmp(cmdBuf, "loginOut")) {
			LoginOut loginOut;
			strcpy(loginOut.userName, "lyd");
			DataHeader dh = { sizeof(loginOut), CMD_LOGINOUT };
			// 5, send request command to server.
			send(_sock, (const char*)&loginOut, sizeof(loginOut), 0);
			//  receive the data returned form server
			LoginResult loginOutRet = {};
			recv(_sock, (char*)&loginOutRet, sizeof(loginOutRet), 0);
			printf("LoginResult:  %d \n", loginOutRet.result);
		}
		else {
			printf("Riceive unsupport command, please enter again\n");
		}
		// 6. 接收服务器信息recv
		//char recvBuf[128] = {};
		//int nlen = recv(_sock, recvBuf, 128, 0);
		//if (nlen > 0) {
		//	//recvBuf is a binary data, must be converted
		//	DataPackage* dp = (DataPackage*)recvBuf;
		//	printf("Age: %d    \nName = %s\n", dp->age, dp->name);
		//}
	}


	// 7. 关闭socket  closesocket
	closesocket(_sock);
	WSACleanup();
	printf("quit!, mission completed.");
	getchar();
	return 0;
}