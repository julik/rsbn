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
    
    int strobe, nul;
    
    bool cachedCoords;
    // Implicit constructor
    Beacon();
    
    // Construct from a line
    Beacon(const char *line);
    
    // Get bearing FROM the beacon TO the aircraft, at the beacon's position in true degrees
    double bearingToAcf(double acfLat, double acfLon);
    
    // Get bearing TO the beacon FROM the aircraft, at the aircraft's position in true degrees
    double bearingFromAcf(double acfLat, double acfLon);
    
    // Get the absolute distance to aircraft, in kilometers
    double distanceFrom(double acfLat, double acfLon, double acfElev);
    
    // Is the beacon on this channel?
    bool hasCode(int strobe, int nul);
    
    // Is the beacon within the reception range?
    // RSBN max reception distance
    // depends on inverse square falloff, according to the following law
    // max_km = 3.57 * sqrt(height_in_meters
    bool isInRangeOf(double acfLat, double acfLon, double acfElev);
    
    // Returns true if we are currently overflying this beacon
    bool isOverflyingNow(double acfLat, double acfLon, double acfElev);
};
