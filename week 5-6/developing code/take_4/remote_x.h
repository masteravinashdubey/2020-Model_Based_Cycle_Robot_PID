#include<arduino.h>

void read_switch();
int jx = 500,jy = 500;
int backwheel, turnwheel ;
int angle_0 = 108;

void Adjust_rmt_Data();


//############################################################
//function name:      read_remote())
//passing arguments:  NONE  
//return :            NONE 
//description:        when data package arrives from remote on serial port 1 it extracts joystick data from it            
//############################################################

void read_remote()
{
    if (Serial1.available()>16 )              //when whole package is available, further data extraction process runs
    {    
        if (Serial1.read()==0x7e)             //ensuring the start byte of data package 
        {
        for (int i =0 ; i<10; i ++)
        {   
            Serial1.read();
        }
          jx = (Serial1.read()<<8) | Serial1.read(); //combining 10 bit of data for joy stick x in one veriable 
          jy = (Serial1.read()<<8) | Serial1.read(); //combining 10 bit of data for joy stick y in one veriable 
          Serial1.read();
          Adjust_rmt_Data();                         //decision making of navigation from remote data
    }
}
}

//############################################################
//function name:      Adjust_rmt_Data()
//passing arguments:  NONE  
//return :            NONE 
//discription:        it makes decision for speed and direction of rare whee motor
//                    it makes decision for speed and angle of turning servo motor            
//############################################################
void Adjust_rmt_Data()
{

//-------------------------------xaxis------------------------- forward / backward /stop  
if (jx < 470)                   backwheel = map(jx, 470, 0, 0, -254);     //backward
else if (jx > 550)              backwheel = map(jx, 550, 1023, 0, 254);   // forward
else if (jx <550 && jx >470)    backwheel = 0;                            //stop
 
  
//---------------------------yaxis----------------------------- turn/ left/right
if (jy < 400)                     turnwheel = map(jy, 400, 0, angle_0, angle_0+60);          //left 
else if (jy > 550)                turnwheel = map(jy, 550, 1023, angle_0, angle_0-65);        // right
else if (jy <550 && jy >470)      turnwheel = angle_0;                                        //straight 
}
