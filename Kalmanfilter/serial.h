/*
* File: serial.h
* Author: André Kramer Orten
*
* Serial implements functions for interaction with the sensors that sends there
* information over the serial line. 
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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with EurobotUiO. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SERIAL_H
#define	SERIAL_H

#include <SerialStream.h>
#include <SerialStream.h>
#include <string>
#include <stdlib.h>     /* atof */
#include <sstream>      // std::stringstream
#include <stdint.h> 	//uint_8t
#include <iostream>    // std::cout, std::endl

using namespace LibSerial;

class Serial {
	public:
		Serial(std::string serial_port);
		~Serial();
		void openSerial(std::string serial_port);
		//void closeSerial();
		bool available();
		void readAll();
		void readChar();
		std::string readLine();
		void closeSerial();
	private:
		SerialStream serial;
};
#endif