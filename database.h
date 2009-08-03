#include "libs.h"
#include "beacon.h"

using namespace std;

class Database : public vector<Beacon>
{
public:
    
    // Selected strobe and nul, arrive from the dataref via callback
    int selStrobe;
    int selNul;
    bool isTuned;
    
    Beacon tunedBc;
    
    bool isReceiving();
    
    Database();
    
    // Make a database of beacons reading them from file at path. Beacons will be
    // placed in the db vector
    Database(char path[1024]);
    
    // Refills the DB with info from the file at path
    void loadDataFrom(char path[1024]);
    
    // Find the closest beacon at these coordinates
    Beacon findClosestByChannel();
    
    // Store acf position for immediate use
    void setPosition(double acfLat, double acfLon, double acfElev);

    // Scan the beacons and select a beacon that works
    void findNearest();

    // Returns distance to aircraft in kilometers
    float getDistance();
    
    // Returns bearing from the beacon to the aircraft, degrees true
    float getBearing();
    
    // Returns bearing from aircraft to the beacon, degrees true
    float getInverseBearing();
    
    // Get the name of the current beacon and write it into the passed char pointer
    void tunedBeaconInfo(char *name);
    
    // Returns TRUE if the linear distance from the aircraft is less than the aircraft's altitude + 2 km 
    bool isOverflyingNow();

private:
    
    double curX, curY, curZ, curLat, curLon, curElev;
};
