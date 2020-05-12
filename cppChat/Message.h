#pragma once
#include<string>
#include<vector>
class Message {
private:
	static const std::string sep;
	std::vector<std::string> Split(std::string original, const std::string& regex);
public:
	std::string request = "";
	std::string sender = "";
	std::string receiver = "";
	std::string data = "";

	Message() = default;
	Message(std::string request, std::string sender, std::string receiver, std::string data) :
		request(request), sender(sender), receiver(receiver), data(data) {}
	std::string MakeString() {
		return request + sep + sender + sep + receiver + sep + data;
	}
	operator std::string() {
		return MakeString();
	}
	Message(std::string message);
};
