#include "database.h"
#include "navigator.h"
#include "libs.h"

class Gate {
public:
    Database * db;
    Navigator * nav;
    
    // XP datarefs. Will be initialized when the database is created by the plugin
    XPLMDataRef acfLatRef;
    XPLMDataRef acfLonRef;
    XPLMDataRef acfAltRef;
    
    XPLMDataRef magVarRef;
    
    XPLMDataRef cdiDevRef;
    XPLMDataRef navOverrideRef;
    
    Gate();
    
    void update(int counter);
    void attachDatarefs();
    
    void setNavOverride(int);
    bool getNavOverride();

private:
    
    int navOverride;
    
    void drawCircle(float radius, float atX, float atY);

    void drawBeacon(Beacon &bc, int atMapX, int atMapY);
};
