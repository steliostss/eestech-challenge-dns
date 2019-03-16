

#include <dht.h>
dht DHT;
#define DHT11_PIN 7

//float volts = analogRead() * 5.0 / 1024.0;
//float amps = volts / 10000.0;  // across 10,000 Ohms
//float microamps = amps * 1000000;
//float lux = microamps * 2.0;

#define LIGHTSENSORPIN A5 //Ambient light sensor reading 


#define ANALOG_IN_SOUND  A0 // X-axis-signal

int TrigPin = 12;
int EchoPin = 11;
int duration;
float distance;


void setup() {
  Serial.begin(9600);
  pinMode(LIGHTSENSORPIN,  INPUT);  
  pinMode (ANALOG_IN_SOUND, INPUT);
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin, INPUT);
}

void loop() {
  //temp
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(100);

  //light
  float lux = analogRead(LIGHTSENSORPIN) * 0.9765625;  // 1000/1024
  Serial.print("Light measured in lux is: ");
  Serial.println(lux);
delay(100);
  //sound

  float Analog;    
  Analog = analogRead (ANALOG_IN_SOUND); 
    
  Serial.print ("Sound measured in the room is: "); Serial.println (Analog);  
 
delay(100);


  //ultrasonic
    digitalWrite(TrigPin,LOW);
    delayMicroseconds(2); // Ensure Trig pin is low
    digitalWrite(TrigPin,HIGH);
    delayMicroseconds(10); // 10us pulse sent to Trig
    digitalWrite(TrigPin,LOW);
    duration = pulseIn(EchoPin,HIGH);
    distance = duration /58.82;
    Serial.print("Distance from nearest object is: ");
    Serial.print(distance);
    Serial.println(" cm");


  
  delay(2000);
  Serial.println("--------------------------------------------------------");

}
