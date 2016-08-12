/*
  NAME:
  Full usage of SonarPing library with smoothing measured values

  DESCRIPTION:
  This sketch demonstrates the use of SonarPing for distance measuring and
  statistical processing the measured distance in order to avoid measuring
  noise and to smooth the measured distance.
  * The ambient air temperature for sound speed compensation is simulated
    by random numbers in the range 10 to 40 centigrades.
  * The measuring distance is limited to the range 5 - 250 cm.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "sonar-ping/sonar-ping.h"

#define SKETCH_VERSION "1.0.0"
#define SKETCH_NAME "DISTANCE-FULL"

// Processing timers
const unsigned int MEASURE_PERIOD = 1000;   // Timer period in miliseconds for measuring
const unsigned int PUBLISH_PERIOD = 60000;  // Timer period in miliseconds for publishing events
Timer timerMeasure(MEASURE_PERIOD, measuring);
Timer timerPublish(PUBLISH_PERIOD, publishing);

// Ultrasonic sensor hardware connection
const byte PIN_TRIGGER = D2;
const byte PING_ECHO   = D6;

// Measuring distance in whole centimeters
const unsigned int DISTANCE_MIN = 5;
const unsigned int DISTANCE_MAX = 250;
const byte TEMPERATURE_MIN = 10;
const byte TEMPERATURE_MAX = 40;
SonarPing sonar = SonarPing(PIN_TRIGGER, PING_ECHO, DISTANCE_MAX, DISTANCE_MIN);
unsigned int distance = 0;
byte temperature = sonar.getTemperature();

void setup() {
  // Start processing timers
  timerMeasure.start();
  timerPublish.start();
  // Publish sketch identification as public events
  Particle.publish("Sketch", String::format("%s %s", SKETCH_NAME, SKETCH_VERSION));
  Particle.publish("Library", String::format("%s %s", "SonarPing", SONARPING_VERSION));
}

void loop() {}

void measuring() {
  // Temperature compensation
  sonar.setTemperature(random(TEMPERATURE_MIN, TEMPERATURE_MAX));
  temperature = sonar.getTemperature();
  // Measure temperature compensated distance
  distance = sonar.getDistance();
}

void publishing() {
  Particle.publish("Distance", String(distance));
  Particle.publish("Temperature", String(temperature));
}
