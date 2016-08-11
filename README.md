<a id="library"></a>
# SonarPing
Library for ultrasonic sensors with two control pins, preferably HC-SR04.
- Library uses function __pulseIn()__ for measuring sound reflection time periods.
- Library intentionally uses only SI measurement units. The conversion to imperial units should be provided in a sketch code or in a separate library in order to avoid duplicities in code using multiple libraries with the same conversion functionality.
- Library does not intentionally use statistical processing of measured values (ping time, distance). There are separate libraries for that purpose to use in order to avoid duplicities in code.
- Library uses the temperature compensation for air speed. It is limited to the temperature range from -128 to +127 centigrades (degrees of Celsius), which contains usual working range of sensors.
- Sound speed in meters per second is calculated from the statement

  ```soundSpeed (m/s) = 331.3 (m/s) + 0.606 * temperature (degC)```

- Default temperature is set to 20 degC.
- Library measures the distance in whole centimeters in regards to practical accuracy of ultrasonic sensors.


<a id="dependency"></a>
## Dependency
- **Particle.h**: Include file with alternative (C++) data type definitions.

<a id="constants"></a>
## Constants
- **SONARPING\_VERSION**: Semantic library version
- **SONARPING\_NAN**: Represents undefined ping time or distance
- **SONARPING\_DISTANCE\_MIN**: Minimal measuring distance (cm)
- **SONARPING\_DISTANCE\_MAX**: Maximal measuring distance (cm)
- **SONARPING\TEMPERATURE\_DEF**: Default ambient temperature (degC)
- **SONARPING\_DELAY\_INTERPING**: Minimal delay in milliseconds between pings from specification

 
<a id="interface"></a>
## Interface
- [SonarPing()](#SonarPing)
- [getDistance()](#getDistance)
- [getDistanceMin()](#getDistanceRange)
- [getDistanceMax()](#getDistanceRange)
- [setTemperature()](#setTemperature)
- [getTemperature()](#getTemperature)


<a id="SonarPing"></a>
## SonarPing()
#### Description
Constructor sets configuration parameters for library and measurement. They all are stored in the private variables of an library instance object.
- Maximal and minimal distance determines the expected range of measurement. Results outside this range are considered as erroneous ones and are ignored.
- Measurement range is limited by limits 2 to 500 cm due to hardware limitation. Limits outside this range are defaulted to those bounderies.

#### Syntax
    SonarPing(uint8_t trigger_pin, uint8_t echo_pin, uint16_t distance_max, uint16_t distance_min);

#### Parameters
<a id="trigger_pin"></a>
- **trigger_pin**: Digital pin number for sensor's trigger pin.


<a id="echo_pin"></a>
- **echo_pin**: Digital pin number for sensor's echo pin.


<a id="distance_max"></a>
- **distance_max**: Maximal accepted measured distance from the sensor to a reflector in centimeters.
  - *Valid values*: positive integer in the range [distance_min](#distance_min) to 500
  - *Default value*: 500


<a id="distance_min"></a>
- **distance_min**: Minimal accepted measured distance from the sensor to a reflector in centimeters.
  - *Valid values*: positive integer in the range 2 to 500
  - *Default value*: 2

#### Returns
Object perfoming the ultrasonic measurement.

#### Example

``` cpp
sonar = SonarPing(2, 3);          // Default measurement range
sonar = SonarPing(2, 3, 100);     // Measured limited distance, e.g., a water barrel
sonar = SonarPing(2, 3, 100, 50); // Measured limited range, e.g., a water level
```

#### See also
[getDistanceMin()](#getDistanceMin)

[getDistanceMax()](#getDistanceMax)

[Back to interface](#interface)


<a id="getDistance"></a>
## getDistance()
#### Description
The method measures distance from the sensor to a reflector based on measuring sound wave time to it and back. Method corrects the measurement by temperature stored in instance object before measurement.

#### Syntax
    void getDistance();

#### Parameters
None

#### Returns
Distance in centimeters

#### See also
[SonarPing()](#SonarPing)

[setTemperature()](#setTemperature)

[Back to interface](#interface)


<a id="getDistanceRange"></a>
## getDistanceMin(), getDistanceMax()
#### Description
The methods return currently set minimal or maximal valid distance for correct measurement. It is useful if the constructor has limited its input distance arguments.

#### Syntax
    uint16_t getDistanceMin();
    uint16_t getDistanceMax();

#### Parameters
None

#### Returns
Actual minimal and maximal value of the valid distance range.

#### See also
[SonarPing()](#SonarPing)

[getDistance()](#getDistance)

[Back to interface](#interface)


<a id="setTemperature"></a>
## setTemperature()
#### Description
The method stores the ambient air temperature to the instance object.
  - Default temperature is set to 20 degC at object initialization.
  - It is not needed to set temperature before every measurement, just then the temperature has changed significantly.

#### Syntax
    void setTemperature(int8_t temperature);

#### Parameters
<a id="temperature"></a>
- **temperature**: Ambient air temperature in centigrades.
  - *Valid values*: integer in the range -128 to +127
  - *Default value*: 20

#### Returns
None

#### See also
[SonarPing()](#SonarPing)

[getTemperature()](#getTemperature)

[Back to interface](#interface)


<a id="getTemperature"></a>
## getTemperature()
#### Description
The method returns currently set temperature in centigrades for correction.

#### Syntax
    int8_t getTemperature();

#### Parameters
None

#### Returns
Temperature in centigrades.

#### See also
[setTemperature()](#setTemperature)

[Back to interface](#interface)
