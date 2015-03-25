#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif



int main() {
	//Prepare the context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:5555");

	while(true) {
		zmq::message_t request;
		//Wait for next request from client
		socket.recv(&request);
		std::cout << "Received Hello" << std::endl;

		//do som thing
		#ifndef _WIN32
		sleep(1);
		#else 
		Sleep(1)
		#endif

		//send reply back
		zmq::message_t reply(5);
		memcpy ((void *) reply.data(), "World", 5);
		socket.send(reply);
	}
	return 0;
}