#pragma once
#include<WS2tcpip.h>
#include<iostream>

#pragma comment(lib,"ws2_32.lib")
class Socket {
private:
	SOCKET sock;
public:
	Socket(const std::string& addr = "127.0.0.1", const int port = 54000);
	void SendMessageToServer(const std::string& message);
	std::string GetMessageFromServer();
};