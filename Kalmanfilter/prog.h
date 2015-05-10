#include "marioKalman.h"
#include "serial.h"
#include <thread> //std::thread

class Prog {
	public:
		Prog();
		~Prog();
		void server();
		void distance();
		void setDistance(std::string);
		std::string getDistance();
		Serial *serial; 	
	private:
		std::string distance1; //Mid sensor for enemy detection
		std::string distance2; //Some other distance sensor
		std::string distance3; //Some other distance sensor
};