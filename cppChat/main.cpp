#include<iostream>
#include<future>
#include<thread>
#include<vector>
#include<thread>
#include"ServerSocket.h"
#include"ClientsManager.h"

using namespace std;

void HandleRequest(ClientSocket* sock);
void ServerQuitMessage();
vector<string> Split(string original, const string& regex);

const int maxThreadCount = 64;
volatile int threadCount = 0;

int main() {
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
	string message = sock->GetMessageFromClient();
	vector<string> request = Split(message, ":-:");   
	if (request[0] == "LogIn") {
		ClientsManager::get()->RegisterClient(request[1],sock);
	}
	else {
		if (request[0] == "MessageTo") {
			ClientsManager::get()->SendMessageTo(request[1],request[2], request[3]);
		}
		else if (request[0] == "MessageToAll") {
			ClientsManager::get()->SendMessageToAll(request[1], request[2]);
		}
		else if (request[0] == "WhoIsOnline") {
			ClientsManager::get()->SendWhoIsOnlineInfo(request[1]);
		}
		else if (request[0] == "LogOut") {
			ClientsManager::get()->RemoveClient(request[1]);
		}
		else if (request[0] == "Exit") {
			exit(0);
		}
		delete sock;
	}
	threadCount--;
}

void ServerQuitMessage() {
	ClientsManager::get()->SendMessageToAll("", "SERVER_OFF");
}

vector<string> Split(string original,const string& regex) {
	vector<string> res;
	size_t pos = 0;
	string token;
	while ((pos = original.find(regex)) != string::npos) {
		token = original.substr(0, pos);
		res.push_back(token);
		original.erase(0, pos + regex.length());
	}
	res.push_back(original);
	return res;
}