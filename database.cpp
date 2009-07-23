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
    
    curX = acfX;
    curY = acfY;
    curZ = acfZ;
    curLat = acfLat;
    curLon = acfLon;
    
    // Optimize - if the tunedBc beacon is already found and preselected keep it,
    // don't alloc any iterators and only check for distance. If it's out of range it's
    // time to search again
    if (isTuned && 
        tunedBc.hasCode(selStrobe, selNul) && 
        tunedBc.distanceFrom(curX, curY, curZ) < 210 && // Use a smaller number for ensured reception (zones overlap sometimes)
        tunedBc.isInRangeOf(acfX, acfY, acfZ)) return;
    
    vector<Beacon> shortList;
    vector<Beacon>::iterator it =  db.begin();
    while( it != db.end() ) {
        Beacon bc = *it;
        if(bc.hasCode(selStrobe, selNul) && bc.isInRangeOf(acfX, acfY, acfZ)) {
            shortList.push_back(bc);
        }
        it++;
    }
    
    // Bail out if none are found
    if(shortList.size() == 0) {
        isTuned = false; return;
    }
    
    tunedBc = shortList.front();
    
    for (vector<Beacon>::iterator curItem = shortList.begin(); curItem != shortList.end(); ++curItem) {
        if(tunedBc.distanceFrom(curX, curY, curZ) > (*curItem).distanceFrom(curX, curY, curZ)) {
            tunedBc = (*curItem);
        }
    }
    
    // Now pick the nearest onbe from the short list
    // None found if we got here
    isTuned = true;
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

void Database::flushCoordinateCache()
{
    vector<Beacon>::iterator it =  db.begin();
    while( it != db.end() ) {
        (*it).cachedCoords = FALSE;
        it++;
    }
    isTuned = FALSE;
}
