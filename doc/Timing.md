# Timing

All of the devices on the openCICAD network must be running at the same baud.

## Rx/Tx Timer Differences
The clocks are synchronized each time a bit changes state. This means that the openCICAD bus i synchronized a minimum of once per byte (due to the inverted byte delimeter).

## Tolerance of Jitter

![Timing diagram](https://github.com/amstan/openCICAD/raw/master/doc/rxtx%20timer%20differences.png)

### Tolerance of Frequency Jitter
The openCICAD bus can withstand at most +-5.555% change in frequency.

This [interactive demonstration](https://raw.github.com/amstan/openCICAD/master/doc/RxTxTimerDifferences.kig) shows how the network responds to changes in frequency.

### Tolerance of Phase Jitter
Assuming that the network maintains constant frequency the openCICAD bus can withstand up to 50% phase jitter due to the fact that the values of bits are read at the middle of transmission.

## Baud Rate
The speed of the communication depends on line length/quality. A high impedance line will require a lower baud rate to ensure successful communications.

## Autobaud
Autotiming might be helpful when the line quality is slow, it could be an easy way to adjust the speed on the fly. This could benefit slow busses, where the devices could detect the increased error rate and switch to a lower speed.

1. A device C will want to find the speed of the bus by listening to the conversation between device A and B
2. Device C will take a guess at the speed, by measuring the shortest pulse
3. Device C will test that speed by trying to decode and check the checksum on a packet
4. If successful the device will communicate at the new speed
