#include "ClientsManager.h"

using namespace std;

bool ClientsManager::RegisterClient(std::string name, ClientSocket* sock)
{
	for (auto const it : clients) {
		if (it.first == name) {
			sock->SendMessageToClient("USERNAME_EXIST");
			return false;
		}
	}
	sock->SendMessageToClient("LOGIN_OK");
	clients.emplace(make_pair(name, sock));
	SendWhoIsOnlineInfo(name);
	SendMessageToAll(name, "NEW_ONLINE_USER:-:"+name);
	return true;
}
bool ClientsManager::RemoveClient(std::string name) {
	auto it = clients.find(name);
	if (it != clients.end()) {
		it->second->SendMessageToClient("Successful logout");
		delete it->second;
		clients.erase(it);
		SendMessageToAll(name, "USER_LOGOUT:-:" + name);
		return true;
	}
	return false;
}
void ClientsManager::SendMessageToAll(string sender, string message) {
	for (auto it : clients) {
		if (it.first != sender) {
			it.second->SendMessageToClient(message);
		}
	}
}
void ClientsManager::SendMessageTo(string sender,string receiver, string message) {
	ClientSocket* recieverSock = clients[receiver];
	if (recieverSock) {
		recieverSock->SendMessageToClient(message);
	}
	else {
		ClientSocket* senderSock = clients[sender];
		if (senderSock) {
			senderSock->SendMessageToClient("User is not connected");
		}
	}
}
void ClientsManager::SendWhoIsOnlineInfo(string sender) {
	string res = "";
	for (const auto &it : clients) {
		res += it.first +"\n";
	}
	SendMessageTo("sever", sender, res);
}


