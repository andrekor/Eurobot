#include <string>
#include <SerialStream.h>
using namespace LibSerial;

#define SERIAL_DELAY 500
	class Serial {
		public:
			Serial(std::string serial_port);
			~Serial();
			// Overloaded write, shifts arg onto serial
			void write(uint8_t arg);
			void write(char arg);
			void write(int arg);
			// Waits for serial to be available, then writes. Timeout after set time.
			uint8_t read();
			// Reads without the wait (useful for burst-read of multi-byte variables).
			uint8_t readNoWait();
			//
			long readLong();
			// Reads and prints everything currently on the serial.
			void printAll();
			// True if serial has byte ready to be red.
			bool available();
		private:
			SerialStream serial;
	};

