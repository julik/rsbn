#include <string>
#include <string.h>
#include <stdio.h>
#include "xpsdk.h"

// RSBN channel number (k)
static XPLMDataRef rsbnK;

// RSBN callsign
static XPLMDataRef rsbnCallsign;

// RSBN name
static XPLMDataRef rsbnName;

// RSBN lat, deg with decimal
static XPLMDataRef rsbnLat;

// RSBN lon. deg with decimal
static XPLMDataRef rsbnLon;

// RSBN radial, deg
static XPLMDataRef rsbnRadal;

// RSBN linear dist in kilometers
static XPLMDataRef rsbnDist;


// start plugin
PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc)
{
    XPLMDebugString("RSBN: Init...\n");
    const char *pluginSignature = "com.julik.rsbn";
    
    // Mac-specific: it IS possible to have XAP installed twice, once in the ACF folder
    // and once in the X-System folder. Probably due to namespace mangling on OS X
    // the system will NOT crash the app when the plugin gets loaded twice. 
    // Thus, we query the enabled plugins for the defined signatures
    // and DO NOT activate should the other plugin already be plugged in
    
    XPLM_API XPLMPluginID other_plug_id = XPLMFindPluginBySignature(pluginSignature);
    if(-1 != other_plug_id) {
        XPLMDebugString("RSBN: will not init twice, bailing\n");
        return 0;
    }

    strcpy(outName, "X-Plane RSBN system");
    strcpy(outSig, pluginSignature);
    strcpy(outDesc, "Turns the power on for the RSBN beacons in ur computor\n");
    
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

