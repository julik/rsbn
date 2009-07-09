#include "beacon.h"

using namespace std;

// Boilerplate
Beacon::Beacon() {}

// Fill the beacon data from a line. Accepts lines like:
// 02|ASTRAKHAN|GV|46.2766666666667|48.0033333333333|1.0
Beacon::Beacon(char *line)
{
    cout << sizeof(*strtok(line, "|"));
    strncpy(channel,     strtok(line, "|"), sizeof(channel));
    strncpy(name,        strtok(NULL, "|"), sizeof(name));
    strncpy(callsign,    strtok(NULL, "|"), sizeof(callsign));
    lat =  atof(strtok(NULL, "|"));
    lon =  atof(strtok(NULL, "|"));
    elev = atof(strtok(NULL, "\n"));
}