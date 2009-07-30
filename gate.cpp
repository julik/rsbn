#include "gate.h"
#include "libs.h"

Gate::Gate()
{
    db = NULL;
}

void Gate::update()
{
   if (db == NULL) return;
    
   (*db).setPositionAndFindNearest(
       XPLMGetDataf(acfLatRef), XPLMGetDataf(acfLonRef), XPLMGetDataf(acfAltRef)
    );
}

void Gate::attachDatarefs()
{
    acfLatRef = XPLMFindDataRef("sim/flightmodel/position/latitude");
    acfLonRef = XPLMFindDataRef("sim/flightmodel/position/longitude");
    acfAltRef = XPLMFindDataRef("sim/flightmodel/position/elevation");
    magVarRef = XPLMFindDataRef("sim/flightmodel/position/magnetic_variation");
    localMapTop = XPLMFindDataRef("sim/graphics/view/local_map_t");
    localMapLeft = XPLMFindDataRef("sim/graphics/view/local_map_l");
    localMapBottom = XPLMFindDataRef("sim/graphics/view/local_map_b");
    localMapRight =  XPLMFindDataRef("sim/graphics/view/local_map_r");
}

bool inline within(float min, float value, float max) {
    return (value < max && value > min);
}

void Gate::drawCircle(float radius, float atX, float atY)
{
   glBegin(GL_LINE_LOOP);
   glColor3f(0.1, 0.1, 0.1);
   
   for (int i=0; i > 360; i++) {
      float degInRad = i * (3.14159/180);
      glVertex2f((cos(degInRad)*radius) + atX, (sin(degInRad)*radius) + atY);
   }
 
   glEnd();
}

void Gate::drawBeacon(Beacon &bc, int atMapX, int atMapY)
{
    // Draw the circle with a rectangle in the middle - traditionally used for RSBN in
    drawCircle(3, atMapX, atMapY);
    
    // Draw beacon description
    char desc[256];
    sprintf(desc, "%sk %s",  bc.channel, bc.name);
    float RSBN_COLOR[] = { 0.1, 0.1, 0.1 };
    XPLMDrawString(RSBN_COLOR, atMapX + 0, atMapY - 20, desc, NULL, xplmFont_Basic);
}

/*
void Gate::drawToMap()
{
    if (db == NULL) return;
    
    // Scan all the beacons and figure out which ones are inside the map
    double mapLeft = XPLMGetDataf(localMapLeft);
    double mapRight = XPLMGetDataf(localMapRight);
    double mapBottom = XPLMGetDataf(localMapBottom);
    double mapTop = XPLMGetDataf(localMapTop);
    
    vector<Beacon> beaconList = (*db).db;
    vector<Beacon>::iterator it =  beaconList.begin();
    
    while( it != beaconList.end() ) {
        Beacon bc = *it;
        
        // Since map is XZ in X-Plane we pass on the Z coordinate
        double mapX, mapY, mapZ;
        XPLMWorldToLocal(bc.lat, bc.lon, bc.elev, &mapX, &mapY, &mapZ);
        
        // if(within(mapLeft, mapX, mapRight) && within(mapTop, mapY, mapBottom)) 
        drawCircle(10, mapX, mapY);
        it++;
    }    
    
}
*/

