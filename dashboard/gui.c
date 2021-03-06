/******************************************************************************/
/* This file has been generated by the uGFX-Studio                            */
/*                                                                            */
/* http://ugfx.org                                                            */
/******************************************************************************/

#include "colors.h"
#include "widgetstyles.h"
#include "gui.h"

// GListeners
GListener glistener;


// GHandles
GHandle ghContainerPage0;
GHandle _ghDial;
GHandle ghCorner1;
GHandle ghCorner2;
GHandle ghCorner3;
GHandle ghCorner4;
GHandle ghFuelGauge;
GHandle tempProgressBar;
GHandle oilProgressBar;

gdispImage _imgTahDialFace;
gdispImage _corner1;
gdispImage _corner2;
gdispImage _corner3;
gdispImage _corner4;
gdispImage _batteryIcon;
gdispImage _oilIcon;
gdispImage _tempIcon;
gdispImage _fuelIcon;
gdispImage logo;
gdispImage _imgFuelGauge;

// Fonts
font_t times16_cyr;
font_t times16_asci;
font_t dejavu;
font_t times;

static void createPagePage0(void)
{
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);

	// create container widget: ghContainerPage0
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 480;
	wi.g.height = 272;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainerPage0 = gwinContainerCreate(0, &wi, 0);

    // Dial widget
    wi.g.show = TRUE;
    wi.g.width = _imgTahDialFace.width;
    wi.g.height = _imgTahDialFace.height;
    wi.g.y = (gdispGetHeight() - _imgTahDialFace.height)/2;
    wi.g.x = (gdispGetWidth() - _imgTahDialFace.width)/2;
    wi.g.parent = ghContainerPage0;
    wi.customDraw = dialDraw_Default;
    wi.text = "Dial Widget";
    _ghDial = dialCreate(0, &wi, &_imgTahDialFace, 250/13.88, 13.88);

    wi.g.show = FALSE;
    wi.g.y = 0;
    wi.g.x = 0;
    wi.g.parent = 0;
    wi.customDraw =  gwinProgressbarDraw_Std;
    wi.text = "";
    wi.g.width = 12;
    wi.g.height = 100;
    oilProgressBar = gwinGProgressbarCreate(gdispGetDisplay(1), 0, &wi);
    gwinProgressbarSetRange(tempProgressBar, 0, 100);

    wi.g.show = FALSE;
    wi.g.y = 0;
    wi.g.x = 0;
    wi.g.parent = 0;
    wi.customDraw =  gwinProgressbarDraw_Std;
    wi.text = "";
    wi.g.width = 12;
    wi.g.height = 100;
    tempProgressBar = gwinGProgressbarCreate(gdispGetDisplay(1), 0, &wi);
    gwinProgressbarSetRange(tempProgressBar, 0, 120);

    // Corner1 widget
    wi.g.show = TRUE;
    wi.g.width = _corner1.width;
    wi.g.height = _corner1.height;
    wi.g.y = 20;
    wi.g.x = 20;
    wi.g.parent = ghContainerPage0;
    wi.customDraw = cornerDraw_Default;
    wi.text = "Corner Widget";
    ghCorner1 = cornerCreate(0, &wi, &_corner1,
                             &_batteryIcon,
                             0,
                             ICON_UP | ICON_RIGHT | PROGRESSBAR_LEFT,
                             "В");

    // Corner2 widget
    wi.g.show = TRUE;
    wi.g.width = _corner2.width;
    wi.g.height = _corner2.height;
    wi.g.y = 20;
    wi.g.x = 460 - 153;
    wi.g.parent = ghContainerPage0;
    wi.customDraw = cornerDraw_Default;
    wi.text = "Corner Widget";
    ghCorner2 = cornerCreate(0, &wi, &_corner2,
                             &_oilIcon, &oilProgressBar,
                             PROGRESSBAR_RIGHT | ICON_UP | ICON_LEFT,
                             "кгс/см²");

    // Corner3 widget
    wi.g.show = TRUE;
    wi.g.width = _corner3.width;
    wi.g.height = _corner3.height;
    wi.g.y = 272 - 132;
    wi.g.x = 460 - 153;
    wi.g.parent = ghContainerPage0;
    wi.customDraw = cornerDraw_Default;
    wi.text = "Corner Widget";
    ghCorner3 = cornerCreate(0, &wi, &_corner3,
                             0, 0,
                             ICON_DOWN | ICON_LEFT, "");

    // Corner4 widget
    wi.g.show = TRUE;
    wi.g.width = _corner4.width;
    wi.g.height = _corner4.height;
    wi.g.y = 272 - 132;
    wi.g.x = 20;
    wi.g.parent = ghContainerPage0;
    wi.customDraw = cornerDraw_Default;
    wi.text = "Corner Widget";
    ghCorner4 = cornerCreate(0, &wi, &_corner4,
                             &_tempIcon, &tempProgressBar,
                             PROGRESSBAR_LEFT | ICON_DOWN | ICON_RIGHT,
                             "°C");

    // Fuel gauge widget
