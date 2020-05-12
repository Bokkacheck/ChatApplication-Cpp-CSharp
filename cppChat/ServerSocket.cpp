#include "ServerSocket.h"
#include<iostream>
#include<memory>

using namespace std;

ServerSocket::ServerSocket(const std::string& addr, const int port)
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't initialize winsock! Quiting" << endl;
		exit(0);
	}
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		cerr << "Can't create socket! Quiting" << endl;
		exit(0);
	}
	sockaddr_in hostInfo;
	hostInfo.sin_family = AF_INET;
	hostInfo.sin_port = htons(port);
	std::wstring stemp = std::wstring(addr.begin(), addr.end());
	LPCWSTR sw = stemp.c_str();
	inet_pton(AF_INET, addr.c_str(), &hostInfo.sin_addr);
	int bindInfo = bind(serverSocket, (sockaddr*)&hostInfo, sizeof(hostInfo));
	if (bindInfo == -1) {
		cout << "Error: " + WSAGetLastError() << endl;
	}
	int listenInfo = listen(serverSocket, SOMAXCONN);
	if (listenInfo == -1) {
		cout << "Error: " + WSAGetLastError() << endl;
	}
	cout << "Server started: address -> " << hostInfo.sin_addr.S_un.S_addr<<endl;
}

ClientSocket* ServerSocket::GetClient() {
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
	if (clientSocket == -1) {
		cout <<"Error: "+ WSAGetLastError() << endl;
	}
	return new ClientSocket(clientSocket);
}
