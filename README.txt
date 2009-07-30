RSBN plugin for X-Plane

-- How it works?

The plugin works without a UI. It will load the database of RSBN beacons, and define a number of datarefs:

rsbn/strobe (int, writable) - the position of the "Strobe" switch, 0 to 4 are allowed
rsbn/nul (int, writable) - the position of the "Nul" switch, 0 to 9

It makes the following datarefs available:

rsbn/online 		(int, readonly) - will contain 1 if the beacon is being received now
rsbn/distance		(double, readonly) - linear distance to the beacon being received in kilometers
rsbn/bearing		(double, readonly) - great circle bearing at the beacon installation point to the aircraft
rsbn/overflight (int, readonly) - will contain 1 if the beacon is being overflown now