#include "marioKalman.h"
#include "serial.h"
#include <thread> //std::thread

class Prog {
	public:
		Prog();
		~Prog();
		void server();
		void distance();
		void setDistance(std::string, std::string, std::string);
		std::string getDistance();
		std::string getDistanceSone1();
		std::string getDistanceSone2();
		std::string getDistanceSone3();
		Serial *serial; 	
	private:
		std::string distance1; //Mid sensor for enemy detection
		std::string distance2; //In the back
		std::string distance3; //The lower sensor
};