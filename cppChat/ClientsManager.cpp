#include "ClientsManager.h"

using namespace std;

void ClientsManager::Send(Message msg) {
	map<string, ClientSocket*>::iterator client = clients.find(msg.receiver);
	if (client != clients.end()) {
		cout << (string)msg << endl;
		client->second->SendMessageToClient(msg);
	}
	else {
		client = clients.find(msg.sender);
		if (client != clients.end()) {
			client->second->SendMessageToClient(Message("NO_USER_FOUND", "server", msg.sender, ""));
		}
	}
};
void ClientsManager::SendToAll(Message msg) {
	for (auto it : clients) {
		if (it.first != msg.sender) {
			it.second->SendMessageToClient(msg);
		}
	}
};

bool ClientsManager::RegisterClient(string name, ClientSocket* sock)
{
	if (name == "server" || name=="") {
		sock->SendMessageToClient(Message("BAD_USERNAME", "server", name, "Username can't be "+name));
		return false;
	}
	for (auto const it : clients) {
		if (it.first == name) {
			sock->SendMessageToClient(Message("BAD_USERNAME","server",name,"Username exist"));
			return false;
		}
	}
	sock->SendMessageToClient(Message("LOGIN_OK","server",name,""));
	clients.emplace(make_pair(name, sock));
	SendOnlineInfo(name);
	SendToAll(Message("NEW_ONLINE_USER","server","ALL",name));
	return true;
}
bool ClientsManager::RemoveClient(string name) {
	auto it = clients.find(name);
	if (it != clients.end()) {
		it->second->SendMessageToClient(Message("LOGOUT_OK","server",name,""));
		delete it->second;
		clients.erase(it);
		SendToAll(Message("USER_LOGOUT","server","ALL",name));
		return true;
	}
	return false;
}
void ClientsManager::SendMessageToAll(string sender, string message) {
	Message msg("MESSAGE", sender, "ALL", message);
	SendToAll(msg);
}
void ClientsManager::SendMessageTo(string sender,string receiver, string message) {
	Message msg("MESSAGE", sender, receiver, message);
	Send(msg);
}
void ClientsManager::SendOnlineInfo(string sender) {
	Message response("ONLINE_USERS", "server", sender, "");
	for (const auto &it : clients) {
		response.data +=it.first+":";
	}
	response.data = response.data.substr(0, response.data.size() - 1);
	cout << response.data << endl;
	Send(response);
}
void ClientsManager::ServerQuitInfo() {
	SendToAll(Message("SERVER_OFF", "server", "ALL", ""));
}


