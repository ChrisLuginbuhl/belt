    /*******************************************************           
    * Dwell Belt --  Down with distractions, up with pants *   
    *                                                      *
    * DIGF-6037-001 Creation & Computation                 *                                       
    *                                                      *  
    * Assignment 3 Nov 10, 201                             *
    *                                                      * 
    * Chris Luginbuhl .                                    *   
    *                                                      *   
    * Purpose:  For use with the dwell belt, manages       *
    * switches, animations and wifi activiity              *   
    *                                                      *   
    * Usage:                                               *   
    *  requires wifi credentials in adruino_secrets.h      *
    *  Remove /* symbols for serial port output            *
    *                                                      *
    * Credits: Makes use of Adafruit's wifi101 WiFiClient  *
    * and strandtest (neopixel) example                    *
    ********************************************************/  

#include <SPI.h>
#include <WiFi101.h>
#include <Adafruit_NeoPixel.h>
#include "arduino_secrets.h"

#define PIN 6
#define STRIPSIZE 36

//function prototypes - preprocessor seems to like to know up front.
void doMode0();
void doMode1();
void doMode2();
void doMode3();
void doMode4();
void checkSwitches();
boolean switchHasMoved();
void printWiFiStatus();
void callAdafruit();
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void theaterChase(uint32_t c, uint8_t wait);
void theaterChaseRainbow(uint8_t wait);
void colourInnerRing(uint32_t c, uint8_t wait);
void colourMiddleRing(uint32_t c, uint8_t wait);
void colourOuterRing(uint32_t c, uint8_t wait);
uint32_t Wheel(byte WheelPos);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);
//Switch is 2P5T, with "0" position open circuit
const int swPos1 = 9;      // Pin connected to switch terminal 1
const int swPos2 = 10;     // Pin connected to switch terminal 2
const int swPos3 = 11;     // Pin connected to switch terminal 3
const int swPos4 = 12;     // Pin connected to switch terminal 4
const int swSend = 13;     // Pin connected to switch Send button (momentary)

int switchPos = 0;        //will store rotary switch position 0-4
int sendPressed = false;

void setup() {

  // initialize the switch & button pins as input. Switches connected to ground when closed.
  pinMode(swSend, INPUT_PULLUP);
  pinMode(swPos1, INPUT_PULLUP);
  pinMode(swPos2, INPUT_PULLUP);
  pinMode(swPos3, INPUT_PULLUP);
  pinMode(swPos4, INPUT_PULLUP);

  WiFi.setPins(8, 7, 4, 2);

  // Set up neopixels
  strip.begin();
  strip.setBrightness(25);  // Lower brightness and save eyeballs!
  strip.show();             // Initialize all pixels to 'off'

  /*
    //Initialize serial and wait for port to open:
    Serial.begin(9600);

    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  */
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

}

void loop() {

  checkSwitches();

  switch (switchPos) {
    case 0:
      doMode0();
      break;
    case 1:
      doMode1();
      break;
    case 2:
      doMode2();
      break;
    case 3:
      doMode3();
      break;
    case 4:
      doMode4();
      break;
  }

  /*
    Serial.print("The switch position is: ");
    Serial.println(switchPos);
    Serial.print("Send pressed: ");
    Serial.println(sendPressed);
    delay(500);
  */
}

void doMode0() {
  //turn off LEDs
  colorWipe(strip.Color(0, 0, 0), 20); // Black (off)
}

void doMode1() {
  //"approach me" display
  //rainbow(15);

  while (true) {
    for (int i = 0; i < 256; i += 4) {
      colourOuterRing(Wheel(i), 0);
      colourMiddleRing(Wheel(i + 32), 0);
      colourInnerRing(Wheel(i + 64), 0);
    }
    strip.show();
    if (switchHasMoved()) {
      break;
    }
  }
}

void doMode2() {
  //"party time" display
  theaterChaseRainbow(65);
}

void doMode3() {
  //"I am struggling with my code" display
  theaterChase(strip.Color(255, 0, 0), 50);
}

