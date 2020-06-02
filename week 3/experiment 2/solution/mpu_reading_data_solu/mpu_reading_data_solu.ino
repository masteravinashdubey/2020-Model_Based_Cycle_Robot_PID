#include <Wire.h>
#define MPUaddr0 0x68    //write addr used if pin AD0 is set to 0 (left unconnected)
#define pwr_mgnt 0x6B     // write address of register to reset mpu to default 
#define set_gyro_sence 0x1B //write the address of the register to set gyro sensitivity
#define set_accl_sence 0x1c //write the address of the register to set accelerometer sensitivity
#define gyro_data_reg 0x43  // write the address of the first data register of the gyro sensor data
#define accl_data_reg 0x3D  // write the address of the first data register of the accl sensor data
#define sample_rate 0x19    //write the address of the register to be used to set sample rate
float AccelY;
float AccelZ;
float degree_gyro = 0;
int Ts = 0.005;

void setup() {
reset_mpu(); // reset mpu
set_gyroSensitivity(0x10); // set gyro to +/- 1000 dps
set_accelSensitivity(0x00);//set accl to +/- 2g
set_sample_rate(500);// set the sample rate for sensors to 500 hz
Serial.begin(9600);
}

void loop() {
 
 read_accel(16384);
 float x_ang_velo = read_gyro (131);
  degree_gyro = gyro_angle (x_ang_velo);
 float degree_accl = accl_angle (AccelY,AccelZ);
 Serial.print("degree_gyro: ");
 Serial.print(degree_gyro);
 Serial.print("degree_accl: ");
 Serial.print(degree_accl);
 delay (5);
}

void reset_mpu(void){
  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(pwr_mgnt);               //write to register 0x6B
  Wire.write(0x00);                   //reset all internal registers to default values
  Wire.endTransmission();             //end comm
  delay(100);
}
void set_gyroSensitivity(uint8_t dps){
  //Config FS_SEL[1:0] bits 4 and 3 in register 0x1B
  //0x00: +/-250 dps (default)
  //0x08: +/-500 dps
  //0x10: +/-1000 dps
  //0x18: +/-2000 dps
  
  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(set_gyro_sence);                   //write to register 0x1B
  Wire.write(dps);                    //setting bit 7 to 1 resets all internal registers to default values
  Wire.endTransmission();             //end comm
}
void set_accelSensitivity(uint8_t g){
  //Config AFS_SEL[1:0] bits 4 and 3 in register 0x1C
  //0x00: +/-2g (default)
  //0x08: +/-4g
  //0x10: +/-8g
  //0x18: +/-16g

  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(set_accl_sence);                   //write to register 0x1C
  Wire.write(g);                      //setting bit 7 to 1 resets all internal registers to default values
  Wire.endTransmission();             //end comm
}
void set_sample_rate (uint8_t rate)
{
  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(sample_rate);            //write to register sample_rate
  Wire.write((int)8000/rate);         //setting divisor of sampling rate
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

    int16_t temp0 = Wire.read() << 8;           //read upper byte of Y
    int16_t temp1 = Wire.read();                //read lower byte of Y
    AccelY = (float) (temp0 | temp1);
    AccelY = AccelY / accelDivisor;
   
    temp0 = Wire.read() << 8;           //read upper byte of Z
    temp1 = Wire.read();                //read lower byte of Z
    AccelZ = (float) (temp0 | temp1);
    AccelZ = AccelZ / accelDivisor;     
  }
}
float read_gyro(float gyroDivisor){
  
  Wire.beginTransmission(MPUaddr0);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPUaddr0, 2);    //read 2 consecutive registers starting at 0x43(x axis data)
  if (Wire.available() >= 6){
    
    int16_t temp0 = Wire.read() << 8;   //read upper byte of X
    int16_t temp1 = Wire.read();        //read lower byte of X
    float GyroX = (float) (temp0 | temp1);
    GyroX = GyroX / gyroDivisor ; 
    return(GyroX); 
  }
}

float gyro_angle (float ang_velo)
{
  degree_gyro = degree_gyro + ang_velo*Ts;
  return (degree_gyro);
}
float accl_angle (float y_data, float z_data)
{
  float accelXangle = (atan2(y_data, abs(z_data))) * 180 / PI; // write logic to derive angle around x in degree
  return (accelXangle);
}
