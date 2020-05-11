#pragma once
#include"Socket.h"
#include<iostream>
#include<future>

class Chat
{
private:
	Socket *sender;
	Socket *reciever;
	std::string addr;
	int port;
	std::string userName;
	std::future<void> responseWaiting;
public:
	Chat(const std::string& addr = "127.0.0.1", int port = 54000) : addr(addr), port(port) {}
	bool LogIn(std::string message);
	void SendMessageToServer(const std::string& message) {
		sender = new Socket(addr, port);
		sender->SendMessageToServer(message);
		delete sender;
	}
	std::string GetMessageFromServer() {
		reciever->GetMessageFromServer();
	}
	static void TurnOnWinSockets();
	static void TurnOffWinSockets() {
		WSACleanup();
	}
	void WaitForResponse();
};

