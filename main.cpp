#define PLUGIN_VERSION  "0.0.2"
#define PLUGIN_SIGNATURE "com.julik.rsbn"
#define USSR_DATA (void*)1
#define CIS_DATA (void*)2
#define ABOUT_SW (void*)3
#define UPDATE_INTERVAL -1

#include "libs.h"
#include "utils.h"
#include "gate.h"
#include "navigator.h"

static Database rsbn;
static Navigator nav;
static Gate proxy;
static vector<XPLMDataRef> rsbnDatarefs;
static XPLMMenuID rsbnMenu;
static int rsbnMenuItem = -1;
static XPWidgetID gWindow = NULL;
static bool plugIsEnabled = TRUE;

#include "accessors_base.h"
#include "accessors_nav.h"

static double getBearingToMag(void *inRefcon)
{
    // jajaja Database* d = reinterpret_cast<Database*>(inRefcon);
    return rsbn.getInverseBearing() + XPLMGetDataf(proxy.magVarRef);
}

static float updateRsbn(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop,  int counter, void *refcon)
{
    if(plugIsEnabled) {
        proxy.update(counter);
        nav.update(rsbn.getBearing(), rsbn.getDistance());
    }
    return UPDATE_INTERVAL;
}

static int infoWindowCB(XPWidgetMessage message, 
    XPWidgetID widget, long param1, long param2)
{
    if (message == xpMessage_CloseButtonPushed) {
        XPHideWidget(gWindow);
        XPDestroyWidget(gWindow, TRUE);
        return 1;
    }
    return 0;
}

static vector<string> pluginInfo()
{
    vector<string> infoLines;
    
    int lineLen = 100;
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
	
	char c_sn[lineLen];
	snprintf(c_sn, lineLen, "rsbn/strobe and rsbn/nul %1d %1d", getStrobe(NULL), getNul(NULL));
    infoLines.push_back(string(c_sn));
    
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

	char c_lat[lineLen];
	snprintf(c_lat, lineLen, "rsbn/beacon_lat (deg) %.2f", getBeaconLat(NULL));
    infoLines.push_back(string(c_lat));

	char c_lon[lineLen];
	snprintf(c_lon, lineLen, "rsbn/beacon_lon (deg) %.2f", getBeaconLon(NULL));
    infoLines.push_back(string(c_lon));
    return infoLines;
}
static void drawDebugInfo(XPWidgetID intoWindow, int x, int y)
{   
    int atLeft = x + 10;
    int atRight = x + 100;
    int atTop = y;
    int endAt;
    vector<string> infoLines = pluginInfo();
    
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
        int h = 225;
        
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
    sprintf(msg, "RSBN: loaded %i beacons\n", (int)rsbn.size());
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
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/strobe",
            xplmType_Int,                                  // The types we support
            TRUE,                                          // Writable
            getStrobe, setStrobe,                          // Integer accessors
            NULL, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, &rsbn));                                   // Refcons not used
    
    // Nul
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nul",
            xplmType_Int,                                  // The types we support
            TRUE,                                             // Writable
            getNul, setNul,                                // Integer accessors
            NULL, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, &rsbn));                                   // Refcons not used
                                        
    // Dataref for distance, wired to a callback on the database
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/distance",
            xplmType_Double,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                                 // Integer accessors
            NULL, NULL,                                    // Float accessors
            getDist, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, NULL));                                   // Refcons not used

    // Dataref for bearing, wired to a callback on the database
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/bearing",
            xplmType_Double,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            getBearing, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, NULL));                                   // Refcons not used

    // Dataref for bearing TO, wired to a callback on the database
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/bearing_to",
            xplmType_Double,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            getBearingTo, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, NULL));                                   // Refcons not used

    // Dataref for bearing TO, wired to a callback on the database
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/bearing_to_mag",
            xplmType_Double,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            getBearingToMag, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, NULL));                                   // Refcons not used
    
    // Will contain 1 if the RSBN beacon is being overflown (is within the blind mushroom of non-reception)
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/overflight",
            xplmType_Int,                                // The types we support
            FALSE,                                             // Writable
            getOverflight, NULL,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, NULL));                                   // Refcons not used
    
    // Will contain 1 if the RSBN beacon set on channel is being received
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/receiving",
            xplmType_Int,           // The types we support
            FALSE,                  // Writable
            getReceiving, NULL,     // Integer accessors
            NULL, NULL,             // Float accessors
            NULL, NULL,             // Doubles accessors
            NULL, NULL,             // Int array accessors
            NULL, NULL,             // Float array accessors
            NULL, NULL,             // Raw data accessors
            &rsbn, NULL));            // Refcons not used
    
    // Will contain the latitude of the current beacon
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/beacon_lat",
            xplmType_Double,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            getBeaconLat, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, NULL));
    
    // Will contain the latitude of the current beacon
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/beacon_lon",
            xplmType_Double,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            getBeaconLon, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &rsbn, NULL));
    
    // RSBN set navigation mode
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nav/in/mode",
            xplmType_Int,                                // The types we support
            TRUE,                                             // Writable
            getNavMode, setNavMode,                              // Integer accessors
            NULL, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &nav, &nav));
    
    // RSBN set navigation mode
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nav/in/azimuth",
            xplmType_Float,                                // The types we support
            TRUE,                                             // Writable
            NULL, NULL,                                 // Integer accessors
            getAzimuth, setAzimuth,                         // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &nav, &nav));

    // RSBN set orbita distance
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nav/in/orbita",
            xplmType_Float,                                // The types we support
            TRUE,                                             // Writable
            NULL, NULL,                                 // Integer accessors
            getOrbita, setOrbita,                         // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &nav, &nav));
 
    // RSBN set target angle
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nav/in/t_angle",
            xplmType_Float,                                // The types we support
            TRUE,                                             // Writable
            NULL, NULL,                                 // Integer accessors
            getTargetAngle, setTargetAngle,                         // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &nav, &nav));
    
    // RSBN set target distance
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nav/in/t_dist",
            xplmType_Float,                                // The types we support
            TRUE,                                             // Writable
            NULL, NULL,                                 // Integer accessors
            getTargetDist, setTargetDist,                         // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &nav, &nav));
    
    // RSBN set track
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nav/in/zpu",
           xplmType_Float,                                // The types we support
           TRUE,                                             // Writable
           NULL, NULL,                                 // Integer accessors
           getZpu, setZpu,                         // Float accessors
           NULL, NULL,                                    // Doubles accessors
           NULL, NULL,                                    // Int array accessors
           NULL, NULL,                                    // Float array accessors
           NULL, NULL,                                    // Raw data accessors
           &nav, &nav));
    
    // RSBN set crosstrack deviation, km
    rsbnDatarefs.push_back(XPLMRegisterDataAccessor("rsbn/nav/out/xtk",
            xplmType_Float,                                // The types we support
            FALSE,                                             // Writable
            NULL, NULL,                              // Integer accessors
            getCrosstrack, NULL,                                    // Float accessors
            NULL, NULL,                                    // Doubles accessors
            NULL, NULL,                                    // Int array accessors
            NULL, NULL,                                    // Float array accessors
            NULL, NULL,                                    // Raw data accessors
            &nav, NULL));
            
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
    XPLMUnregisterFlightLoopCallback(updateRsbn, NULL);
    for (vector<XPLMDataRef>::iterator cRef = rsbnDatarefs.begin(); cRef != rsbnDatarefs.end(); ++cRef) {
        XPLMUnregisterDataAccessor((*cRef));
    }
    rsbnDatarefs.clear();
    rsbn.clear();
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
