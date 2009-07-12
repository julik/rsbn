#include "libs.h"
#include "database.h"

Database rsbn;

// start plugin
PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc)
{
    XPLMDebugString("RSBN: Init...\n");
    const char *pluginSignature = "com.julik.rsbn";
    strcpy(outName, "X-Plane RSBN system");
    strcpy(outSig, pluginSignature);
    strcpy(outDesc, "Turns the power on for the RSBN beacons in ur computor\n");
    
    // Init the instance
    rsbn = Database("data/ussr.dat");
    
    rsbn.acfX = XPLMFindDataRef("sim/flightmodel/position/local_x");
    rsbn.acfY = XPLMFindDataRef("sim/flightmodel/position/local_y");
    rsbn.acfZ = XPLMFindDataRef("sim/flightmodel/position/local_z");
    rsbn.acfLat = XPLMFindDataRef("sim/flightmodel/position/longitude");
    rsbn.acfLon = XPLMFindDataRef("sim/flightmodel/position/longitude");
    
    // Register a callback
    return 1;
}

// Clean up
PLUGIN_API void	XPluginStop(void)
{
    /// Clean up
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

