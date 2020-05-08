#pragma once;
#include<WS2tcpip.h>
#include<string>
#include"ClientSocket.h"
#pragma comment(lib,"ws2_32.lib")

class ServerSocket{
private:
	SOCKET serverSocket;
public:
	ServerSocket(const std::string& addr = "127.0.0.1",const int port=1234);
	ClientSocket* GetClient();
};