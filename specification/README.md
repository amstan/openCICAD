# openCICAD

## Overview

openCICAD is a serial interdevice communications protocol designed for robust communications over large networks with large numbers of devices. The name is a reference to the [Cicada][Cicada Wiki] bug.

## Protocol Specification

The protocol is divided into 3 logical layers.

### [Layer 1 - Physical](physical.md)

The physical layer defines the electrical characteristics of the bus. This includes logic levels, timing and cabling requirements.

### [Layer 2 - Data](data.md)

The data layer describes the message structure, addressing scheme, collision detection methodology and physical acknowledgement.

### [Layer 3 - Application](application.md)

The application layer describes the various types of messages that can be sent using the openCICAD protocol.

## Message Types

The data layer defines a field in the header of messages that defines the type of message being sent. This message type field also doubles as a method of prioritizing traffic on the openCICAD network.

The types of messages are maintained in the [Message Types](spec/messagetypes.md) document. You will find a complete list of the permissible types of messages on the openCICAD network.

[Cicada Wiki]: http://en.wikipedia.org/wiki/Cicada
