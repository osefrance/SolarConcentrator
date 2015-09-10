//**************************************************************
//  Open Souce Écologie - Solar Heliostat controller
//  Name: SeguiLSol25 - README file
//  Authors: Andrea Sannuto @ndrsan, Jean Walter, ... (other contributors add yourself here)
//  Website: www.osefrance.fr  @OSE_Fr
//  Rev 001 
//  Date 08 April 2015
// This Readme files is meant to collect all the intended modification that need to be integrated into the software.
// this software is released under the GPL V.3 
// for more recents update, check out the git page: https://github.com/osefrance/SolarConcentrator
// http://bit.ly/1JnwSGd
//**************************************************************

it features a simple microstepping of 1/16th of 200 full steps stepper motor and makes a microstep every 54seconds or so.

will need to add:

release 1:
- change routines to State-machine instead of using delay
- reset to 0 position using an optical sensor (as interrupt)
- first setup on the morning (need rtc implementation) using 2 light sensors on the absorber edges (sensors as interrupts)
- last setup on the evening in order to understand the error and use the new delay time for the day after.
release 2:
- multiple mirrors using an IC (need wire implementation)
release 3:
- safety shut off of the mirrors
- statistical information
...

