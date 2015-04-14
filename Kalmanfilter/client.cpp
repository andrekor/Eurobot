#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

int xPos = 50;
int yPos = 50;
int main() {

	//convert the position to string to send it
	std::stringstream ss;
	ss << xPos << "," << yPos;
	std::string result = ss.str();

	//Prepare the context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);
	socket.connect("tcp://localhost:5555");

	//Make message ready to send
	//std::vector<char> cvec(result.begin(), std.end());

	zmq::message_t request(result.length()); //size
	memcpy((void*) request.data(), result.c_str(), result.length());
	std::cout << "Sending Hello" << std::endl;
	socket.send(request);

	//Get the reply
	zmq::message_t reply;
	socket.recv (&reply);

	std::string rp1 = std::string(static_cast<char*>(reply.data()), reply.size());
	std::cout << rp1 << std::endl;
	return 0;
}