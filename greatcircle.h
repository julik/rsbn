/*
Common spherical trig functions used by the plugin
*/

#include <math.h>
#define PI 3.14159265358979323846
#define KM_IN_MINUTE 1.8522 // AKA one Nautical Mile

inline double deg2rad(double deg) {
  return (deg * PI / 180);
}

inline double rad2deg(double rad) {
  return (rad * 180 / PI);
}

double gcDistance(double lat1, double lon1, double lat2, double lon2) {
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  return rad2deg(dist) * 60 * KM_IN_MINUTE;
}

// SImplified great circle bearing - http://www.dtcenter.org/met/users/docs/write_ups/gc_simple.pdf
double gcBearingTo(double lat1, double lon1, double lat2, double lon2)
{
    lat1 = deg2rad(lat1);
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(lat2);
    lon2 = deg2rad(lon2);

    double deltaL = lon2 - lon1;
    // S is cos(lat2) * sin(deltaLon)
    double s = cos(lat2) * sin(deltaL);
    double c = (cos(lat1) * sin(lat2)) - (sin(lat1)*cos(lat2)*cos(deltaL));
    
    // This formula returns quadrant as a SIGN of the result, that is - a bearing of 260
    // will be returned as -100. We account for that.
    double degPosOrNeg = rad2deg(atan2(s, c));
    if (degPosOrNeg < 0) degPosOrNeg = 360.0 + degPosOrNeg;
    
    return degPosOrNeg;
}