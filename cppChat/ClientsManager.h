#pragma once

#include<iostream>
#include<map>
#include"ClientSocket.h";

using namespace std;

class ClientsManager {
private:
    std::map<std::string, ClientSocket*> clients;
    ClientsManager() = default;
public:
    ClientsManager(const ClientsManager& cm) = delete;
    static ClientsManager* get() {
        static ClientsManager* instance = new ClientsManager();
        return instance;
    }
    bool RegisterClient(std::string name, ClientSocket* sock);
    bool RemoveClient(std::string name);
    ClientSocket* GetClient(string name) {
        return clients[name];
    }
    void SendMessageToAll(string sender, string message);
};