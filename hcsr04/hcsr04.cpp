#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "hcsr04.h"
#include <thread>
#include <iostream>

//TODO denglisch beseitigen, Header Datei, Klasse


void HCSR04::chronometry_interrupt()
{

    if (digitalRead(echo))
    {
        start = std::chrono::high_resolution_clock::now();
    }
    else {
        auto timediff = std::chrono::high_resolution_clock::now() - start;

        long long timediff_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timediff).count();

        //SONIC_SPEED in m/s 
        distance_result = timediff_microseconds * (SONIC_SPEED / 20000.0);
        
        wait_for_echo = false;

    }
    

}

void HCSR04::config(void (*handler)()) {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(brake_light, OUTPUT);
    //Interrupt initalisieren
    wiringPiISR(echo, INT_EDGE_BOTH, handler);
 
}


void HCSR04::distance_measurement() {
    
    using namespace std::chrono_literals;
    //für 10 Microsekunden HighLevel an Trigger um signal auszulesen
    digitalWrite(trigger,LOW);
    std::this_thread::sleep_for(2ms);
    digitalWrite(trigger,HIGH);
    std::this_thread::sleep_for(10ms);
    digitalWrite(trigger, LOW);

    
    //std::cout << "Startwert:"<<std::chrono::duration_cast<std::chrono::microseconds>(start).count() << std::endl;
    wait_for_echo = true;
    
}


void HCSR04::brake_light_on() {
    digitalWrite(brake_light, HIGH);
}

void HCSR04::brake_light_off() {
    digitalWrite(brake_light, LOW);
}

