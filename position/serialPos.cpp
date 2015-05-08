#include <SerialStream.h>
#include <SerialStream.h>
#include <string>
#include <stdlib.h>     /* atof */
#include <sstream>      // std::stringstream
#include <stdint.h> 	//uint_8t
#include <iostream>    // std::cout, std::endl

void openSerial();
void closeSerial();
bool available();
void readAll();
void readChar();
void readLine();

using namespace LibSerial;
SerialStream serial;

int main() {
	openSerial();
	while(1) {
	//	readAll();
		readLine();
	}
	closeSerial();
}


void openSerial() {
	// Create and open the serial port for communication. 
	//SerialStream serial; 
	serial.Open( "/dev/ttyUSB8" );
	// The various available baud rates are defined in SerialStreamBuf class. 
	//All serial port settings will be placed in
	// in the SerialPort class.
	serial.SetBaudRate( SerialStreamBuf::BAUD_9600);
	serial.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
	serial.SetNumOfStopBits( SerialStreamBuf::DEFAULT_NO_OF_STOP_BITS );
	serial.SetParity( SerialStreamBuf::PARITY_NONE );
	serial.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ); //FLOW_CONTROL_HARD	
}

void closeSerial(){
	serial.Close();
}

bool available() {
	return serial.rdbuf()->in_avail();
}

void readLine() {
	uint8_t next_char = 0x00;
	//std::stringstream ss;
	std::string s;
	if (available()) {
		while(next_char != '\n') {
			next_char = serial.get();
			s.push_back(next_char);
			if (next_char == '\n') {
				std::cout << "break line" << std::endl;
				break;
			}
		}
		std::cout << s;
	}

}

void readChar() {
	char next_char;
	serial.get(next_char);
	std::stringstream ss;
	std::string s;
	//int data;+
	//serial >> data;
	//std::cout << next_char;
	ss << next_char;
	s = ss.str();
	double dist = atof(s.c_str());
	//double dist2 = std::stod(next_char, 0);
	std::cout << dist;
}

void readAll() {
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