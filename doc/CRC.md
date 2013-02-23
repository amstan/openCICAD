The crc needs to be calculated by all the receiving nodes real time and Ack the message.

[Web-based CRC calculator](http://depa.usst.edu.cn/chenjq/www2/SDesign/JavaScript/CRCcalculation.htm)

## CRC-16
The polynomial for the CRC was chosen to be 0xC86C = x^16 + x^15 + x^14 + x^11 + x^6 + x^5 + x^3 + x^2 + 1.

According to [this paper](https://github.com/amstan/openCICAD/raw/f803ea50e8b95cbeb35dbc2ddf5a203411480454/doc/koopman04_crc_poly_embedded.pdf), 0xC86C is one of the best 16 bit checksum polynomials.

## How to calculate the CRC

* take 16 bits, XOR with 0xC86C
* crop the most significant 0 bits
* add more bits from the stream to the end till it's 16 bits
* restart

**Transmitter Example:** 0xAAAA -> CRC:0xFFF6

    10101010101010100000000000000000
    11100100001101100
    010011101001110000
     11100100001101100
     011110010000111000
      11100100001101100
      00010110001010100000
         11100100001101100
         010101010110011000
          11100100001101100
          010011101111101000
           11100100001101100
           011110011100001000
            11100100001101100
            00010111101100100000
               11100100001101100
               010110011010011000
                11100100001101100
                010101110111101000
                 11100100001101100
                 010010101100001000
                  11100100001101100
                  011100011011001000
                   11100100001101100
                   00000111010100100 = 0x0EA4

**Reciever Checking Example:** 0xAAAB0EA4-> 0x0000

    10101010101010100000111010100100
    11100100001101100
    010011101001110000
     11100100001101100
     011110010000111000
      11100100001101100
      00010110001010100011
         11100100001101100
         010101010110011111
          11100100001101100
          010011101111100110
           11100100001101100
           011110011100010101
            11100100001101100
            00010111101111001010
               11100100001101100
               010110011101001100
                11100100001101100
                010101111001000001
                 11100100001101100
                 010010110001011010
                  11100100001101100
                  011100100001101100 
                   11100100001101100
                   00000000000000000= 0x0000