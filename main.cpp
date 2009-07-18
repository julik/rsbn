#include "libs.h"
#include "database.h"
#define PLUGIN_SIGNATURE "com.julik.rsbn"

static Database rsbn = NULL;
static XPLMWindowID gWindow = NULL;

inline int clamp(int min, int value, int max) {
    return value > max ? max : (value < min ? min : value);
}

// Data access callbacks. All X-Plane callbacks for data sets/gets need a void refcon pointer at the start
int getStrobe(void* inRefcon) {
    return rsbn.selStrobe;
}

void setStrobe(void* inRefcon, int newStrobe) {
    rsbn.selStrobe = clamp(0, newStrobe, 3);
}

int getNul(void* inRefcon) {
    return rsbn.selNul;
}

void setNul(void* inRefcon, int newNul) {
    rsbn.selNul = clamp(0, newNul, 9);
}

float getDist(void* inRefcon) {
    return rsbn.getDistance();
}

float getBearing(void* inRefcon) {
    return rsbn.getBearing();
}

int getOverflight(void* inRefcon) {
    // TODO
    return 0;
}

int getOnline(void* inRefcon) {
    // TODO
    return 1;
}

void inspectorWindowCB( XPLMWindowID    inWindowID, void * inRefcon)
{

	int		left, top, right, bottom;
	float	color[] = { 1.0, 1.0, 1.0 };
	XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

	XPLMDrawTranslucentDarkBox(left, top, right, bottom);

	char c_brg[64];
	snprintf(c_brg, 64, "Bearing %f", XPLMGetDataf(rsbn.bearingRef));

	char c_dist[64];
	snprintf(c_dist, 64, "Dist %fkm", XPLMGetDataf(rsbn.distRef));
    
    char c_inf[100];
    rsbn.currentBeaconInfo(c_inf);

	XPLMDrawString(color, left + 5, top - 40,
		(char*)(c_brg),  NULL, xplmFont_Basic);

	XPLMDrawString(color, left + 5, top - 60,
		(char*)(c_dist), NULL, xplmFont_Basic);

	XPLMDrawString(color, left + 5, top - 80,
		(char*)(c_inf), NULL, xplmFont_Basic);
}

/// Convert a Mac-style path with double colons to a POSIX path. Google for "FSRef to POSIX path"
// if you want to know the ass-backwards way of doing it properly, but we won't link to Carbon
// only for this mmkay?
static inline std::string carbonPathToPosixPath(const std::string &carbonPath)
{
    // Check if we are on a Mac first, could be also #ifdef APL
    std::string sep = XPLMGetDirectorySeparator();
    if(sep != std::string(":")) return carbonPath;
    
    // Prepend the "Volumes" superdir to the path
    std::string posixPath = std::string("/Volumes/") + carbonPath;
    // Replace dots with slashes
    for(unsigned int i = 0; i < posixPath.length(); i++) {
        if(posixPath[i] == ':') posixPath[i] = '/';
    }
    return posixPath;
}

// Overrides XPLMGetDirectorySeparator() to return the POSIX / instead of : for paths on OS X
static inline std::string getDirSeparator()
{
    std::string sep = XPLMGetDirectorySeparator();
    if(sep == std::string(":")) {
        return std::string("/");
    } else {
        return sep;
    }
}

void detectDatabasePath(char *dataPath, char *datafileName)
{
    char myPath[1024];
    XPLMGetPluginInfo(XPLMGetMyID(), NULL, myPath, NULL, NULL);
    XPLMExtractFileAndPath(myPath);
    
    string normalizedPath = carbonPathToPosixPath(string(myPath));
    string datafilePathS = normalizedPath + getDirSeparator() + "data" + getDirSeparator() + datafileName;
    
    XPLMDebugString(("RSBN: Loading beacons from " + datafilePathS + "\n").c_str());
    strcpy(dataPath, datafilePathS.c_str());
}

// start plugin
PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc)
{
    XPLMDebugString("RSBN: Init...\n");
    strcpy(outName, "X-Plane RSBN system");
    strcpy(outSig, PLUGIN_SIGNATURE);
    strcpy(outDesc, "RSBN beacons");
    
    char dataPath[1024];
    detectDatabasePath(dataPath, "ussr.dat");
    rsbn = Database(dataPath);
    
    char msg [256];
    sprintf(msg, "RSBN: loaded %d beacons from %s\n", rsbn.size(), "ussr.dat");
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
                                        
    // Dataref for distance, wired to a callback on the database
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

    // Dataref for bearing, wired to a callback on the database
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
    
    // Will contain 1 if the RSBN beacon is being overflown (is within the blind mushroom of non-reception)
    rsbn.overflightRef = XPLMRegisterDataAccessor("rsbn/overflight",
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
    rsbn.receptionRef = XPLMRegisterDataAccessor("rsbn/online",
                                             xplmType_Int,                                // The types we support
                                             FALSE,                                             // Writable
                                             getOnline, NULL,                              // Integer accessors
                                             NULL, NULL,                                    // Float accessors
                                             NULL, NULL,                                    // Doubles accessors
                                             NULL, NULL,                                    // Int array accessors
                                             NULL, NULL,                                    // Float array accessors
                                             NULL, NULL,                                    // Raw data accessors
                                             NULL, NULL);                                   // Refcons not used
    
    
    // Preselect 10k
    rsbn.selStrobe = 1; rsbn.selNul = 0;
    
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