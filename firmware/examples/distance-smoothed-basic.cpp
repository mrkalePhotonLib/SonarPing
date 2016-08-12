/*
  NAME:
  Basic usage of SonarPing library with smoothing measured distance

  DESCRIPTION:
  This sketch demonstrates the use of SonarPing library for distance
  measuring with default arguments and with statistical processing the
  measured distance in order to avoid measuring noise and to smooth the
  measured distance.
  * The ambient air temperature for sound speed compensation is defaulted
    to library default.
  * For statistical processing the library SmoothSensorData is used.
    While the distance is measured in whole centimeters, the smoothing
    can be provided directly.
  * The sketch measures in burst of consecutive measurements distant by
    default time delay among them defined by statistical library. On the
    other hand, the sonar library has built-in time delay between consecutive
    ping according to the hardware specification.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "sonar-ping/sonar-ping.h"
#include "smooth-sensor-data/smooth-sensor-data.h"

#define SKETCH_VERSION "1.0.0"
#define SKETCH_NAME "DISTANCE-SMOOTHED-BASIC"

// Processing timers
const unsigned int MEASURE_PERIOD = 1000;   // Timer period in miliseconds for measuring
const unsigned int PUBLISH_PERIOD = 60000;  // Timer period in miliseconds for publishing events
Timer timerMeasure(MEASURE_PERIOD, measuring);
Timer timerPublish(PUBLISH_PERIOD, publishing);

// Ultrasonic sensor hardware connection
const byte PIN_TRIGGER = D2;
const byte PING_ECHO   = D6;

// Measuring distance in whole centimeters
unsigned int distance = 0;
SonarPing sonar(PIN_TRIGGER, PING_ECHO);
SmoothSensorData samples();


void setup() {
  // Start processing timers
  timerMeasure.start();
  timerPublish.start();
  // Publish sketch identification as public events
  Particle.publish("Sketch", String::format("%s %s", SKETCH_NAME, SKETCH_VERSION));
  Particle.publish("Library", String::format("%s %s", "SonarPing", SONARPING_VERSION));
  Particle.publish("Library", String::format("%s %s", "SmoothSensorData", SMOOTHSENSORDATA_VERSION));
}

void loop() {}

void measuring() {
  while (samples.registerData(sonar.getDistance()));  // Readings to buffer
  distance = samples.getMedian();
}

void publishing() {
  Particle.publish("Distance", String(distance));
}
