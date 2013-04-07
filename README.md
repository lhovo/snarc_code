snarc_code
==========

Code for the Snarc boards, includes libraries for controlling doors via RFID and other access methods
This is a redesign and expansion of DavidBuzz code found here -> https://github.com/davidbuzz/snarc/tree/master/Firmware

To make this code compile you MUST place the libaries included in this repository into the libary folder of your arduino program, this is due to the Ethernet libary has been modified to support the passing of the Chip Select pin from the code into the libary. The code also adds support for the WIZ ethernet chip W5200 as the orignal libary only comes with support for the W5100.
