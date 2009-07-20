#include "libs.h"
#include "beacon.h"

using namespace std;

class   Database
{
public:
    
    // Injected from the datafile
    vector<Beacon> db;
    
    // Selected strobe and nul, arrive from the dataref via callback
    int selStrobe;
    int selNul;
    
    Beacon * tunedBc;
    
    // Boilerplate constructor
    Database();

    bool isReceiving();
    
    // Make a database of beacons reading them from file at path. Beacons will be
    // placed in the db vector
    Database(char path[1024]);
    
    // Refills the DB with info from the file at path
    void loadDataFrom(char path[1024]);
    
    // Find the closest beacon at these coordinates
    Beacon findClosestByChannel();
    
    // Scan the beacons and select a beacon that works
    void setPositionAndFindNearest(float acfX, float acfY, float acfZ, float acfLat, float acfLon);
    
    // How many beacons are in the database?
    int size();
    
    // Returns distance to aircraft in kilometers
    float getDistance();
    
    // Returns bearing from the beacon to the aircraft
    float getBearing();
    
    // Get the name of the current beacon and write it into the passed char pointer
    void tunedBeaconInfo(char *name);
    
    // Returns TRUE if the linear distance from the aircraft is less than the aircraft's altitude + 2 km 
    bool Database::isOverflyingNow();

private:
    
    double curX;
    double curY;
    double curZ;
    double curLat;
    double curLon;
};
