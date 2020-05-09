#include<iostream>
#include<future>
#include<vector>
#include<list>
#include<thread>
#include"ServerSocket.h"
#include"ClientsManager.h"

using namespace std;

void HandleRequest(ClientSocket* sock);
void FuturesClean();
void ServerQuitMessage();
vector<string> Split(string original, const string& regex);

list<future<void>*>* futures;

int main() {
	futures = new list<future<void>*>();
	auto futuresClean = async(launch::async, FuturesClean);
	ServerSocket serverSocket("127.0.0.1",54000);
	atexit(ServerQuitMessage);
	while (true) {
		ClientSocket *sock = serverSocket.GetClient();
		future<void>* f = new future<void>(async(launch::async, HandleRequest, move(sock)));
		futures->push_back(f);
	}
}

void ServerQuitMessage() {
	ClientsManager::get()->SendMessageToAll("", "SERVER_OFF");
}

void HandleRequest(ClientSocket* sock) {
	string message = sock->GetMessageFromClient();
	vector<string> request = Split(message, ":-:");
	cout << message << endl;
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
}

void FuturesClean() {
	while (true) {
		this_thread::sleep_for(chrono::seconds(5));
		for (auto it = futures->begin(); it != futures->end();) {
			if ((*it)->wait_for(chrono::seconds(0)) == future_status::ready) {
				(*it)->get();
				it = futures->erase(it);
			}
			else {
				++it;
			}
		}
	}
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