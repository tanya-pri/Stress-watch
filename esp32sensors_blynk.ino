#define USE_ARDUINO_INTERRUPTS true 

#define BLYNK_TEMPLATE_ID           "TMPLsCE-ddS3"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "uId00bD5FjhyYNZl3ebfrGhXH5gJXEVd"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <PulseSensorPlayground.h>
#include <Temperature_LM75_Derived.h>
Generic_LM75 temperature;

const int PulseWire = A9;
const int GSRWire = A7;
int GSRsensorValue;
// PulseSensor PURPLE WIRE connected to ANALOG PIN 9
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.

PulseSensorPlayground pulseSensor;
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MovuinoTest";
char pass[] = "MovuinoTest";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = temperature.readTemperatureC();

  //Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  while(!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  // Double-check the "pulseSensor" object was created and "began" seeing a signal.

  pulseSensor.begin();

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.

  if (pulseSensor.begin())
    Serial.println("We created a pulseSensor Object !");

  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(1000L, getValues);
}

void getValues(void) {
  
  float temp = temperature.readTemperatureC();
  float HeartRate = pulseSensor.getBeatsPerMinute();
  float GSR;
  float GSR_sum = 0;
  
  Serial.printf("Temperature = %f\n", temp);
  Blynk.virtualWrite(V4, (int)temp); //convert float temp to integer

  Serial.printf("Heart rate = %d\n", HeartRate);
  Blynk.virtualWrite(V6, (int)HeartRate); //convert float pulse to integer

  for(int i=0;i<10;i++)           //Average the 10 measurements to remove the glitch
      {
      GSR_sum += analogRead(GSRWire);
      delay(5);
      }
   GSR = GSR_sum / 10;
   Serial.printf("GSR = %f\n", GSR);
  Blynk.virtualWrite(V5, (int)GSR); //convert float pulse to integer

}


void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
