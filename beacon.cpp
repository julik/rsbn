#include "beacon.h"
#include <math.h>

using namespace std;

// Boilerplate
Beacon::Beacon() {}

// Fill the beacon data from a line. Accepts lines like:
// 02|ASTRAKHAN|GV|46.2766666666667|48.0033333333333|1.0
// channel|name|callsign|lat with decimal degrees|lon with decimal degrees|elevation, meters MSL
// The lines are generated by pre-processing scripts written in Ruby for ease of parsing
Beacon::Beacon(char *line)
{
    strncpy(channel,     strtok(line, "|"), sizeof(channel));
    strncpy(name,        strtok(NULL, "|"), sizeof(name));
    strncpy(callsign,    strtok(NULL, "|"), sizeof(callsign));
    strtok(NULL, "|"); // Skip VOR freq
    lat =  atof(strtok(NULL, "|"));
    lon =  atof(strtok(NULL, "|"));
    elev = atof(strtok(NULL, "\n"));
}

// Get the absolute distance to aircraft, in meters
double Beacon::distanceToBeacon(XPLMDataRef acfXRef, XPLMDataRef acfYRef, XPLMDataRef acfZRef)
{
    // Compute the beacon coordinates if none have been defined yet. TODO: optimize
    XPLMWorldToLocal(lat, lon, elev, &locX, &locY, &locZ);
    
	// Compute absolute distance to acf, cartesian
    double acfX = XPLMGetDataf(acfXRef);
	double acfY = XPLMGetDataf(acfYRef);
	double acfZ = XPLMGetDataf(acfZRef);

	double relX = locX - acfX;
	double relY = locY - acfY;
	double relZ = locZ - acfZ;

	// XY
	double distXYsq = pow(relX,2) + pow(relY,2);

	// and distance, in meters. abs(1) for floats is not portable
	double dist = fabs(sqrt( distXYsq + pow(relZ, 2) ));
    return dist;
}

// Get the bearing to aircraft
// http://www.movable-type.co.uk/scripts/latlong.html
double Beacon::bearingToAcf(XPLMDataRef acfLatRef, XPLMDataRef acfLonRef)
{
	// Compute relative bearing to the beacon (Azimut), relative to the beacon true north. Since true north is used
	// we can operate in lat/lon directly, but to do it we first need the aircraft position in lat/lon terms
	// We compute FROM the beacon as opposed to TO, and then reciprocate the heading
	double lat1 = lat;
	double lon1 = lon;
    
    double lat2 = XPLMGetDataf(acfLatRef);
    double lon2 = XPLMGetDataf(acfLonRef);
    
	double bearing = atan2(cos(lat1)*sin(lat2)-sin(lat1) * cos(lat2) * cos(lon2-lon1), sin(lon2-lon1) * cos(lat2));
    return bearing;
}