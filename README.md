openCICAD (Controller Inter Communication Array Databus) is a serial inter device communication protocol. The name is a referrence to the [Cicada](http://en.wikipedia.org/wiki/Cicada) bug. The protocol is similar to CAN.

* The protocol is decentralized.
* There is no master and slave concept.
* Messages have IDs not the devices.
* In case of a collision, the priority is given to the message with lower ID.

The documentation for this protocol can be found on the [wiki](https://github.com/amstan/openCICAD/wiki).

This repository includes an implementation of the protocol.