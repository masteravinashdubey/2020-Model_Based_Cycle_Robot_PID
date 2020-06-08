#define ledPin 13
int i = 1;
void setup() {
cli();               //to reinitialize all interupts 
TCCR1A = B00000000;  //clear whole register  
TCCR1B = B00001101;  //setting up prescaler to 1024
OCR1AH = 0x00;       //high byte of compare match register
OCR1AL = 0xFE;       //low byte of compare match register
TIMSK1 = B00000010;  //enabling timer interrupt on compare match register A 
sei();               //to enable all interrupts 

pinMode(13,OUTPUT);
}
ISR(TIMER1_COMPA_vect)     //writing ISR with vector for timer 1             
{
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
