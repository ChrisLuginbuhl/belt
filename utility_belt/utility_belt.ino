/*
  Web client

  This sketch connects to a website (http://www.google.com)
  using a WiFi shield.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  Circuit:
   WiFi shield attached

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 31 May 2012
  by Tom Igoe
*/


#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h"

                          //Switch is 2P5T, with "0" position open circuit
const int swPos1 = 9;     // Pin connected to switch terminal 1 
const int swPos2 = 10;     // Pin connected to switch terminal 2 
const int swPos3 = 11;     // Pin connected to switch terminal 3 
const int swPos4 = 12;     // Pin connected to switch terminal 4 
const int swSend = 13;     // Pin connected to switch Send button (momentary)

int switchPos = 0;        //will store rotary switch position 0-4
int sendPressed = false;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "io.adafruit.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;


void setup() {
 // initialize the switch & button pins as input. Switches connected to ground when closed.
  pinMode(swSend, INPUT_PULLUP);
  pinMode(swPos1, INPUT_PULLUP);
  pinMode(swPos2, INPUT_PULLUP);
  pinMode(swPos3, INPUT_PULLUP);
  pinMode(swPos4, INPUT_PULLUP);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  WiFi.setPins(8, 7, 4, 2);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  /*
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    //callAdafruit();
  }
*/
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
//    while (true);
  }
  //Serial.println(digitalRead(swPos2));
  if (digitalRead(swPos1) == LOW) {
    switchPos = 1; 
  } else if (digitalRead(swPos2) == LOW) {
    switchPos = 2; 
  } else if (digitalRead(swPos3) == LOW) {
    switchPos = 3; 
  } else if (digitalRead(swPos4) == LOW) {
    switchPos = 4;
  } else {
    switchPos = 0; 
  }
  if (digitalRead(swSend) == LOW) {
    sendPressed = true;
  } else {
    sendPressed = false;
  }
  Serial.print("The switch position is: ");
  Serial.println(switchPos);
  Serial.print("Send pressed: ");
  Serial.println(sendPressed);
  delay(500);
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void callAdafruit() {   
    client.println("GET /api/groups/luginbu/send.json?x-aio-key=d973c87082a34f29b9244f5ae6401cec&p5test=+39 HTTP/1.1");
    client.println("Host: io.adafruit.com");
    client.println("Connection: close");
    client.println();
  }



