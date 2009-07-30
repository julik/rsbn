/*
Common spherical trig functions used by the plugin
*/

#include <math.h>
#define PI 3.14159265358979323846
#define KM_IN_MINUTE 1.8522

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
