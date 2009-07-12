#include "libs.h"
#include "database.h"

static Database rsbn = NULL;
static XPLMWindowID gWindow = NULL;

float rsbn_flCallback(float elapsedSinceLastCall, float timeElapsed,  int counter, void *refcon) {
    rsbn.mainLoop();
    return -1;
}


// Data access callbacks. All X-Plane callbacks for data sets/gets need a void refcon pointer at the start
int getStrobe(void* inRefcon) {
    return rsbn.selStrobe;
}

void setStrobe(void* inRefcon, int newStrobe) {
    rsbn.selStrobe = newStrobe;
}

int getNul(void* inRefcon) {
    return rsbn.selNul;
}

void setNul(void* inRefcon, int nul) {
    rsbn.selNul = nul;
}

float getDist(void* inRefcon) {
    return rsbn.getDistance();
}

float getBearing(void* inRefcon) {
    return rsbn.getBearing();
}

void inspectorWindowCB( XPLMWindowID    inWindowID, void * inRefcon)
{

	int		left, top, right, bottom;
	float	color[] = { 1.0, 1.0, 1.0 };
	XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

	XPLMDrawTranslucentDarkBox(left, top, right, bottom);

	char c_chan[64];
	snprintf(c_chan, 64, "Channel %d%dk", rsbn.selStrobe, rsbn.selNul);

	char c_brg[64];
	snprintf(c_brg, 64, "Bearing %f", XPLMGetDataf(rsbn.bearingRef));

	char c_dist[64];
	snprintf(c_dist, 64, "Dist %fkm", XPLMGetDataf(rsbn.distRef));
    
    char c_name[100];
    rsbn.currentBeaconName(c_name);
    
	XPLMDrawString(color, left + 5, top - 20,
		(char*)(c_chan),  NULL, xplmFont_Basic);

	XPLMDrawString(color, left + 5, top - 40,
		(char*)(c_brg),  NULL, xplmFont_Basic);

	XPLMDrawString(color, left + 5, top - 60,
		(char*)(c_dist), NULL, xplmFont_Basic);

	XPLMDrawString(color, left + 5, top - 80,
		(char*)(c_name), NULL, xplmFont_Basic);
}

// start plugin
PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc)
{
    XPLMDebugString("RSBN: Init...\n");
    const char *pluginSignature = "com.julik.rsbn";
    strcpy(outName, "X-Plane RSBN system");
    strcpy(outSig, pluginSignature);
    strcpy(outDesc, "RSBN beacons");
    
    // TODO - just generate a gawddamn .cpp and use it inline
    char dataPath[] = "/Users/julik/Desktop/X-Plane-Beta/Resources/plugins/rsbn/data/ussr.dat";
    rsbn = Database(dataPath);
    
    char msg [256];
    sprintf(msg, "RSBN: loaded %d beacons\n", rsbn.size());
    XPLMDebugString(msg);
    
    // Grab the datarefs the plug needs for computation
    rsbn.acfXRef = XPLMFindDataRef("sim/flightmodel/position/local_x");
    rsbn.acfYRef = XPLMFindDataRef("sim/flightmodel/position/local_y");
    rsbn.acfZRef = XPLMFindDataRef("sim/flightmodel/position/local_z");
    rsbn.acfLatRef = XPLMFindDataRef("sim/flightmodel/position/longitude");
    rsbn.acfLonRef = XPLMFindDataRef("sim/flightmodel/position/longitude");

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
                                        
    // Init the datarefs for channels
    rsbn.distRef = XPLMRegisterDataAccessor("rsbn/dist",
                                             xplmType_Float,                                // The types we support
                                             FALSE,                                             // Writable
                                             NULL, NULL,                                 // Integer accessors
                                             getDist, NULL,                                    // Float accessors
                                             NULL, NULL,                                    // Doubles accessors
                                             NULL, NULL,                                    // Int array accessors
                                             NULL, NULL,                                    // Float array accessors
                                             NULL, NULL,                                    // Raw data accessors
                                             NULL, NULL);                                   // Refcons not used

    // Init the datarefs for channels
    rsbn.bearingRef = XPLMRegisterDataAccessor("rsbn/bearing",
                                             xplmType_Float,                                // The types we support
                                             FALSE,                                             // Writable
                                             NULL, NULL,                              // Integer accessors
                                             getBearing, NULL,                                    // Float accessors
                                             NULL, NULL,                                    // Doubles accessors
                                             NULL, NULL,                                    // Int array accessors
                                             NULL, NULL,                                    // Float array accessors
                                             NULL, NULL,                                    // Raw data accessors
                                             NULL, NULL);                                   // Refcons not used

    // Preselect 10k
    rsbn.selStrobe = 1; rsbn.selNul = 0;
    
    // Register a callback
    XPLMRegisterFlightLoopCallback(rsbn_flCallback, -1, NULL);
    
    
    // And init the inspector
    gWindow = XPLMCreateWindow(
						50, 800, 300, 600,
						1,
						inspectorWindowCB,
						NULL,
						NULL,
						NULL);
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