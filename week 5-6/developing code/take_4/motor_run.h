#include <ServoTimer2.h>
#include<arduino.h>

ServoTimer2 turnservo;

#define Rwheel_en 3
#define Rwheel_m1 27
#define Rwheel_m2 29

#define Backwheel_en 2
#define backwheel_m1 25 
#define backwheel_m2 23

#define turn_servo_pin 6
void motorA_direction(int);
void motorB_direction(int);
void motor_signal_A(int, int);
void motor_signal_B(int, int);


//############################################################
//function name:      run_backwheel()
//passing arguments:  NONE
//return :            NONE
//discription:        it  runs backwheel motor according to commands received from remote
//                     
//############################################################
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
//############################################################
//function name:      run_front_servo()
//passing arguments:  NONE
//return :            NONE
//discription:        it  sets servomotor angle according to commands received from remote                
//############################################################
void run_front_servo()
{
turnservo.attach (turn_servo_pin);
turnservo.write(map(turnwheel, 0,180,750, 2250));
}

//############################################################
//function name:      run_Rwheel()
//passing arguments:  torque to be set and calculated from PID
//return :            NONE
//discription:        it  sets servomotor angle according to commands received from remote                
//############################################################
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

