/*
Adapted from rfduino library
 
changed to show colour selected by device

SOURCED FROM RFDUINO LIBRARY
AVAILABLE AT http://www.rfduino.com/
*/
 
#include <RFduinoGZLL.h>
 
device_t role = HOST;
 
// pins for the Leds
int red_led = 2;
int green_led = 3;
int blue_led = 4;
 
void setup()
{
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
 
  // start the GZLL stack 
  RFduinoGZLL.begin(role);
}
 
void loop()
{
}
 
void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  char ledReceived = data[0];
  int ledToDisplay = ledReceived + 0;
 
  // this test is not needed for a single device
  //if (device == DEVICE0)
  //  digitalWrite(green_led, state);
 
  // no data to piggyback on the acknowledgement sent back to the Device
  // RFduinoGZLL.sendToDevice(device, "OK");
  switch(ledToDisplay)
  {
    case 0:
      digitalWrite(red_led, LOW);
      digitalWrite(green_led, LOW);
      digitalWrite(blue_led, LOW);
      break;
    case 2:
      digitalWrite(red_led, HIGH);
      digitalWrite(green_led, LOW);
      digitalWrite(blue_led, LOW);
      break;
    case 3:
      digitalWrite(red_led, LOW);
      digitalWrite(green_led, HIGH);
      digitalWrite(blue_led, LOW);
      break;
    case 4:
      digitalWrite(red_led, LOW);
      digitalWrite(green_led, LOW);
      digitalWrite(blue_led, HIGH);
      break;
    default:
      break;
  }
}
