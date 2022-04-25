#ifndef write_event
#define write_event
#include "write_event.h"
#include <Particle.h>

// sets if events should be published to serial as well as the Particle Cloud
const bool      LIVE_PUBLISH        = true;
const bool      MIRROR_TO_SERIAL    = true;
const double    MESSAGE_INTERVAL    = 1500;

//Write an event to the cloud and/or the serial output depending on settings
void writeEvent( String text ){
  if ( LIVE_PUBLISH == true){
    Particle.publish(text);
  }
  if ( MIRROR_TO_SERIAL == true){
    Serial.println(text);
  }
}

// Write an event to the cloud and/or the serial output depending on settings
// overloaded to accept and print data
void writeEvent( String text, float data ){
    if ( LIVE_PUBLISH == true){
        Particle.publish(text, String::format("%f", data), PRIVATE);
    }
    if ( MIRROR_TO_SERIAL == true){
        Serial.print(text);
        Serial.print(" ");
        Serial.println(data);
    }
}

void writeEvent( String text, String data ){
  
    if ( LIVE_PUBLISH == true){
        Particle.publish(text, data, PRIVATE);
    }
    if ( MIRROR_TO_SERIAL == true){
        Serial.print(text);
        Serial.print(" ");
        Serial.println(data);
  }
}

float writeWarning( String text, float lastUpdate){
    if ( (millis() - lastUpdate ) > MESSAGE_INTERVAL || lastUpdate == 0 ){
    writeEvent(text);
    return millis();
    }
    return lastUpdate;
}

// considers time since last update before calling writeEvent
float writeWarning( String text, String data,  float lastUpdate){
    if ( (millis() - lastUpdate ) > MESSAGE_INTERVAL || lastUpdate == 0 ){
    writeEvent(text, data);
    return millis();
    }
    return lastUpdate;
}

// considers time since last update before calling writeEvent
float writeWarning( String text, float data,  float lastUpdate){
    if ( (millis() - lastUpdate ) > MESSAGE_INTERVAL || lastUpdate == 0 ){
    writeEvent(text, data);
    return millis();
    }
    return lastUpdate;
}

#endif