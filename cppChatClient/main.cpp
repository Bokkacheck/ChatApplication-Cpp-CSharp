#include<iostream>
#include<string>
#include<iostream>
#include"Chat.h"
using namespace std;

volatile bool work = true;

int main() {
	Chat::TurnOnWinSockets();
	Chat chat("127.0.0.1", 54000);
	string input;
	cout << "Enter username to start chat" << endl;
	while (true) {
		getline(cin, input);
		if (chat.LogIn(input)) {
			break;
		}
	}
	while (work) {
		cout << ">";
		getline(cin, input);
		chat.SendMessageToServer(input);
	}
	Chat::TurnOffWinSockets();
}