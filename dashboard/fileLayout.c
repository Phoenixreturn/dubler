#include "fileLayout.h"

FRESULT formatQuadFlash() {
  uint8_t workBuffer[2048];
  return f_mkfs("QSPI", FM_FAT | FM_SFD, 4*512, workBuffer, 2048);
}

void writeSimpleFile(char* name, uint8_t *buff, UINT n) {
  FATFS fileSystem;
  FIL testFile;
  f_mount(&fileSystem, "QSPI", 1);
  volatile UINT sizeB = 0;

  volatile FRESULT ttt = f_open(&testFile, name, FA_CREATE_ALWAYS | FA_WRITE);
  volatile FRESULT writeFl = f_write (&testFile, buff, n, &sizeB);
  volatile closeSt = f_close(&testFile);
}

void readSimpleFile(char* name, uint8_t *buff, UINT n) {
  FATFS fileSystem;
  FIL testFile;
  f_mount(&fileSystem, "QSPI", 1);
  volatile UINT sizeB = 0;

  volatile FRESULT ttrt = f_open(&testFile, name, FA_READ);
  volatile FRESULT readStatus =  f_read (&testFile, buff, n, &sizeB);
  volatile closeSrt = f_close(&testFile);
}

void writeUgfxImageFromROMtoFatFS(char* romName, char* fatName) {
  gfileMount('F', "QSPI");
  GFILE* imgFile;
  GFILE *imgFatFile;
  imgFile = gfileOpen(romName, "rb");
  imgFatFile = gfileOpen(fatName, "wx");

  uint32_t imgSize = gfileGetSize(imgFile);
  uint8_t *imgBuf = gfxAlloc(gfileGetSize(imgFile));

  gfileRead(imgFile, imgBuf, gfileGetSize(imgFile));
  gfileWrite(imgFatFile, imgBuf, imgSize);
  gfxFree(imgBuf);
  gfileClose(imgFatFile);
  gfileClose(imgFile);
}

GFILE* readUgfxFile(char *name) {
  volatile bool_t resultMount = gfileMount('F', "QSPI");
  GFILE* imgFile;
  imgFile = gfileOpen(name, "rb");

  volatile uint16_t imgSize = gfileGetSize(imgFile);
  volatile uint8_t *imgBuf = gfxAlloc(gfileGetSize(imgFile));

  gfileRead(imgFile, imgBuf, gfileGetSize(imgFile));
  return imgFile;
}
