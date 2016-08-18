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

const unsigned int PERIOD_MEASURE = 30000;   // Timer period in ms
Timer timerMeasure(PERIOD_MEASURE, measuring);

// Ultrasonic sensor hardware connection
const byte PIN_TRIGGER = D2;
const byte PING_ECHO   = D6;

// Measuring distance in whole centimeters
SonarPing sonar(PIN_TRIGGER, PING_ECHO);

void setup() {
  timerMeasure.start();
  // Publish sketch identification as public events
  Particle.publish("Sketch",  String::format("%s %s", SKETCH_NAME, SKETCH_VERSION));
  Particle.publish("Library", String::format("%s %s", "SonarPing", SONARPING_VERSION));
}

void loop() {}

void measuring() {
  unsigned int distance = sonar.getDistance();
  Particle.publish("Distance", String(distance));
}
