#pragma once
#include<WS2tcpip.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")

class ClientSocket {
	SOCKET pSocket;
public:
	ClientSocket(SOCKET pSock): pSocket(pSock) {}
	void SendMessageToClient(std::string message) {
		send(pSocket,message.c_str() , message.size()+1, 0);
	}
	std::string GetMessageFromClient() {
		char buff[4096];
		ZeroMemory(buff, 4096);
		int byteRecieved = recv(pSocket, buff, 4096, 0);
		if (byteRecieved == SOCKET_ERROR)
		{
			return "error";
		}
		return std::string(buff,0, byteRecieved);
	}
};