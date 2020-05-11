#include "Chat.h"

using namespace std;
extern volatile bool work;
 
bool Chat::LogIn(std::string userName) {
	reciever = new Socket(addr, port);
	reciever->SendMessageToServer("LogIn:-:"+ userName);
	string response = reciever->GetMessageFromServer();
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
	delete reciever;
	return false;
}

void Chat::WaitForResponse() {
	int i = 0;
	while (work) {
		string message = reciever->GetMessageFromServer();
		cout<<message<<endl<<">";
		if (message == "SERVER_OFF" || message == "NO_RECEIVE" ) {
			cout<< "Exiting.." << endl;
			work = false;
		}
	}
	delete reciever;
}

void Chat::TurnOnWinSockets() {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't start winsock ERROR # " << wsOk << endl;
	}
}