# ESP8266-WiFi-Client-with-Wireless
Combined WiFi and Wireless Client Side unit with optional GUI for PC, Tablet or Mobile. This may also be used as the centralised transmitter for the premises. 

Oscars to Marco Schwartz for his original aREST modules and to Suat Özgür and his team for RCSwitch - modifications by me Dave Clapham.
Appliances, Lights etc may be switched via WiFi & Wireless using the ESP8266 (one port) or ESP8266-12e nodeMCU device multiple ports. On the basic ESP8266-1 module connect the relay to D2 and the Wireless module TX data to D0. On ESP8266-12e (recommended) an ON/OFF relay connection uses D2 (GPIO4) and if you want a 500ms pulse output connect one or two relays to D1 (GPIO5) and the contacts are used to momentarily bridge a low power ON/OFF switch such as on an Aircon unit simulating a press of the button.

Use the cheap optically coupled relays that are available in various size boards as 1, 2, 4, 8 or 16 relays. 

Add a 433Mhz transmitter to provide an inhouse Centralised Wireless relay point to drive wireless devices such as sockets, garage doors, awnings etc. using ESP8266-12e nodeMCU device. Connect transmitter to DO (GPIO16) The 433Mhz antenna should be 17.2 cms in length and can be coiled for only short distances but a straight antenna gives a far superior range.
 
Message format for relays : ip address /digital/gpio pin #/ On or Off (0 or 1)
example : 192.168.0.100/digital/2/0  so ON command to relay on gpio 2 

Message format for relay PULSE : ip address /pulse/gpio pin #/

ESP8266-1 example : 192.168.0.100/pulse/2/ pulse relay ON & OFF for 500ms on gpio 2 (2 is only choice on ESP8266-1)

ESP8266-12e example : 192.168.0.100/pulse/4 or 5/  (D2 or D1) 

Message format for wireless : ip address /wireless/wireless socket number + 20 / Off or On (0 or 1)
example : 192.168.0.100/22/0  so OFF command directed to wireless socket #2 (on/off notation reversed)

If using the GUI just enter the ip address on your PC, Tablet or mobile e.g.   http://192.168.0.100/
Use GUI buttons on the display for on and off relays, pulse for aircon for pulsed on/off, wireless for 315/433mhz wireless devices
Import required libraries.
 
