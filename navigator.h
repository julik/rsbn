using namespace std;

class Navigator
{
public:
    
    // Cross-track deviation in kilometers, negative is right-of-track
    double outXtk;
    
    // Set to TRUE if the lamp has to blink
    bool outApproaching;
    
    // Set to TRUE if the lamp has to blink
    bool outOverflying;
    
    // Selected RSBN computer mode
    int selMode;
    
    // Selected radial
    double selAzimuth;
    
    // Selected distance (orbita)
    double selOrbita;
    
    // Selected target ange (bearing from the beacon to the SRP point)
    double selTargetAngle;

    // Selected target distance (distance from the beacon to the SRP point)
    double selTargetDist;

    // Selected track (ZPU)
    double selTrack;
    
    void update(double acfBrg, double acfDist);
    
    Navigator();
private:
    void changeLampState(double acfBrg, double acfDist);
    void computeSrp(double  acfBrg, double acfDist);
};
