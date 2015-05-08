#include <SerialStream.h>
#include <SerialStream.h>
#include <string>
#include <stdlib.h>     /* atof */
#include <sstream>      // std::stringstream
#include <stdint.h> 	//uint_8t
#include <iostream>    // std::cout, std::endl

using namespace LibSerial;
SerialStream serial;

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

};