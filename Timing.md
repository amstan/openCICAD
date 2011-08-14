In order for devices to understand each other they'll each need an internal timer. They should have relatively the same period.

## RX/TX Timer Differences
Using the current(8c815d6a6f1e9d9fddbd) algorithm of syncing the clock at every pulse edge there can be a maximum difference of +- 5.555% between the RX/TX timers.

Sources of problems at lower/higher speeds:

* Between 7.1% to 5.6%: EoM Problems, might loose priority during SoM if slower, violate the priority of others of faster.
* Lower than 7.1%: Will cause 8 consecutive bits to be read wrong, reading 7/9 consecutive bits instead.

An interactive demonstration can be found in [doc/rxtx timer differences.kig](https://github.com/amstan/openCICAD/blob/master/doc/rxtx%20timer%20differences.kig):
![Timing diagram](https://github.com/amstan/openCICAD/raw/master/doc/rxtx%20timer%20differences.png)

### Tests
An [experiment](https://github.com/amstan/openCICAD/commit/8c815d6a6f1e9d9fddbdd716e7f3e26aec59e194#commitcomment-530397) was done to check the results. Using an 11 period time length. RX works between 97.7%-105.9%.

## Changing the timing
The speed of the communication depends on line length/quality. The longer the line the slower the communications have to go because of capacitance/other effects. The devices could be made to work slower by introducing them in a faster bus and transmitting a setup cicad packet to slow down communications.

## Autotiming
Autotiming might be helpful when the line quality is slow, it could be an easy way to adjust the speed on the fly. This could benefit slow busses, where the devices could detect the increased error rate and switch to a lower speed.

1. A device C will want to find the speed of the bus by listening to the conversation between device A and B
2. Device C will take a guess at the speed, by measuring the shortest pulse
3. Device C will test that speed by trying to decode and check the checksum on a packet
4. If successful the device will communicate at the new speed