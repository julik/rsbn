#include "libs.h"

class   Beacon
{
public:
    // Injected from the datafile
    char        channel[3];
    char        callsign[4];
    char        name[256];
    double      lat;
    double      lon;
    double      elev;
    
    // If the data is precompiled
    bool        cachedCoords;
    
    // Gets computed -  local OpenGL X-Plane coords for the beacon
    double      locX;
    double      locY;
    double      locZ;
    
    // Implicit constructor
    Beacon();
    
    // Construct from a line
    Beacon(char *line);
    
    // Get bearing FROM the beacon TO the aircraft ("azimut ot"), at the beacon's position in true degrees
    // If the onboard eqpt supports "azimut na" ("to" mode) it's the responsibility of the onboard set
    // to reciprocate the bearing
    double Beacon::bearingToAcf(XPLMDataRef acfLat, XPLMDataRef acfLon);
    
    // Get the absolute distance to aircraft, in meters
    double Beacon::distanceFrom(XPLMDataRef acfX, XPLMDataRef acfY, XPLMDataRef acfZ);
    
    // Is the beacon within the reception range?
    // RSBN max reception distance
    // depends on inverse square falloff, according to the following law
    // max_km = 3.57 * sqrt(height_in_meters
    bool Beacon::isInRangeOf(XPLMDataRef acfX, XPLMDataRef acfY, XPLMDataRef acfZ);
    
    // Returns true if we are currently overflying this beacon
    bool Beacon::isOverflyingNow(XPLMDataRef acfX, XPLMDataRef acfY, XPLMDataRef acfZ);
};