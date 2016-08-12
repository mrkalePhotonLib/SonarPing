/*
  NAME:
  Basic usage of SonarPing library

  DESCRIPTION:
  This sketch demonstrates the use of SonarPing library for distance
  measuring with default arguments.
  * The ambient air temperature for sound speed compensation is defaulted
    to library default.
  * The measuring distance range is defaulted to library defaults.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "sonar-ping/sonar-ping.h"

#define SKETCH_VERSION "1.0.0"
#define SKETCH_NAME "DISTANCE-BASIC"

// Processing timers
const unsigned int MEASURE_PERIOD = 1000;   // Timer period in miliseconds for measuring
const unsigned int PUBLISH_PERIOD = 60000;  // Timer period in miliseconds for publishing events
Timer timerMeasure(MEASURE_PERIOD, measuring);
Timer timerPublish(PUBLISH_PERIOD, publishing);

// Ultrasonic sensor hardware connection
const byte PIN_TRIGGER = D2;
const byte PING_ECHO   = D6;

// Measuring distance in whole centimeters
SonarPing sonar = SonarPing(PIN_TRIGGER, PING_ECHO);
unsigned int distance = SONARPING_NAN;

void setup() {
  // Start processing timers
  timerMeasure.start();
  timerPublish.start();
  // Publish sketch identification as public events
  Particle.publish("Sketch",  String::format("%s %s", SKETCH_NAME, SKETCH_VERSION));
  Particle.publish("Library", String::format("%s %s", "SonarPing", SONARPING_VERSION));
}

void loop() {}

void measuring() {
  distance = sonar.getDistance();
}

void publishing() {
  Particle.publish("Distance", String(distance));
}
