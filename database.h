#include "beacon.h"
using namespace std;

class   Database
{
public:
    
    // Injected from the datafile
    vector<Beacon> db;
    
    // XP datarefs. Will be initialized when the database is created by the plugin
    XPLMDataRef acfXRef;
    XPLMDataRef acfYRef;
    XPLMDataRef acfZRef;
    XPLMDataRef acfLatRef;
    XPLMDataRef acfLonRef;
    
    XPLMDataRef strobeRef;
    XPLMDataRef nulRef;
    XPLMDataRef distRef;
    XPLMDataRef bearingRef;
    
    // Selected strobe and nul, arrive from the dataref via callback
    int selStrobe;
    int selNul;
    
    // Boilerplate constructor
    Database();
    
    // Make a database of beacons reading them from file at path. Beacons will be
    // placed in the db vector
    Database(char path[1024]);
    
    // Find the closest beacon at these coordinates
    Beacon findClosestByChannel(int strobe, int nul);
    
    // How many beacons are in the database?
    int size();
    
    // Do I have to explain that too?
    void mainLoop();
    
    // Returns distance to aircraft in kilometers
    float getDistance();
    
    // Returns bearing from the beacon to the aircraft
    float getBearing();
    
    // Get the name of the current beacon and write it into the passed char pointer
    void currentBeaconName(char *name);
};
