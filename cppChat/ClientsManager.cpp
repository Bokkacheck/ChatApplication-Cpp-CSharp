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
	return true;
}
bool ClientsManager::RemoveClient(std::string name) {
	auto it = clients.find(name);
	if (it != clients.end()) {
		it->second->SendMessageToClient("Successful logout");
		delete it->second;
		clients.erase(it);
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
