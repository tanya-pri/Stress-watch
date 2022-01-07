#include <Temperature_LM75_Derived.h>
Generic_LM75 temperature;

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = A1;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 400;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

const int GSR = A0;
int sensorValue = 0;
int gsr_average = 0;

void setup() {
  while(!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 

   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() {
  //measure of temperature
  {
  Serial.println(" ");
  Serial.print("Temperature = ");
  Serial.print(temperature.readTemperatureC());
  Serial.println(" C");
  }
  
  //measure of sweating
  {
  long sum = 0;
  for (int i = 0; i < 10; i++)    //Average the 10 measurements to remove the glitch
  {
    sensorValue = analogRead(GSR);
    sum += sensorValue;
    delay(5);
  }
  gsr_average = sum / 10;
  Serial.print("sweating: "); 
  Serial.print(gsr_average);
  Serial.println(" S"); 
  }


  {
  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now.                                           
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
//  Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
  Serial.print("BPM: ");                        // Print phrase "BPM: " 
  Serial.println(myBPM);                        // Print the value inside of myBPM. 
  }
  }

  delay(1000);
 
}
