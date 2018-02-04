#ifndef _FUEL_H
#define _FUEL_H

#include "gfx.h"


/* A fuel gauge widget object.
 * Treat it as a black box. Never access it directly.
 */
typedef struct FuelGaugeObject_t {
    GWidgetObject w;  // Base Class

    gdispImage* imgFace;
    int value;
    bool_t rendered;
} FuelGaugeObject;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * Create a fuel gauge widget.
	 */
    GHandle fuelGCreate(GDisplay* g, FuelGaugeObject* so, GWidgetInit* pInit, gdispImage* imgFace, int value);
    #define fuelCreate(so, pI, imgFace, value) fuelGCreate(GDISP, so, pI, imgFace, value)
	
    /**
    * Set the angle of the fuel needle in degrees.
    */
    void fuelSetValue(GHandle gh, int value);

    /**
    * Get the current angle of the fuel needle in degrees.
    */
    uint16_t fuelGetValue(GHandle gh);

    /**
    * The default rendering routine
    */
    void fuelDraw_Default(GWidgetObject* gw, void* param);

#ifdef __cplusplus
}
#endif

#endif /* _FUEL_H */
