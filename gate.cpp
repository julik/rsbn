#include "gate.h"
#include "libs.h"
static const int CYCLES_BEFORE_LOOKUP = 10;

Gate::Gate()
{
    db = NULL;
    navOverride = FALSE;
}

void Gate::update(int counter)
{
   if (db == NULL) return;
   
   (*db).setPosition(
       XPLMGetDatad(acfLatRef), XPLMGetDatad(acfLonRef), XPLMGetDatad(acfAltRef)
    );
    
   // Optimize - only do lookups every Nth frame since continuous reception is more important
   if (0 == (counter % CYCLES_BEFORE_LOOKUP)) (*db).findNearest();
   
   if (navOverride && nav) {
       float xtk = (*nav).outXtk;
       XPLMSetDataf(cdiDevRef, xtk);
   }
}

bool Gate::getNavOverride()
{
    return navOverride;
}

void Gate::setNavOverride(int v)
{
    if(v == FALSE) {
        navOverride = FALSE;
        XPLMSetDatai(navOverrideRef, FALSE);
    } else {
        navOverride = TRUE;
        XPLMSetDatai(navOverrideRef, TRUE);
    }
}

void Gate::attachDatarefs()
{
    acfLatRef = XPLMFindDataRef("sim/flightmodel/position/latitude");
    acfLonRef = XPLMFindDataRef("sim/flightmodel/position/longitude");
    acfAltRef = XPLMFindDataRef("sim/flightmodel/position/elevation");
    magVarRef = XPLMFindDataRef("sim/flightmodel/position/magnetic_variation");
    cdiDevRef = XPLMFindDataRef("sim/cockpit/radios/nav1_hdef_dot");   
    
}