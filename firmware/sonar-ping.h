/*
  NAME:
  SonarPing

  DESCRIPTION:
  Library for ultrasonic sensor HC-SR04 or similar sensors.
  - Library uses function pulseIn() for measuring sound reflection times.
  - Library intentionally uses only SI measurement units. The conversion
    to imperial units should be provided in a sketch code or in a separate
    library in order to avoid duplicities in code using multiple
    libraries with the same conversion functionality.
  - Library does not intentionally use statistical processing of measured
    values (ping time, distance). There are separate libraries for that
    purpose to use in order to avoid duplicities in code.
  - Library uses the temperature compensation for air speed. It is limited
    to the temperature range from -128 to +127 centigrades (degrees
    of Celsius), which contains usual working range of sensors.
  - Sound speed in meters per second is calculated from the statement
    soundSpeed (m/s) = 331.3 (m/s) + 0.606 * temperature (degC). Default
    temperature is set to 20 degC.
  - Library measures the distance in whole centimeters in regards to
    practical accuracy of ultrasonic sensors.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkalePhotonLib/SonarPing.git
 */
#ifndef SONARPING_H
#define SONARPING_H
#define SONARPING_VERSION "SonarPing 1.0.0"

#include "Particle.h"

// You shoudln't change these values unless you really know what you're doing.
#define SONARPING_NAN             0      // Represents undefined ping time or distance
#define SONARPING_DISTANCE_MIN    2      // Minimal measuring distance (cm)
#define SONARPING_DISTANCE_MAX    500    // Maximal measuring distance (cm)
#define SONARPING_TEMPERATURE_DEF 20     // Default ambient temperature (degC)
#define SONARPING_DELAY_INTERPING 29     // Minimal delay in milliseconds between pings from specification

// Macro functions
#define swap(a, b) { if (a > b) { int16_t t = a; a = b; b = t; } }

class SonarPing
{
public:
//-------------------------------------------------------------------------
// Public methods
//-------------------------------------------------------------------------

/*
  Constructor with configuration parameters

  DESCRIPTION:
  Setting up configuration parameters for library and measurement. They all
  are stored in the private variables.
  - Maximal and minimal distance determines the expected range of measurement.
    Results outside this range are considered as erroneous ones and are
    ignored.
  - Measurement range is limited by limits 2 to 500 cm due to hardware
    limitation. Limits outside this range are defaulted to those bounderies.

  PARAMETERS:
  trigger_pin  - digital pin number for sensor's trigger pin.
  echo_pin     - digital pin number for sensor's echo pin.
  distance_max - maximal accepted measured distance from the sensor to
                 a reflector in centimeters.
                 - Defaulted and limited to 500 cm.
  distance_min - minimal accepted measured distance from the sensor to
                 a reflector in centimeters.
                 - Defaulted and limited to 2 cm.

  RETURN: object
*/
  SonarPing(uint8_t trigger_pin, uint8_t echo_pin, \
            uint16_t distance_max = SONARPING_DISTANCE_MAX, \
            uint16_t distance_min = SONARPING_DISTANCE_MIN);

 /*
  Measure distance in centimeters from the sensor to a reflector.

  DESCRIPTION:
  The method measures distance from the sensor to a reflector based
  on measuring sound wave time to it and back.
  - Method corrects the measurement by temperature stored in class object
    before measurement.

  PARAMETERS: none

  RETURN: Distance in centimeters
*/
  uint16_t getDistance();

//-------------------------------------------------------------------------
// Public setters
//-------------------------------------------------------------------------

/*
  Setter for ambient air temperature in centigrades for sound speed correction.

  DESCRIPTION:
  The method stores the ambient temperature to the class instance object.
  - Default temperature is set to 20 degC at object initialization.
  - It is not needed to set temperature before every measurement, just then
    the temperature has changed significantly.

  PARAMETERS:
  temperature - ambient air temperature in centigrades
                - Limited to -128 ~ +127 degC by data type.
                - Defaulted to 20 degC.

  RETURN: none
*/
  void setTemperature(int8_t temperature = SONARPING_TEMPERATURE_DEF);

//-------------------------------------------------------------------------
// Public getters
//-------------------------------------------------------------------------

/*
  Getter of the actual temperature for sound speed correction.

  DESCRIPTION:
  The method returns currently set temperature in centigrades for correction.

  PARAMETERS: none

  RETURN: Temperature in centigrades
*/
  int8_t getTemperature();

/*
  Getters of the actual minimal and maximal valid distance

  DESCRIPTION:
  The methods return current minimal or maximal distance valid for
  measuring respectively set by the constructor.

  PARAMETERS: none

  RETURN: Actual Minimal resp. maximal valid value
*/
  uint16_t getDistanceMax();
  uint16_t getDistanceMin();

private:
//-------------------------------------------------------------------------
// Private attributes
//-------------------------------------------------------------------------
  uint8_t  _pinTrig;
  uint8_t  _pinEcho;
  uint16_t _maxDistance;
  uint16_t _minDistance;
  int8_t   _temperature = SONARPING_TEMPERATURE_DEF;

//-------------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------------
/*
  Calculate sound pace in microseconds per centimeter with temperature correction.

  DESCRIPTION:
  The method calculates number of microseconds the sound wave needs to travel
  one centimeter at the current temperature set previously.
  - It is reciprocal value to sound speed.
  - For a reasonable precision it is expressed as integer.
  - Within the temperature range -15 ~ +40 degC it is in the range 31 ~ 28 us/cm
    and temperature influence is 2 ~ -1 us/cm (6.6 ~ -3.4%) in comparison
    to reference temperature 20 degC.

  PARAMETERS: none

  RETURN: Sound pace in microseconds per centimeter
*/
  uint8_t soundPace(void);

/*
  Measure sound ping round trip time to reflector and back in microseconds.

  DESCRIPTION:
  The method makes the sensor to broadcast a sound impuls, then waits for
  the echo pulse and measures its length as the time of sound round trip
  from sensor to a reflector and back.
  - Method uses firmware function pulseIn() so that the default timeout is
    3 seconds at waiting for echo signal.
  - Method cancels measuring the echo pulse if it takes longer or shorter
    than expected from maximal and minimal distance range set in constructor.
  - Method corrects sound speed by temperature stored in class object
    before measurement.

  PARAMETERS: none

  RETURN: Sound round trip time in microseconds
*/
  uint16_t pingTime();

};
#endif
