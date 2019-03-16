//float volts = analogRead() * 5.0 / 1024.0;
//float amps = volts / 10000.0;  // across 10,000 Ohms
//float microamps = amps * 1000000;
//float lux = microamps * 2.0;

#define LIGHTSENSORPIN A5 //Ambient light sensor reading 

void setup() {
  pinMode(LIGHTSENSORPIN,  INPUT);  
  Serial.begin(9600);
}

void loop() {

  float lux = analogRead(LIGHTSENSORPIN) * 0.9765625;  // 1000/1024
  Serial.println(lux);
delay(100);
}
