#include "database.h"
#include "libs.h"

class Gate {
public:
    Database * db;
        
    // XP datarefs. Will be initialized when the database is created by the plugin
    XPLMDataRef acfLatRef;
    XPLMDataRef acfLonRef;
    
    XPLMDataRef acfAltRef;
    
    XPLMDataRef localMapLeft;
    XPLMDataRef localMapBottom;
    XPLMDataRef localMapRight;
    XPLMDataRef localMapTop;
    
    Gate();
    
    void update();
    
    void attachDatarefs();
    
    void drawToMap();
    
private:
    
    void drawCircle(float radius, float atX, float atY);

    void drawBeacon(Beacon &bc, int atMapX, int atMapY);
};