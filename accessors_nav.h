static int getNavMode(void* inRefcon)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    return n.selMode;
}

static void setNavMode(void* inRefcon, int newMode)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    n.selMode = newMode;
}

static float getAzimuth(void* inRefcon)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    return n.selAzimuth;
}

static void setAzimuth(void* inRefcon, float a)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    n.selAzimuth = a;
}

static float getOrbita(void* inRefcon)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    return n.selOrbita;
}

static void setOrbita(void* inRefcon, float a)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    n.selOrbita = a;
}

static float getTargetAngle(void* inRefcon)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    return n.selTargetAngle;
}

static void setTargetAngle(void* inRefcon, float a)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    n.selTargetAngle = a;
}

static float getTargetDist(void* inRefcon)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    return n.selTargetDist;
}

static void setTargetDist(void* inRefcon, float a)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    n.selTargetDist = a;
}

static float getZpu(void* inRefcon)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    return n.selTrack;
}

static void setZpu(void* inRefcon, float a)
{
    Navigator n = *(reinterpret_cast<Navigator*>(inRefcon));
    n.selTrack = a;
}