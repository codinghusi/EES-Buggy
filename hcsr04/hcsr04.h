#include <stdint.h>
#include <chrono>

class HCSR04 {
private:
	double distance_result;
	std::chrono::high_resolution_clock::time_point start;
	bool wait_for_echo;
	uint8_t trigger;
	uint8_t echo;
	uint8_t bremslicht;



public:
//Pin 3 in wp ist gpio22 motorhead
	HCSR04(uint8_t t = 3, uint8_t e = 4, uint8_t bl=5) {
		distance_result = 450;
		wait_for_echo = false;
		start = std::chrono::high_resolution_clock::now();
		trigger = t;
		echo = e;
		bremslicht = bl;
	}

	void config(void (*handler)());
	void distance_messung();
	void chronometryInterrupt();
	void notbremsung();
	void bremslicht_ein();
	void bremslicht_aus();
	double get_distanceresult(){return distance_result;}
	void set_distanceresult(double d){distance_result=d;}
	bool get_waitforecho(){
		auto timediff = std::chrono::high_resolution_clock::now() - start;
        long long timediff_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timediff).count();
		if(timediff_microseconds > 26224)
			wait_for_echo =false;

		return wait_for_echo;}
	std::chrono::high_resolution_clock::time_point get_start(){return start;}


};
