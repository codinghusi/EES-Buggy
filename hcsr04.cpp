#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "hcsr04.h"
#include <thread>
#include <iostream>

//TODO denglisch beseitigen, Header Datei, Klasse


void HCSR04::chronometryInterrupt()
{
   
    auto timediff = std::chrono::high_resolution_clock::now() - start;

    long long timediff_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timediff).count();

    //std::cout << "Zeitdifferenz:" << timediff_microseconds << std::endl;

    distance_result = timediff_microseconds  * (34320. / 2000000.0) - 40.;
    wait_for_echo = false;


}

void HCSR04::config(void (*handler)()) {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(bremslicht, OUTPUT);
    //Interrupt fuer den Interrupt initalisieren
    wiringPiISR(echo, INT_EDGE_FALLING, handler);

}


void HCSR04::distance_messung() {
    
    //für 10 Microsekunden HighLevel an Trigger um signal auszulesen
    //digitalWrite(trigger,LOW);
    //std::this_thread::sleep_for(std::chrono::microseconds(2));
    digitalWrite(trigger,HIGH);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    digitalWrite(trigger, LOW);

    start = std::chrono::high_resolution_clock::now();
    //std::cout << "Startwert:"<<std::chrono::duration_cast<std::chrono::microseconds>(start).count() << std::endl;
    wait_for_echo = true;
    
}



void HCSR04::notbremsung() {
    //Anhalteweg = Reaktionszeit + Bremsweg
    //Reaktionszeit beim Menschen = Geschwindigkeit[km/h]*0,3
    //Bremsweg  = Geschwindigkeit[km/h]*0,1*Geschwindigkeit[km/h]*0,1
    //Notbremsung halbiert den Bremsweg

}

void HCSR04::bremslicht_ein() {
    digitalWrite(bremslicht, HIGH);
}

void HCSR04::bremslicht_aus() {
    digitalWrite(bremslicht, LOW);
}

