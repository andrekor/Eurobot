/*
* File: prog.cpp
* Author: André Kramer Orten
*
* Contains the server from where the interaction between the robot and its 
* sensors is handled.
*
* Copyright (c) 2015 André Kramer Orten <andrekor@ifi.uio.no>. All Rights Reserved.
*
* This file is part of EurobotUiO.
*
* EurobotUiO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* EurobotUiO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See thread
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with EurobotUiO. If not, see <http://www.gnu.org/licenses/>.
*/

#include "prog.h"

void readDistance(Prog *p);
void server(Prog *p);
std::string handleZMQInput(Prog *p, std::string input);
std::string kalmanPos(std::string);

/*Fetches the serial input. If its valid 
result it puts it sets the distance*/
void readDistance(Prog *p) {
	while(1) {
		usleep(100000);
		std::string a;
		a = p->serial->readLine();
		if (a.length() > 1) {
 			//sets the distance variable in Prog
 			p->setDistance(a);
 		}
	}
}

/*Depending on the request, the server
should respond with either the position, 
or the distances from the distance sensors*/
void server(Prog *p) {
	//Prepare the context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:5900");
	while(true) {
		zmq::message_t request;
		//Wait for next request from client
		socket.recv(&request);
		usleep(500000); //sleeps for half a second
		
		//Fetch the request from client
		std::string rp1 = std::string(static_cast<char*>(request.data()), request.size());
		//std::cout << rp1 << std::endl;
		char c = rp1[0]; //The ID of the request
		std::string result = "Invalid"; //if no valid ID
		if (c == '1') {
			std::string stringPos = rp1.substr(rp1.find(",")+1, rp1.length());
			/*
			std::cout << stringPos << std::endl;
			std::string s1 = stringPos.substr(0, stringPos.find(","));
			std::string s2 = stringPos.substr(stringPos.find(",")+1, stringPos.length());
			std::cout << s1 << " - " << s2 << std::endl;
			*/
			result = kalmanPos(stringPos);
		}
		else if (c == '2') {
			std::cout << "Reply with distance: " << result << std::endl;
			result = p->getDistance(); //sone 1
		}
		else if (c == '3') {
			std::cout << "Reply with distance: " << result << std::endl;
			result = p->getDistance(); //sone 2
		}
		else if (c == '4') {
			result = p->getDistance();//sone 3
			std::cout << "Reply with distance: " << result << std::endl;
		}

		std::stringstream ss;
		//std::string result = ss.str();
		zmq::message_t reply(result.length());
		memcpy ((void *) reply.data(), result.c_str(), result.length());
		socket.send(reply);
	}
}

std::string kalmanPos(std::string position) {
	marioKalman *mario = new marioKalman(); //Kalmanfilter
	mat temp(3, 1); //temp matrice
	std::string s = position.substr(position.find(",")+1, position.length()); //contains y,theta
	double x = std::stod(position.substr(0, position.find(","))); 
    double y = std::stod(s.substr(0, s.find(",")));
    double theta = std::stod(s.substr(s.find(",")+1, s.length()));

	std::cout << x << ", " << y << ", " << theta << std::endl;
	temp(0,0) = x;
	temp(1, 0) = y;
	temp(1, 0) = theta;
	//double x = atof(position.substr(0, position.find(",")));
	//double y = atof(position.substr(position.find(",")+1, position.length()));
	
	mario->setState(temp); //Sets the state from encoders
	mario->predict(); //runs the predict phase
	mario->update(); // runs the update phase
	mat pos = mario->getState(); //Gets the state
	std::stringstream ss;
	ss << pos(0) << "," << pos(1) << "," << pos(2) << ",0";
	return ss.str();
}


/*Not sure if i should use this function, or just do it inside the server function*/
std::string handleZMQInput(Prog *p, std::string input) {
	std::cout << "Request: " << input << std::endl;
	char c;
	c = input[0];
	if (c == '1')
		std::cout << "Position from kalmanfilter with the rest of the input" << std::endl;
	if (c == '2')
		return p->getDistance();
	if (c == '3')
		return p->getDistance(); //Shoudl be sone 2
	if (c == '4')
		return p->getDistance(); //shoudl be sone 3
	return "Invalid";
}


Prog::~Prog() {}

Prog::Prog() {
	serial = new Serial("/dev/ttyUSB3"); 
}

/**/
void Prog::server() {
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
		std::cout << "Reply with distance: " << ss << std::endl;
		zmq::message_t reply(result.length());
		memcpy ((void *) reply.data(), result.c_str(), result.length());
		socket.send(reply);
	}
}

void Prog::setDistance(std::string dis) {
	distance1 = dis;
}

std::string Prog::getDistance() {
	return distance1;
}


int main() {
	Prog *p = new Prog();

	//Threads the distance reader and the zmq server
	std::thread distance(readDistance, p);
	std::thread zmq(server, p);

	//joins the threads with the main thread(kills the thread)
	if (distance.joinable())
		distance.join();
	if (zmq.joinable())
		zmq.join();
} 
