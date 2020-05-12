#include<iostream>
#include<future>
#include<thread>
#include<vector>
#include<thread>
#include"ServerSocket.h"
#include"ClientsManager.h"
#include"Message.h"

using namespace std;

void HandleRequest(ClientSocket* sock);
void ServerQuitMessage();

const int maxThreadCount = 64;
volatile int threadCount = 0;

int main(char* argv[],int argc) {
	ServerSocket serverSocket("127.0.0.1",54000);
	atexit(ServerQuitMessage);
	while (true) {
		if (threadCount < maxThreadCount) {
			thread(HandleRequest, serverSocket.GetClient()).detach();
		}
	}
}

void HandleRequest(ClientSocket* sock) {
	threadCount++;
	Message req = sock->GetMessageFromClient();
	if (req.request == "LogIn") {
		ClientsManager::get()->RegisterClient(req.sender,sock);
	}
	else {
		if (req.request == "MessageTo") {
			cout << (string)req << endl;
			ClientsManager::get()->SendMessageTo(req.sender,req.receiver, req.data);
		}
		else if (req.request == "MessageToAll") {
			ClientsManager::get()->SendMessageToAll(req.sender, req.data);
		}
		else if (req.request == "WhoIsOnline") {
			ClientsManager::get()->SendOnlineInfo(req.sender);
		}
		else if (req.request == "LogOut") {
			ClientsManager::get()->RemoveClient(req.sender);
		}
		else if (req.request == "Exit") {
			exit(0);
		}
		delete sock;
	}
	threadCount--;
}

void ServerQuitMessage() {
	ClientsManager::get()->ServerQuitInfo();
}