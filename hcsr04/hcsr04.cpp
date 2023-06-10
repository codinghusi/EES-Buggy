#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "hcsr04.h"
#include <thread>
#include <iostream>

//TODO denglisch beseitigen, Header Datei, Klasse


void HCSR04::chronometryInterrupt()
{

    if (digitalRead(echo))
    {
        start = std::chrono::high_resolution_clock::now();
    }
    else {
        auto timediff = std::chrono::high_resolution_clock::now() - start;

        long long timediff_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timediff).count();

        //std::cout << "Zeitdifferenz:" << timediff_microseconds << std::endl;

        distance_result = timediff_microseconds * (34320. / 2000000.0);
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
    
    //für 10 Microsekunden HighLevel an Trigger um signal auszulesen
    digitalWrite(trigger,LOW);
    std::this_thread::sleep_for(std::chrono::microseconds(2));
    digitalWrite(trigger,HIGH);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
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

