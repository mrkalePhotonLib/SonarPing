/*
  NAME:
  Full usage of SonarPing library

  DESCRIPTION:
  This sketch demonstrates the use of SonarPing for distance measuring.
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

#define SKETCH "DISTANCE-FULL 1.1.0"

const unsigned int PERIOD_MEASURE = 30000;   // Timer period in ms
Timer timerMeasure(PERIOD_MEASURE, measuring);

// Ultrasonic sensor hardware connection
const byte PIN_TRIGGER = D2;
const byte PING_ECHO   = D6;

// Measuring distance in whole centimeters
const unsigned int DISTANCE_MIN = 5;
const unsigned int DISTANCE_MAX = 250;
const byte TEMPERATURE_MIN = 10;
const byte TEMPERATURE_MAX = 40;
SonarPing sonar(PIN_TRIGGER, PING_ECHO, DISTANCE_MAX, DISTANCE_MIN);

void setup() {
  timerMeasure.start();
  // Publish sketch and library identification as public events
  Particle.publish("Sketch",  String(SKETCH));
  Particle.publish("Library", String(SONARPING_VERSION));
}

void loop() {}

void measuring() {
// Temperature compensation (random maximum excluded)
  sonar.setTemperature(random(TEMPERATURE_MIN, TEMPERATURE_MAX + 1));
  // Measure temperature compensated distance
  unsigned int distance = sonar.getDistance();
  Particle.publish("Distance", String(distance));
  Particle.publish("Temp/DistMin/DistMax", String::format("%2d/%4d/%4d", sonar.getTemperature(), sonar.getDistanceMin(), sonar.getDistanceMax()));
}
