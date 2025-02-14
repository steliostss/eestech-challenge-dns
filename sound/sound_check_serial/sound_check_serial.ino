// Declaration and initialization of the input pin
int Analog_Eingang = A0; // X-axis-signal
int Digital_Eingang = 3; // Button
  
void setup ()
{
  pinMode (Analog_Eingang, INPUT);
  pinMode (Digital_Eingang, INPUT);
       
  Serial.begin (9600); // Serial output with 9600 bps
}
  
// The program reads the current value of the input pins
// and outputs it via serial out
void loop ()
{
  float Analog;
  int Digital;
    
  // Current value will be read and converted to voltage
  Analog = analogRead (Analog_Eingang); 
  Digital = digitalRead (Digital_Eingang);
    
  //... and outputted here
  Serial.print ("Analog voltage value: "); Serial.print (Analog, 4);  
  Serial.print (" Extreme value: ");
  
  if(Digital==1)
  {
      Serial.println (" reached");
  }
  else
  {
      Serial.println (" not reached yet");
  }
  Serial.println ("----------------------------------------------------------------");
  delay (500);
}

