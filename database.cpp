#include "database.h"
using namespace std;

Database::Database() {}

Database::Database(char path[1024])
{
    selStrobe = 1;
    selNul = 0;
    isReceiving = FALSE;
    loadDataFrom(path);
}

void Database::loadDataFrom(char path[1024])
{
    fstream inFile;
    inFile.open(path);
    if (!inFile) return;
    
    char line[256];
    db.clear();
    while (inFile.getline(line, sizeof(line)))  {
        db.push_back(Beacon(line));
    }
    inFile.close();
}

void Database::performLookup(double acfX, double acfY, double acfZ, double acfLat, double acfLon)
{
    curX = acfX;
    curY = acfY;
    curZ = acfZ;
    curLat = acfLat;
    curLon = acfLon;
    
    // TODO - opt - if the current beacon is in range keep it
    
    char channelCode[3];
    sprintf(channelCode, "%1d%1d", selStrobe, selNul);
    
    vector<Beacon>::iterator it =  db.begin();
    Beacon bc;
    while( it != db.end() ) {
        Beacon bc = *it;
        if( strcmp(bc.channel, channelCode) == 0) { /* && bc.isInRangeOf(acfX, acfY, acfZ)) { */
            current = bc;
            isReceiving = true;
            return;
        }
        it++;
    }
    isReceiving = false;
}

int Database::size()
{
    return db.size();
}

float Database::getDistance()
{
    // Get the selected channels
    return (isReceiving) ? current.distanceFrom(curX, curY, curZ) : 0.0;
}

bool Database::isOverflyingNow()
{
    return (isReceiving) && (getDistance() < curY);
}

// TODO refactor
float Database::getBearing()
{
    // Get the selected channels
    if(isReceiving) {
        return current.bearingToAcf(curLat, curLon);
    } else  {
        return 0.0;
    }
}

void Database::currentBeaconInfo(char *name)
{
    // Get the selected channels
    if (isReceiving) {
        sprintf(name, "RSBN %s %s %sk",  current.name, current.callsign, current.channel);
    } else {
        // No beacon found, swallow
        strcpy(name, "<NO RECEPTION>");
    }
}
