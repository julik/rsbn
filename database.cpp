#include "database.h"
using namespace std;

Database::Database() {
    selStrobe = 1;
    selNul = 0;
}

Database::Database(char path[1024])
{
    selStrobe = 1;
    selNul = 0;
    loadDataFrom(path);
}

void Database::loadDataFrom(char path[1024])
{
    fstream inFile;
    inFile.open(path);
    if (!inFile) return;
    
    char line[256];
    clear();
    while (inFile.getline(line, sizeof(line)))  {
        push_back(Beacon(line));
    }
    inFile.close();
    isTuned = FALSE;
}

void Database::setPosition(double acfLat, double acfLon, double acfElev)
{
    curLat = acfLat;
    curLon = acfLon;
    curElev = acfElev;
}

void Database::findNearest()
{
    // Optimize - if the tunedBc beacon is already found and preselected keep it,
    // don't alloc any iterators and only check for distance. If it's out of range it's
    // time to search again
    if (tunedBeaconWithinRange()) return;
    
    vector<Beacon> shortList;
    vector<Beacon>::iterator it =  begin();
    while( it != end() ) {
        Beacon bc = *it;
        if( bc.hasCode(selStrobe, selNul) && bc.isInRangeOf(curLat, curLon, curElev)) {
            shortList.push_back(bc);
        }
        it++;
    }
    
    // Bail out if none are found
    if( 0 == shortList.size()) {
        isTuned = false;
        return;
    }
    
    // I know this is a bubble sort but with 3-4 choices and the optimization above this is tolerable.
    // Finds the beacon with the shortest distance to the aircraft
    tunedBc = shortList.front();
    for (vector<Beacon>::iterator curItem = shortList.begin(); curItem != shortList.end(); ++curItem) {
        if(tunedBc.distanceFrom(curLat, curLon, curElev) > (*curItem).distanceFrom(curLat, curLon, curElev)) {
            tunedBc = (*curItem);
        }
    }
    isTuned = true;
}

bool Database::tunedBeaconWithinRange()
{
    return (isTuned && tunedBc.hasCode(selStrobe, selNul) &&
        tunedBc.isInRangeOf(curLat, curLon, curElev));
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

float Database::getBearing()
{
    // Get the selected channels
    if(isTuned) {
        return tunedBc.bearingToAcf(curLat, curLon);
    } else  {
        return 0.0;
    }
}

float Database::getInverseBearing()
{
    // Get the selected channels
    if(isTuned) {
        return tunedBc.bearingFromAcf(curLat, curLon);
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
