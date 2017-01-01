// Original aRest modules  marco schwartz modifications Dave Clapham
// Lights or power control via WiFi & Wireless using the ESP8266-12e nodeMCU device 
// Message format for relays : ip address /digital/gpio pin #/ On/Off (0 or 1)
//  example : 192.168.0.100/digital/2/0  so ON command to relay on gpio 2 
//
// Message format for relay PULSE : ip address /pulse/gpio pin #/
// example : 192.168.0.100/pulse/2/ pulse relay ON & OFF for 500ms on gpio 2 (2 is only choice, couple both relays for 2 x contacts) 
//
// Message format for wireless : ip address /wireless/wireless socket number/ Off/On (0 or 1)
// example : 192.168.0.100/2/0  so OFF command directed to wireless socket #2 (on/off notation reversed)
// use buttons for on and off relays, pulse for aircon for pulsed on/off, wireless for 315/433mhz wireless devices
// Import required libraries

// Globals - declare RC Wireless messages
const long w1_off = 3572860706;  
const long w1_on  = 3572862764;
const long w2_off = 3572859686;
const long w2_on  = 3572861738;
const long w3_off = 3572860193;
const long w3_on  = 3572862254; // 3572862254
const long w4_off = 3572860968; // all OFF
const long w4_on  = 3572859936; // all ON
const long w5_off = 0;
const long w5_on  = 0;
const long w6_off = 0;
const long w6_on  = 0;
const int pulsedelay = 1500; // pulse delay for pulse command in ms
const boolean debug = true;

#include <ESP8266WiFi.h>
#include <RCSwitch.h>   // wireless setup
RCSwitch mySwitch = RCSwitch(); // wireless setup

#include <aRESTm.h>
#include <aREST_UIm.h>

// Create aREST instance
aREST_UI rest = aREST_UI();

// WiFi parameters
const char* ssid = "Your SSID";
const char* password = "Your WiFi Password";
const String st_html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
float eltime;

// The port to listen for incoming TCP connections 
#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

void setup(void){
   // Start Serial
  Serial.begin(9600);
  // Create UI
  rest.title("Wireless Sockets");
  rest.button(4); // GPIO4 D2
  rest.pulse(5); // GPIO5 D1
  rest.button(2);  // D4 Blue LED 
  rest.wireless(21); // Wireless sockets/appliances 
  rest.wireless(22); // rest.wireless(wireless socket # + 20 offset)
  rest.wireless(23);
  // rest.wireless(24);
  // Give name and ID to device
  rest.set_id("4");
  rest.set_name("Wireless Sockets TX");
   
  // Wireless setup
  mySwitch.setProtocol(1);  // Optional set protocol 1 0r 2 (default is 1, will work for most outlets)  
  mySwitch.enableTransmit(16);  // RC TX module is connected to ESP8266 PIN #16
  mySwitch.setPulseLength(250);  // Optional set pulse length for RC TX (must be after protocol set)
  mySwitch.setRepeatTransmit(3);  // Optional set number of transmission repetitions for RC TX.

  
  digitalWrite(2,HIGH);  // turn off LED
  digitalWrite(4,HIGH); // turn off relay1 port (GPIO4 D2)
  digitalWrite(5,HIGH); // turn off relay2 port (GPIO5 D1)
    
  // Connect to WiFi
  WiFi.begin(ssid, password);
  IPAddress ip(192, 168, 0, 119);     // * comment out 4 lines for DHP
  IPAddress gateway(192, 168, 0, 1);  // *
  IPAddress subnet(255, 255, 255, 0); // *
  WiFi.config(ip, gateway, subnet);   // *
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);  // 250
    Serial.print(".");
  }
   Serial.println("");
   Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  eltime = millis();
  Serial.println(String(eltime/1000) + " Seconds");
}

void loop() {
//  unsigned int ar = analogRead(A0); // read power drawn  // 
//  unsigned int powerdrawn = ar / 1024 / * modrange * ACV; // power in watts test software
//  if (debug) Serial.println(powerdrawn);

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  int timer = 120;  // set a timeout
  int onoff = 0;
  while(!client.available()){
    timer--;
    if(!timer % 12) yield();  // dummy to preserve stack
    if (timer <= 0) {  // timeout
      Serial.println("No Request");
      client.print(st_html + "No Request" + "</html>\n");
      client.flush();
      client.stop();
      return;  
    }
    delay(1);
  }
digitalWrite(0,HIGH);  // send carrier burst to sync up receiver and signal level 
delay(40); 
digitalWrite(0,LOW); 
delay(10);
rest.handle(client);  // service client
}
