#include <stdint.h>
#include "../config.h"
#include <chrono>

class HCSR04 {
private:
	double distance_result;
	std::chrono::high_resolution_clock::time_point start;
	bool wait_for_echo;
	uint8_t trigger;
	uint8_t echo;
	uint8_t brake_light;
 
public:
	//Pin 3 in wp ist gpio22 motorhead
	HCSR04(uint8_t t = ECHO, uint8_t e = TRIGGER, uint8_t bl = BRAKE_LIGHT);

	void config(void (*handler)());
	void distance_measurement();
	void chronometry_interrupt();
	void brake_light_on();
	void brake_light_off();
	
	inline double get_distance_result() {
		return distance_result;
	}
	
	inline void set_distance_result(double d) {
		distance_result = d;
	}

	inline std::chrono::high_resolution_clock::time_point get_start() {
		return start;
	}

	bool get_wait_for_echo();
};
