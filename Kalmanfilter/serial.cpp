#include "serial.h"

Serial::Serial() {
	// Create and open the serial port for communication. 
	//SerialStream serial; 
	serial.Open( "/dev/ttyUSB0" );
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
void Serial::readLine() {
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