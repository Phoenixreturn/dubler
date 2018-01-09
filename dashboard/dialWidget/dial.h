#ifndef _DIAL_H
#define _DIAL_H

#include "gfx.h"

/**
 * The Event Type for a dial event
 */
#define GEVENT_DIAL		  (GEVENT_USER_FIRST + 0)

/* A dial widget object.
 * Treat it as a black box. Never access it directly.
 */
typedef struct DialObject_t {
	GWidgetObject w;  // Base Class

	gdispImage* imgFace;
	float angle; // degrees
	uint16_t value;
	float zeroAngle;
	float ratio;
	bool_t rendered;
} DialObject;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * Create a dial widget.
	 */
	GHandle dialGCreate(GDisplay* g, DialObject* so, GWidgetInit* pInit, gdispImage* imgFace,
	                    float zeroAngle,
	                    float ratio);
	#define dialCreate(so, pI, imgFace, zeroAngle, ratio) dialGCreate(GDISP, so, pI, \
	                                                                  imgFace, \
	                                                                  zeroAngle, \
                                                                      ratio)

	void dialDraw_Default(GWidgetObject* gw, void* param);
	void dialSetValue(GHandle gh, uint16_t value);
	uint16_t dialGetValue(GHandle gh);

#ifdef __cplusplus
}
#endif

#endif /* _DIAL_H */