//    wi.g.show = TRUE;
//    wi.g.width = _imgFuelGauge.width;
//    wi.g.height = _imgFuelGauge.height;
//    wi.g.y = 143;
//    wi.g.x = 345;
//    wi.g.parent = ghContainerPage0;
//    wi.customDraw = fuelDraw_Default;
//    wi.text = "Fuel Gauge Widget";
//    ghFuelGauge = fuelCreate(0, &wi, &_imgFuelGauge, 40);
//    fuelSetValue(_ghDial, 40);
}

void guiShowPage(unsigned pageIndex)
{
	// Hide all pages
	gwinHide(ghContainerPage0);

	// Show page selected page
	switch (pageIndex) {
	case 0:
		gwinShow(ghContainerPage0);
		break;

	default:
		break;
	}
}

void guiCreate(void)
{
	GWidgetInit wi;

	// Prepare fonts
	times16_asci = gdispOpenFont("times18");
	times16_cyr = gdispOpenFont("times_cyr16");
	dejavu = gdispOpenFont("DejaVuSans16.c");

	// Prepare images
    gdispImageOpenFile(&_imgTahDialFace, "F|tahometer");
    gdispImageOpenFile(&_imgFuelGauge, "F|fuelGauge");
    gdispImageOpenFile(&_corner1, "F|corner1");
    gdispImageOpenFile(&_corner2, "F|corner2");
    gdispImageOpenFile(&_corner3, "F|corner3");
    gdispImageOpenFile(&_corner4, "F|corner4");
    gdispImageOpenFile(&_oilIcon, "F|oilIcon");
    gdispImageOpenFile(&_tempIcon, "F|tempIcon");
    gdispImageOpenFile(&_batteryIcon, "F|batteryIcon");
    gdispImageOpenFile(&_fuelIcon, "F|fuelIcon");
    gdispImageOpenFile(&logo, "F|logo");

	// GWIN settings
	gwinWidgetClearInit(&wi);
	gwinSetDefaultFont(times16_asci);
	gwinSetDefaultStyle(&gray, FALSE);
	gwinSetDefaultColor(black_studio);
	gwinSetDefaultBgColor(gray_studio);

	// Create all the display pages
	createPagePage0();

	// startup logo
   gdispGImageDraw(gdispGetDisplay(0),
                   &logo,
                   0,
                   0,
                   logo.width,
                   logo.height,
                   0,
                   0);
	chThdSleepMilliseconds(4000);

	gdispGClear(gdispGetDisplay(0), Black);

	// Select the default display page
	guiShowPage(0);

}

void guiEventLoop(void)
{
  zeroAll = false;
  while (1) {
    wdgReset(&WDGD1);
    chMtxLock(&mtx1);
    if(zeroAll) {
      zeroAll = false;
      cornerSetValue(ghCorner1, 0);
      cornerSetValue(ghCorner2, 0);
      cornerSetValue(ghCorner4, 0);
      gwinProgressbarSetPosition(oilProgressBar, 0);
      gwinProgressbarSetPosition(tempProgressBar, 0);
    }
    chMtxUnlock(&mtx1);
    chThdSleepMilliseconds(1000);
  }
}
