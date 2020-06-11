#include<Wire.h>
int i=0;
 long previous_time=0,current_time=0,duration, accel_rawX,accel_rawY,accel_rawZ,accel_refX,accel_refY,accel_refZ,sumX1,sumY1,sumZ1;
long gyro_rawX,gyro_rawY,gyro_rawZ,gyro_refX,gyro_refY,gyro_refZ,sumX,sumY,sumZ;
float  inst_angle_x, inst_angle_y, inst_angle_z, gyro_angle_x=0.00, gyro_angle_y=0.00, gyro_angle_z=0.00;
float Gx,Gy,Gz,roll,pitch,yaw;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();
setupMPU();
average1();
average();
}

void loop() {
    readAccelrawdata();
  // plotAccelrawdata();
   gForce(); 
  previous_time=current_time;
    current_time=millis();
 
  duration=current_time-previous_time;
 readGyrorawdata();
  gForce1(); 
  complimentary();
// plotGyrorawdata();
 
  
 delay(50);
}


void setupMPU(){
  //power management tasks
  Wire.beginTransmission(0b1101000);//slave device address sequence
  Wire.write(0x6B);//internal register address sequence
  Wire.write(0b00000000);//data transfer bit sequence
  Wire.endTransmission();
  //configer gyroscope
  Wire.beginTransmission(0b1101000);//slave device address sequence
  Wire.write(0x1B);//internal register address sequence
  Wire.write(0x00001000);//data transfer bit sequence
  Wire.endTransmission();
   //configer accelerometer
  Wire.beginTransmission(0b1101000);//slave device address sequence
  Wire.write(0x1C);//internal register address sequence
  Wire.write(0b00000000);//data transfer bit sequence
  Wire.endTransmission();
}
void readAccelrawdata(){
  Wire.beginTransmission(0b1101000);//slave device address sequence
  Wire.write(0x3B);//internal register address sequence
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);//request Accel registers(3B-40)
  while(Wire.available()<6);
  accel_rawX=Wire.read()<<8|Wire.read();
  accel_rawY=Wire.read()<<8|Wire.read(); 
  accel_rawZ=Wire.read()<<8|Wire.read(); 
}
void average(){
  for(i=0;i<=1000;i++){
     readAccelrawdata();
   sumX1 += accel_rawX;
   sumY1 += accel_rawY;
   sumZ1 += accel_rawZ;  
  }
  accel_refX=sumX1/1000;
  accel_refY=sumY1/1000;
  accel_refZ=sumZ1/1000;
}

void gForce(){
  Gx=(accel_rawX-accel_refX)/(16384.00);
  Gy=(accel_rawY-accel_refY)/(16384.00);
  Gz=(accel_rawZ-accel_refZ)/(16384.00)+1;
roll=atan2(Gy,Gz);
pitch=atan2(-Gx,sqrt(pow(Gy,2)+pow(Gz,2)));
roll=roll*57;
pitch=pitch*57;
}
/*void plotAccelrawdata(){
  Serial.print(Gx);//blue on plotter
  Serial.print(",");//red on plotter
   Serial.print(roll);//green on plotter
  Serial.print(",");
   Serial.println(pitch);
}*/
void readGyrorawdata(){
  Wire.beginTransmission(0b1101000);//slave device address sequence
  Wire.write(0x43);//internal register address sequence
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);//request Accel registers(3B-40)
  while(Wire.available()<6);
  gyro_rawX=Wire.read()<<8|Wire.read();
  gyro_rawY=Wire.read()<<8|Wire.read(); 
  gyro_rawZ=Wire.read()<<8|Wire.read(); 
}


void gForce1(){
  inst_angle_x=((gyro_rawX-gyro_refX)/(65.50))/1000;
  inst_angle_y=((gyro_rawY-gyro_refY)/(65.50))/1000;
  inst_angle_z=((gyro_rawZ-gyro_refZ)/(65.50))/1000;
   inst_angle_x= inst_angle_x*duration;
    inst_angle_y= inst_angle_y*duration;
     inst_angle_z= inst_angle_z*duration;
      gyro_angle_x= gyro_angle_x+inst_angle_x;
       gyro_angle_y= gyro_angle_y+inst_angle_y; 
       gyro_angle_z= gyro_angle_z+inst_angle_z;
}
void average1(){
  for(i=0;i<=1000;i++){
     readGyrorawdata();
   sumX += gyro_rawX;
   sumY += gyro_rawY;
   sumZ += gyro_rawZ;  
  }
  gyro_refX=sumX/1000;
  gyro_refY=sumY/1000;
  gyro_refZ=sumZ/1000;
}
/*void plotGyrorawdata(){
  Serial.print(gyro_angle_x);//blue on plotter
  Serial.print(",");//red on plotter
   Serial.print(gyro_angle_x);//green on plotter
  Serial.print(",");
   Serial.println(gyro_angle_z);
    Serial.println(duration);
     
}*/
void complimentary(){
  roll=(0.96*gyro_angle_x + 0.04*roll);
  pitch=(0.96*gyro_angle_y +0.04*pitch);
  yaw=gyro_angle_z;
   Serial.print(roll);//blue on plotter
  Serial.print(",");//red on plotter
   Serial.print(pitch);//green on plotter
  Serial.print(",");
   Serial.println(yaw);
}
