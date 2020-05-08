#include<iostream>
#include<string>
#include<iostream>
#include"Chat.h"
using namespace std;

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
	while (true) {
		cout << ">";
		getline(cin, input);
		for (int i = 0; i < 20; i++) {
			chat.SendMessageToServer(input);
		}
	}
	Chat::TurnOffWinSockets();
}