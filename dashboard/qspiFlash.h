/*
 * qspiFlash.h
 *
 *  Created on: 20 сент. 2017 г.
 *      Author: admin
 */

#ifndef UGFX_DEMOS_CUSTOM_DASHBOARD_QSPIFLASH_H_
#define UGFX_DEMOS_CUSTOM_DASHBOARD_QSPIFLASH_H_

#include "hal.h"

qspistate_t startQSPIConfiguredForQuad();
qspistate_t startQSPI();
qspistate_t getDriverState();
void writeEnableQuad();
void writeEnableExSPI();

// Volatile Register
uint8_t readVolatileRegisterExSPI();
uint8_t readVolatileRegisterQuadSPI();
void writeVolatileRegisterQuadSPI();

// Enhanced Volatile Register
uint8_t readEnhVolatileRegisterExSPI();
uint8_t readEnhVolatileRegisterQuadSPI();
void writeEnhVolatileRegisterExSPI();

void sectorErase(uint32_t address);
uint8_t readStatusRegisterProgressBitQuadMode();
void quadIndirectWriteMode(uint8_t *buf, uint32_t n, uint32_t address);
void quadIndirectReadMode(uint8_t *buf, uint32_t n, uint32_t address);
void quadMappedModeEnable();

#endif /* UGFX_DEMOS_CUSTOM_DASHBOARD_QSPIFLASH_H_ */
