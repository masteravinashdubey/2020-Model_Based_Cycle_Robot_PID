#include<Wire.h>
#define MPUaddr0 0x68    //addr used if pin AD0 is set to 0 (left unconnected)
#define MPUaddr1 0x69    //addr used if pin AD0 is set to 1 (wired to VDD)


float AccelX, AccelY, AccelZ = 0;              ////scaled data from MPU accl(force)
float accelYangle = 0;////angle calculated from accl(degree)
float GyroX, GyroY, GyroZ = 0;                 ////scaled data from MPU  gyro(degree/sec)
float pitch, gyroYang;
void readGyro(float);
void readAccel(float);
void initiate_pitch();

//--------------------------------------------------------------------------
float Ts = 0.005; //////main frequency  - 200hz
float alpha_comp = 0.96;  //perfectly tuned filter at alfa = 0.98
float gyroffset = 1.75;   //offset in anglar velocity
//-----------------------------------------------------------------
float dataFusion()
{   
  readAccel(16384.0);                                                                         //read XYZ Accel data from registers 0x3B to 0x40 
  readGyro(32.75);                                                                            //read XYZ Gyro data from registers 0x43 to 0x48`
    
 //accelYangle = (atan2(AccelX, abs(AccelZ))) * 180 / PI;                                     //in  degree(method 1)
  gyroYang =  pitch + GyroY* Ts;
  accelYangle = ((atan(-1 * AccelX / sqrt(pow(AccelY, 2) + pow(AccelZ, 2)))) * 180 / PI);     //in  degree(method 2)
  pitch       = (alpha_comp* (pitch + GyroY* Ts)) + ((1.0 - alpha_comp) * accelYangle);       // in degree

return (-pitch );
}

void readAccel(float accelDivisor)
{
  Wire.beginTransmission(MPUaddr0);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPUaddr0, 6);        //read 6 consecutive registers starting at 0x3B
  if (Wire.available() >= 6){
    int16_t temp0 = Wire.read() << 8;   //read upper byte of X
    int16_t temp1 = Wire.read();        //read lower byte of X
    AccelX = (float) (temp0 | temp1);
    AccelX = AccelX / accelDivisor;
    
    Wire.read();           //read upper byte of Y
    Wire.read();           //read lower byte of Y
    temp0 = Wire.read() << 8;           //read upper byte of Z
    temp1 = Wire.read();                //read lower byte of Z
    AccelZ = (float) (temp0 | temp1);
    AccelZ = AccelZ / accelDivisor;     
  }
 
}

void readGyro(float gyroDivisor){
  
  Wire.beginTransmission(MPUaddr0);
  Wire.write(0x45);
  Wire.endTransmission();
  Wire.requestFrom(MPUaddr0, 6);    //read 6 consecutive registers starting at 0x43
  if (Wire.available() >= 6){
    
    int16_t temp0 = Wire.read() << 8;   //read upper byte of X
    int16_t temp1 = Wire.read();        //read lower byte of X
    GyroY = (float) (temp0 | temp1);
    GyroY = GyroY / gyroDivisor ;
    GyroY = GyroY-gyroffset;
//Serial.println(GyroY);
  }  
}

void setAccelSensitivity(uint8_t g_factor){

  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(0x1C);                   //write to register 0x1C
  Wire.write(g_factor);               //setting bit 7 to 1 resets all internal registers to default values
  Wire.endTransmission();             //end comm
}

void setGyroSensitivity(uint8_t dps){

  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(0x1B);                   //write to register 0x1B
  Wire.write(dps);                    //setting bit 7 to 1 resets all internal registers to default values
  Wire.endTransmission();             //end comm
}

void resetMPU()
{
  Wire.beginTransmission(MPUaddr0);   //initialize comm with MPU @ 0x68
  Wire.write(0x6B);                   //write to register 0x6B
  Wire.write(0x00);                   //reset all internal registers to default values
  Wire.endTransmission();             //end comm
  delay(100);
  setGyroSensitivity(0x10);   //programmable range of +/-250, +/-500, +/-1000, +/-2000 DPS
  setAccelSensitivity(0x00);  //programmable range of +/-2g, +/-4g, +/-8g, +/-16g

  initiate_pitch();

}
void initiate_pitch()
{ //Serial.println ("pitch initiation started");
  for (int i=0; i < 1000; i++)
  {
   readAccel(16384.0);
   pitch = pitch +((atan(-1 * AccelX / sqrt(pow(AccelY, 2) + pow(AccelZ, 2)))) * 180 / PI);
   delay (3);
  }
  pitch = pitch/1000;
  while (1)
  {
    dataFusion();
    if (pitch < 1 && pitch >-1)
    break;
  }
}



