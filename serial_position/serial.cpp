#include "serial.h"
//TODO: try catch
Serial::Serial(std::string serial_port) {
	serial.Open(serial_port.c_str());
	serial.SetBaudRate( SerialStreamBuf::BAUD_38400 );
	serial.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
	serial.SetNumOfStopBits( SerialStreamBuf::DEFAULT_NO_OF_STOP_BITS );
	serial.SetParity( SerialStreamBuf::PARITY_NONE );
	serial.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ); //FLOW_CONTROL_HARD
}

Serial::~Serial() {
	serial.Close();
}

void Serial::write(uint8_t arg) {
	serial << arg;
}

void Serial::write(char arg) {
	serial << arg;
}

void Serial::write(int arg) {
	write((uint8_t) arg); //hack!!!!
}

uint8_t Serial::readNoWait(){
	int r = 0;
	if(available()) {
		r = serial.get();
	} else {
		usleep(SERIAL_DELAY);
		if(available()) {
			r = serial.get();
		}
	}
	return r;
}

uint8_t Serial::read() {
	auto t_start = std::chrono::high_resolution_clock::now();
	auto t_end = std::chrono::high_resolution_clock::now();
	uint8_t byte = 0;
	double timepassed = std::chrono::duration<double, std::milli>(t_end-t_start).count();
	while(timepassed < 1000) {
		if(available()) {
			byte = readNoWait();
			break;
		}
		t_end = std::chrono::high_resolution_clock::now();
		timepassed = std::chrono::duration<double, std::milli>(t_end-t_start).count();
	}
	return byte;
}

long Serial::readLong() {
	auto t_start = std::chrono::high_resolution_clock::now();
	auto t_end = std::chrono::high_resolution_clock::now();
	uint8_t bytes[4];
	std::fill(std::begin(bytes), std::end(bytes), 0);
	for(int i = 0; i < 4; i++) {
		double timepassed = std::chrono::duration<double, std::milli>(t_end-t_start).count();
		while(timepassed < 100) {
			if(available()) {
				bytes[i] = readNoWait();
				break;
			}
		t_end = std::chrono::high_resolution_clock::now();
		timepassed = std::chrono::duration<double, std::milli>(t_end-t_start).count();
	}
}

}

bool Serial::available() {
	return serial.rdbuf()->in_avail();
}

void Serial::printAll() {
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
}