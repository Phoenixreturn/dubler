#include "can_id_transformer.h"
#include "stdint.h"

uint32_t transformEngineSpeed(uint8_t data8[8], uint8_t dataLength) {
  uint32_t temp = data8[4] << 8;
  temp = temp + data8[3];
  return temp*0.125;
}

float transformOilPressure(uint8_t data8[8], uint8_t dataLength) {
  float temp = data8[3]*4*0.0102;
  return temp;
}

uint32_t transformCoolantTemperature(uint8_t data8[8], uint8_t dataLength) {
  uint32_t temp = data8[0] - 40;
  return temp;
}

float transformBatteryVoltage(uint8_t data8[8], uint8_t dataLength) {
  uint32_t temp = data8[7] << 8;
  temp = temp + data8[6];
  return temp*0.05;
}
