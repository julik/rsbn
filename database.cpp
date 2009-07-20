#include "database.h"
using namespace std;

Database::Database() {
    selStrobe = 1;
    selNul = 0;
    current = NULL;
}

Database::Database(char path[1024])
{
    selStrobe = 1;
    selNul = 0;
    current = NULL;
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

void Database::setPositionAndFindNearest(double acfX, double acfY, double acfZ, double acfLat, double acfLon)
{
    curX = acfX;
    curY = acfY;
    curZ = acfZ;
    curLat = acfLat;
    curLon = acfLon;
    
    // TODO - optimize - if the current beacon is in range keep it
    
    char channelCode[3];
    sprintf(channelCode, "%1d%1d", selStrobe, selNul);
    
    std::cout << "Validating nearest beacon for " << channelCode;
    
    vector<Beacon>::iterator it =  db.begin();
    while( it != db.end() ) {
        Beacon bc = *it;
        if(strcmp(bc.channel, channelCode) == 0){
            std::cout << "Beacon found on " << channelCode << "k";
            current = &bc; return;
        }
        it++;
    }
}

int Database::size()
{
    return db.size();
}

float Database::getDistance()
{
    // Get the selected channels
    return (current != NULL) ? (*current).distanceFrom(curX, curY, curZ) : 0.0;
}

bool Database::isOverflyingNow()
{
    return (current != NULL) && (getDistance() < curY);
}

bool Database::isReceiving()
{
    return (current != NULL);
}

// TODO refactor
float Database::getBearing()
{
    // Get the selected channels
    if(current != NULL) {
        return (*current).bearingToAcf(curLat, curLon);
    } else  {
        return 0.0;
    }
}

void Database::currentBeaconInfo(char *name)
{
    // Get the selected channels
    if (current != NULL) {
        sprintf(name, "RSBN %s %s %sk",  (*current).name, (*current).callsign, (*current).channel);
    } else {
        // No beacon found, swallow
        strcpy(name, "<NO RECEPTION>");
    }
}
