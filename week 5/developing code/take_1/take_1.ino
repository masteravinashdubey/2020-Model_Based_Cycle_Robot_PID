#include "remote_x.h"
#include "motor_run.h"
 



void setup() {
 

Serial1.begin(115200);
Serial.begin(9600);
}

void loop() {

read_remote();
run_backwheel();
run_front_servo();

}
