#include "database.h"
#include "beacon.h"
// RSBN reception distance is usually about 450 km
#define MAX_TRANSMISSION_RANGE 450000

using namespace std;

Database::Database(char *path)
{
    fstream inFile;
    inFile.open("data/ussr.dat");
    if (!inFile) {
        cout << "Unable to open file";
        return;
    }
    
    char line[256];
    while (inFile.getline(line, sizeof(line)))  {
        db.push_back(Beacon(line));
    }
    inFile.close();
}

Beacon findClosetsByChannel(int strobe, int nul);
{
    char channelCode[3];
    sprintf(channelCode, "%1d%1d", strobe, nul);
    
    it =  db.begin();
    Beacon bc;
    while( the_iterator != the_vector.end() ) {
        Beacon bc = *the_iterator;
        if((strcmp(bc.channel, channelCode) == 0) && (bc.distanceToBeacon(acfX, acfY, acfZ) < MAX_TRANSMISSION_RANGE) {
            return bc;
        }
        ++the_iterator;
    }
}