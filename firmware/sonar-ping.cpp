#include "sonar-ping.h"

SonarPing::SonarPing(uint8_t trigger_pin, uint8_t echo_pin, \
  uint16_t distance_max, uint16_t distance_min)
{
  // Maintain input parameters
  _pinTrig = trigger_pin;
  _pinEcho = echo_pin;
  _minDistance = constrain(distance_min, SONARPING_DISTANCE_MIN, SONARPING_DISTANCE_MAX);
  _maxDistance = constrain(distance_max, _minDistance, SONARPING_DISTANCE_MAX);
  swap(_minDistance, _maxDistance);
  // Setup pins
  pinMode(_pinTrig, OUTPUT);
  pinMode(_pinEcho, INPUT);
}

// Measure sound ping round trip time (microseconds) to reflector
uint16_t SonarPing::pingTime()
{
  uint16_t pingTime;
  // Wait necessary time from potential previous ping (from specs)
  delay(SONARPING_DELAY_INTERPING);
  // Broadcast sound ping
  digitalWriteFast(_pinTrig, LOW);
  delayMicroseconds(4);   // Wait for pin to go LOW (from testing)
  digitalWriteFast(_pinTrig, HIGH);
  delayMicroseconds(10);  // Wait for trigger pin become HIGH - from specs
  digitalWriteFast(_pinTrig, LOW);
  // Measure echo signal
  pingTime = pulseIn(_pinEcho, HIGH);
  // Pulse is outside of the expected range
  if (pingTime < distance2time(_minDistance)) return SONARPING_NAN;
  if (pingTime > distance2time(_maxDistance)) return SONARPING_NAN;
  //Return correct ping time
  return pingTime;
}

// Measure distance
uint16_t SonarPing::getDistance()
{
  uint16_t ping;
  uint8_t  pace;
  ping = pingTime();
  if (ping == SONARPING_NAN) return SONARPING_NAN;
  pace = soundPace();
  // Round up distance to whole centimeters
  return (ping + pace - 1) / pace;
}

// Store current temperature for further calculations
void SonarPing::setTemperature(int8_t temperature)
{
  _temperature = temperature;
}

// Sound pace corrected by temperature
uint8_t SonarPing::soundPace(void)
{
  return (2000000.0 / (33130.0 + 60.6 * _temperature));
}

// Sound travel time
uint16_t SonarPing::distance2time(uint16_t distance)
{
  return distance * 2 * soundPace();
}

// Getters
int8_t   SonarPing::getTemperature() { return _temperature; };
uint16_t SonarPing::getDistanceMin() { return _minDistance; };
uint16_t SonarPing::getDistanceMax() { return _maxDistance; };
