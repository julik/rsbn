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
    double bearingToAcf(double acfLat, double acfLon);
    
    // Get the absolute distance to aircraft, in meters
    double distanceFrom(double acfX, double acfY, double acfZ);
    
    // Is the beacon within the reception range?
    // RSBN max reception distance
    // depends on inverse square falloff, according to the following law
    // max_km = 3.57 * sqrt(height_in_meters
    bool isInRangeOf(double acfX, double acfY, double acfZ);
    
    // Returns true if we are currently overflying this beacon
    bool isOverflyingNow(double acfX, double acfY, double acfZ);
    
    // Translate the beacon coords into X-Plane coordinate system and cache XYZ coords
    void translateCoords();
};