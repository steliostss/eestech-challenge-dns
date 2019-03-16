#include <dht.h>
dht DHT;

//TODO: Add setup, includes and global variables
#define ENTRANCE_PROX_SENSOR_TRIG 12
#define ENTRANCE_PROX_SENSOR_ECHO 11
#define EXIT_PROX_SENSOR_TRIG 10
#define EXIT_PROX_SENSOR_ECHO 9
#define INSIDE_LUMINOCITY_SENSOR A5
#define OUTSIDE_LUMINOCITY_SENSOR A4
#define INSIDE_TEMP_SENSOR 7
#define OUTSIDE_TEMP_SENSOR 8
#define LOUDNESS_SENSOR A0

#define AC 0
#define LIGHTS_FRONT 1
#define LIGHTS_BACK 2
#define WINDOWS_OPEN 3
#define WINDOWS_CLOSE 4
#define SHADES_OPEN 6
#define SHADES_CLOSE 7
#define MICROPHONE 5

#define TEMP_EPSILON 2   //DHT11 temperature accuracy
#define OPTIMAL_TEMP 25

#define TIME_20MIN_MS 1200000
#define TIME_QUANTUM 500
#define WIDTH_LIMIT 120 //in cm
// Sensor readings as global variables
float luminocity_inside = 0; // in lux
float luminocity_outside = 0; // in lux
float temperature_inside = 0; // in degrees C
float temperature_outside = 0; // in degrees C
float loudness = 0;    //
int persons = 1;    // This reading is the reason we cannot introduce delays
int quantums = 0;
int quantums_empty = 0;
int shade = 1;
int lights_auto = 1;
int microphone_value = 100;
int duration_exit;
float distance_exit = WIDTH_LIMIT + 1;
int duration_entrance;
float distance_entrance = WIDTH_LIMIT + 1;
bool entree;
bool exitee;


void setup() {
  Serial.begin(9600);
  pinMode(INSIDE_LUMINOCITY_SENSOR,  INPUT);  
  pinMode(OUTSIDE_LUMINOCITY_SENSOR,  INPUT);  
  pinMode (LOUDNESS_SENSOR, INPUT);
  pinMode(ENTRANCE_PROX_SENSOR_TRIG,OUTPUT);
  pinMode(ENTRANCE_PROX_SENSOR_ECHO , INPUT);
  pinMode(EXIT_PROX_SENSOR_TRIG,OUTPUT);
  pinMode(EXIT_PROX_SENSOR_ECHO , INPUT);
  pinMode( AC, OUTPUT);
  pinMode(LIGHTS_FRONT, OUTPUT);
  pinMode(LIGHTS_BACK, OUTPUT);
  pinMode(WINDOWS_OPEN, OUTPUT);
  pinMode(WINDOWS_CLOSE, OUTPUT);
  pinMode(SHADES_OPEN, OUTPUT);
  pinMode(SHADES_CLOSE, OUTPUT);
  pinMode(MICROPHONE, OUTPUT);

}

