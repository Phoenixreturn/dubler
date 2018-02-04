#include "corner.h"
#include "src/gwin/gwin_class.h"

#define gh2obj ((CornerObject*)gh)
#define gw2obj ((CornerObject*)gw)

static font_t mainFont;
static mutex_t mtx1;

static void mainDraw(GWidgetObject* gw, void* param);
static void drawIconDown(GWidgetObject* gw, void* param);
static void drawIconUp(GWidgetObject* gw, void* param);
static void drawValue(CornerObject* gw, float value);

// The statusbar VMT
static const gwidgetVMT cornerVMT = {
	{
		"Corner",						// The classname
		sizeof(CornerObject),			// The object size
		_gwidgetDestroy,			// The destroy routine
		_gwidgetRedraw,				// The redraw routine
		0,							// The after-clear routine
	},
	cornerDraw_Default,				// The default drawing routine
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

GHandle cornerGCreate(GDisplay* g, CornerObject* so, GWidgetInit* pInit,
                      gdispImage* imgFace, gdispImage* imgIcon,
                      GHandle* progressBar, uint16_t flags, char* prefix)
{
	// Create the base class (the actual widget)
	if (!(so = (CornerObject*)_gwidgetCreate(g, &so->w, pInit, &cornerVMT))) {
		return 0;
	}

	// Initialize the object struct
	so->imgFace = imgFace;
	so->progress = progressBar;
	so->imgIcon = imgIcon;
	so->flags = flags;
	so->prefix = prefix;
	so->rendered = FALSE;
	so->value = 0;
    chMtxObjectInit(&mtx1);
    mainFont = gdispOpenFont("501548");

//	 Set the initial visibility
	gwinSetVisible((GHandle)so, pInit->g.show);

	return (GHandle)so;
}

void cornerSetValue(GHandle gh, float value)
{
  // Make sure that this is a dial
    if (gh->vmt != (gwinVMT*)&cornerVMT)
        return;
    void* param;
    gh2obj->value = value;
    _gwinUpdate(gh);
}

uint16_t cornerGetValue(GHandle gh)
{
	// Make sure that this is a corner
	if (gh->vmt != (gwinVMT*)&cornerVMT)
		return ((uint16_t)-1);

	return gwinProgressbarGetPosition(gh2obj->progress);
}

void cornerDraw_Default(GWidgetObject* gw, void* param)
{
    if(!gw2obj->rendered) {
      mainDraw(gw, param);
      gw2obj->rendered = TRUE;
    }

    drawValue(gw2obj, gw2obj->value);
}

static void drawValue(CornerObject* obj, float value) {
  GDisplay* disp1 = gdispGetDisplay(1);
  GWidgetObject* gw = &obj->w;
  chMtxLock(&mtx1);
  if(value <= 1000) {
      char str[10];
      sprintf(str, "%.1f", value);
      if(gw2obj->progress) {
        gwinProgressbarSetPosition(gw2obj->progress, value);
      }
      if(gw2obj->flags & PROGRESSBAR_LEFT) {
        gdispGFillArea(disp1,
                       gw->g.x + gw->g.width/6,
                       gw->g.y + gw->g.height/3,
                       gw->g.width/2,
                       gw->g.height/3,
                       GDISP_STARTUP_COLOR);
        gdispGDrawStringBox(disp1,
                           gw->g.x + gw->g.width/6,
                           gw->g.y + gw->g.height/3,
                           gw->g.width/2,
                           gw->g.height/3,
                           str,
                           mainFont,
                           HTML2COLOR(0x999999),
                           justifyCenter);
      } else if(gw2obj->flags & PROGRESSBAR_RIGHT) {
        gdispGFillArea(disp1,
                       gw->g.x + gw->g.width/3 + 6,
                       gw->g.y + gw->g.height/3,
                       gw->g.width/2,
                       gw->g.height/3,
                       GDISP_STARTUP_COLOR);
        gdispGDrawStringBox(disp1,
                           gw->g.x + gw->g.width/3 + 6,
                           gw->g.y + gw->g.height/3,
                           gw->g.width/2,
                           gw->g.height/3,
                           str,
                           mainFont,
                           HTML2COLOR(0x999999),
                           justifyCenter);
      }
  }
  chMtxUnlock(&mtx1);
}

static void mainDraw(GWidgetObject* gw, void* param) {
    gdispGImageDraw(gdispGetDisplay(0), gw2obj->imgFace,
                    gw->g.x, gw->g.y, gw->g.width, gw->g.height, 0, 0);

    if(gw2obj->imgIcon) {
      if(gw2obj->flags & ICON_UP) {
        drawIconUp(gw, param);
      } else if(gw2obj->flags & ICON_DOWN){
        drawIconDown(gw, param);
      }
    }

    //   Set the progressBar visible
    if(gw2obj->progress) {
      GHandle progressBar = *(gw2obj->progress);
      volatile uint16_t temp = gw2obj->flags;
      if(temp & PROGRESSBAR_LEFT) {
        gwinMove(progressBar, gw->g.x + 8, gw->g.y + 5);
        gwinShow(progressBar);
      } else if(temp & PROGRESSBAR_RIGHT) {
        gwinMove(progressBar,
                 gw->g.x + gw2obj->imgFace->width - 8 - progressBar->width,
                 gw->g.y + 6);
        gwinShow(progressBar);
      }
    }
}

static void drawIconUp(GWidgetObject* gw, void* param) {
    GDisplay* disp = gdispGetDisplay(0);

    if(gw2obj->flags & ICON_RIGHT) {
      gdispGImageDraw(disp,
                      gw2obj->imgIcon,
                      gw->g.x + gw2obj->imgFace->width - gw2obj->imgIcon->width - 20 ,
                      gw->g.y + 8,
                      gw2obj->imgIcon->width,
                      gw2obj->imgIcon->height,
                      0, 0);
      gdispGDrawStringBox(disp,
                          gw->g.x + gw2obj->imgFace->width - gw2obj->imgIcon->width - 120,
                          gw->g.y + 3,
                          100,
                          24,
                          gw2obj->prefix,
                          gdispOpenFont("times18"),
                          HTML2COLOR(0x999999),
                          justifyRight);
    } else if(gw2obj->flags & ICON_LEFT) {
      gdispGImageDraw(disp,
                     gw2obj->imgIcon,
                     gw->g.x + 20 ,
                     gw->g.y + 8,
                     gw2obj->imgIcon->width,
                     gw2obj->imgIcon->height,
                     0, 0);
      gdispGDrawStringBox(disp,
                         gw->g.x + gw2obj->imgIcon->width + 25,
                         gw->g.y + 3,
                         100,
                         24,
                         gw2obj->prefix,
                         gdispOpenFont("times18"),
                         HTML2COLOR(0x999999),
                         justifyLeft);
    }
}

static void drawIconDown(GWidgetObject* gw, void* param) {
  GDisplay* disp = gdispGetDisplay(0);

  if(gw2obj->flags & ICON_RIGHT) {
    gdispGImageDraw(disp,
                    gw2obj->imgIcon,
                    gw->g.x + gw2obj->imgFace->width - gw2obj->imgIcon->width - 25 ,
                    gw->g.y + gw2obj->imgFace->height - gw2obj->imgIcon->height - 8,
                    gw2obj->imgIcon->width,
                    gw2obj->imgIcon->height,
                    0, 0);
    gdispGDrawStringBox(disp,
                       gw->g.x + gw2obj->imgFace->width - gw2obj->imgIcon->width - 130,
                       gw->g.y + gw2obj->imgFace->height - 30,
                       100,
                       24,
                       gw2obj->prefix,
                       gdispOpenFont("times18"),
                       HTML2COLOR(0x999999),
                       justifyRight);
  } else if(gw2obj->flags & ICON_LEFT) {
    gdispGImageDraw(disp,
                   gw2obj->imgIcon,
                   gw->g.x + 30 ,
                   gw->g.y + gw2obj->imgFace->height - gw2obj->imgIcon->height - 8,
                   gw2obj->imgIcon->width,
                   gw2obj->imgIcon->height,
                   0, 0);
    gdispGDrawStringBox(disp,
                       gw->g.x + gw2obj->imgIcon->width + 25,
                       gw->g.y + gw2obj->imgFace->height - 30,
                       100,
                       24,
                       gw2obj->prefix,
                       gdispOpenFont("times18"),
                       HTML2COLOR(0x999999),
                       justifyLeft);
  }
}
