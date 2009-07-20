#include "beacon.h"
extern void XPLMWorldToLocal(double lat, double lon, double elev, double &locX, double &locY, double &locZ);

using namespace std;

#define RAD_RATIO 57.2957795
#define MAX_DISTANCE_KM 496.0 // see here http://zi-16.narod.ru/study/study-all.htm

double inline radToDeg(double rad) {
    return rad * RAD_RATIO;
}

double inline degToRad(double deg) {
    return deg / RAD_RATIO;
}

// Boilerplate
Beacon::Beacon() {
}

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

void Beacon::translateCoords()
{
}

// Get the absolute distance to aircraft, in kilometers
double Beacon::distanceFrom(double acfX, double acfY, double acfZ)
{
    // Compute the beacon coordinates if none have been defined yet
    XPLMWorldToLocal(lat, lon, elev, &locX, &locY, &locZ);

	// Compute absolute distance to acf, cartesian
    double relX = locX - acfX;
    double relY = locY - acfY;
    double relZ = locZ - acfZ;

	// XY
	double distXYsq = pow(relX,2) + pow(relY,2);
    
	// and distance, in meters. abs(1) for doubles is not portable
	double dist = sqrt(distXYsq + pow(relZ, 2));
    
    return dist / 1000.0;

}

// Is the beacon within the reception range?
// RSBN max reception distance
// depends on inverse square falloff, according to the following law
// max_km = 3.57 * sqrt(height_of_acft_in_meters)
// There is also a "mushroom" of inop whose radius is roughly eql to H
bool Beacon::isInRangeOf(double acfX, double acfY, double acfZ) {
    double maxDist = MAX_DISTANCE_KM; // 3.57 * sqrt(acfY);
    double dist = distanceFrom(acfX, acfY, acfZ);
    return (dist < maxDist); // NOT yet -  && (dist > (XPLMGetDataf(acfYRef) / 1000));
}

// Check if we are overflying now
bool Beacon::isOverflyingNow(double acfX, double acfY, double acfZ) {
    double dist = distanceFrom(acfX, acfY, acfZ);
    return ( dist < (acfY / 1000));
}


// Get the bearing to aircraft
// http://www.movable-type.co.uk/scripts/latlong.html
double Beacon::bearingToAcf(double acfLatDeg, double acfLonDeg)
{
	// Compute relative bearing to the beacon (Azimut), relative to the beacon true north. Since true north is used
	// we can operate in lat/lon directly, but to do it we first need the aircraft position in lat/lon terms
	// We compute FROM the beacon as opposed to TO, and then reciprocate the heading
	double lat1 = degToRad(acfLatDeg);
	double lon1 = degToRad(acfLonDeg);
    
    double lat2 = degToRad(lat);
    double lon2 = degToRad(lon);
    
    // Radians to degrees
	double brgRad = atan2(cos(lat1)*sin(lat2)-sin(lat1) * cos(lat2) * cos(lon2-lon1), sin(lon2-lon1) * cos(lat2));
    return radToDeg(brgRad);
}