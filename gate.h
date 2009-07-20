#include "database.h"
#include "libs.h"

class Gate {
public:
    Database * db;
        
    // XP datarefs. Will be initialized when the database is created by the plugin
    XPLMDataRef acfXRef;
    XPLMDataRef acfYRef;
    XPLMDataRef acfZRef;
    XPLMDataRef acfLatRef;
    XPLMDataRef acfLonRef;
     
    XPLMDataRef localMapLeft;
    XPLMDataRef localMapBottom;
    XPLMDataRef localMapRight;
    XPLMDataRef localMapTop;
    
    void update();
    
    void attachDatarefs();
};