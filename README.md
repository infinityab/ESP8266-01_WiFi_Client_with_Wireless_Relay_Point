# ESP8266-WiFi-Client-with-Wireless
Combined WiFi and Wireless Client Side with optional GUI for PC, Tablet or Mobile.

Oscars to Marco Schwartz for his original aREST modules and to Suat Özgür and his team for RCSwitch - modifications Dave Clapham.
Appliances, Lights etc may be switched via WiFi & Wireless using the ESP8266 (one port) or ESP8266-12e nodeMCU device multiple ports.

Optionally add 433Mhz transmitter ($1) to build an inhouse Wireless cell network to also drive wireless devices such as sockets, 
garage doors, awnings etc.

Message format for relays : ip address /digital/gpio pin #/ On or Off (0 or 1)
example : 192.168.0.100/2/0  so ON command to relay on gpio 2 

Message format for wireless : ip address /wireless/wireless socket number + 20 / Off or On (0 or 1)
example : 192.168.0.100/22/0  so OFF command directed to wireless socket #2 (on/off notation reversed)

If using the GUI just enter the ip address on your PC, Tablet or mobile e.g.   http://192.168.0.100/
Use GUI buttons for on and off relays, pulse for aircon for pulsed on/off, wireless for 315/433mhz wireless devices
Import required libraries
