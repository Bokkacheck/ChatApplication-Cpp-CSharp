#pragma once
#include<string>
#include<vector>
class Message {
public:
	std::string request = "";
	std::string sender = "";
	std::string receiver = "";
	std::string data = "";

	Message() = default;
	Message(std::string request, std::string sender, std::string receiver, std::string data) :
		request(request), sender(sender), receiver(receiver), data(data) {}
	std::string MakeString() {
		return request + ":-:" + sender + ":-:" + receiver + ":-:" + data;
	}
	Message(std::string message);
	std::vector<std::string> Split(std::string original, const std::string& regex);
};