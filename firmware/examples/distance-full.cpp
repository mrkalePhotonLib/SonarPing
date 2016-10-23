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

// Debugging
#define PHOTON_PUBLISH_DEBUG      // Publishing debug events to the particle cloud
#define PHOTON_PUBLISH_VALUE      // Publishing regular events to the particle cloud
#define PHOTON_SERIAL             // Outputing to the serial port

// Libraries
#include "sonar-ping/sonar-ping.h"
#define SKETCH "DISTANCE-FULL 1.2.0"

const unsigned char  COEF_PUBLISH = 6;      // The multiplier of publishing measurements
const unsigned int PERIOD_MEASURE = 500;    // Measuring timer period in ms 

// Hardware cofiguration
const unsigned char PIN_TRIGGER = D1;
const unsigned char PING_ECHO   = D2;

// Measuring
const unsigned int DISTANCE_MIN = 5;
const unsigned int DISTANCE_MAX = 250;
const unsigned char TEMPERATURE_MIN = 10;
const unsigned char TEMPERATURE_MAX = 40;
SonarPing sonar(PIN_TRIGGER, PING_ECHO, DISTANCE_MAX, DISTANCE_MIN);
unsigned int distance;

void setup() {
#ifdef PHOTON_SERIAL
    Serial.begin(9600);
    while(!Serial.available()); // Wait for pressing ENTER in Serial Terminal
#endif
  // Publish sketch and libraries identification as public events
#ifdef PHOTON_PUBLISH_DEBUG
    Particle.publish("Sketch",  String(SKETCH));
    Particle.publish("Library", String(SONARPING_VERSION));
#endif
#ifdef PHOTON_SERIAL
    Serial.println(SKETCH);
    Serial.println(SONARPING_VERSION);
    // Print header
    Serial.println("---");
    Serial.println("DistCur\tTemp\tDistMin\tDistMax");
#endif
}

void loop() {
  measure();
}

void measure() {
    static unsigned long tsMeasure = millis();
    if (millis() - tsMeasure >= PERIOD_MEASURE) {
      tsMeasure = millis();
      // Temperature compensation (random(): maximum excluded)
      sonar.setTemperature(random(TEMPERATURE_MIN, TEMPERATURE_MAX + 1));
      distance = sonar.getDistance();
      publish();
    }
}

void publish() {
    static byte cntMeasure = 0;
    if (++cntMeasure >= COEF_PUBLISH) {
      cntMeasure = 0;
#ifdef PHOTON_PUBLISH_VALUE
      Particle.publish("Distance", String(distance));
      Particle.publish("Temp/DistMin/DistMax", String::format("%2d/%4d/%4d", sonar.getTemperature(), sonar.getDistanceMin(), sonar.getDistanceMax()));
#endif
#ifdef PHOTON_SERIAL
      Serial.print(distance);
      Serial.print("\t");
      Serial.print(sonar.getTemperature());
      Serial.print("\t");
      Serial.print(sonar.getDistanceMin());
      Serial.print("\t");
      Serial.print(sonar.getDistanceMax());
      Serial.println();      
#endif
    }
}
