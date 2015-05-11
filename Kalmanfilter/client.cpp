/*
* File: client.cpp
* Author: André Kramer Orten
*
* Protocoll for interaction with the server for EurobotUiO 2015 is
* in             | What                                   |  return value 
* ------------------------------------------------------------------
* "1,X,Y,theta"  | Position estimate. (Kalmanfilterd pos) |  X,Y,theta
* "2"	         | Distance from sone 1	                  |  Distance
* "3"	         | Distance from sone 2                   |  Distance
* "4"	         | Distance from sone 3                   |  Distance
* ------------------------------------------------------------------
*
* Copyright (c) 2015 André Kramer Orten. All Rights Reserved.
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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with EurobotUiO. If not, see <http://www.gnu.org/licenses/>.
*/


#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

double xPos = 50.03;
double yPos = 60.44;
double theta = 90.3;
int main() {

	//convert the position to string to send it
	std::stringstream ss;
	ss << "4," << xPos << "," << yPos << "," << theta;
	std::string result = ss.str();

	//Prepare the context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);
	socket.connect("tcp://localhost:5900");

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