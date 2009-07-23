#include "libs.h"

class   Beacon
{
public:
    // Injected from the datafile
    char        channel[3];
    char        callsign[4];
    char        vorFreq[8];
    char        name[256];
    
    double      lat, lon, elev;
    double      locX, locY, locZ;
    
    int strobe, nul;
    
    bool cachedCoords;
    // Implicit constructor
    Beacon();
    
    // Construct from a line
    Beacon(char *line);
    
    // Get bearing FROM the beacon TO the aircraft ("azimut ot"), at the beacon's position in true degrees
    // If the onboard eqpt supports "azimut na" ("to" mode) it's the responsibility of the onboard set
    // to reciprocate the bearing
    double bearingToAcf(double acfLat, double acfLon);
    
    // Get the absolute distance to aircraft, in meters
    float distanceFrom(double acfX, double acfY, double acfZ);
    
    // Is the beacon on this channel?
    bool hasCode(int strobe, int nul);
    
    // Is the beacon within the reception range?
    // RSBN max reception distance
    // depends on inverse square falloff, according to the following law
    // max_km = 3.57 * sqrt(height_in_meters
    bool isInRangeOf(double acfX, double acfY, double acfZ);
    
    // Returns true if we are currently overflying this beacon
    bool isOverflyingNow(double acfX, double acfY, double acfZ);
};