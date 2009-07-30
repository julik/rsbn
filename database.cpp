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

void Database::setPositionAndFindNearest(double acfLat, double acfLon, double acfElev)
{
    curLat = acfLat;
    curLon = acfLon;
    curElev = acfElev;
    
    // Optimize - if the tunedBc beacon is already found and preselected keep it,
    // don't alloc any iterators and only check for distance. If it's out of range it's
    // time to search again
    if (isTuned &&
        tunedBc.hasCode(selStrobe, selNul) &&
        tunedBc.isInRangeOf(acfLat, acfLon, acfElev)) return;
    
    vector<Beacon> shortList;
    vector<Beacon>::iterator it =  db.begin();
    while( it != db.end() ) {
        Beacon bc = *it;
        if( bc.hasCode(selStrobe, selNul) && bc.isInRangeOf(acfLat, acfLon, acfElev)) {
            shortList.push_back(bc);
        }
        it++;
    }
    
    // Bail out if none are found
    if( 0 == shortList.size()) {
        isTuned = false;
        return;
    }
    
    tunedBc = shortList.front();
    
    for (vector<Beacon>::iterator curItem = shortList.begin(); curItem != shortList.end(); ++curItem) {
        if(tunedBc.distanceFrom(curLat, curLon, curElev) > (*curItem).distanceFrom(curLat, curLon, curElev)) {
            tunedBc = (*curItem);
        }
    }
    isTuned = true;
}

int Database::size()
{
    return db.size();
}

float Database::getDistance()
{
    // Get the selected channels
    return (isTuned ? tunedBc.distanceFrom(curLat, curLon, curElev) : 0.0);
}

bool Database::isOverflyingNow()
{
    return (isTuned ? tunedBc.isOverflyingNow(curLat, curLon, curElev) : FALSE);
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

void Database::flushCoordinateCache()
{
    vector<Beacon>::iterator it =  db.begin();
    while( it != db.end() ) {
        (*it).cachedCoords = FALSE;
        it++;
    }
    isTuned = FALSE;
}
