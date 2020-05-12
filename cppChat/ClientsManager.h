#pragma once

#include<iostream>
#include<map>
#include"ClientSocket.h";
#include"Message.h"

using namespace std;

class ClientsManager {
private:
    std::map<std::string, ClientSocket*> clients;

    ClientsManager() = default;

    void Send(Message msg);
    void SendToAll(Message msg);
public:
    ClientsManager(const ClientsManager& cm) = delete;
    static ClientsManager* get() {
        static ClientsManager* instance = new ClientsManager();
        return instance;
    }

    bool RegisterClient(std::string name, ClientSocket* sock);
    bool RemoveClient(std::string name);

    void SendMessageToAll(string sender, string message);
    void SendMessageTo(string sender,string receiver, string message);
    void SendOnlineInfo(string sender);
    void ServerQuitInfo();
};