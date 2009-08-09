#include "navigator.h"
#define MODE_NONE 0
#define MODE_AZIMUT_NA 1
#define MODE_AZIMUT_OT 2
#define MODE_ORBITA_CCW 3
#define MODE_ORBITA_CW 4
#define MODE_SRP 5

#define PI 3.14159265358979323846
inline double deg2rad(double deg) {
    return (deg * PI / 180);
}

inline double rad2deg(double rad) {
    return (rad * 180 / PI);
}

/**
rsbn/nav/in/mode // 0 is none, 1 - azimut na, 2 - azimut ot, 3 - orbita lev, 4 - orbita prav, 5 - SRP
rsbn/nav/capture // set to 1 to override nav1
rsbn/nav/in/azimuth
rsbn/nav/in/orbita
rsbn/nav/in/target/bearing
rsbn/nav/in/target/distance
rsbn/nav/in/target/heading

rsbn/nav/out/xtk // cross-track deviation in KM. A deviation of 5.4 kilometers will FULLY deflect the HSI, left deviation is negative
rsbn/nav/out/approaching // set to 1 when we are approaching target, < 10 deg on track and < 10 km on distance
rsbn/nav/out/overflying // set to 1 when we are oveflying the target (< 1.1 deg on track and < 1.1 km on distance)
rsbn/nav/out/nav_captured // set to 1 when the beacon is being received and nav is captured
**/

Navigator::Navigator()
{
    outXtk = 0;
    selMode = MODE_NONE;
    selAzimuth  = 0;
    selOrbita = 0;
    selTargetAngle = 0;
    selTargetDist = 0;
    selTrack = 0;
}

// Compute cross-track on a certain radial when flying FROM the point.
double xtkOnRadial(double onRadialDeg, bool isFrom, double acfBrg, double acfDist)
{
	// Compute the crosstrack. Crosstrack will be, effectively, 
	// the sine of the angle between the bearing that we are flying and the bearing we want to have
	// We subtract it that way so that LEFT deviation (CCW) is negative and RIGHT (CW) dev
	// is positive
	double angularDevRad = deg2rad(acfBrg) - deg2rad(onRadialDeg);
	
	double xtk = sin(angularDevRad) * acfDist;
	return xtk;
}

double xtkOnArc(double onDistance, bool isClockwise, double acfBrg, double acfDist)
{
	
	double distDiff = onDistance - acfDist; 
	// If we are in CW mode - crosstrack is positive (steer right when difference is positive)
	return isClockwise ? distDiff : (distDiff * -1);
}

void Navigator::computeSrp(double acfBrg, double acfDist)
{
    // First we need to know the effective BEARING from the point
    // of destination to the airplane. To do so, we compute the angle
    // between the bearing line of the airplane and the bearing to the point first.
    double brgDiff = deg2rad(acfBrg - selTargetAngle);
    // Since we have two sides here and an angle, we can solve the triangle 
    // using the law of cosines (duh!)
    double distSq = pow(acfDist, 2) + pow(selTargetDist, 2) - (2*acfDist*selTargetDist*cos(brgDiff));
    double distToPoint = sqrt(distSq);
    
    // Now that we know the distance we can deduce the angle between the line
    // from the aircraft to the point and the line from the point to the beacon
    double alphaDiff = acos( (distToPoint + selTargetDist - acfDist) / (-1 * (distToPoint * selTargetDist)));
    
    // Determine the angle from the track to the target line
    double angularDeviation = deg2rad(selTrack - selTargetAngle) - alphaDiff;
    
    // Using the distance to target, compute the cross-track
    outXtk = sin(angularDeviation) * distToPoint;

    // Using the same thing, compute distance to target
    // distToTarget = cos(angularDeviation) * distToPoint;
    
}

void Navigator::changeLampState(double acfBrg, double acfDist)
{
    bool inAz, inDist, appr;
    inAz = FALSE; inDist = FALSE; appr = FALSE;
    
    if(fabs(acfBrg - selAzimuth) < 1.1) inAz = TRUE;
    if(fabs(acfDist - selOrbita) < 1.1) inDist = TRUE;
    if(fabs(acfDist - selOrbita) < 10) appr = TRUE;
    
    if (inAz && inDist) {
        outOverflying = true;
        outApproaching = true;
    } else if (appr && inAz) {
        outApproaching = true;
    } else {
        outOverflying = false;
        outApproaching = false;
    }
}
void Navigator::update(double acfBrg, double acfDist)
{
    
	if (MODE_NONE == selMode) return;
	
	if (MODE_AZIMUT_NA == selMode) {
	    
		outXtk = xtkOnRadial(selAzimuth, FALSE, acfBrg, acfDist);
	} else if (MODE_AZIMUT_OT == selMode) {
		
		outXtk = xtkOnRadial(selAzimuth, TRUE, acfBrg, acfDist);
	} else if (MODE_ORBITA_CCW == selMode) {
		
		outXtk = xtkOnArc(selOrbita, FALSE, acfBrg, acfDist);
	
	} else if (MODE_ORBITA_CW == selMode) {
		
		outXtk = xtkOnArc(selOrbita, TRUE, acfBrg, acfDist);
	
	} else if (MODE_SRP == selMode) {
		
		computeSrp(acfBrg, acfDist);
	    
	}
    changeLampState(acfBrg, acfDist);
}