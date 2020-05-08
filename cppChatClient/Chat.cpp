#include "Chat.h"

using namespace std;
 
bool Chat::LogIn(std::string userName) {
	reciever = Socket(addr, port);
	reciever.SendMessageToServer("LogIn:-:"+ userName);
	string response = reciever.GetMessageFromServer();
	if (response == "LOGIN_OK") {
		std::cout << "Successfully logged" << endl;
		this->userName = userName;
		responseWaiting = async(std::launch::async, &Chat::WaitForResponse,this);
		return true;
	}
	else if(response == "USERNAME_EXIST"){
		cout << "User name already exist, try somethig else" << endl;
	}
	else {
		cout << "Unknown error" << endl;
	}
	return false;
}

void Chat::WaitForResponse() {
	while (true) {
		cout<<reciever.GetMessageFromServer()<<endl<<">";
	}
}

void Chat::TurnOnWinSockets() {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't start winsock ERROR # " << wsOk << endl;
	}
}