#include <stdio.h>
#include "math.h"
#include "navigator.h"

Navigator n;

void inspekt(char* testName)
{
    printf("\n%s\n=========\n", testName);
    printf("outXtk: %0.2f\noutOverflying: %1d\noutApproaching: %1d\n", 
        n.outXtk,
        n.outOverflying,
        n.outApproaching);
}

void assert_d(float v, float ref) {
    if(fabs(v - ref) > 0.1) {
        printf("Delta too big %0.2f vs %0.2f\n", v, ref);
        throw(-1);
    }
}
void assert(int v)
{
    if(!v) {
        printf("Got false\n");
        throw(-1);
    }
}

void assert_not(int v) { assert(!v); }

void checkAzimuthModes()
{
    inspekt("Clean slate");
    n.selMode = 1;
    n.update(45, sqrt(100 * 2));
    inspekt("Bearing 45, distance 10, az 0 Na");
    assert_d(n.outXtk, -10.0);
    assert(!n.outApproaching);
    assert(!n.outOverflying);

    n.selMode = 2;
    n.update(45, sqrt(100 * 2));
    inspekt("Bearing 45, distance 10, az 0 Ot");
    assert_d(n.outXtk, 10.0);
    assert(!n.outApproaching);
    assert(!n.outOverflying);
    
    n.selMode = 2;
    n.selAzimuth = 145;
    n.update(145, sqrt(100 * 2));
    inspekt("Bearing 145, distance 10, az 145 Ot");
    assert_d(n.outXtk, 0.0);
    assert(!n.outApproaching);
    assert(!n.outOverflying);
}

int main(){

try {
    
    checkAzimuthModes();
    return 0;
} catch(int e) {
    return e;
}

}
