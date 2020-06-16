#include "remote_x.h"
#include "motor_run.h"
#include "mpu_header.h"
//######################################################---------------------------------------------declarations

float Kp = -600, Ki = 0, Kd = 0;
float set_point = 2.32;
float error= 0, error_d = 0,  error_i = 0 , prev_error = 0;
float Rmotor_torque ;
uint32_t      timer;
//######################################################---------------------------------------------SETUP

void setup() {
Serial1.begin(115200);
Serial.begin(250000);
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
//Serial.print(" ------- ");
//Serial.print(pitch1);
//Serial.print(" ------- ");
//Serial.print(pitch2);
//Serial.print(" ------- ");
//Serial.print((Kd* error_d));
//Serial.print(" ------- ");
//Serial.print((Ki * error_i));
//Serial.print(" ------- ");
//Serial.print((Kp * error));
Serial.print(" ------- ");
Serial.println(pitch);
//######################################################

while(micros() - timer < Ts * 1000000); ////minimum time gap remains to burn is 1.2 ms so we can afford Fs = 200 Hz ~ Ts = 5 ms is optimum
timer = micros();
}
