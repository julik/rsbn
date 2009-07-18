#include "database.h"
using namespace std;

// RSBN max reception distance
// depends on inverse square falloff, according to the following law
// max_km = 3.57 * sqrt(height_in_meters)
#define NO_BEACONS_IN_RANGE 1

Database::Database(char path[1024])
{
    fstream inFile;
    inFile.open(path);
    if (!inFile) {
        char info[256];
        sprintf(info, "RSBN: Unable to open %s\n", path);
        XPLMDebugString(info);
        return;
    }
    
    char line[256];
    while (inFile.getline(line, sizeof(line)))  {
        db.push_back(Beacon(line));
    }
    
    inFile.close();
}

Beacon Database::findClosestByChannel(int strobe, int nul)
{
    char channelCode[3];
    sprintf(channelCode, "%1d%1d", strobe, nul);
    
    vector<Beacon>::iterator it =  db.begin();
    Beacon bc;
    while( it != db.end() ) {
        Beacon bc = *it;
        if( (strcmp(bc.channel, channelCode) == 0) && bc.isInRangeOf(acfXRef, acfYRef, acfZRef)) {
            return bc;
        }
        it++;
    }
    throw NO_BEACONS_IN_RANGE;
}

int Database::size() {
    return db.size();
}

float Database::getDistance() {
    // Get the selected channels
    try {
        return findClosestByChannel(selStrobe, selNul).distanceFrom(acfXRef, acfYRef, acfZRef);
    } catch (int e) {
        // No beacon found, swallow
        return 0.0;
    }
}

float Database::getBearing() {
    // Get the selected channels
    try {
        Beacon bc = findClosestByChannel(selStrobe, selNul);
        return bc.bearingToAcf(acfLatRef, acfLonRef);
    } catch (int e) {
        // No beacon found, swallow
        return 0.0;
    }
}

void Database::currentBeaconInfo(char *name) {
    // Get the selected channels
    try {
        Beacon bc = findClosestByChannel(selStrobe, selNul);
        sprintf(name, "RSBN %s %s %sk",  bc.name, bc.callsign, bc.channel);
    } catch (int e) {
        // No beacon found, swallow
        strcpy(name, "<NO RECEPTION>");
    }
}