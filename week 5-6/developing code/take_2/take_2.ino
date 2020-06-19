#include "remote_x.h"
#include "motor_run.h"
#include "mpu_header.h"
//######################################################---------------------------------------------declarations

float Kp = -180, Ki = -0.1, Kd = -0.6;
//float Kp = -27014.7125538481, Ki = -21183.1159387893, Kd = -5137.85556983136;-6.98870101025249
//float Kp = -27000, Ki = 0, Kd = -5137.85556983136;
//######################################################---------------------------------------------declarations

float set_point = 2.16;
float error= 0, error_d = 0,  error_i = 0 , prev_error = 0;
float Rmotor_torque ;
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
//timer1 = micros();
read_remote();
run_backwheel();
run_front_servo();
//Serial.println((micros()- timer1));

float bot_inclination = dataFusion();
//Serial.println((micros()- timer1));
//######################################################---------------------------------------------PID controler here

error = set_point - bot_inclination;
error_i = error_i + error * Ts;
error_d = (error - prev_error)/Ts;
prev_error = error;

Rmotor_torque = (Kp * error) + (Ki * error_i)+ (Kd* error_d);
run_Rwheel(Rmotor_torque);

//######################################################
//Serial.print(" ------- ");
//Serial.print(accelYangle);
//Serial.print(" ------- ");
//Serial.print(gyroYang);
//Serial.print(" ------- ");
//Serial.print(GyroY);
//Serial.print(" ------- ");
//Serial.print(AccelX);
//Serial.print(" ------- ");
//Serial.print(AccelZ);
Serial.print(" ------- ");
Serial.println(pitch);
//######################################################

while(micros() - timer < Ts * 1000000); ////minimum time gap remains to burn is 1.2 ms so we can afford Fs = 200 Hz ~ Ts = 5 ms is optimum
timer = micros();
}
