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
  * For statistical processing the library SmoothSensorData is used.
    While the distance is measured in whole centimeters, the smoothing
    can be provided directly.
  * The sketch measures in burst of consecutive measurements distant by
    default time delay among them defined by statistical library. On the
    other hand, the sonar library has built-in time delay between consecutive
    ping according to the hardware specification. So that, the delay
    between consecutive measurement can be avoide and set to 0 in order not
    to increasing delay between measurements in vain.
  * Because the limited measurement is used by SonarPing library directly,
    the filtering data at statistical processing by SmoothSensorData library
    is not necessary.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "sonar-ping/sonar-ping.h"
#include "smooth-sensor-data/smooth-sensor-data.h"

#define SKETCH_VERSION "1.0.0"
#define SKETCH_NAME "DISTANCE-SMOOTHED-FULL"

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

// Statistical processing
const uint8_t SAMPLE_COUNT = 5;
const uint8_t SAMPLE_DELAY = 0;
SmoothSensorData samples = SmoothSensorData(SAMPLE_COUNT, SAMPLE_DELAY);
unsigned int distMed = SONARPING_NAN;
unsigned int distAvg = SONARPING_NAN;
unsigned int distMin = SONARPING_NAN;
unsigned int distMax = SONARPING_NAN;

void setup() {
  // Start processing timers
  timerMeasure.start();
  timerPublish.start();
  // Publish sketch identification as public events
  Particle.publish("Sketch",  String::format("%s %s", SKETCH_NAME, SKETCH_VERSION));
  Particle.publish("Library", String::format("%s %s", "SonarPing", SONARPING_VERSION));
  Particle.publish("Library", String::format("%s %s", "SmoothSensorData", SMOOTHSENSORDATA_VERSION));
}

void loop() {}

void measuring() {
// Temperature compensation (random maximum excluded)
  sonar.setTemperature(random(TEMPERATURE_MIN, TEMPERATURE_MAX + 1));
  // Measure temperature compensated distance in burst
  while (samples.registerData(sonar.getDistance()));
  distMed = samples.getMedian();
  distAvg = samples.getAverage();
  distMin = samples.getMinimum();
  distMax = samples.getMaximum();
}

void publishing() {
  Particle.publish("Med/Avg/Min/Max", String::format("%3d / %3d / %3d / %3d", distMed, distAvg, distMin, distMax));
  Particle.publish("Temp / DistMin / DistMax", String::format("%3d / %3d / %3d", sonar.getTemperature(), sonar.getDistanceMin(), sonar.getDistanceMax()));
}
