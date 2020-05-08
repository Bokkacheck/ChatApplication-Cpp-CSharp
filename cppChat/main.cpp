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
vector<string> Split(string original, const string& regex);
list<future<void>*>* futures;

int main() {
	futures = new list<future<void>*>();
	auto futuresClean = async(launch::async, FuturesClean);
	ServerSocket serverSocket("127.0.0.1",54000);
	while (true) {
		ClientSocket *sock = serverSocket.GetClient();
		future<void>* f = new future<void>(async(launch::async, HandleRequest, move(sock)));
		futures->push_back(f);
	}
}

void HandleRequest(ClientSocket* sock) {
	string message = sock->GetMessageFromClient();
	vector<string> request = Split(message, ":-:");
	if (request[0] == "LogIn") {
		cout << "login" << endl;
		ClientsManager::get()->RegisterClient(request[1],sock);
	}
	else {
		if (request[0] == "MessageTo") {
			ClientSocket* cs = ClientsManager::get()->GetClient(request[2]);
			if (cs != nullptr) {
				cs->SendMessageToClient(request[3]);
			}
			else {
				ClientsManager::get()->GetClient(request[1])->SendMessageToClient("User is not connected");
			}
		}
		else if (request[0] == "MessageToAll") {
			ClientsManager::get()->SendMessageToAll(request[1], request[2]);
		}
		else if (request[0] == "LogOut") {
			ClientsManager::get()->RemoveClient(request[1]);
		}
		sock->SendMessageToClient("Response: " + message);
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