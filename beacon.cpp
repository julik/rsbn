#include "beacon.h"
#include "greatcircle.h"

using namespace std;

static const float MAX_DISTANCE_KM = 496.0; // see here http://zi-16.narod.ru/study/study-all.htm

// Boilerplate
Beacon::Beacon() {
}

// Fill the beacon data from a line. Accepts lines like:
// 02|ASTRAKHAN|GV|46.2766666666667|48.0033333333333|1.0
// channel|name|callsign|lat with decimal degrees|lon with decimal degrees|elevation, meters MSL
// The lines are generated by pre-processing scripts written in Ruby for ease of parsing
Beacon::Beacon(const char *line)
{
    char p[strlen(line)];
    
    strncpy(p, line, strlen(line));
    strncpy(channel,    strtok(p, "|"), sizeof(channel));
    strncpy(name,       strtok(NULL, "|"), sizeof(name));
    strncpy(callsign,   strtok(NULL, "|"), sizeof(callsign));
    strncpy(vorFreq,    strtok(NULL, "|"), sizeof(vorFreq));
    lat =  atof(strtok(NULL, "|"));
    lon =  atof(strtok(NULL, "|"));
    elev = atof(strtok(NULL, "\n"));
}

bool Beacon::hasCode(int strobe, int nul)
{
    char code[] = "01";
    sprintf(code, "%d%d", strobe, nul);
    return strcmp(code, channel) == 0;
}

// Get the absolute distance to aircraft, in kilometers, taking the altitude into account
double Beacon::distanceFrom(double acfLat, double acfLon, double acfElev)
{
    double linearDist = gcDistance(acfLat, acfLon, lat, lon);
    double tanDist = sqrt(pow(linearDist, 2) + pow(((acfElev - elev) / 1000), 2));
    return tanDist;
}

// Is the beacon within the reception range?
// RSBN max reception distance
// depends on inverse square falloff, according to the following law
// max_km = 3.57 * sqrt(height_of_acft_in_meters)
// (see Chyorny and Korablin)
// There is also a "cone of silence" of inop whose radius is roughly eql to H
bool Beacon::isInRangeOf(double acfLat, double acfLon, double acfElev)
{
    double dS = distanceFrom(acfLat, acfLon, acfElev);
    return dS < (3.57 * sqrt(acfElev)) && dS < MAX_DISTANCE_KM;
}

// Check if we are overflying now
bool Beacon::isOverflyingNow(double acfLat, double acfLon, double acfElev) {
    return distanceFrom(acfLat, acfLon, acfElev) < 1;
}


// Get the bearing to aircraft
double Beacon::bearingToAcf(double acfLatDeg, double acfLonDeg)
{
    return gcBearingTo(lat, lon, acfLatDeg, acfLonDeg);
}

// Get the bearing from the aircraft
double Beacon::bearingFromAcf(double acfLatDeg, double acfLonDeg)
{
    return gcBearingTo(acfLatDeg, acfLonDeg, lat, lon);
}
