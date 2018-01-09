#ifndef _CORNER_H
#define _CORNER_H

#include "gfx.h"

#define PROGRESSBAR_RIGHT 0x01
#define PROGRESSBAR_LEFT  0x02
#define ICON_UP 0x04
#define ICON_DOWN 0x08
#define ICON_LEFT 0x10
#define ICON_RIGHT 0x20

typedef struct CornerObject_t {
    GWidgetObject w;  // Base Class

    gdispImage* imgFace;
    gdispImage* imgIcon;
    GHandle* progress;
    bool_t rendered;
    uint16_t flags;
    char* prefix;
    float value;
} CornerObject;

#ifdef __cplusplus
extern "C" {
#endif

    GHandle cornerGCreate(GDisplay* g, CornerObject* so, GWidgetInit* pInit,
                            gdispImage* imgFace, gdispImage* imgIcon,
                            GHandle* progressBar, uint16_t flags, char* prefix);
    #define cornerCreate(so, pI, imgFace, imgIcon, progressBar, flags, prefix) \
        cornerGCreate(GDISP, so, pI, imgFace, imgIcon, progressBar, flags, prefix)
	
    void cornerSetValue(GHandle gh, float value);
    uint16_t cornerGetValue(GHandle gh);
    void cornerDraw_Default(GWidgetObject* gw, void* param);

#ifdef __cplusplus
}
#endif

#endif
