#include<arduino.h>
#include <Servo.h>
Servo turnservo;

#define Rwheel_en 3
#define Rwheel_m1 27
#define Rwheel_m2 29

#define Backwheel_en 2
#define backwheel_m1 25 
#define backwheel_m2 23

#define turn_servo_pin 7
void motorA_direction(int);
void motorB_direction(int);
void motor_signal_A(int, int);
void motor_signal_B(int, int);

void run_backwheel(){
if (backwheel > 0)
{
    analogWrite(Backwheel_en,abs(backwheel));
    digitalWrite(backwheel_m1, HIGH);
    digitalWrite(backwheel_m2, LOW);
}
else if (backwheel < 0)
{
    analogWrite(Backwheel_en,abs(backwheel));
    digitalWrite(backwheel_m1, LOW);
    digitalWrite(backwheel_m2, HIGH);
}
else
{
    analogWrite(Backwheel_en,0);
    digitalWrite(backwheel_m1, LOW);
    digitalWrite(backwheel_m2, LOW);
}
}

void run_front_servo()
{
turnservo.attach (turn_servo_pin);
turnservo.write(turnwheel);
}

void run_Rwheel(float torque)
{
torque = constrain (torque, -255, 255 );
if (torque > 0)
{
    analogWrite(Rwheel_en,abs(torque));
    digitalWrite(Rwheel_m1, HIGH);
    digitalWrite(Rwheel_m2, LOW);
}
else if (torque < 0)
{
    analogWrite(Rwheel_en,abs(torque));
    digitalWrite(Rwheel_m1, LOW);
    digitalWrite(Rwheel_m2, HIGH);
}
else
{
    analogWrite(Rwheel_en,0);
    digitalWrite(Rwheel_m1, LOW);
    digitalWrite(Rwheel_m2, LOW);
}
}

