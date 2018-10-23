/*
Device code – changes colour on the host when the
button is pressed
 
SOURCED FROM RFDUINO LIBRARY
AVAILABLE AT http://www.rfduino.com/
*/
 
#include <RFduinoGZLL.h>
 
device_t role = DEVICE0;
 
// pin for Button A on the RGB Shield
int button_a = 5;
 
// starting state is off
char state = 0;
char ledState = 0;
 
// rssi values
int currentRSSI = 0;
int totalRSSI = 0;
float averageRSSI = 0.0;
int numReadings = 10;
int readingCounter = 0;
float proximityThresholdRSSI = -70.0;
 
// LED
int red_led = 2;
int green_led = 3;
int blue_led = 4;
 
int current_led = red_led;
 
char ledToSend = 0;
 
// timing
int currentTime;
int previousTime;
int interval = 200;
 
// button press timing
long pressedTime = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
 
void setup()
{
  pinMode(button_a, INPUT);
 
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
 
  // start the GZLL stack
  RFduinoGZLL.begin(role);
  Serial.begin(9600);
}
 
void loop()
{
  currentTime = millis();
 
  if (digitalRead(button_a) == HIGH && state == 0 && millis() - pressedTime > debounce)
  {
    pressedTime = millis();
    state = 1;
    Serial.println("pressed");
    setLEDColour();
  }
 
  if (digitalRead(button_a) == LOW && state == 1)
  {
    state = 0;
    Serial.println("released");
  }
 
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
  RFduinoGZLL.sendToHost(ledToSend);
  previousTime = currentTime;
  digitalWrite(current_led, ledState);
}
 
// set the LEDs to on or off if close enough
void setLED()
{
   // set LED on host
   if (averageRSSI <= proximityThresholdRSSI)
   {
      ledState = 0;
      ledToSend = 0;
   }
   else
   {
      ledState = 1;
      ledToSend = current_led;
   }
 
}
 
void setLEDColour()
{
  switch(current_led)
  {
    case 2: //red
      current_led = green_led;
      ledToSend = 3;
      break;
    case 3: //green
      current_led = blue_led;
      ledToSend = 4;
      break;
    case 4: //blue
      current_led = red_led;
      ledToSend = 2;
      break;
    default:
      current_led = green_led;
      break;
  }
 
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(blue_led, LOW);
  digitalWrite(current_led, HIGH);
  delay(500);
  digitalWrite(current_led, LOW);
}
 
// from the rfduino library
// called when a signal is received
void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  Serial.println(rssi);
  totalRSSI += rssi;
  readingCounter ++;
}
