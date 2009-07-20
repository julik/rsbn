#define PLUGIN_SIGNATURE "com.julik.rsbn"
#define USSR_DATA (void*)1
#define CIS_DATA (void*)2

#include "libs.h"
#include "utils.h"
#include "gate.h"

static Database rsbn;
static Gate proxy;

// static Cartographer map = NULL;
static XPLMMenuID rsbnMenu;
static int rsbnMenuItem = -1;
static XPLMWindowID gWindow = NULL;

inline int clamp(int min, int value, int max) {
    return value > max ? max : (value < min ? min : value);
}

// Data access callbacks. All X-Plane callbacks for data sets/gets need a void refcon pointer at the start
static int getStrobe(void* inRefcon) {
    return rsbn.selStrobe;
}

static void setStrobe(void* inRefcon, int newStrobe) {
    rsbn.selStrobe = clamp(0, newStrobe, 3);
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

static int getOnline(void* inRefcon) {
    return rsbn.isReceiving;
}

static float updateRsbn(float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop,  int counter, void *refcon)
{
    proxy.update();
    return -1;
}

static void inspectorWindowCB( XPLMWindowID    inWindowID, void * inRefcon)
{

	int		left, top, right, bottom;
	float	color[] = { 1.0, 1.0, 1.0 };
	XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

	XPLMDrawTranslucentDarkBox(left, top, right, bottom);

	char c_brg[64];
	snprintf(c_brg, 64,  "rsbn/bearing (deg) %f", getBearing(NULL));

	char c_dist[64];
	snprintf(c_dist, 64, "rsbn/distance (km) %f", getDist(NULL));
    
    char c_inf[100];
    rsbn.currentBeaconInfo(c_inf);

	XPLMDrawString(color, left + 5, top - 40,
		(char*)(c_brg),  NULL, xplmFont_Basic);

	XPLMDrawString(color, left + 5, top - 60,
		(char*)(c_dist), NULL, xplmFont_Basic);

	XPLMDrawString(color, left + 5, top - 80,
		(char*)(c_inf), NULL, xplmFont_Basic);
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
}

// start plugin
PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc)
{
    XPLMDebugString("RSBN: Init...\n");
    strcpy(outName, "X-Plane RSBN system");
    strcpy(outSig, PLUGIN_SIGNATURE);
    strcpy(outDesc, "RSBN beacons");
    
    rsbn_initMenu();
    rsbn_selectDataset(NULL, USSR_DATA);
    
    // Strobe
    XPLMRegisterDataAccessor("rsbn/strobe",
                                             xplmType_Int,                                  // The types we support
                                             TRUE,                                             // Writable
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
    XPLMRegisterDataAccessor("rsbn/dist",
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
                                             xplmType_Int,                                // The types we support
                                             FALSE,                                             // Writable
                                             getOnline, NULL,                              // Integer accessors
                                             NULL, NULL,                                    // Float accessors
                                             NULL, NULL,                                    // Doubles accessors
                                             NULL, NULL,                                    // Int array accessors
                                             NULL, NULL,                                    // Float array accessors
                                             NULL, NULL,                                    // Raw data accessors
                                             NULL, NULL);                                   // Refcons not used
    
    // Assign the gateway objects
    proxy.db = &rsbn;   
    proxy.attachDatarefs();
    
    // Register the main callback
    XPLMRegisterFlightLoopCallback(updateRsbn, -1, NULL);
    
    // And init the inspector
    gWindow = XPLMCreateWindow(
                       500,                 // inLeft,    
                       800,                 // inTop,    
                       300,                 // inRight,    
                       600,                 // inBottom,    
                       1,                   // inIsVisible,    
                       inspectorWindowCB,   // inDrawCallback,    
                       NULL,                // inKeyCallback,    
                       NULL,                // inMouseCallback,    
                       NULL);               // inRefcon);
    
    // XPLMRegisterDrawCallback( drawMapCB, xplm_Phase_LocalMap2D, 0, 0);
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
}


// Enable plugin
PLUGIN_API int XPluginEnable(void)
{
    return 1;
}


PLUGIN_API void XPluginReceiveMessage(XPLMPluginID fromWhom, long msg, void *param)
{
}