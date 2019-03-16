int TrigPin = 12;
int EchoPin = 11;
int duration;
float distance;

void setup() {
    Serial.begin(9600);
    pinMode(TrigPin,OUTPUT);
    pinMode(EchoPin, INPUT);
}

void loop() {
    digitalWrite(TrigPin,LOW);
    delayMicroseconds(2);   // Ensure Trig pin is low

    digitalWrite(TrigPin,HIGH);
    delayMicroseconds(10);  // 10us pulse sent to Trig

    digitalWrite(TrigPin,LOW);

    duration = pulseIn(EchoPin,HIGH);
    distance = duration /58.82;

    Serial.print(distance);
    Serial.println(" cm");
    
    delay(1000);            //refresh each second
}