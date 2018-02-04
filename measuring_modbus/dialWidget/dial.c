#include "dial.h"
#include "src/gwin/gwin_class.h"

#define NEEDLE_THICKNESS		6
#define NEEDLE_SCALE_OFFSET		15

#define gh2obj ((DialObject*)gh)
#define gw2obj ((DialObject*)gw)
#define imgFaceWidth gw2obj->imgFace->width
#define imgFaceHeight gw2obj->imgFace->height

static font_t mainFont;
static mutex_t mtx1;

static void dialSetAngle(GHandle gh, float angle);
static float dialGetAngle(GHandle gh);
static void mainDraw(GWidgetObject* gw);

// The statusbar VMT
static const gwidgetVMT dialVMT = {
	{
		"Dial",						// The classname
		sizeof(DialObject),			// The object size
		_gwidgetDestroy,			// The destroy routine
		_gwidgetRedraw,				// The redraw routine
		0,							// The after-clear routine
	},
	dialDraw_Default,				// The default drawing routine
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

GHandle dialGCreate(GDisplay* g, DialObject* so, GWidgetInit* pInit,
                    gdispImage* imgFace,
                    float zeroAngle,
                    float ratio)
{
	// Create the base class (the actual widget)
	if (!(so = (DialObject*)_gwidgetCreate(g, &so->w, pInit, &dialVMT))) {
		return 0;
	}

	// Initialize the object struct
	so->imgFace = imgFace;
	so->rendered = FALSE;
	so->value = 0;
	so->ratio = ratio;
	so->zeroAngle = zeroAngle;
	so->angle = -zeroAngle;

	chMtxObjectInit(&mtx1);
	mainFont = gdispOpenFont("501524");

//	 Set the initial visibility
	gwinSetVisible((GHandle)so, pInit->g.show);

	return (GHandle)so;
}

static void dialSetAngle(GHandle gh, float angle)
{
	// Make sure that this is a dial
	if (gh->vmt != (gwinVMT*)&dialVMT)
		return;

	gh2obj->angle = angle;

	_gwinUpdate(gh);
}

static float dialGetAngle(GHandle gh)
{
	// Make sure that this is a dial
	if (gh->vmt != (gwinVMT*)&dialVMT)
		return ((uint16_t)-1);

	return gh2obj->angle;
}

void dialDraw_Default(GWidgetObject* gw, void* param)
{
    GDisplay* disp1 = gdispGetDisplay(1);
    GDisplay* pixmap = gdispPixmapCreate(imgFaceWidth, imgFaceHeight);
    pixel_t* surface;
    char str[10];
    float tempValue = gw2obj->value/10;
    sprintf(str, "%.1f", tempValue);

	MatrixFloat2D m1, m2;

	// Make sure that this is a dial
	if (gw->g.vmt != (gwinVMT*)&dialVMT)
		return;

	if(!gw2obj->rendered) {
	    mainDraw(gw);
	    gw2obj->rendered = TRUE;
	}

	gdispGFillArea(disp1, gw->g.x, gw->g.y, gw->g.width, gw->g.height, GDISP_STARTUP_COLOR);

	// Calculate the needle polygon (center of rotaton must be at 0/0)
	point needleShape[4];
	needleShape[0].x = -gw->g.width/2 + NEEDLE_SCALE_OFFSET;
	needleShape[0].y = 0;
	needleShape[1].x = 0;
	needleShape[1].y = NEEDLE_THICKNESS;
	needleShape[2].x = 0;
	needleShape[2].y = -NEEDLE_THICKNESS;

	// Transform the needle shape
	gmiscMatrixFloat2DApplyRotation(&m1, 0, -gw2obj->angle);
	gmiscMatrixFloat2DApplyTranslation(&m2, &m1, gw->g.width/2, gw->g.height/2);
	gmiscMatrixFloat2DApplyToPoints(&needleShape, &needleShape, &m2, 4);

	// Draw the needle
	gdispGFillConvexPoly(pixmap, 0, 0, needleShape, 3, Red);
    gdispGFillCircle(pixmap, gw->g.width/2, gw->g.height/2, 15, HTML2COLOR(0x000011));
//	gdispGFillConvexPoly(disp, gw->g.x, gw->g.y, needleShape, 3, Red);
//	gdispGFillCircle(disp, gw->g.width/2 +gw->g.x, gw->g.height/2 + gw->g.y, 15, HTML2COLOR(0x000011));

    gdispGBlitArea (disp1,
                    gw->g.x,
                    gw->g.y,
                    gw->g.width,
                    gw->g.height,
                    0, 0,
                    gw->g.width,
                    gdispPixmapGetBits(pixmap));
    chMtxLock(&mtx1);
    gdispGDrawStringBox(disp1, gw->g.x + gw->g.width/2 - gw->g.width/4,
            gw->g.y + gw->g.height*3/4 - 15, gw->g.width/2, gw->g.height/8,
                str, mainFont, HTML2COLOR(0x111111), justifyCenter);
    chMtxUnlock(&mtx1);
	gdispPixmapDelete(pixmap);
}

void dialSetValue(GHandle gh, uint16_t value) {
  // Make sure that this is a dial
  if (gh->vmt != (gwinVMT*)&dialVMT)
      return;
  void* param;
  if(value > gh2obj->value) {
    if(value - gh2obj->value > 15) {
      gh2obj->value = value;
      gh2obj->angle = -gh2obj->zeroAngle + value/gh2obj->ratio;

      _gwinUpdate(gh);
    }
  } else {
    if(gh2obj->value - value > 15) {
      gh2obj->value = value;
      gh2obj->angle = -gh2obj->zeroAngle + value/gh2obj->ratio;

      _gwinUpdate(gh);
    }
  }
}

uint16_t dialGetValue(GHandle gh) {
  // Make sure that this is a dial
      if (gh->vmt != (gwinVMT*)&dialVMT)
          return ((uint16_t)-1);

      return gh2obj->value;
}

static void mainDraw(GWidgetObject* gw) {
    // Draw the face
    GDisplay* disp = gdispGetDisplay(0);
    gdispGImageDraw(disp, gw2obj->imgFace,
    gw->g.x, gw->g.y, imgFaceWidth, imgFaceHeight, 0, 0);
}

// Limit the scope of the macros. Required for single-file compilation.
#undef gh2obj
#undef gw2obj
#undef imgFaceWidth
#undef imgFaceHeight
