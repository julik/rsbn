float inline normalizeAngle(float angle)
{
    // Get rid of overruns
    float truncated = fmod(angle, 360);
    return (truncated < 0) ? (360 - truncated) : truncated;
}

static int getNavMode(void* inRefcon)
{
    return nav.selMode;
}

static void setNavMode(void* inRefcon, int newMode)
{
    nav.selMode = newMode;
}

static float getAzimuth(void* inRefcon)
{
    return nav.selAzimuth;
}

static void setAzimuth(void* inRefcon, float a)
{
    nav.selAzimuth = normalizeAngle(a);
}

static float getOrbita(void* inRefcon)
{
    return nav.selOrbita;
}

static void setOrbita(void* inRefcon, float a)
{
    nav.selOrbita = a;
}

static float getTargetAngle(void* inRefcon)
{
    return nav.selTargetAngle;
}

static void setTargetAngle(void* inRefcon, float a)
{
    nav.selTargetAngle = normalizeAngle(a);
}

static float getTargetDist(void* inRefcon)
{
    return nav.selTargetDist;
}

static void setTargetDist(void* inRefcon, float a)
{
    nav.selTargetDist = a;
}

static float getZpu(void* inRefcon)
{
    return nav.selTrack;
}

static void setZpu(void* inRefcon, float a)
{
    nav.selTrack = normalizeAngle(a);
}

static float getCrosstrack(void* inRefcon)
{
    return nav.outXtk;
}