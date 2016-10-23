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

// Debugging
#define PHOTON_PUBLISH_DEBUG      // Publishing debug events to the particle cloud
#define PHOTON_PUBLISH_VALUE      // Publishing regular events to the particle cloud
#define PHOTON_SERIAL             // Outputing to the serial port

// Libraries
#include "sonar-ping/sonar-ping.h"
#define SKETCH "DISTANCE-BASIC 1.2.0"

const unsigned char  COEF_PUBLISH = 1;       // The multiplier of publishing measurements
const unsigned int PERIOD_MEASURE = 3000;    // Measuring timer period in ms 

// Hardware cofiguration
const unsigned char PIN_TRIGGER = D1;
const unsigned char PING_ECHO   = D2;

// Measuring
SonarPing sonar(PIN_TRIGGER, PING_ECHO);
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
    Serial.println("Distance");
#endif
}

void loop() {
  measure();
}

void measure() {
    static unsigned long tsMeasure = millis();
    if (millis() - tsMeasure >= PERIOD_MEASURE) {
      tsMeasure = millis();
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
#endif
#ifdef PHOTON_SERIAL
      Serial.println(distance);
#endif
    }
}
