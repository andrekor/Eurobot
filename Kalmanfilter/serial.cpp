/*
* File: serial.cpp
* Author: André Kramer Orten
*
* Serial implements functions for communicating to the MD49 motor controller via serial.
* Can be run at either 9600 or 38400 baud (IMPORTANT: changed with hardware pins on the controller.)
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
#include "serial.h"

Serial::Serial(std::string serial_port) {
	// Create and open the serial port for communication. 
	//SerialStream serial; 
	serial.Open(serial_port.c_str());
	// The various available baud rates are defined in SerialStreamBuf class. 
	//All serial port settings will be placed in
	// in the SerialPort class.
	serial.SetBaudRate( SerialStreamBuf::BAUD_9600);
	serial.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
	serial.SetNumOfStopBits( SerialStreamBuf::DEFAULT_NO_OF_STOP_BITS );
	serial.SetParity( SerialStreamBuf::PARITY_NONE );
	serial.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ); //FLOW_CONTROL_HARD	
}

void Serial::openSerial(std::string serial_port) {
	// Create and open the serial port for communication. 
	//SerialStream serial; 
	serial.Open( serial_port.c_str() );
	// The various available baud rates are defined in SerialStreamBuf class. 
	//All serial port settings will be placed in
	// in the SerialPort class.
	serial.SetBaudRate( SerialStreamBuf::BAUD_9600);
	serial.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
	serial.SetNumOfStopBits( SerialStreamBuf::DEFAULT_NO_OF_STOP_BITS );
	serial.SetParity( SerialStreamBuf::PARITY_NONE );
	serial.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ); //FLOW_CONTROL_HARD		
}

Serial::~Serial(){
	serial.Close();
}

bool Serial::available() {
	return serial.rdbuf()->in_avail();
}

/*
Reads one line, and 
*/
std::string Serial::readLine() {
	uint8_t next_char = 0x00;
	//std::stringstream ss;
	std::string s = "";
	if (available()) {
		while(next_char != '\n') {
			next_char = serial.get();
			if (next_char == 255)
				return "";
			s.push_back(next_char);
			if (next_char == '\n') {
				//std::cout << "break line" << std::endl;
				break;
			}
		}
		//std::cout << s;
	}
	return s;

}


void Serial::readChar() {
	char next_char;
	serial.get(next_char);
	std::stringstream ss;
	std::string s;
	ss << next_char;
	s = ss.str();
	double dist = atof(s.c_str());
	std::cout << dist;
}

void Serial::readAll() {
	std::stringstream ss;
	int itr = 0;
	uint8_t b = 0;
	while(available()) {
		b = serial.get();
		ss << (int) b;
		(void) b;
		itr++;
		if(itr > 100) {
			break;
		}
	}
	std::cout << ss << std::endl;
}