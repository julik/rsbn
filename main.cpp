#define PLUGIN_VERSION  "0.0.1"
#define PLUGIN_SIGNATURE "com.julik.rsbn"
#define USSR_DATA (void*)1
#define CIS_DATA (void*)2
#define ABOUT_SW (void*)3

#include "libs.h"
#include "utils.h"
#include "gate.h"

static Database rsbn;
static Gate proxy;

static XPLMMenuID rsbnMenu;
static int rsbnMenuItem = -1;
static XPWidgetID gWindow = NULL;
static bool plugIsEnabled = TRUE;

inline int clamp(int min, int value, int max) {
    return value >= max ? max : (value <= min ? min : value);
}

// Data access callbacks. All X-Plane callbacks for data sets/gets need a void refcon pointer at the start
static int getStrobe(void* inRefcon) {
    return rsbn.selStrobe;
}

static void setStrobe(void* inRefcon, int newStrobe) {
    rsbn.selStrobe = clamp(0, newStrobe, 4);
}

static int getNul(void* inRefcon) {
    return rsbn.selNul;
}

static void setNul(void* inRefcon, int newNul) {
    rsbn.selNul = clamp(0, newNul, 9);
}

static float getDist(void* inRefcon) {
    return rsbn.getDistance();
}

static float getBearing(void* inRefcon) {
    return rsbn.getBearing();
}

static int getOverflight(void* inRefcon) {
    return rsbn.isOverflyingNow();
}

static int getReceiving(void* inRefcon) {
    return rsbn.isReceiving();
}

static float updateRsbn(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop,  int counter, void *refcon)
{
    if(plugIsEnabled) proxy.update();
    return -1;
}

static float getBearingTo(void *inRefcon)
{
    return rsbn.getInverseBearing();
}

static float getBearingToMag(void *inRefcon)
{
    return rsbn.getInverseBearing() + XPLMGetDataf(proxy.magVarRef);
}

/*
static int drawMapCB(XPLMDrawingPhase inPhase, int inIsBefore, void *inRefcon)
{
    if(inPhase != xplm_Phase_LocalMap3D) return 1;
    
    proxy.drawToMap();
    return 1;
}

 */

static int infoWindowCB(XPWidgetMessage message, XPWidgetID widget,
        long param1, long param2)
{
    if (message == xpMessage_CloseButtonPushed) {
        XPHideWidget(gWindow);
        XPDestroyWidget(gWindow, TRUE);
        return 1;
    }
    return 0;
}

static void drawDebugInfo(XPWidgetID intoWindow, int x, int y)
{
    int lineLen = 100;
    
    int atLeft = x + 10;
    int atRight = x + lineLen;
    int atTop = y;
    vector<string> infoLines;
    
    char c_info[lineLen];
    strcpy(c_info, PLUGIN_VERSION);
    
    char c_attrib[] = "By Julik, beacon data by Andrey Pryadko";
    infoLines.push_back(string(c_attrib));
    char c_lic[] = "This plugin is free software, licensed under the MIT license";
    infoLines.push_back(string(c_lic));
    
    infoLines.push_back(string(" "));

    char c_inf[lineLen];
    rsbn.tunedBeaconInfo(c_inf);
    infoLines.push_back(string(c_inf));
	
    char c_brg[lineLen];
	snprintf(c_brg, lineLen,  "rsbn/bearing (dTrue) %.2f", getBearing(NULL));
    infoLines.push_back(string(c_brg));

	char c_brg_to[lineLen];
	snprintf(c_brg_to, lineLen,  "rsbn/bearing_to (dTrue) %.2f", getBearingTo(NULL));
    infoLines.push_back(string(c_brg_to));

	char c_brg_to_mag[lineLen];
	snprintf(c_brg_to_mag, lineLen,  "rsbn/bearing_to_mag (dMag) %.2f", getBearingToMag(NULL));
    infoLines.push_back(string(c_brg_to_mag));

	char c_dist[lineLen];
	snprintf(c_dist, lineLen, "rsbn/distance (km) %.2f", getDist(NULL));
    infoLines.push_back(string(c_dist));

	char c_ovr[lineLen];
	snprintf(c_ovr, lineLen, "rsbn/overflight %1d", getOverflight(NULL));
    infoLines.push_back(string(c_ovr));

	char c_reception[lineLen];
	snprintf(c_reception, lineLen, "rsbn/receiving %1d", getReceiving(NULL));
    infoLines.push_back(string(c_reception));

	char c_sn[lineLen];
	snprintf(c_sn, lineLen, "rsbn/strobe and rsbn/nul %1d %1d", getStrobe(NULL), getNul(NULL));
    infoLines.push_back(string(c_sn));
    
    int endAt;
    for (vector<string>::iterator curItem = infoLines.begin(); curItem != infoLines.end(); ++curItem) {
        string line = (*curItem);
        atTop-= 15; endAt = atTop - 10;
        
        XPCreateWidget(atLeft, atTop, atRight, endAt,
             1, line.c_str(), 0, intoWindow, xpWidgetClass_Caption);
    }
}

