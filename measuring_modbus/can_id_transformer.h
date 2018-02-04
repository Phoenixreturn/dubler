#ifndef CAN_ID   /* Include guard */
#define CAN_ID

#include "stdint.h"

uint32_t transformEngineSpeed(uint8_t data8[8], uint8_t dataLength);
float transformOilPressure(uint8_t data8[8], uint8_t dataLength);
uint32_t transformCoolantTemperature(uint8_t data8[8], uint8_t dataLength);
float transformBatteryVoltage(uint8_t data8[8], uint8_t dataLength);

#endif
