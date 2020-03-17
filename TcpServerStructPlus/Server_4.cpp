#define WIN32_LEAN_AND_MEAN //  Introduce macros to advoid early conflicting libraries and reduce references to other dependency libraries
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")		//Additional dependency lib libraries can also set in properties.
//#pragma warning(disable:4996)  //error4996:inet_addr: Use inet_pton() or InetPton() instead of define

//  Establish simple TCP and server using Scoket API
//  Establish a C/S network program that can continuously process requests
//  Use Internet Struct data

//  Server and Client must be consistent in byte size and memory alignment
enum CMD {
	CMD_LOGIN,
	CMD_LOGINOUT,
	CMD_ERROR

};
struct DataHeader {
	short dataLength;
	short cmd;
};
//DataPackage
struct Login {
	char userName[32];
	char passWord[32];
};

struct LoginResult {
	int result;

};

struct LoginOut {
	char userName[32];
};

struct LoginOutResult {
	int result;
};

int main() {

	//Start up windows socket 2.x environment
	//MAKEWORD(a, b)����b.a�汾��Winsock,  a��ʾ��λbyte�� b��ʾ��λbyte
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsadata;

	//  Call winSocket startup function
	WSAStartup(wVersionRequested, &wsadata);

	//��Socket API�������׵�TCP�����

	// 1. Establish a Socket
	//socket(ipv4/ipv6,  type of Internet,  InternetProtocol)
	//AF_INEF: ipv4,   
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 2. Bind the port that accepts client connections  "bind"
	sockaddr_in _sin = { };
	_sin.sin_family = AF_INET;   //The same as the _sock(ipv4)
	_sin.sin_port = htons(4567);  //host to net unsigned short(htons)
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//Use "INADDR_ANY" if you don't know the specific addr
	//InetPton(AF_INET, _T("127.0.0.1"), &_sin.sin_addr.S_un.S_addr);
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR) {
		printf("Error, failed to bind Internet Port using to receive client connecting\n");
	}
	else {
		printf("Succeed in binding port\n");
	}

	// 3. Listening to the network port     "listen"
	if (listen(_sock, 5 == SOCKET_ERROR)) {
		printf("Error, failed in listenning Internet Port\n");
	}
	else {
		printf("Succeed in listenning Internet Port\n");
	}
	// 4. Waiting to receive client connection   "accept"
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	//char msgBuf[] = "Hello , I'm server.";

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock) {
		printf("failed to receive the socket of client......");
	}
	printf("A new client has joined...: IP = %s \n", inet_ntoa(clientAddr.sin_addr));

	char _recvBuf[128] = {};
	while (true) {

		DataHeader header = {};
		// 5, Receive client data
		int nLen = recv(_cSock, (char*)&header, sizeof(header), 0);
		if (nLen <= 0) {
			printf("Client has quit!, mission completed.");
			break;
		}
		printf("Receive command: %d ;        Data Length:    %d\n", header.cmd, header.dataLength);
		switch (header.cmd) {
			case CMD_LOGIN: {
				Login login = {};
				recv(_cSock, (char*)&login, sizeof(Login), 0);
				// Ignore the process of judging if user password is correct 
				LoginResult ret = { 1 };
				send(_cSock, (char*)&header, sizeof(DataHeader), 0);
				send(_cSock, (char*)&ret, sizeof(LoginResult), 0);
			}
			break;
			case CMD_LOGINOUT:
			{
				LoginOut loginOut = {};
				recv(_cSock, (char*)&loginOut, sizeof(LoginOut), 0);
				// Ignore the process of judging if user password is correct 
				LoginOutResult ret = { 1 };
				send(_cSock, (char*)&header, sizeof(header), 0);
				send(_cSock, (char*)&ret, sizeof(ret), 0);
			}
			break;
			default:
				header.cmd = CMD_ERROR;
				header.dataLength = 0;
				send(_cSock, (char*)&header, sizeof(header), 0);
			break;

		}
		// 6, Deal with request.
	/*	if (0 == strcmp(_recvBuf, "getInfo")) {
			DataPackage dp = { 20, "TimingTone" };
			send(_cSock, (const char*)&dp, sizeof(DataPackage) + 1, 0);
		}*/
		//else if (0 == strcmp(_recvBuf, "getAge")) {
		//	char msgBuf[] = "100000";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
		//else {
		//	char msgBuf[] = "What can i do for you?";
		//	// 7. Send data to client             send
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}

	}

	// 8. Close socket                  closesocket
	closesocket(_sock);

	// Clean up Windows Socket environment
	WSACleanup();
	printf("Client has quit, mission completed.");
	getchar();
	return 0;
}