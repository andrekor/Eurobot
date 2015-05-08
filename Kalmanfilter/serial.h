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
		Serial();
		~Serial();
		void openSerial();
		//void closeSerial();
		bool available();
		void readAll();
		void readChar();
		void readLine();
	private:
		SerialStream serial;
};
#endif