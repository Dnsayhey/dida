#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <stdint.h>

class LightSensor {
 public:
  static LightSensor& getInstance();

  void begin(uint8_t pin);
  uint16_t readRaw();
  uint8_t readBrightness();
  const char* getLightLevel();

  void calibrate(uint16_t minVal, uint16_t maxVal) {
    _minValue = minVal;
    _maxValue = maxVal;
  }

 private:
  LightSensor() = default;
  LightSensor(const LightSensor&) = delete;
  LightSensor& operator=(const LightSensor&) = delete;

  uint8_t _pin;
  uint16_t _minValue = 0;
  uint16_t _maxValue = 4095;
};

#endif