void doMode4() {

  //"Coding emergency" display + call for help via Facebook
  int wait = 100;
  int i;
  int j;

  //Red rings converge on Send button. Silly if() statements are checking if the button was pressed.
  while (!sendPressed) {
    colourOuterRing(strip.Color(255, 0, 0), 0);
    strip.show();
    delay(wait);
    colourOuterRing(strip.Color(0, 0, 0), 0);
    colourMiddleRing(strip.Color(255, 0, 0), 0);
    strip.show();
    delay(wait);
    if (digitalRead(swSend) == LOW) {
      sendPressed = true;
      break;
    }
    colourMiddleRing(strip.Color(0, 0, 0), 0);
    colourInnerRing(strip.Color(255, 0, 0), 0);
    strip.show();
    delay(wait);
    colourInnerRing(strip.Color(0, 0, 0), 0);
    delay(wait);
    if (digitalRead(swSend) == LOW) {
      sendPressed = true;
      break;
    }
    delay(wait);
    if (digitalRead(swSend) == LOW) {
      sendPressed = true;
      break;
    }
    delay(wait);
    if (digitalRead(swSend) == LOW) {
      sendPressed = true;
      break;
    }
    delay(wait);
    if (digitalRead(swSend) == LOW) {
      sendPressed = true;
      break;
    }
    if (switchHasMoved()) {
      break;
    }

  }

  if (sendPressed) {
    callAdafruit();  //She'll know what to do.

    // fade soothing green lights  until help arrives or the universe ends.
    while (true) {
      for (i = 0; i < 256; i += 4) {
        for (j = 0; j < strip.numPixels(); j++) {
          strip.setPixelColor(j, strip.Color(0, i, 0));
        }
        strip.show();
        delay(20);
      }
      for (i = 0; i < 256; i += 4) {
        for (j = 0; j < strip.numPixels(); j++) {
          strip.setPixelColor(j, strip.Color(0, 255 - i, 0));
        }
        strip.show();
        delay(20);
      }
    }
  }
}

void checkSwitches() {
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
}

//used to exit a colour animation if the mode switch is moved
boolean switchHasMoved() {
  int pos = switchPos;
  checkSwitches();
  //exit the for loop if the switch has been moved
  if (pos != switchPos) {
    return true;
  } else {
    return false;
  }
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
  ///////please enter your sensitive data in the Secret tab/arduino_secrets.h
  char ssid[] = SECRET_SSID;        // your network SSID (name)
  char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
  int keyIndex = 0;            // your network key Index number (needed only for WEP)

  int status = WL_IDLE_STATUS;
  char server[] = "io.adafruit.com";    // name address for Google (using DNS)
  WiFiClient client;
  // attempt to connect to WiFi network:

  colorWipe(strip.Color(0, 0, 0), 5);
  colorWipe(strip.Color(255, 191, 0), 40);
  while (status != WL_CONNECTED) {
    /*
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
    */
    colorWipe(strip.Color(255, 191, 0), 40);
    // Connect to WPA/WPA2 network. Would change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(3000);
    colourInnerRing(strip.Color(0, 255, 0), 50);
    delay(3500);
    colourMiddleRing(strip.Color(0, 255, 0), 50);
    delay(3500);

  }
  /*
    Serial.println("Connected to wifi");
    printWiFiStatus();
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
  */

  if (client.connect(server, 80)) {
    //    Serial.println("connected to server");

    // Make a HTTP request. A data value of 42 posts to the Digital Futures 2019 facebook group. Any other value posts to my test group.
    client.println("GET /api/groups/luginbu/send.json?x-aio-key=d973c87082a34f29b9244f5ae6401cec&p5test=+42 HTTP/1.1");
    client.println("Host: io.adafruit.com");
    client.println("Connection: close");
    client.println();
    colourOuterRing(strip.Color(0, 255, 0), 50);
    // if there are incoming bytes available
    // from the server, read them and print them:
    /*
        while (client.available()) {
          char c = client.read();
          Serial.write(c);
        }
    */
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      /*
            Serial.println();
            Serial.println("disconnecting from server.");
      */
      client.stop();
    }
    // do nothing forevermore:
    //    while (true);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    if (switchHasMoved()) {
      break;
    }
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    if (switchHasMoved()) {
      break;
    }
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 1; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();
      if (switchHasMoved()) {
        break;
      }
      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j += 6) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();
      if (switchHasMoved()) {
        break;
      }
      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Fill the inner ring dots one after the other with a color
void colourInnerRing(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < 8; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void colourMiddleRing(uint32_t c, uint8_t wait) {
  for (uint16_t i = 8; i < 20; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colourOuterRing(uint32_t c, uint8_t wait) {
  for (uint16_t i = 20; i < 36; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

