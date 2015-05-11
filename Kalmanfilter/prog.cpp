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

#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

void readDistance(Prog *p);
void server(Prog *p);
std::string handleZMQInput(Prog *p, std::string input);
std::string kalmanPos(std::string, marioKalman *m);
std::vector<double> split(std::string s, char c);

/*Fetches the serial input. If its valid 
result it puts it sets the distance*/
void readDistance(Prog *p) {
	while(1) {
		usleep(50000); 
		std::string a;
		a = p->serialDistance->readLine();
		if (a.length() > 1) {
			std::string d1 = "0"; // temp
			std::string d2 = "0"; // temp
			std::string d3 = "0"; // temp
			int count;
			if (a.find("A") >= 0) {
				count = (a.find("B")-1)-a.find("A");
				d1 = a.substr(a.find("A")+1, count); // sone 1
			}
			if (a.find("B") > 0) {
				count = (a.find("C")-1)-a.find("B");
				d2 = a.substr(a.find("B")+1, count); // sone 2
			}
			if (a.find("C") > 0) {
				count = (a.length()-1)-a.find("C");
				d3 = a.substr(a.find("C")+1, count); // sone 3
			}
 			//sets the distance variable in Prog
 			p->setDistance(d1, d2, d3);
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
		std::cout << rp1 << std::endl;
		char c = rp1[0]; //The ID of the request
		std::string result = "Invalid"; //if no valid ID
		if (c == '1') {
			int count = (rp1.length()-1)-rp1.find(",");
			std::string stringPos = rp1.substr(rp1.find(",")+1, count);
			time_t now;
			time(&now); // same as now = time(NULL)
			if ((now - p->getTime())< 5) {
				result = kalmanPos(stringPos, p->mario);
				//std::cout << "Reply with kalman position " << result << std::endl;
			} else {
				result = stringPos;
				std::cout << "Reply with position " << result << std::endl;
			}
		}
		else if (c == '2') {
			result = p->getDistanceSone1(); //sone 1
			std::cout << "Reply with distance: " << result << std::endl;
		}
		else if (c == '3') {
			result = p->getDistanceSone2(); //sone 2
			std::cout << "Reply with distance: " << result << std::endl;
		}
		else if (c == '4') {
			std::cout << "Reply with distance: " << result << std::endl;
			result = p->getDistanceSone3();//sone 3
		}
		zmq::message_t reply(result.length());
		memcpy ((void *) reply.data(), result.c_str(), result.length());
		socket.send(reply);
	}
}

std::string kalmanPos(std::string position, marioKalman *mario) {
	//marioKalman *mario = new marioKalman(); //Kalmanfilter

	mat temp(3, 1); //temp matrice
	std::string s = position.substr(position.find(",")+1, position.length()); //contains y,theta
	double x = std::stod(position.substr(0, position.find(","))); 
    double y = std::stod(s.substr(0, s.find(",")));
    double theta = std::stod(s.substr(s.find(",")+1, s.length()));

	//std::cout << x << ", " << y << ", " << theta << std::endl;
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
	//ss << pos(0);
	ss << pos(0) << "," << pos(1) << "," << pos(2);
	return ss.str();
}


/*Not sure if i should use this function, or just do it inside the server function*/
std::string handleZMQInput(Prog *p, std::string input) {
	std::cout << "Request: " << input << std::endl;
	char c;
	c = input[0];
	if (c == '1')
		std::cout << "Position from kalmanfilter with the rest of the input" << std::endl;
	else {
		if (c == '2')
			return p->getDistanceSone1();
		if (c == '3')
			return p->getDistanceSone2(); //Shoudl be sone 2
		if (c == '4')
			return p->getDistanceSone3(); //shoudl be sone 3
	}
	return "Invalid";
}


Prog::~Prog() {}

Prog::Prog() {
	serialDistance = new Serial("/dev/ttyUSB1"); //opens the communication to the distance arduino
	serialBeacon = new Serial("/dev/ttyUSB0");  //opens the communication to the position arduino
	distance1 = "0";
	distance2 = "0";
	distance3 = "0";
	prevMeasure = "0";
	time(&timeSincePrevMeasure); //initialize to something high
	mario = new marioKalman(); //Initialize the kalman filter
	mario->setMeasure(22.0,100.0,0.0);
}

/*
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
*/

void Prog::setPrevMeasure(std::string measure) {
	time_t now;
	prevMeasure = measure;
	time(&now);  /* get current time; same as: now = time(NULL)  */
	std::vector<double> vec = split(measure, ',');
//	std::cout << "measure: " << measure << " vec size: " << vec.size() <<std::endl;
	if (vec.size() == 3) {
		mario->setMeasure(vec[0], vec[1], vec[2]);
		std::cout << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
		timeSincePrevMeasure = now;
	}
}

time_t Prog::getTime() {
	return timeSincePrevMeasure;
}

void Prog::setDistance(std::string dis1, std::string dis2, std::string dis3) {
	//std::cout << dis1 << "  -  " << dis2 << "  -  " << dis3 << std::endl;
	distance1 = dis1;
	distance2 = dis2;
	distance3 = dis3;
}
/*
std::string Prog::getDistance() {
	return distance1;
}
*/
std::string Prog::getDistanceSone1() {
	return distance1;
}

std::string Prog::getDistanceSone2() {
	return distance2;
}

std::string Prog::getDistanceSone3() {
	return distance3;
}

std::vector<double> split(std::string input, char c) {
 	std::vector<double> args;
	std::istringstream f(input);
	std::string s;
	while(getline(f, s, ',')) {
		args.push_back(atof(s.c_str()));
	//	std::cout << s.c_str() << std::endl;
	}
	return args;
}

void beaconPos(Prog *p) {
	while(1) {
		usleep(1000); 
		std::string a;
		a = p->serialBeacon->readLine(); //reads the line from the Position arduino
		if (a.length() > 0) {
			p->setPrevMeasure(a);
		}
	}
}

int main() {
	Prog *p = new Prog();

	//Threads the distance reader and the zmq server
	std::thread distance(readDistance, p);
	std::thread zmq(server, p);
	std::thread beacon(beaconPos, p);

	//joins the threads with the main thread(kills the thread)
	if (distance.joinable())
		distance.join();
	if (zmq.joinable())
		zmq.join();
	if (beacon.joinable()) {
		beacon.join();
	}
} 
