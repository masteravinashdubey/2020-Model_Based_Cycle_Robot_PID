#include <Wire.h>
#define MPUaddr0       <define registernumber>    //write addr used if pin AD0 is set to 0 (left unconnected)
#define pwr_mgnt       <define registernumber>    //write address of register to reset mpu to default 
#define set_gyro_sence <define registernumber>    //write the address of the register to set gyro sensitivity
#define set_accl_sence <define registernumber>    //write the address of the register to set accelerometer sensitivity
#define gyro_data_reg  <define registernumber>    //write the address of the first data register of the gyro sensor data
#define accl_data_reg  <define registernumber>    //write the address of the first data register of the accl sensor data
#define sample_rate    <define registernumber>    //write the address of the register to be used to set sample rate
float AccelY;
float AccelZ;
float degree_gyro = 0;
int Ts = 0.005;


// set up your gyro at the time of initiation
void setup() {
reset_mpu();                                          // reset mpu
set_gyroSensitivity(<pass apropriete argument>);      // set gyro to +/- 1000 dps
set_accelSensitivity(<pass apropriete argument>);     //set accl to +/- 2g
set_sample_rate(500);                                 // set the sample rate for sensors to 500 hz
Serial.begin(9600);                                   //initiating Serial communication 
}


//in this section call the functions which to be used every cycle;
void loop() {
 
 read_accel(16384);                       
 float x_ang_velo = read_gyro (131);
 degree_gyro = gyro_angle (x_ang_velo);
 float degree_accl = accl_angle (AccelY,AccelZ);

 //write the code to disply on serial monitor
 //---------------here-----------------
 //--------------------------------
 delay (5);
}

void reset_mpu(void){
//--------------------------------------------------- 
//write a code to reset mpu by power manegement code
//------------------here----------------------------
//---------------------------------------------------
  delay(100);
}
void set_gyroSensitivity(uint8_t dps){
  //Config FS_SEL[1:0] bits 4 and 3 in register 0x1B
  //0x00: +/-250 dps (default)
  //0x08: +/-500 dps
  //0x10: +/-1000 dps
  //0x18: +/-2000 dps
  
  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
//--------------------------------------------------- 
//write a code to set apropreate sencitivity of gyroscope
//here argument dps contains the data/ factor to be written to appropreate register
//------------------here----------------------------
//---------------------------------------------------
  Wire.endTransmission();             //end comm
}
void set_accelSensitivity(uint8_t g){
  //Config AFS_SEL[1:0] bits 4 and 3 in register 0x1C
  //0x00: +/-2g (default)
  //0x08: +/-4g
  //0x10: +/-8g
  //0x18: +/-16g

  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
//--------------------------------------------------- 
//write a code to set apropreate sencitivity of accelerometer
//here argument 'g' contains the data/ factor to be written to appropreate register
//------------------here----------------------------
//---------------------------------------------------
  Wire.endTransmission();             //end comm
}
void set_sample_rate (uint8_t rate)
{
  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
//--------------------------------------------------- 
//write a code to set apropreate semple rate for all sensors on mpu6050 
//here argument 'rate' contains the sample rate to be set
//------------------here----------------------------
//---------------------------------------------------
  Wire.endTransmission();
}
void read_accel(float accelDivisor){
  //+/-2g, use divisor of 16384
  //+/-4g, use divisor of 8192
  //+/-8g, use divisor of 4096
  //+/-16g, use divisor of 2048
  
  Wire.beginTransmission(MPUaddr0);
  Wire.write(accl_data_reg);
  Wire.endTransmission();
  Wire.requestFrom(MPUaddr0, 4);    //read 4 consecutive registers starting form y axis data
  if (Wire.available() >= 4){
    //--------------------------------------------------- 
    //write a code to combine 2 byte data in one veriable of y and z axis
    //here argument 'accelDivisor' contains the data/ factor to be devided from raw data to covert in to factor of 'g'(grevitational )
    //you have to store the resultant data of y axis and z axia to AccelY and AccelZ
    //------------------here----------------------------
    //---------------------------------------------------    
  }
}
float read_gyro(float gyroDivisor){
  
  Wire.beginTransmission(MPUaddr0);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPUaddr0, 2);    //read 2 consecutive registers starting at 0x43(x axis data)
  if (Wire.available() >= 6){
    //--------------------------------------------------- 
    //write a code to combine 2 byte data in one veriable of GyroX
    //here argument 'gyroDivisor' contains the data/ factor to be devided from raw data to covert in to degree/second
    //you have to store the resultant data of x axis to GyroX
    //------------------here----------------------------
    //---------------------------------------------------  
    return(GyroX); 
  }
}

float gyro_angle (float ang_velo)
{
    //--------------------------------------------------- 
    //write a logic to derive angle astimation from gyro angular velocity data
    //here argument 'ang_velo' contains the data of angular velocity in degree/second
    //you have to store the resultant data of rotation around  x axis and return it
    //------------------here----------------------------
    //---------------------------------------------------
}
float accl_angle (float y_data, float z_data)
{

    //--------------------------------------------------- 
    //write a logic to derive angle astimation from accelerometer data of y and z axis
    //here argument 'y_data' and 'z_data' contains the data of grevitational force on respective axis
    //you have to store the resultant data of rotation around x axis and return it
    //------------------here----------------------------
    //---------------------------------------------------
}
