#include<Wire.h>
#define MPUaddr0 0x68    //addr used if pin AD0 is set to 0 (left unconnected)
#define MPUaddr1 0x69    //addr used if pin AD0 is set to 1 (wired to VDD)
#define blue 31
#define green 39

float AccelX, AccelY, AccelZ = 0;              ////scaled data from MPU accl(force)
float accelYangle = 0;////angle calculated from accl(degree)
float GyroX, GyroY, GyroZ = 0;                 ////scaled data from MPU  gyro(degree/sec)
float pitch, gyroYang;
void readGyro(float);
void readAccel(float);
void initiate_pitch();

//--------------------------------------------------------------------------
float Ts = 0.004; //////main frequency  - 200hz
float gyroffset = 1.75;   //offset in angular velocity
//-----------------------------------------------------------------

//############################################################
//function name:      dataFusion()
//passing arguments:  filter coefficient of complementary filter()default is 0.97
//return :            filterd data of inclination in degree 
//discription:        it grabs data from MPU 6050 with SPI protocol and calculates angle from raw data and also performs data fusion on it                 
//############################################################

float dataFusion(float alpha_comp = 0.97)
{   
  accelYangle = (atan2(-AccelX, -AccelZ)) * 180 / PI;   //calculate angle from accel data in  degree(method 1)
  gyroYang =  pitch + GyroY* Ts;                        //calculate angle from gyro data in degree
  pitch       = (alpha_comp* (pitch + GyroY* Ts)) + ((1.0 - alpha_comp) * accelYangle);       //fuse data using complementry filter algorythem in degree
  return (pitch);                                       //return data of data fusion 
}

//############################################################
//function name:      readAccel()
//passing arguments:  divisor to convert raw data in to MKS unit of physical quantity 
//return :            NONE 
//discription:        it grabs data from MPU 6050 with SPI protocol and calclates forces on X & Z axis from raw data                
//############################################################

void readAccel(float accelDivisor)
{
  Wire.beginTransmission(MPUaddr0);   //begin transmission with mpu on defined address
  Wire.write(0x3B);                   //define register to be accesed 
  Wire.endTransmission();             //over the commanding 
  Wire.requestFrom(MPUaddr0, 6);      //read 6 consecutive registers starting at 0x3B
  if (Wire.available() >= 6){
    //combining raw data received from 2 diffrent bytes in one variable for X axis
    int16_t temp0 = Wire.read() << 8;   //read upper byte of X
    int16_t temp1 = Wire.read();        //read lower byte of X
    AccelX = (float) (temp0 | temp1);
    AccelX = AccelX / accelDivisor;     //converting data to force in factor of gravitational acceleration 

    //wasting 2 bites as they are for y axis and not for oure use
    Wire.read();           //read upper byte of Y
    Wire.read();           //read lower byte of Y

    //combining raw data received from 2 different bytes in one veriable for Z axis
    temp0 = Wire.read() << 8;           //read upper byte of Z
    temp1 = Wire.read();                //read lower byte of Z
    AccelZ = (float) (temp0 | temp1);
    AccelZ = AccelZ / accelDivisor;     //converting data to force in factor of gravitetional accilaration 
  }
}
//############################################################
//function name:      readGyro()
//passing arguments:  divisor to convert raw data in to MKS unit of physical quantity 
//return :            NONE 
//discription:        it grabs data from MPU 6050 with SPI protocol and calclates angular velocity around Y axis in degree/sec                
//############################################################
void readGyro(float gyroDivisor){
  
  Wire.beginTransmission(MPUaddr0);
  Wire.write(0x45);
  Wire.endTransmission();
  Wire.requestFrom(MPUaddr0, 6);    //read 6 consecutive registers starting at 0x43
  if (Wire.available() >= 6){
    //combining raw data received from 2 diffrent bytes in one veriable for Y axis
    int16_t temp0 = Wire.read() << 8;   //read upper byte of X
    int16_t temp1 = Wire.read();        //read lower byte of X
    GyroY = (float) (temp0 | temp1);
    GyroY = GyroY / gyroDivisor ;       // converting data ti to degree/sec
    GyroY = -GyroY+gyroffset;           // removing constant offset from gyro angular velocity because of manufacturing errors.=
  }  
}
//############################################################
//function name:      setAccelSensitivity()
//passing arguments:  sensitivity to be set in accelerometer 
//return :            NONE 
//description:        it sets accelerometer to defined full scale range  (details are in data sheet of mpu)             
//############################################################
void setAccelSensitivity(uint8_t g_factor){

  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(0x1C);                   //write to register 0x1C
  Wire.write(g_factor);               //setting bit 7 to 1 resets all internal registers to default values
  Wire.endTransmission();             //end comm
}
//############################################################
//function name:      setAccelSensitivity()
//passing arguments:  sensitivity to be set in gyro scop 
//return :            NONE 
//description:        it sets gyroscope to defined full scale range  (details are in data sheet of mpu)             
//############################################################
void setGyroSensitivity(uint8_t dps){

  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(0x1B);                   //write to register 0x1B
  Wire.write(dps);                    //setting bit 7 to 1 resets all internal registers to default values
  Wire.endTransmission();             //end comm
}

//############################################################
//function name:      resetMPU()
//passing arguments:  NONE  
//return :            NONE 
//description:        it sets sensitivity and other perameters of sensors to default values at the initialaization of bot            
//############################################################

void resetMPU()
{
  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(0x6B);                   //write to register 0x6B
  Wire.write(0x00);                   //reset all internal registers to default values
  Wire.endTransmission();             //end comm
  delay(100);
  setGyroSensitivity(0x10);   //programmable range of +/-250, +/-500, +/-1000, +/-2000 DPS
  setAccelSensitivity(0x00);  //programmable range of +/-2g, +/-4g, +/-8g, +/-16g

  initiate_pitch();           //it initializes the pitch to a proper angle to remove offsets to be integrated 

}

//############################################################
//function name:      initiate_pitch()
//passing arguments:  NONE  
//return :            NONE 
//discription:        it initializes the pitch to a proper angle to remove offsets to be integrated 
//                    during this period we need to keep bot pteady an any position and it will take 1000 samples to from accelerometer 
//                    to calculate precise current angle (initial condetions)            
//############################################################
void initiate_pitch()
{ //taking 1000 samples and finding its average 
  for (int i=0; i < 1000; i++)        
  {
   readAccel(16384.0);
   pitch = pitch +((atan(-1 * AccelX / sqrt(pow(AccelY, 2) + pow(AccelZ, 2)))) * 180 / PI);
   delay (3);
  }
  pitch = pitch/1000;
  digitalWrite(green,LOW);
  //we stuckes our MCU to till bot gets to +/-1 degree for better initializatio of PID controlet functioning
  while (1)
  { readAccel(16384.0);      //read XYZ Accel data from registers 0x3B to 0x40 
    readGyro(32.75); 
    dataFusion();
    if (pitch < 1 && pitch >-1)
    break;
  }
  digitalWrite(blue,HIGH);
}
