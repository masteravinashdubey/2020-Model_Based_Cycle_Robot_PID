#define ledPin 13
int i = 1;
void setup() {
cli();               //to reinitialize all interupts 
//-----------------------------------------------------------
//--------------------------------------------------------write your register---
 //setting up prescaler to 1024
 //high byte of compare match register and
 //low byte of compare match register according to requirement
 //enabling timer interrupt on compare match register A 
//-----------------------------------------------------------
//-----------------------------------------------------------


sei();               //to enable all interrupts 

pinMode(13,OUTPUT);  //initializing built in LED pin as output
}
ISR("<write appropreate vector address here>")     //writing ISR with vector for timer 1             
{
  //writing logic to toggal LED...
  if (i == 1)
  {
  digitalWrite(ledPin, HIGH);  
  i = 2;
  }
  else {
   digitalWrite(ledPin, LOW);
   i = 1; 
  }

}
void loop() {

}
