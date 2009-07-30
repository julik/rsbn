#include <math.h>

#define PI 3.14159265358979323846


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
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  dist = dist * 1.609344;
  return (dist);
}
