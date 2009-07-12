#include "database.h"
using namespace std;

// RSBN reception distance is usually about 450 km
#define MAX_RECEPTION_RANGE 450000.0
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
        if( (strcmp(bc.channel, channelCode) == 0) && (bc.distanceFrom(acfXRef, acfYRef, acfZRef) < MAX_RECEPTION_RANGE)) {
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
        Beacon bc = findClosestByChannel(selStrobe, selNul);
        return bc.distanceFrom(acfXRef, acfYRef, acfZRef) / 1000;
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

void Database::currentBeaconName(char *name) {
    // Get the selected channels
    try {
        Beacon bc = findClosestByChannel(selStrobe, selNul);
        strcpy(name, "RSBN ");
        strcpy(name, bc.name);
    } catch (int e) {
        // No beacon found, swallow
        strcpy(name, "<NO RECEPTION>");
    }
}

void Database::mainLoop() {
    XPLMSetDataf(distRef,    getDistance());
    XPLMSetDataf(bearingRef, getBearing());
}