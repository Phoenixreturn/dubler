#include "fuelGauge.h"
#include "src/gwin/gwin_class.h"

#define NEEDLE_THICKNESS        4
#define NEEDLE_SCALE_OFFSET     3

#define gh2obj ((FuelGaugeObject*)gh)
#define gw2obj ((FuelGaugeObject*)gw)
#define imgFaceWidth gw2obj->imgFace->width
#define imgFaceHeight gw2obj->imgFace->height

static void mainDraw(GWidgetObject* gw, void* param);

// The statusbar VMT
static const gwidgetVMT fuelVMT = {
	{
		"FuelGauge",						// The classname
		sizeof(FuelGaugeObject),			// The object size
		_gwidgetDestroy,			// The destroy routine
		_gwidgetRedraw,				// The redraw routine
		0,							// The after-clear routine
	},
	fuelDraw_Default,				// The default drawing routine
	#if GINPUT_NEED_MOUSE
		{
			0,						// Process mouse down events
			0,						// Process mouse up events
			0,						// Process mouse move events
		},
	#endif
	#if GINPUT_NEED_KEYBOARD || GWIN_NEED_KEYBOARD
		{
			0						// Process keyboard events
		},
	#endif
	#if GINPUT_NEED_TOGGLE
		{
			0,						// Toggle role
			0,						// Assign Toggles
			0,						// Get Toggles
			0,						// Process toggle off events
			0,						// Process toggle on events
		},
	#endif
	#if GINPUT_NEED_DIAL
		{
			0,						// No dial roles
			0,						// Assign Dials
			0,						// Get Dials
			0,						// Process dial move events
		},
	#endif
};

GHandle fuelGCreate(GDisplay* g, FuelGaugeObject* so, GWidgetInit* pInit,
                      gdispImage* imgFace, int value)
{
	// Create the base class (the actual widget)
	if (!(so = (FuelGaugeObject*)_gwidgetCreate(g, &so->w, pInit, &fuelVMT))) {
		return 0;
	}

	// Initialize the object struct
	so->imgFace = imgFace;
	so->rendered = FALSE;
	so->value = value;

//	 Set the initial visibility
	gwinSetVisible((GHandle)so, pInit->g.show);

	return (GHandle)so;
}

void fuelSetValue(GHandle gh, int value)
{
    // Make sure that this is a fuel
    if (gh->vmt != (gwinVMT*)&fuelVMT)
        return;

    gh2obj->value = value;

    _gwinUpdate(gh);
}

uint16_t fuelGetValue(GHandle gh)
{
	// Make sure that this is a corner
	if (gh->vmt != (gwinVMT*)&fuelVMT)
		return ((uint16_t)-1);

	return gh2obj->value;
}

void fuelDraw_Default(GWidgetObject* gw, void* param)
{
    GDisplay* disp = gdispGetDisplay(1);
    MatrixFloat2D m1, m2;

    // Make sure that this is a fuel gauge
    if (gw->g.vmt != (gwinVMT*)&fuelVMT)
      return;

    if(!gw2obj->rendered) {
      mainDraw(gw, param);
      gw2obj->rendered = TRUE;
    }

    gdispGFillArea(disp,
                   gw->g.x,
                   gw->g.y,
                   gw->g.width,
                   gw->g.height,
                   GDISP_STARTUP_COLOR);

    // Calculate the needle polygon (center of rotation must be at 0/0)
    point needleShape[4];
    needleShape[0].x = -gw->g.width/2 + NEEDLE_SCALE_OFFSET;
    needleShape[0].y = 0;
    needleShape[1].x = 0;
    needleShape[1].y = NEEDLE_THICKNESS;
    needleShape[2].x = 0;
    needleShape[2].y = -NEEDLE_THICKNESS;

    // Transform the needle shape
    gmiscMatrixFloat2DApplyRotation(&m1, 0, -gw2obj->value + 180);
    gmiscMatrixFloat2DApplyTranslation(&m2, &m1, gw->g.width/2, gw->g.height/2);
    gmiscMatrixFloat2DApplyToPoints(&needleShape, &needleShape, &m2, 4);

    // Draw the needle
    gdispGFillConvexPoly(disp, gw->g.x, gw->g.y, needleShape, 3, Red);
    gdispGFillCircle(disp,
                     gw->g.width/2 +gw->g.x,
                     gw->g.height/2 + gw->g.y,
                     5,
                     HTML2COLOR(0x515151));
}

static void mainDraw(GWidgetObject* gw, void* param) {
  // Draw the face
     GDisplay* disp = gdispGetDisplay(0);
     gdispGImageDraw(disp, gw2obj->imgFace,
     gw->g.x, gw->g.y, imgFaceWidth, imgFaceHeight, 0, 0);
}
