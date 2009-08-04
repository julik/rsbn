// Data access callbacks. All X-Plane callbacks for data sets/gets need a void refcon pointer at the start
static int getStrobe(void *inRefcon) 
{
    return rsbn.selStrobe;
}

static void setStrobe(void *inRefcon, int newStrobe) 
{
    rsbn.selStrobe = clamp(0, newStrobe, 4);
}

static int getNul(void* inRefcon) 
{
    return rsbn.selNul;
}

static void setNul(void* inRefcon, int newNul) 
{
    rsbn.selNul = clamp(0, newNul, 9);
}

static double getDist(void* inRefcon) 
{
    return rsbn.getDistance();
}

static double getBearing(void* inRefcon) 
{
    return rsbn.getBearing();
}

static int getOverflight(void* inRefcon) 
{
    return rsbn.isOverflyingNow();
}

static int getReceiving(void* inRefcon) 
{
    return rsbn.isReceiving();
}

static double getBearingTo(void *inRefcon)
{
    return rsbn.getInverseBearing();
}

static double getBeaconLat(void *inRefcon)
{
    return (rsbn.isTuned) ? rsbn.tunedBc.lat : 0;
}

static double getBeaconLon(void *inRefcon)
{
    return (rsbn.isTuned) ? rsbn.tunedBc.lon : 0;
}
