#include "remote_x.h"
#include "motor_run.h"
#include "mpu_header.h"
#include "testing_print.h"
//------------#######################################----------------------------------------------# 
//            ||  chose your Controller   || ------- uncomment line 1 to implement PID controller  #
//            ||      from HERE           || ------- uncomment line 2 to implement LQR controller  #            
//------------#######################################----------------------------------------------#
#define choose_PID    // Line 1
//#define choose_LQR    // Line 2



//######################################################---------------------------------------------declarations
float set_point = -3.32;
float error= 0, error_d = 0,  error_i = 0 , prev_error = 0,W_Rwheel;
float Rmotor_torque ;
float bot_inclination;
volatile long counter = 0,prev_counter=0;
void PID_controle();
void integrate_error();
//######################################################-----------------------------------------LQR gains
float K1 = 0, K2 = 0,K3 = 0;
//######################################################---------------------------------------------SETUP

void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000UL);
  pinMode(green,OUTPUT);      
  pinMode(blue,OUTPUT);  
  pinMode(M_enc_1,INPUT);
  pinMode(M_enc_2,INPUT);
  attachInterrupt(digitalPinToInterrupt(M_enc_1), rotary, RISING);
  Adjust_rmt_Data();
  digitalWrite(green,HIGH);
  resetMPU();
  Set_up_timers();
}
//######################################################-------------------------------------------LOOP
void loop() {
  //update_gains ();
  read_remote();
  run_backwheel();
  run_front_servo();
  print_data(counter,bot_inclination);
}

//######################################################----------------------here is the ISRs of interrupt sequences are defined 
ISR(TIMER1_COMPA_vect)     //writing ISR with vector for timer 1A  // run pid calculation after 10ms it willl take 200 micro sec time         
{ 
  #ifdef choose_PID
  PID_controle();
  #endif
  
  #ifdef choose_LQR
  LQR_controle();
  #endif
}
 
ISR(TIMER1_COMPB_vect)     //writing ISR with vector for timer 1B  //run data fusion after 7 ms it will take 400 micro sec time            
{ bot_inclination = dataFusion(0.98);  }    //returns filterd inclination of bot                                      
  
ISR(TIMER1_COMPC_vect)     //writing ISR with vector for timer 1C  // grabing data at 4ms it takes around 700 micro sec          
{ sei();                   //after entring in an ISR globle interrupts gets disabled 
  readAccel(16384.0);      //read XYZ Accel data from registers 0x3B to 0x40 
  readGyro(32.75);         //reading gyro data 
  }
//############################################################
//function name:      rotary()
//passing arguments:  NONE  
//return :            NONE 
//description:                      
//############################################################
void rotary() {
if(digitalRead(M_enc_2))
counter--;
else
counter++;
}
//######################################################---------------------------------------------LQR controler here
// for LQr controller o/p states will be inclination and angular velocity of cycle bot and angular velocity of reation wheel
void LQR_controle()
{ 
  W_Rwheel = (counter - prev_counter)/Ts;
  prev_counter = counter; 
  Rmotor_torque = (K1 * (set_point - bot_inclination)*(3.14/360)) + (K2 * GyroY ) + (K3 * (W_Rwheel)*(3.14/270));     // LQR algorithm 
  //-----------------------------------------------------|^|--converting in redian----------------------|^|---converting encoder tics/sec in redian/secon
  run_Rwheel(Rmotor_torque);  
}

//######################################################---------------------------------------------PID controler here
void PID_controle()
{
  error = set_point - bot_inclination;      //it derives the output error w.r.t. set point for PID controller
  integrate_error();                        //it integrates arror and constrains it within the renge of motor saturation (anti winding)
  error_d = (error - prev_error) / Ts;      //it differentiats error 
  prev_error = error;                       // keeping trace back of previous error 

  Rmotor_torque = (Kp * error) + (Ki * error_i) + (Kd * error_d);     // PID algorithm 
  run_Rwheel(Rmotor_torque); 
}

//############################################################
//function name:      integrate_error()
//passing arguments:  NONE
//return :            NONE
//discription:        it integrates 'error' and also assures that integration values stays between -255 to 255 
//                    it prevents to windup integrator towards infinity for constant error because of machine saturation 
//############################################################

void integrate_error()
{
  if ((Ki * error_i <255 & Ki*error_i > -255)) error_i = error_i + error * Ts;             // integrate normally if "Ki*error_i" value is within the range of saturation(-255 to 255)
  
  else if (Ki * error_i > 255)                 error_i = float(int(255 / Ki));            // if "Ki*error_i" value is exeding 255 just turn of integrator
  
  else if ( Ki * error_i < -255)               error_i = float(int(-255 / Ki));          //if "Ki*error_i" value becoming lower than -255 just turn of integrator 
}

//############################################################
//function name:      Set_up_timers()
//passing arguments:  NONE  
//return :            NONE 
//description:        in configurs timers and also mode of oprating timer and generating interrupt 
//                    here we are using timer 1 and its 3 compare match registers and when match with OCR1A occurs its resets timer1 and generates interrupt
//                    when timer starts OCR1C register gentrates interrupt at 4ms; OCR1B at 7ms and OCR1A at 10ms and thus uniform sampling gets assureds              
//############################################################
void Set_up_timers()
{
    cli();               //to reinitialize all interupts 
    TCCR1A = B00000000;  //clear whole register  
    TCCR1B = B00001101;  //setting up prescaler to 1024 and ctc mode too
    
    OCR1AH = 0x00;       //high byte of compare match register
    OCR1AL = 0x3F;       //low byte of compare match register//interupt after 10ms = 0x9D
    
    OCR1BH = 0x00;       //high byte of compare match register
    OCR1BL = 0x2F;//0x6F;       //low byte of compare match register//interrupt after 7ms = 0X6F
    
    OCR1CH = 0x00;       //high byte of compare match register
    OCR1CL = 0x20;       //low byte of compare match register//intrrupt after 4ms  = 0X3F
    
    TIMSK1 = B00001110;  //enabling timer interrupt on compare match register A 
    sei();   
}
