#include "beacon.h"
using namespace std;

class   Database
{
public:
    // Injected from the datafile
    vector<Beacon> db;
    
    // XP datarefs. Will be initialized when the database is created by the plugin
    XPLMDataRef acfX;
    XPLMDataRef acfY;
    XPLMDataRef acfZ;
    XPLMDataRef acfLat;
    XPLMDataRef acfLon;
    
    // Boilerplate constructor
    Database();
    
    // Make a database of beacons reading them from file at path. Beacons will be
    // placed in the db vector
    Database(char *path);
    
    // Find the closest beacon at these coordinates
    Beacon findClosetsByChannel(int strobe, int nul);
};
