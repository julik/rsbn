
class   Beacon
{
public:
    // Injected from the datafile
    char        channel[3];
    char        callsign[4];
    char        name[256];
    double      lat;
    double      lon;
    double      elev;
    
    // Dd we already compute the global X-Plane coords for the beacon, 1 is yes
    bool cachedPos;
    double      globalX;
    double      globalY;
    double      globalZ;
    
    // Implicit constructor
    Beacon();
    
    // Construct from a line
    Beacon(char *line);
    
    // Get local X-Plane coordinates
    void localCoords();
};
