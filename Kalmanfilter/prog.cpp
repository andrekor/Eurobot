#include "marioKalman.h"
#include "serial.h"

void server();

int main() {
	Serial *s = new Serial();	
	marioKalman *m = new marioKalman();
	server();
	/*while(1) {
		s->readLine();
	}*/
//	m->server();
} 

/**/
void server() {
	//Prepare the context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:5900");
	marioKalman *mario = new marioKalman();

	while(true) {
		zmq::message_t request;
		//Wait for next request from client
		socket.recv(&request);
		//Her should we receive the position from the encoders
		//and calculate the kalman position, before replying with this.
		//std::cout << "Received Hello" << std::endl;

		sleep(1);
		//Fetch the position from the client
		std::string rp1 = std::string(static_cast<char*>(request.data()), request.size());
		std::cout << rp1 << std::endl;

		//Here should we calculate the positio, with rp1, as pos from encoders

		//convert the position to string to send it
		std::stringstream ss;
		mat pos = mario->getState(); //Gets the state
		ss << pos(0) << "," << pos(1) << "," << pos(2) << ",0";
		std::string result = ss.str();

		zmq::message_t reply(result.length());
		memcpy ((void *) reply.data(), result.c_str(), result.length());
		socket.send(reply);
	}
}

