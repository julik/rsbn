#include "database.h"
using namespace std;

Database::Database() {
    selStrobe = 1;
    selNul = 0;
    isTuned = FALSE;
}

Database::Database(char path[1024])
{
    selStrobe = 1;
    selNul = 0;
    tunedBc = NULL;
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

void Database::setPositionAndFindNearest(double acfX, double acfY, 
    double acfZ, double acfLat, double acfLon)
{
    
    char channelCode[3];
    sprintf(channelCode, "%1d%1d", selStrobe, selNul);
    
    curX = acfX;
    curY = acfY;
    curZ = acfZ;
    curLat = acfLat;
    curLon = acfLon;
    
    // Optimize - if the tunedBc beacon is already found and preselected keep it,
    // don't alloc any iterators
    if (isTuned && strcmp(tunedBc.channel, channelCode) == 0 && tunedBc.isInRangeOf(acfX, acfY, acfZ)) return;
    
    vector<Beacon>::iterator it =  db.begin();
    while( it != db.end() ) {
        Beacon bc = *it;
        if(strcmp(bc.channel, channelCode) == 0 && tunedBc.isInRangeOf(acfX, acfY, acfZ)) {
            isTuned = true;
            tunedBc = bc;
            return;
        }
        it++;
    }
    
    // None found if we got here
    isTuned = false;
}

int Database::size()
{
    return db.size();
}

float Database::getDistance()
{
    // Get the selected channels
    return (isTuned ? tunedBc.distanceFrom(curX, curY, curZ) : 0.0);
}

bool Database::isOverflyingNow()
{
    return (isTuned ? (getDistance() < curY) : FALSE);
}

bool Database::isReceiving()
{
    return (isTuned);
}

// TODO refactor
float Database::getBearing()
{
    // Get the selected channels
    if(isTuned) {
        return tunedBc.bearingToAcf(curLat, curLon);
    } else  {
        return 0.0;
    }
}

void Database::tunedBeaconInfo(char *name)
{
    // Get the selected channels
    if (isTuned) {
        sprintf(name, "%sk %s",  tunedBc.channel, tunedBc.name);
    } else {
        // No beacon found, swallow
        strcpy(name, "<NO RECEPTION>");
    }
}
