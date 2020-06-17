#include "remote_x.h"
#include "motor_run.h"
#include "mpu_header.h"
//######################################################---------------------------------------------declarations

float Kp = -120.80820362847, Ki = 0, Kd = 0;
//float Kp = -27014.7125538481, Ki = -21183.1159387893, Kd = -5137.85556983136;-6.98870101025249
//float Kp = -27000, Ki = 0, Kd = -5137.85556983136;
//######################################################---------------------------------------------declarations

float set_point = 2.32;
float error= 0, error_d = 0,  error_i = 0 , prev_error = 0;
float Rmotor_torque ;
float bot_inclination;
void PID_controle();
//######################################################---------------------------------------------SETUP

void setup() {
Serial1.begin(115200);
Serial.begin(250000);
Wire.begin();
Wire.setClock(400000UL);
              //to enable all interrupts 

Adjust_rmt_Data();
resetMPU();
cli();               //to reinitialize all interupts 
TCCR1A = B00000000;  //clear whole register  
TCCR1B = B00001101;  //setting up prescaler to 1024 and ctc mode too

OCR1AH = 0x01;       //high byte of compare match register
OCR1AL = 0x39;       //low byte of compare match register//interupt after 20ms

OCR1BH = 0x01;       //high byte of compare match register
OCR1BL = 0x19;       //low byte of compare match register//interrupt after 19ms

OCR1CH = 0x01;       //high byte of compare match register
OCR1CL = 0x09;       //low byte of compare match register//intrrupt after 18ms

TIMSK1 = B00001110;  //enabling timer interrupt on compare match register A 
sei(); 
}

//######################################################-------------------------------------------LOOP
void loop() {
read_remote();
run_backwheel();
run_front_servo();
//######################################################
//Serial.print(" ------- ");
//Serial.print(accelYangle);
//Serial.print(" ------- ");
//Serial.print(gyroYang);
//Serial.print(" ------- ");
//Serial.print((Kd* error_d));
//Serial.print(" ------- ");
//Serial.print((Ki * error_i));
//Serial.print(" ------- ");
//Serial.print((Kp * error));
Serial.print(" ------- ");
Serial.println(pitch);
//######################################################
}

ISR(TIMER1_COMPA_vect)     //writing ISR with vector for timer 1A  // run pid calculation after 20ms it willl take 200 micro sec time         
{
  PID_controle();
  }
ISR(TIMER1_COMPB_vect)     //writing ISR with vector for timer 1B  //run data fusion after 19 ms it will take 400 micro sec time            
{ 
  bot_inclination = dataFusion();
  }
ISR(TIMER1_COMPC_vect)     //writing ISR with vector for timer 1C  // grabing data at 18 ms it takes around 700 micro sec          
{
  readAccel(16384.0);                                                                         //read XYZ Accel data from registers 0x3B to 0x40 
  readGyro(32.75); 
  }

//######################################################---------------------------------------------PID controler here
void PID_controle()
{
error = set_point - bot_inclination;
error_i = error_i + error * Ts;
error_d = (error - prev_error)/Ts;
prev_error = error;

Rmotor_torque = (Kp * error) + (Ki * error_i)+ (Kd* error_d);
run_Rwheel(Rmotor_torque);
}

