#include"Socket.h"
#include<iostream>

using namespace std;

Socket::Socket(const string& addr, const int port) {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create socket ERROR #" << WSAGetLastError() << endl;
		WSACleanup();
		exit(0);
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, addr.c_str(), &hint.sin_addr);

	int conResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (conResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, ERROR #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		exit(0);
	}
}

void Socket::SendMessageToServer(const string& message) {
	send(sock, message.c_str(), message.size() + 1, 0);
}
string Socket::GetMessageFromServer() {
	char buf[4096];
	ZeroMemory(buf,4096);
	int byteRecieved = recv(sock, buf, 4096, 0);
	if (byteRecieved <= 0) {
		return "NO_RECIVE";
	}
	else {
		return string(buf, 0, byteRecieved);
	}
}
