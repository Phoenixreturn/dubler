#include "qspiFlash.h"

#define  VOL_REG_DUMMY_10       ((uint8_t)0b10100000)
#define  VOL_ENH_REG_QUAD_MODE  ((uint8_t)0b01111111)


#if CH_KERNEL_MAJOR > 2
    #define CPU_CACHE_LINE_SIZE         32
    #define CACHE_FLUSH(buf, sz)        dmaBufferFlush((buf), (sz)+(CPU_CACHE_LINE_SIZE-1))
    #define CACHE_INVALIDATE(buf, sz)   dmaBufferInvalidate((buf), (sz)+(CPU_CACHE_LINE_SIZE-1))
#else
    #define CACHE_FLUSH(buf, sz)
    #define CACHE_INVALIDATE(buf, sz)
#endif

extern QSPIDriver QSPID1;
uint8_t CONFIGURED = 0;

QSPIConfig quadConfig = {
    NULL, /* callback */
    STM32_DCR_CSHT(1) | QUADSPI_DCR_FSIZE_0 | QUADSPI_DCR_FSIZE_3 | QUADSPI_DCR_FSIZE_4
};

qspistate_t startQSPI() {
  qspiStart(&QSPID1, &quadConfig);
  return QSPID1.state;
}

qspistate_t startQSPIConfiguredForQuad() {
  if(!CONFIGURED) {
    qspiStart(&QSPID1, &quadConfig);

     writeEnhVolatileRegisterExSPI();
     writeVolatileRegisterQuadSPI();
  } else {
    CONFIGURED = 1;
  }

  return QSPID1.state;
}

qspistate_t getDriverState() {
  return QSPID1.state;
}

void writeEnableQuad() {
    qspi_command_t writeEnable;
    writeEnable.alt = 0;
    writeEnable.addr = 0;
    writeEnable.cfg =  QUADSPI_CCR_IMODE_1 | QUADSPI_CCR_IMODE_0 | 0x6;

    qspiCommand(&QSPID1, &writeEnable);
}

void writeEnableExSPI() {
    qspi_command_t writeEnable;
    writeEnable.alt = 0;
    writeEnable.addr = 0;
    writeEnable.cfg =  QUADSPI_CCR_IMODE_0 | 0x6;

    qspiCommand(&QSPID1, &writeEnable);
}

uint8_t readVolatileRegisterExSPI() {
  uint8_t volatileRegister[1];

  qspi_command_t volatileRegRead;
  volatileRegRead.alt = 0;
  volatileRegRead.addr = 0;
  volatileRegRead.cfg =  QUADSPI_CCR_IMODE_0 | 0x85 | QUADSPI_CCR_DMODE_0;

  qspiReceive(&QSPID1, &volatileRegRead, 1, volatileRegister);

  return volatileRegister[0];
}

uint8_t readVolatileRegisterQuadSPI() {
  uint8_t volatileRegister[1];

  qspi_command_t volatileRegRead;
  volatileRegRead.alt = 0;
  volatileRegRead.addr = 0;
  volatileRegRead.cfg =  QUADSPI_CCR_IMODE_0 | QUADSPI_CCR_IMODE_1
      | 0x85 | QUADSPI_CCR_DMODE_0 | QUADSPI_CCR_DMODE_1;

  qspiReceive(&QSPID1, &volatileRegRead, 1, volatileRegister);

  return volatileRegister[0];
}

void writeVolatileRegisterQuadSPI() {
    uint8_t volatileRegister[1] = { 0b10001011 };

   qspi_command_t volatileRegWrite;
   volatileRegWrite.alt = 0;
   volatileRegWrite.addr = 0;
   volatileRegWrite.cfg =  QUADSPI_CCR_IMODE_0 | QUADSPI_CCR_IMODE_1
       | 0x81 | QUADSPI_CCR_DMODE_0 | QUADSPI_CCR_DMODE_1;

   writeEnableQuad();
   qspiSend(&QSPID1, &volatileRegWrite, 1, volatileRegister);
}

uint8_t readEnhVolatileRegisterExSPI() {
  uint8_t volatileRegister[1];

  qspi_command_t volatileRegRead;
  volatileRegRead.alt = 0;
  volatileRegRead.addr = 0;
  volatileRegRead.cfg =  QUADSPI_CCR_IMODE_0 | 0x65 | QUADSPI_CCR_DMODE_0;

  qspiReceive(&QSPID1, &volatileRegRead, 1, volatileRegister);

  return volatileRegister[0];
}

