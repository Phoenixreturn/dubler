/*
 * fileLayout.h
 *
 *  Created on: 28 сент. 2017 г.
 *      Author: admin
 */

#ifndef UGFX_DEMOS_CUSTOM_DASHBOARD_FILELAYOUT_H_
#define UGFX_DEMOS_CUSTOM_DASHBOARD_FILELAYOUT_H_

#include "gfx.h"
#include "src/gfile/gfile_fs.h"
#include "3rdparty/fatfs-0.13/source/ff.h"

GFILE* readUgfxFile(char *name);
FRESULT formatQuadFlash();
void writeSimpleFile(char* name, uint8_t *buff, UINT n);
void readSimpleFile(char* name, uint8_t *buff, UINT n);
void writeUgfxImageFromROMtoFatFS(char* romName, char* fatName);

#endif /* UGFX_DEMOS_CUSTOM_DASHBOARD_FILELAYOUT_H_ */
