// write_event.h
#ifndef write_event
#define write_event
#include <Particle.h>



//Write an event to the cloud and/or the serial output depending on settings
void writeEvent( String text );

// Write an event to the cloud and/or the serial output depending on settings
// overloaded to accept and print data
void writeEvent( String text, float data );

void writeEvent( String text, String data );

float writeWarning( String text, float lastUpdate);

// considers time since last update before calling writeEvent
float writeWarning( String text, String data,  float lastUpdate);

// considers time since last update before calling writeEvent
float writeWarning( String text, float data,  float lastUpdate);

#endif /* write_event */