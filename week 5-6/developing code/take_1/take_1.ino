#include "remote_x.h"
#include "motor_run.h"
#include "mpu_header.h"
//######################################################---------------------------------------------declarations

float Kp = -100, Ki = 0, Kd = 0;
float set_point = 5.5;
float error= 0, error_d = 0,  error_i = 0 , prev_error = 0;
float Rmotor_torque ;
uint32_t      timer;
//######################################################---------------------------------------------SETUP

void setup() {
Serial1.begin(115200);
Serial.begin(9600);
Wire.begin();
Wire.setClock(400000UL);
Adjust_rmt_Data();
resetMPU();

timer = micros();
}
//######################################################-------------------------------------------LOOP

void loop() {

read_remote();
run_backwheel();
run_front_servo();
float bot_inclination = dataFusion();

//######################################################---------------------------------------------PID controler here

error = set_point - bot_inclination;
error_i = error_i + error * Ts;
error_d = (error - prev_error)/Ts;
prev_error = error;

Rmotor_torque = (Kp * error) + (Ki * error_i)+ (Kd* error_d);
run_Rwheel(Rmotor_torque);

//######################################################
Serial.print(" ------- ");
Serial.print(255);
Serial.print(" ------- ");
Serial.print(bot_inclination);
Serial.print(" ------- ");
Serial.print(error);
Serial.print(" ------- ");
Serial.println(Rmotor_torque);
while(micros() - timer < Ts * 1000000); 
timer = micros();
}
