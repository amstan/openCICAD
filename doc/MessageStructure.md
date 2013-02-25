# Message Specification

## Dominant vs Recessive
In software the dominant bit is defined as 1 and the recessive bit is defined as 0.

## Endianness
The openCICAD protocol is big endian (network order).

## Message Structure
![Message Structure](https://raw.github.com/amstan/openCICAD/master/doc/MessageStructure.svg)

## Start of Message (SoM)
The first bit of any message is a dominant bit.

## Byte Delimiters
Each byte in an openCICAD message ends with a byte delimeter. The value of the delimeter is the inverse of the LSB of the most recent byte sent. 

## Message Type
The message type bytes form an operation code for the message as well as establishing message priority for collision resolution.

The message type is 2 bytes long which allows for 65536 different kinds of messages. When a collision occurs on the openCICAD bus the message with the lower priority stops transmitting to allow the more important message to finish.

The first two bits of the message type determine the range. There is also a special testing range.

    00       - Unimportant, low priority packets (Example: Firmware Update Data)
    01 .. 10 - Standard packets (Example: Light switch commands)
    11       - Very important packets (Example: Emergency stop commands)

The testing range is located from 0xBFFFFF00 to 0xBFFFFFFF. Any message in this range must be ignored if the device is to be considered openCICAD compliant.

## Address

The address field denotes the 32-bit address of the destination device.

## Control Byte

### Reserved
The reserved section is 4 bits wide. These bits are reserved for future use and must be written to the openCICAD bus as recessive.

### Length
The length field specifies how long the data secion of the message is. The length field is 4 bits wide which puts an upper limit of 16 bytes on the data section of the message.

## Data 0 .. Data n
These bytes contain the data of this message. This data is structured according to the message type specified in the header.

## Cyclic Redundancy Check
This field contains the CRC for the entire message and is calculated using Baicheva's 16 bit polynomial (0xC86C).

The CRC is calculated by the destination device in realtime. The receiver should know whether the data is valid or not by the time it is ready to assert a dominant bit during the Acknowledge bit period. If the CRC is invalid the receiver will not assert a dominant bit during the acknowledge.

[Web-based CRC calculator](http://depa.usst.edu.cn/chenjq/www2/SDesign/JavaScript/CRCcalculation.htm)

## Message Delimiter
To prevent a momentary recessive glitch when the transmitter stops transmitting a dominant CRC2 delimiter and the acknowleger starts transmitting a dominant ACK.

## Acknowledge
If the destination device receives this message without error they must drive the line dominant for the entire width of the acknowledge bit.

## End of Message (EoM)
The final section of the message is the End of Message. This portion of the message is 10 recessive bits. It is impossible to have a message that is more than 9 consecutive zeros.
