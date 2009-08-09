# RSBN plugin for X-Plane

## WHAT IS RSBN

RSBN (Radiosistema Blizhney Navigatsii) was the soviet TACAN analogue. It had approximately the same features,
namely:

* VOR-like operation (fly a radial to or from the beacon, intercept radial)
* Automatic flying of arcs at a specified distance ("Orbita" mode)
* Automatic flight in the vicinity of a beacon (SRP mode), to a specified point on a track parallel to a certain radial
* Notification when reaching a point on a radial at a specified distance
* ILS functionality ("Katet" mode) if the beacon had additional equipment coupled to it at the airfield
* "Meeting" mode where two airplanes could find each other in "peer-to-peer" radio comm
* "Ident" mode whereby pressing a button in the cockpit would "blip" the airplane on the radar screen (a small ATC
station could be driven from the RSBN station on-site)

RSBN beacons were also mobile (often mounted on trucks) and easy to deploy in the field. To this day the Russian
military use RSBN for different operations, and many military RSBN stations are enabled from time to time to provide
for seasonal operations on the most important airfields.

Unlike VORs, beacons were always oriented to true north.

A beacon is tuned using the "channel" just as TACAN. Channels have two digits - 0 to 4 and 0 to 9, this makes
44 channels available at any given time. RSBN station handling ILS-like operations could service less airplanes
at the same time, so soviet airfields would often operate a spare RSBN unit to provide glideslope and localizer.
This functionality is not implemented in the plugin though.

## PLUGIN INFO

The plugin works without a UI. It will load the database of RSBN beacons, and define a number of datarefs:

		rsbn/strobe (int, writable) - the position of the "Strobe" switch, 0 to 4 are allowed
		rsbn/nul (int, writable) - the position of the "Nul" switch, 0 to 9

These two datarefs specify the current RSBN channel and can be coupled to usual X-Plane knobs, or to custom
instruments made using other plugins. The default channel that is tuned when the instrument is initialized
is 10k, that is Tallinn in Estonia and the Baltic and Chkalovsky when in the Moscow region.

It makes the following datarefs available for reading for basic navigation purposes:

		rsbn/receiving 		(int, readonly) - will contain 1 if the beacon is being received now
		rsbn/overflight (int, readonly) - will contain 1 if the beacon is being overflown now (the distance to the beacon
		is less than 1 km)
		
		rsbn/distance		(double, readonly) - linear distance to the beacon being received in kilometers
		rsbn/bearing		(double, readonly) - great circle bearing at the beacon installation point to the aircraft
		rsbn/bearing_to		(double, readonly) - great circle bearing FROM the aircraft TO the beacon, true
		rsbn/bearing_to_mag		(double, readonly) - magnetic bearing FROM the aircraft TO the beacon. Fly THERE.
		rsbn/beacon_lat		(double, readonly) - geo latitude of the current beacon (can be used for calculations)
		rsbn/beacon_lon		(double, readonly) - geo longitude of the current beacon (can be used for calculations)

The following datarefs are made available to implement the navigation functionalities (not evailable on all airplanes)

		rsbn/nav/mode	(int, writable) - the selected navigation mode:
				0 - navigation is disengaged
				1 - radial from
				2 - radial to
				3 - arc counterclockwise
				4 - arc clockwise
				5 - SRP (arbitrary track)
		rsbn/nav/in/azimuth (float, writable) - the selected radial, in degrees
		rsbn/nav/in/orbita (float, writable) - the selected arc radius, in KM
		rsbn/nav/in/t_angle (float, writable) - bearing from the beacon to the target point (SRP mode)
		rsbn/nav/in/t_dist (float, writable) - distance from the beacon to the target point (SRP mode)
		rsbn/nav/in/zpu (float, writable) - track heading to target (SRP mode)
		rsbn/nav/in/nav1_override (int, writable) - set to 1 to "hijack" the nav1 receiver and HSI for RSBN instead of VOR
		

By using these the following datarefs will be provided for reading:

		rsbn/nav/out/xtk (float, readonly) - cross-track deviation in km, negative is "right of track"
		rsbn/nav/out/approaching (int, readonly) - set to 1 when approaching target
		rsbn/nav/out/overflying (int, readonly) - set to 1 when overflying target
		
When nav override is engaged the RSBN set will feed your HSI gage with the selected track as your OBS
and the crosstrack deviation on the OBS needle, counting "dots" in kilometers (full OBS needle deflection
is a deviation from track of about 5-6 km)

## DATASETS

The plugin ships with two datasets. One is for the former USSR and DDR (German Democratic Republic), the other
is for the CIS (post-1991) state (when most RSBN stations have been put offline and dusmantled). You can
switch between the datasets using the plugin menu.

The plugin does not change or affect scenery in any way - you can use any scenery you like.

## DEVELOPMENT AND BUILDING

If you want to contribute to the plugin, you can always check out the latest source at

	http://github.com/julik/rsbn/tree/master

To build on Mac, you will need XCode tools and the 10.4 SDK in
addition to the X-Plane SDK. On Windows, use the latest version of MiniGW and it's associated compilers.
On Linux it's enough to have the SDK installed.

## LICENSE

(The MIT License)

Plugin - Copyright (c) 2009 Julik Tarkhanov.
RSBN data - Copyright (c) 2004 - 2009 Andrey Pryadko

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
