#pragma once
#include"Message.h"

using namespace std;

const string Message::sep = ":-:";

Message::Message(string message) {
	vector<string> splited = Split(message, sep);
	request = splited[0];
	sender = splited[1];
	receiver = splited[2];
	data = splited[3];
}
vector<string> Message::Split(string original, const string& regex) {
	vector<string> res;
	size_t pos = 0;
	string token;
	while ((pos = original.find(regex)) != string::npos) {
		token = original.substr(0, pos);
		res.push_back(token);
		original.erase(0, pos + regex.length());
	}
	res.push_back(original);
	return res;
}