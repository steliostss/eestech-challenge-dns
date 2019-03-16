// Declaration and initialization of the input pin
int Analog_Eingang = A0;
int Digital_Eingang = 3;
  
void setup ()
{
  pinMode (Analog_Eingang, INPUT);
  pinMode (Digital_Eingang, INPUT);
       
  Serial.begin (9600);
}
  
// The program reads the current value of the input pins
// and outputs it via serial out 
void loop ()
{
  float Analog;
  int Digital;
    
  // Current value will be read and converted to the voltage
  Analog = analogRead (Analog_Eingang) * (5.0 / 1023.0); 
  Digital = digitalRead (Digital_Eingang);
    
  // and outputted here
  Serial.print ("Analog voltage value:"); Serial.print (Analog, 4);  Serial.print ("V, ");
  Serial.print ("Extreme value:");
  
  if(Digital==1)
  {
      Serial.println (" reached");
  }
  else
  {
      Serial.println (" not reached yet");
  }
  Serial.println ("----------------------------------------------------------------");
  delay (200);
}
