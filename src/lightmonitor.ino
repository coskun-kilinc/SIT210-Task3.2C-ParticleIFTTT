// This #include statement was automatically added by the Particle IDE.
#include "write_event.h"

// This #include statement was automatically added by the Particle IDE.
#include <BH1750.h>

/*
 * Project SIT210-Task3_2C-IFTT
 * Description: an embedded device that uses the Particle device to measure the amount of sunlight exposure a terrarium has
 * use the IFTTT service which will provide you with a notification throughout the day when the sunlight hits your terrarium and another notification when it stops
 * accounts for time since last notification to ensure it does not spam the user
 * Author: Coskun (Josh) Kilinc
 * Date: 24/04/2022
 */

BH1750 sensor(0x23, Wire);
// required exposure time in milliseconds (time in minutes multiplied by 60,000)
const double REQUIRED_EXPOSURE = 0.30*60000;
const double LIGHT_THRESHOLD = 300;
const double CYCLE_LENGTH = 0.016*60*60*1000;


float exposureTime = 0, timeSinceCycleStart = 0, lastUpdateOn = 0, lastUpdateOff = 0, lastWarning = 0;
float start, end;

bool gettingLight = false, exposureForDay = false;


SYSTEM_THREAD(ENABLED);

// setup() runs once, when the device is first turned on.
void setup() {
  // Initialise light-level sensor
  sensor.begin();
  sensor.set_sensor_mode(BH1750::forced_mode_high_res2);
  Serial.begin();
  start = millis();
  end = 0;
  Serial.print("Starting Terrarium monitoring application.");
}

static void measureExposure( float lightLevel, float elapsedTime )
{
    // if exposure level has been reached, increment timeSinceLastExposure with elapsed time
    timeSinceCycleStart += elapsedTime;
    // activates if light level is above the threshold
    if ( lightLevel > LIGHT_THRESHOLD ){
    /* if sensor was not previously getting light, alert that terrarium has started receiving light
     * also test for how long since last update since this could get irritating if light level is constantly changing
     */
    if ( gettingLight == false )
    { 
        gettingLight = true;
        lastUpdateOn = writeWarning("Light_Status", "Receiving_Light", lastUpdateOn);
    }
    // if sensor was already getting light just increment the exposure time with the elapsed time since last loop
    else
    {
      exposureTime += elapsedTime;
    } 
    // if exposure time + exposure difference exceeds or reaches the required exposure, change exposureForDay to true
    // no need to test how long since last update as this can only be reached every 24 hours be default
    if ( exposureTime >= REQUIRED_EXPOSURE  )
    {
      if ( exposureForDay == false ){
        exposureForDay = true;
        writeEvent("Light_Status", "Exposure_Reached");
        lastWarning = 0;
      }  
      // warns for continuos overexposure
      // checks for time since last warning to not overwhelm the user with warnings.
      lastWarning = writeWarning("Light_Status", "Exposure_Exceeded",  lastWarning);
    }
}
  // if sensor is not getting light
  else
  {
    // change status to false
    if ( gettingLight == true ){
      gettingLight = false;
      // alert user and  update how much exposure terrarium has gotten so far.
      // checks to make sure the user hasn't been getting too many messages first
      lastUpdateOff = writeWarning("Light_Status", "Not_Receiving_Light", lastUpdateOff);
    }
  }
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  float elapsedTime = millis()- start;
  start = millis();
  // make a new measurement, turns the device on if it is off
  sensor.make_forced_measurement();
  // reads the measurement and assigns it to temporary variable lightLevel
  float lightLevel = sensor.get_light_level();
  measureExposure(lightLevel, elapsedTime);
  // if it has been 24 hours since exposure for day was reached, reset counter and set exposureForDay to faslse
  if (timeSinceCycleStart >= CYCLE_LENGTH  )
  { 
    writeEvent("Light_Status", "Cycle_Restarted");  
    timeSinceCycleStart = 0, lastUpdateOn = 0, lastUpdateOff = 0, lastWarning = 0;
    exposureTime -= REQUIRED_EXPOSURE;
    writeEvent("Current exposure time:", exposureTime);
    exposureForDay = false;
  }
}