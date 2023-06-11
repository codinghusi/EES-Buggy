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
	/**
	 * @brief Construct a new HCSR04 object
	 * @param trigger The trigger pin (to start distance measurement)
	 * @param echo The echo pin (triggers the interrupt)
	 * @param brake_light The echo pin (triggers the interrupt)
	*/
	HCSR04(uint8_t trigger = TRIGGER, uint8_t echo = ECHO, uint8_t brake_light = BRAKE_LIGHT);

	/**
	 * @param handler function pointer will be called when echo triggers its interrupt
	*/
	void config(void (*handler)());

	/**
	 * Start distance measurement.
	 * Is finished when is_waiting_for_echo() returns false.
	*/
	void distance_measurement();

	/**
	 * Call this method from echo interrupt.
	*/
	void chronometry_interrupt();

	/**
	 * Turn on brake light.
	*/
	void brake_light_on();

	/**
	 * Turn off brake light.
	*/
	void brake_light_off();
	
	/**
	 * @return latest measurement in cm
	*/
	inline double get_distance_result() {
		return distance_result;
	}
	
	/**
	 * @param d set latest measurement in cm
	*/
	inline void set_distance_result(double d) {
		distance_result = d;
	}

	/**
	 * @return time when distance measurement started
	*/
	inline std::chrono::high_resolution_clock::time_point get_start() {
		return start;
	}

	/**
	 * @return true if distance measurement is running
	*/
	bool is_waiting_for_echo();
};
