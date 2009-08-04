// Data access callbacks. All X-Plane callbacks for data sets/gets need a void refcon pointer at the start
static int getStrobe(void *inRefcon) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).selStrobe;
}

static void setStrobe(void *inRefcon, int newStrobe) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    (*d).selStrobe = clamp(0, newStrobe, 4);
}

static int getNul(void* inRefcon) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).selNul;
}

static void setNul(void* inRefcon, int newNul) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    (*d).selNul = clamp(0, newNul, 9);
}

static double getDist(void* inRefcon) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).getDistance();
}

static double getBearing(void* inRefcon) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).getBearing();
}

static int getOverflight(void* inRefcon) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).isOverflyingNow();
}

static int getReceiving(void* inRefcon) {
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).isReceiving();
}

static double getBearingTo(void *inRefcon)
{
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).getInverseBearing();
}

static double getBearingToMag(void *inRefcon)
{
    Database* d = reinterpret_cast<Database*>(inRefcon);
    return (*d).getInverseBearing() + XPLMGetDataf(proxy.magVarRef);
}

static double getBeaconLat(void *inRefcon)
{
    Database d = (*(reinterpret_cast<Database*>(inRefcon)));
    return (d.isTuned) ? d.tunedBc.lat : 0;
}

static double getBeaconLon(void *inRefcon)
{
    Database d = (*(reinterpret_cast<Database*>(inRefcon)));
    return (d.isTuned) ? d.tunedBc.lon : 0;
}
