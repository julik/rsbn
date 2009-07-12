#include "libs.h"

using namespace std;

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
    
    // Gets computed -  local OpenGL X-Plane coords for the beacon
    double      locX;
    double      locY;
    double      locZ;
    
    // Implicit constructor
    Beacon();
    
    // Construct from a line
    Beacon(char *line);
    
    // Get bearing FROM the beacon to the aircraft ("azimut ot"). at the beacons's position in true degrees
    double Beacon::bearingToAcf(XPLMDataRef acfLat, XPLMDataRef acfLon);
    
    // Get bearing FROM the beacon to the aircraft ("azimut ot"). at the beacons's position in true degrees
    double Beacon::bearingToAcf(XPLMDataRef acfLat, XPLMDataRef acfLon);
    
    // Get the absolute distance to aircraft, in meters
    double Beacon::distanceToBeacon(XPLMDataRef acfX, XPLMDataRef acfY, XPLMDataRef acfZ);
};
