#include <vector>
#include "beacon.h"
#include <iostream>
#include <fstream>

using namespace std;

// Fill the beacon data from a line. Accepts lines like:
// 02|ASTRAKHAN|GV|46.2766666666667|48.0033333333333|1.0
Beacon::Beacon(char *line)
{
    strncpy(channel,     strtok(line, "|"), sizeof(channel));
    strncpy(name,        strtok(NULL, "|"), sizeof(name));
    strncpy(callsign,    strtok(NULL, "|"), sizeof(callsign));
    strtok(NULL, "|"); // Skip VOR freq
    lat =  atof(strtok(NULL, "|"));
    lon =  atof(strtok(NULL, "|"));
    elev = atof(strtok(NULL, "\n"));
}

vector<Beacon> beaconList;

int main() {
    
    fstream inFile;
    inFile.open("data/ussr.dat");
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    
    char line[256];
    
    while (inFile.getline(line, sizeof(line)))  {
        Beacon bc(line);
        beaconList.push_back(bc);
    }
    inFile.close();
    
    vector<Beacon>::iterator it;
    for(it = beaconList.begin(); it < beaconList.end(); it++) {
        Beacon bc = *it;
        if(strcmp(bc.channel, "10") == 0) {
            cout << bc.channel << "k " << bc.callsign << " RSBN " << bc.name << " Lat:" << bc.lat << " Lon:" << bc.lon << "\n";
        }
    }
    return 0;
}