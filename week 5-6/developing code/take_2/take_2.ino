#include "remote_x.h"
#include "motor_run.h"
#include "mpu_header.h"
#include "testing_print.h"
//######################################################---------------------------------------------declarations
float set_point = -3.32;
float error = 0, error_d = 0,  error_i = 0 , prev_error = 0;
float Rmotor_torque ;
int grid = 10;
void integrate_error();

//######################################################---------------------------------------------SETUP

void setup() {
  Serial1.begin(115200);
  Serial.begin(250000);
  Wire.begin();
  Wire.setClock(400000UL);
  pinMode(green,OUTPUT);      
  pinMode(blue,OUTPUT);  
  Adjust_rmt_Data();
  digitalWrite(green,HIGH);
  resetMPU();
  timer = micros();
}
//######################################################-------------------------------------------LOOP

void loop() {
  timer = micros();
  update_gains (); // it increment and decrements gain as per commanded (in testing_print.h)
  
  //######################################################---------------------------------------------PID controller here

  read_remote();                              //it read remote from serial port 1 (in remote_x.h)
  run_backwheel();                            //it commands back wheel motor according to remote (in motor_run.h)
  run_front_servo();                          //it commands turning servo motor according to remote (in motor_run.h)

  float bot_inclination = dataFusion(0.98); //it communicates with mpu derives data for gyro and accel and fuses them together
                                            //returns filterd inclination of bot
                                            //argument to be pass = filter coefficient of complementary filter default is 0.97
  //######################################################---------------------------------------------PID controller here

  error = set_point - bot_inclination;      //it derives the output error w.r.t. set point for PID controller
  integrate_error();                        //it integrates arror and constrains it within the renge of motor saturation (anti winding)
  error_d = (error - prev_error) / Ts;      //it differentiats error 
  prev_error = error;                       // keeping trace back of previous error 

  Rmotor_torque = (Kp * error) + (Ki * error_i) + (Kd * error_d);     // PID algorithm 
  run_Rwheel(Rmotor_torque);                                          //it runs reaction wheel motor according to PID calculation
                                                                      // pass the argument of PID calculation 

  //######################################################
  //print_data(pitch, error,5);                //the function is to monitor parameters just pass the perameter as argument (in testing_print.h)(max 5 argument is allowed)
  //######################################################
  
  //the while loop burns excessive time to set uniform sampling for defined in Ts varieble
  while (micros() - timer < Ts * 1000000); ////minimum time gap remains to burn is 1.2 ms so we can afford Fs = 200 Hz ~ Ts = 5 ms is optimum
  timer = micros();
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