uint8_t readEnhVolatileRegisterQuadSPI() {
  uint8_t volatileRegister[1];

  qspi_command_t volatileRegRead;
  volatileRegRead.alt = 0;
  volatileRegRead.addr = 0;
  volatileRegRead.cfg =  QUADSPI_CCR_IMODE_0 | QUADSPI_CCR_IMODE_1
      | 0x65 | QUADSPI_CCR_DMODE_0 | QUADSPI_CCR_DMODE_1;

  qspiReceive(&QSPID1, &volatileRegRead, 1, volatileRegister);

  return volatileRegister[0];
}

void writeEnhVolatileRegisterExSPI() {
    uint8_t volatileRegister[1] = { 0b01011111 };
    qspi_command_t volatileRegWrite;
    volatileRegWrite.alt = 0;
    volatileRegWrite.addr = 0;
    volatileRegWrite.cfg =  QUADSPI_CCR_IMODE_0 | 0x61 | QUADSPI_CCR_DMODE_0;

    writeEnableExSPI();
    qspiSend(&QSPID1, &volatileRegWrite, 1, volatileRegister);
}

void sectorErase(uint32_t address) {
    qspi_command_t sectorErase;
    sectorErase.alt = 0;
    sectorErase.addr = address;
    sectorErase.cfg =  QUADSPI_CCR_IMODE_1 | QUADSPI_CCR_IMODE_0
            | 0xD8 | QUADSPI_CCR_ADSIZE_1
            | QUADSPI_CCR_ADMODE_1 | QUADSPI_CCR_ADMODE_0;

    writeEnableQuad();
    qspiCommand(&QSPID1, &sectorErase);
}

uint8_t readStatusRegisterProgressBitQuadMode() {
  uint8_t buf[1];

  qspi_command_t statusRegister;
  statusRegister.alt = 0;
  statusRegister.addr = 0;
  statusRegister.cfg =  QUADSPI_CCR_IMODE_0 | QUADSPI_CCR_IMODE_1
      | 0x5 | QUADSPI_CCR_DMODE_0 | QUADSPI_CCR_DMODE_1;

  qspiReceive(&QSPID1, &statusRegister, 1, buf);

  return buf[0] & 1;
}

void quadIndirectWriteMode(uint8_t *buf, uint32_t n, uint32_t address) {
    qspi_command_t indirectWriteMode;
    indirectWriteMode.alt = 0;
    indirectWriteMode.addr = address;
    indirectWriteMode.cfg =  QUADSPI_CCR_IMODE_1 | QUADSPI_CCR_IMODE_0
         | 0x12 | QUADSPI_CCR_ADSIZE_1
         | QUADSPI_CCR_ADMODE_1 | QUADSPI_CCR_ADMODE_0
         | QUADSPI_CCR_DMODE_1 | QUADSPI_CCR_DMODE_0;

    writeEnableQuad();
    CACHE_FLUSH(buf, n);
    qspiSend(&QSPID1, &indirectWriteMode, n, buf);
    CACHE_INVALIDATE(buf, n);
}

void quadIndirectReadMode(uint8_t *buf, uint32_t n, uint32_t address) {
    qspi_command_t indirectReadMode;
    indirectReadMode.alt = 0;
    indirectReadMode.addr = address;
    indirectReadMode.cfg =  QUADSPI_CCR_IMODE_1 | QUADSPI_CCR_IMODE_0
             | 0xEB | QUADSPI_CCR_ADSIZE_1
             | QUADSPI_CCR_ADMODE_1 | QUADSPI_CCR_ADMODE_0
             | QUADSPI_CCR_DCYC_3
             | QUADSPI_CCR_DMODE_1 | QUADSPI_CCR_DMODE_0;

    CACHE_FLUSH(buf, n);
    qspiReceive(&QSPID1, &indirectReadMode, n, buf);
    CACHE_INVALIDATE(buf, n);
}

void quadMappedModeEnable() {
    qspi_command_t indirectReadMode;
    indirectReadMode.alt = 0;
    indirectReadMode.addr = 0;
    indirectReadMode.cfg =  QUADSPI_CCR_IMODE_1 | QUADSPI_CCR_IMODE_0
              | 0xEB | QUADSPI_CCR_ADSIZE_1
              | QUADSPI_CCR_ADMODE_1 | QUADSPI_CCR_ADMODE_0
              | QUADSPI_CCR_DCYC_1 | QUADSPI_CCR_DCYC_3
              | QUADSPI_CCR_DMODE_1 | QUADSPI_CCR_DMODE_0;

    qspi_lld_map_flash(&QSPID1, &indirectReadMode, NULL);
}
