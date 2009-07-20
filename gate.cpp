#include "gate.h"
#include "libs.h"

void Gate::update()
{
   (*(db)).performLookup(XPLMGetDataf(acfXRef), XPLMGetDataf(acfYRef), XPLMGetDataf(acfZRef), XPLMGetDataf(acfLatRef), XPLMGetDataf(acfLonRef));
}

void Gate::attachDatarefs()
{
    acfXRef = XPLMFindDataRef("sim/flightmodel/position/local_x");
    acfYRef = XPLMFindDataRef("sim/flightmodel/position/local_y");
    acfZRef = XPLMFindDataRef("sim/flightmodel/position/local_z");
    acfLatRef = XPLMFindDataRef("sim/flightmodel/position/longitude");
    acfLonRef = XPLMFindDataRef("sim/flightmodel/position/longitude");
    
    localMapTop = XPLMFindDataRef("sim/graphics/view/local_map_t");
    localMapLeft = XPLMFindDataRef("sim/graphics/view/local_map_l");
    localMapBottom = XPLMFindDataRef("sim/graphics/view/local_map_b");
    localMapRight =  XPLMFindDataRef("sim/graphics/view/local_map_r");
}