void rsbn_selectDataset(void* menuRef, void* selection)
{
    char dataPath[1024];
    if (USSR_DATA == selection) {
        detectDatabasePath(dataPath, "ussr.dat");
        if (menuRef != NULL) XPLMCheckMenuItem(rsbnMenu, 1, xplm_Menu_Unchecked);
        if (menuRef != NULL) XPLMCheckMenuItem(rsbnMenu, 0, xplm_Menu_Checked);
    } else if (CIS_DATA == selection) {
        detectDatabasePath(dataPath, "cis.dat");
        if (menuRef != NULL) XPLMCheckMenuItem(rsbnMenu, 0, xplm_Menu_Unchecked);
        if (menuRef != NULL) XPLMCheckMenuItem(rsbnMenu, 1, xplm_Menu_Checked);
    } else if (ABOUT_SW == selection) {
        // Init the inspector
        int x = 100;
        int y = 700;
        int w = 400;
        int h = 195;
        
        int x2 = x + w;
        int y2 = y - h;
        
        gWindow = XPCreateWidget(x, y, x2, y2, 1, "RSBN system", 1, NULL,  xpWidgetClass_MainWindow);
        drawDebugInfo(gWindow, x, y - 10);
        XPSetWidgetProperty(gWindow, xpProperty_MainWindowHasCloseBoxes, 1);
        XPAddWidgetCallback(gWindow, infoWindowCB);
        return;
    }
    
    rsbn.loadDataFrom(dataPath);
    char msg[256];
    sprintf(msg, "RSBN: loaded %d beacons\n", rsbn.size());
    XPLMDebugString(msg);
}

void rsbn_initMenu()
{
    XPLMMenuID pluginsMenu = XPLMFindPluginsMenu();
    
    if (-1 == rsbnMenuItem) {
        rsbnMenuItem = XPLMAppendMenuItem(pluginsMenu, "RSBN",  NULL, 1);
    }
    
    rsbnMenu = XPLMCreateMenu("RSBN", pluginsMenu, rsbnMenuItem, rsbn_selectDataset, NULL);
    XPLMAppendMenuItem(rsbnMenu, "USSR beacons", USSR_DATA, 1);
    XPLMAppendMenuItem(rsbnMenu, "CIS beacons", CIS_DATA, 1);
    XPLMAppendMenuSeparator(rsbnMenu);
    XPLMAppendMenuItem(rsbnMenu, "About...", ABOUT_SW, 1);
}

// start plugin
PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc)
{
    XPLMDebugString("RSBN: Init...\n");
    strcpy(outName, "RSBN beacons for X-Plane");
    strcpy(outName, PLUGIN_VERSION);
    strcpy(outSig, PLUGIN_SIGNATURE);
    strcpy(outDesc, "Provides RSBN navigation facilties");
    
    rsbn_initMenu();
    rsbn_selectDataset(NULL, USSR_DATA);
    
    // Strobe
    XPLMRegisterDataAccessor("rsbn/strobe",
            xplmType_Int,                                  // The types we support
            TRUE,                                          // Writable
            getStrobe, setStrobe,                          // Integer accessors
            NULL, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            NULL, NULL);                                   // Refcons not used
    
    // Nul
    XPLMRegisterDataAccessor("rsbn/nul",
            xplmType_Int,                                  // The types we support
            TRUE,                                             // Writable
            getNul, setNul,                                // Integer accessors
            NULL, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            NULL, NULL);                                   // Refcons not used
                                        
    // Dataref for distance, wired to a callback on the database
    XPLMRegisterDataAccessor("rsbn/distance",
            xplmType_Float,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                                 // Integer accessors
            getDist, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            NULL, NULL);                                   // Refcons not used

    // Dataref for bearing, wired to a callback on the database
    XPLMRegisterDataAccessor("rsbn/bearing",
            xplmType_Float,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            getBearing, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            NULL, NULL);                                   // Refcons not used

    // Dataref for bearing TO, wired to a callback on the database
    XPLMRegisterDataAccessor("rsbn/bearing_to",
            xplmType_Float,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            getBearingTo, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            NULL, NULL);                                   // Refcons not used

    // Dataref for bearing TO, wired to a callback on the database
    XPLMRegisterDataAccessor("rsbn/bearing_to_mag",
            xplmType_Float,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            getBearingToMag, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            NULL, NULL);                                   // Refcons not used
    
    // Will contain 1 if the RSBN beacon is being overflown (is within the blind mushroom of non-reception)
    XPLMRegisterDataAccessor("rsbn/overflight",
            xplmType_Int,                                // The types we support
            FALSE,                                             // Writable
            getOverflight, NULL,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            NULL, NULL);                                   // Refcons not used
    
    // Will contain 1 if the RSBN beacon set on channel is being received
    XPLMRegisterDataAccessor("rsbn/receiving",
            xplmType_Int,           // The types we support
            FALSE,                  // Writable
            getReceiving, NULL,     // Integer accessors
            NULL, NULL,             // Float accessors
            NULL, NULL,             // Doubles accessors
            NULL, NULL,             // Int array accessors
            NULL, NULL,             // Float array accessors
            NULL, NULL,             // Raw data accessors
            NULL, NULL);            // Refcons not used
    
    // Assign the gateway objects
    proxy.db = &rsbn;   
    proxy.attachDatarefs();
    
    // Register the main callback
    XPLMRegisterFlightLoopCallback(updateRsbn, -1, NULL);
    
    // XPLMRegisterDrawCallback( drawMapCB, xplm_Phase_LocalMap3D, 0, NULL);
    return 1;
}

// Clean up
PLUGIN_API void XPluginStop(void)
{
    XPLMDestroyWindow(gWindow);
}


// Disable plugin
PLUGIN_API void XPluginDisable(void)
{
    plugIsEnabled = FALSE;
}


// Enable plugin
PLUGIN_API int XPluginEnable(void)
{
    plugIsEnabled = TRUE;
    return 1;
}


PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, long inMsg, void *inParam)
{
    if (inMsg == XPLM_MSG_AIRPORT_LOADED || inMsg == XPLM_MSG_PLANE_LOADED) {
        // Force an update of all of the beacon coordinates in X-Plane XYZ
    }
}