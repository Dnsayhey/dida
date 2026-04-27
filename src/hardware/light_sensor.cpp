#include "light_sensor.h"

#include <Arduino.h>

LightSensor& LightSensor::getInstance() {
  static LightSensor instance;
  return instance;
}

// 初始化光敏传感器
void LightSensor::begin(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, INPUT);
}

// 获取原始ADC值（0-4095）
uint16_t LightSensor::readRaw() { return analogRead(_pin); }

// 转换为亮度值（0-255）
uint8_t LightSensor::readBrightness() {
  uint16_t rawValue = readRaw();
  return 255 - map(rawValue, _minValue, _maxValue, 0, 255);
}

// 获取光强等级描述
const char* LightSensor::getLightLevel() {
  uint8_t val = readBrightness();
  if (val < 30) return "Dark";
  if (val < 100) return "Low";
  if (val < 180) return "Medium";
  return "Bright";
}
