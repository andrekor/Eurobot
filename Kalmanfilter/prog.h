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
		void setPrevMeasure(std::string);
		time_t getTime();
		std::string getDistanceSone1();
		std::string getDistanceSone2();
		std::string getDistanceSone3();
		Serial *serialDistance; 	
		Serial *serialBeacon;
		marioKalman *mario;
	private:
		std::string distance1; //Mid sensor for enemy detection
		std::string distance2; //In the back
		std::string distance3; //The lower sensor
		std::string prevMeasure; //The previouse values from beacons
		time_t timeSincePrevMeasure; //contains the time since previous measure
};