void loop() {
    // This is done every quantum

    //Check proximity sensors
    
    digitalWrite(EXIT_PROX_SENSOR_TRIG,LOW);
    delayMicroseconds(2); // Ensure Trig pin is low
    digitalWrite(EXIT_PROX_SENSOR_TRIG,HIGH);
    delayMicroseconds(10); // 10us pulse sent to Trig
    digitalWrite(EXIT_PROX_SENSOR_TRIG,LOW);
    duration_exit = pulseIn(EXIT_PROX_SENSOR_ECHO,HIGH);
    distance_exit = duration_exit /58.82;
    Serial.print("Distance in exit door is ");
    Serial.println(distance_exit);


    digitalWrite(ENTRANCE_PROX_SENSOR_TRIG,LOW);
    delayMicroseconds(2); // Ensure Trig pin is low
    digitalWrite(ENTRANCE_PROX_SENSOR_TRIG,HIGH);
    delayMicroseconds(10); // 10us pulse sent to Trig
    digitalWrite(ENTRANCE_PROX_SENSOR_TRIG,LOW);
    duration_entrance = pulseIn(ENTRANCE_PROX_SENSOR_ECHO,HIGH);
    distance_entrance = duration_entrance /58.82;
    Serial.print("Distance in entrance door is ");
    Serial.println(distance_entrance);

    if (distance_entrance < WIDTH_LIMIT) {
      entree = 1;
    }
    else {
      entree = 0;
    }

    
    if (distance_exit < WIDTH_LIMIT) {
      exitee = 1;
    }
    else {
      exitee = 0;
    }
    
    // Turn off AC if empty for more than 20 minutes
    if (persons == 0 && entree == 0) {
        quantums_empty++;
        if (quantums_empty > TIME_20MIN_MS / TIME_QUANTUM) {
            // Turn off AC
            digitalWrite(AC,LOW);
        }
    }

    if (persons == 1 && exitee == 1 && entree == 0) {
        // All my friends are gone
        persons = 0;

        // Turn off lights
        digitalWrite(LIGHTS_FRONT, LOW);
        digitalWrite(LIGHTS_BACK, LOW);
        
    }


    
    // Update persons
    persons = persons + (entree ? 1 : 0) - (exitee ? 1 : 0);
    
    if (persons<0) persons =0; // no reason to have negativity in here
    
    Serial.print("There are ");
    Serial.print(persons);
    Serial.println(" persons inside the room right now");
    // do all these only if there are people inside, no reason to do anything otherwise
    if (persons > 0){
        quantums_empty = 0;


    // If there are no people inside why bother

    // Check serial input
    // TODO: If there is a command bypass luminocity checks

    String command = Serial.readString();
    if (command.equals("K")) {
      lights_auto = 0;
      digitalWrite(LIGHTS_BACK, HIGH);
      Serial.println(command);
    }
    if (command.equals("E")) {
      lights_auto = 0;
      digitalWrite(LIGHTS_FRONT, HIGH);
      Serial.println("FRONT LIGHTS ON");
    }
    if (command.equals("ON")) {
      lights_auto = 0;
      digitalWrite(LIGHTS_BACK, HIGH);
      digitalWrite(LIGHTS_FRONT, HIGH);
      Serial.println("ALL LIGHTS ON");
    }
    if (command.equals("OFF")) {
      lights_auto = 0;
      digitalWrite(LIGHTS_BACK, LOW);
      digitalWrite(LIGHTS_FRONT,LOW);
      Serial.println("ALL LIGHTS OFF");
    }
    
    if (command.equals( "AUTO")) {
     lights_auto = 1;
     Serial.println("BACK TO AUTO");
    }


    if (lights_auto == 1) {
      // Check luminocity sensors

      luminocity_inside = analogRead(INSIDE_LUMINOCITY_SENSOR) * 0.9765625; 
      luminocity_outside = analogRead(OUTSIDE_LUMINOCITY_SENSOR) * 0.9765625; 
      Serial.print("Luminocity outside right now is ");
      Serial.print (luminocity_outside);
      Serial.println(" lux");
      Serial.print("Luminocity inside right now is ");
      Serial.print (luminocity_inside);
      Serial.println(" lux"); 
      // Update lighting or shades
      if (luminocity_inside < 250) {
          if (luminocity_outside > 250) {
              digitalWrite(SHADES_OPEN, HIGH);
              digitalWrite(SHADES_CLOSE, LOW);
              //Open window shades
              //Also, let's save the environment, turn the lights off
              digitalWrite(LIGHTS_BACK, LOW);
              digitalWrite(LIGHTS_FRONT, LOW);
          } else {
              digitalWrite(LIGHTS_BACK, HIGH);
              digitalWrite(LIGHTS_FRONT, HIGH);
              // Turn on both amphitheatre lighting circuits
          }
      }
    }
    
    // Check temperature sensor
    //temperature has to be measured every 2 secs, aka 4 quanta 
    if (quantums % 4 == 0){

      int chk = DHT.read11(INSIDE_TEMP_SENSOR);
      temperature_inside = DHT.temperature;
      
      chk = DHT.read11(OUTSIDE_TEMP_SENSOR);
      temperature_outside = DHT.temperature;

      Serial.print("Temperature outside right now is ");
      Serial.print (temperature_outside);
      Serial.println(" degrees Celsius");
      Serial.print("Temperature inside right now is ");
      Serial.print (temperature_inside);
      Serial.println(" degrees Celsius");
    }


    // Update air conditioning
    // If temp inside is not within 25 +- 2 (and there are persons inside)
    if (temperature_inside > OPTIMAL_TEMP + TEMP_EPSILON ||
        temperature_inside < OPTIMAL_TEMP - TEMP_EPSILON ) {
        // If temp outside *is* within 25 +- 2 then open windows (to save energy)
        if (temperature_outside < OPTIMAL_TEMP + TEMP_EPSILON &&
            temperature_outside > OPTIMAL_TEMP - TEMP_EPSILON) {
            // Open windows 
            digitalWrite(WINDOWS_OPEN, HIGH);
            digitalWrite(WINDOWS_CLOSE, LOW);
            //also, close the AC, we dont wanna waste any more energy
            digitalWrite(AC, LOW);

        } else {
            //Turn on air conditioning 
            digitalWrite(AC, HIGH);
            //Dont leave windows while having the AC on 
            digitalWrite(WINDOWS_OPEN, LOW);
            digitalWrite(WINDOWS_CLOSE, HIGH);
                       
        }
    }


    loudness = analogRead(LOUDNESS_SENSOR);
    Serial.print("Loudness inside the room is ");
    Serial.println(loudness);
    
    if (loudness > 255) {
        analogWrite(MICROPHONE, microphone_value + (loudness-255) *155/(1024-255)); 
        
        //Analog output
    }


    }

    quantums++;
    delay(TIME_QUANTUM);
}
