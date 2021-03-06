/*
Common spherical trig functions used by the plugin
*/

#include <math.h>
#include "trig.h"

static const double R_KAVRAISKOGO = 6373;
static const double NINE_MINUTES_IN_RAD = 0.00261799388;

// Convert geographical latitude on the geoid to 
// latitude on the Kavraisky spheroid
double latGeoToSpherical(double thetaGeoDeg)
{
    double theta = deg2rad(thetaGeoDeg);
    double thetaS = theta - (NINE_MINUTES_IN_RAD * (sin(2*theta)));
    return rad2deg(thetaS);
}

// Convert latitude on the Kavraisky spheroid
// to geographical latitude on the geoid
double latSphericalToGeo(double thetaSphericalDeg)
{
    double thetaS = deg2rad(thetaSphericalDeg);
    double thetaGeo = thetaS + (NINE_MINUTES_IN_RAD * (sin(2*thetaS)));
    return rad2deg(thetaGeo);
}

double gcDistance(double lat1, double lon1, double lat2, double lon2)
{
    lat1 = deg2rad(latGeoToSpherical(lat1));
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(latGeoToSpherical(lat2));
    lon2 = deg2rad(lon2);
    
    double deltaL = lon1 - lon2; // ???
    double cosDist = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(deltaL);
    return acos(cosDist) * R_KAVRAISKOGO;
}

// Simplified great circle bearing - http://www.dtcenter.org/met/users/docs/write_ups/gc_simple.pdf.
double gcBearingTo(double lat1, double lon1, double lat2, double lon2)
{
    lat1 = deg2rad(latGeoToSpherical(lat1));
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(latGeoToSpherical(lat2));
    lon2 = deg2rad(lon2);

    double deltaL = lon2 - lon1;
    double s = cos(lat2) * sin(deltaL);
    double c = (cos(lat1) * sin(lat2)) - (sin(lat1)*cos(lat2)*cos(deltaL));
    
    // This formula returns quadrant as a SIGN of the result, that is - a bearing of 260
    // will be returned as -100. We account for that.
    double degPosOrNeg = rad2deg(atan2(s, c));
    if (degPosOrNeg < 0) degPosOrNeg = 360.0 + degPosOrNeg;
    
    return degPosOrNeg;
}
