/*
This sketch demonstrates how to send data from a Device
to a Host in a Gazell network.

When Button A on the Device is pressed and released,
the green led on the host will toggle.
*/

#include <RFduinoGZLL.h>

device_t role = DEVICE0;

// pin for Button A on the RGB Shield
int button_a = 5;

// starting state is off
char state = 0;

// rssi values
int currentRSSI = 0;
int totalRSSI = 0;
float averageRSSI = 0.0;
int numReadings = 10;
int readingCounter = 0;
float proximityThresholdRSSI = -70.0;

// LED
int green_led = 3;

// timing
int currentTime;
int previousTime;
int interval = 200;

void setup()
{
  pinMode(button_a, INPUT);

  pinMode(green_led, OUTPUT);

  // start the GZLL stack
  RFduinoGZLL.begin(role);
  Serial.begin(9600);
}

void loop()
{
  currentTime = millis();

  if (currentTime - previousTime > interval)
  {
    pingHost();
  }

  if (readingCounter == numReadings)
  {
    averageRSSI = (float)totalRSSI/(float)readingCounter;
    Serial.print("Average Reading: ");
    Serial.println(averageRSSI);
    totalRSSI=0;
    readingCounter = 0;
    setLED();
  }
}

// sends state to host and gets current RSSI strength
void pingHost()
{
  RFduinoGZLL.sendToHost(state);
  previousTime = currentTime;
  digitalWrite(green_led, state);
}

// set the LEDs to on or off if close enough
void setLED()
{
   // set LED on host
   if (averageRSSI <= proximityThresholdRSSI)
   {
      state = 0;
   }
   else
   {
      state = 1;
   }

}

// from the rfduino library
// called when a signal is received
void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  Serial.println(rssi);
  totalRSSI += rssi;
  readingCounter ++;
}
