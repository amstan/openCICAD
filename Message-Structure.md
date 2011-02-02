![Message Structure](https://github.com/amstan/openCICAD/raw/master/doc/MessageStructure.png)

### SoM
Start of Message. Always the Dominant Bit.

### ID
Message ID. Lower Will always take priority.

If most significant bit is set, it means the current byte is not the last one. This goes up to 3 bytes.

    ID Length   # of Possible IDs
    1           128
    2           128*128
    3           12

### CTRL

![Control Byte](https://github.com/amstan/openCICAD/raw/master/doc/ControlByte.png)

### DATA

4 bits of length allows a maximum of 16 bytes of data.

### CRC

Checksum for the whole message. The CRC will be done using Baicheva's 16 bit polynomial (0xC86C).

### ACK

If peers recieved the message without errors, they drive this bit to Dominant.

### EoM

End of Message. 9 Recessive Bits.  

### Byte delimiters.

At the end of each byte, there's a bit inserted that's the opposite of the previous, to facilitate syncing.