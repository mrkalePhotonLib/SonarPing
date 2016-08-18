/*
  NAME:
  Full usage of SonarPing library with complete statistical processing of
  measured values

  DESCRIPTION:
  This sketch demonstrates the use of SonarPing for distance measuring and
  statistical processing the measured distance in order to avoid measuring
  noise and to smooth the measured distance.
  * The ambient air temperature for sound speed compensation is simulated
    by random numbers in the range 10 to 40 centigrades.
  * The measuring distance is limited to the range 5 - 250 cm.
  * For statistical processing the library SmoothSensorData and
    RunningStatistic is used.
  * While the distance is measured in whole centimeters, the statistical 
    processing can be provided directly.
  * The sketch measures in burst of consecutive measurements and calculates
    all provided statistical types from them by the library SmoothSensorData.
    Then it calculates running statistics of the equivalent type by the
    library RunningStatistic from previous serie of those measurement bursts.
  * Because the sonar library as well as SmoothSensorData library have
    built-in time delay between consecutive measurements, their delays are
    cummulated. In specific case on of them or both should be set explicitly
    by corresponding input arguments.
  * Because the limited measurement is used by SonarPing library directly,
    the filtering data at statistical processing is not necessary.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "sonar-ping/sonar-ping.h"
#include "smooth-sensor-data/smooth-sensor-data.h"
#include "running-statistic/running-statistic.h"

#define SKETCH_VERSION "1.0.0"
#define SKETCH_NAME "DISTANCE-SMOOTHED-FULL"

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

// Statistical processing
SmoothSensorData samples;
RunningStatistic distanceAvg;
RunningStatistic distanceMed(RUNNINGSTATISTIC_MEDIAN);
RunningStatistic distanceMin(RUNNINGSTATISTIC_MINIMUM);
RunningStatistic distanceMax(RUNNINGSTATISTIC_MAXIMUM);

void setup() {
  timerMeasure.start();
  // Publish sketch identification as public events
  Particle.publish("Sketch",  String::format("%s %s", SKETCH_NAME, SKETCH_VERSION));
  Particle.publish("Library", String::format("%s %s", "SonarPing", SONARPING_VERSION));
  Particle.publish("Library", String::format("%s %s", "SmoothSensorData", SMOOTHSENSORDATA_VERSION));
  Particle.publish("Library", String::format("%s %s", "RunningStatistic", RUNNINGSTATISTIC_VERSION));
}

void loop() {}

// Measuring and statistical processing
void measuring() {
  // Temperature compensation (random maximum excluded)
  sonar.setTemperature(random(TEMPERATURE_MIN, TEMPERATURE_MAX + 1));
  // Measure temperature compensated distance in burst
  while (samples.registerData(sonar.getDistance()));
  // Calculate running statistics for eqivalent smoothed statistical type
  unsigned int distMed = distanceMed.getStatistic(samples.getMedian());
  unsigned int distAvg = distanceAvg.getStatistic(samples.getAverage());
  unsigned int distMin = distanceMin.getStatistic(samples.getMinimum());
  unsigned int distMax = distanceMax.getStatistic(samples.getMaximum());
  Particle.publish("Med/Avg/Min/Max", String::format("%3d/%3d/%3d/%3d", distMed, distAvg, distMin, distMax));
  Particle.publish("Temp/DistMin/DistMax", String::format("%3d/%3d/%3d", sonar.getTemperature(), sonar.getDistanceMin(), sonar.getDistanceMax()));
}
