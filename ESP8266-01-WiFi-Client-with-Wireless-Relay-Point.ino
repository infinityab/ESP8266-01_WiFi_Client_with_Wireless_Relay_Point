// Original aRest modules  marco schwartz modifications Dave Clapham
// Lights or power control via WiFi & Wireless using the ESP8266 (one port) or ESP8266-12e nodeMCU device multiple ports
// Message format for relays : ip address /digital/gpio pin #/ On/Off (0 or 1)
//  example : 192.168.0.100/2/0  so ON command to relay on gpio 2 
//
// Message format for wireless : ip address /wireless/wireless socket number + 20 / Off/On (0 or 1)
// example : 192.168.0.100/22/0  so OFF command directed to wireless socket #2 (on/off notation reversed)
// use buttons for on and off relays, pulse for aircon for pulsed on/off, wireless for 315/433mhz wireless devices
// Import required libraries

// Globals - declare RC Wireless messages
const long w1_off = 3572860706;  
const long w1_on  = 3572862764;
const long w2_off = 3572859686;
const long w2_on  = 3572861738;
const long w3_off = 3572860193;
const long w3_on  = 3572862254;
const long w4_off = 3572860968; // all OFF
const long w4_on  = 3572859936; // all ON
const long w5_off = 0;
const long w5_on  = 0;
const long w6_off = 0;
const long w6_on  = 0;
const int NumWirelessDevs = 4;     // set this to zero if no wireless devices
const int pulsedelay = 1000; // pulse delay for pulse command in ms

#include <ESP8266WiFi.h>
#include <RCSwitch.h>   // wireless setup
RCSwitch mySwitch = RCSwitch(); // wireless setup

#include <aRESTm.h>
#include <aREST_UIm.h>

// Create aREST instance
aREST_UI rest = aREST_UI();

// WiFi parameters
const boolean debug = true;
const char* ssid = "Your SSID";
const char* password = "Your WiFi Password";
const String st_html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
long eltime;
// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);
void setup(void)
{  
  // Start Serial
  if(debug) Serial.begin(9600);
  
  // Create UI
  rest.title("AirCon Control");
  rest.pulse(2); // GPIO 2
  if (NumWirelessDevs > 0) {   // check & set wireless devices
    for( int i = 0; i < NumWirelessDevs; i++) {
    rest.wireless(i+21); // offset added to avoid clash with ESP8266 pins
    }
  }
  if(!debug) rest.button(1); // Blue LED
  if(!debug) digitalWrite(1,HIGH); // set the pins OFF
  digitalWrite(2,HIGH);
    
    // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");
  
    // Wireless setup
  mySwitch.enableTransmit(0);  // RC TX module is connected to ESP8266 GPIO PIN #0
  mySwitch.setPulseLength(250);  // Optional set pulse length for RC TX.
  mySwitch.setRepeatTransmit(3);  // Optional set number of transmission repetitions for RC TX.
    // mySwitch.setProtocol(2);  // Optional set protocol (default is 1, will work for most outlets)
  
   // Connect to WiFi
  WiFi.begin(ssid, password);
  IPAddress ip(192, 168, 0, 120);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
  if(debug) Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(debug) Serial.println("");
  if(debug) Serial.println("WiFi connected");

  // Start the server
  server.begin();
  if(debug) Serial.println("Server started");
  
  // Print the IP address
  if(debug) Serial.println(WiFi.localIP());
  eltime = millis();
  if(debug) Serial.println(eltime);
}

void loop() {

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  int timer = 120;  // set a timeout
  int onoff = 0;
  while(!client.available()){
    timer--;
    if(!timer % 12){ 
      if (onoff) {
        if(!debug) digitalWrite(1,1);  // blink the blue LED 
        onoff = 0;         // when waiting for request
      }
      else {
        if(!debug) digitalWrite(1,0);
        onoff = 1;  
      }
    }
    if (timer <= 0) {
      if(!debug) digitalWrite(1,1); // switch off blue LED
      if(debug) Serial.println("No Request");
      client.print(st_html + "No Request" + "</html>\n");
      client.flush();
      client.stop();
      return;  
    }
    delay(10);
  }
  if(!debug) digitalWrite(1,1);
rest.handle(client);
}
