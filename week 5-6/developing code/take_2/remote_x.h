#include<arduino.h>

void read_switch();
int jx = 500,jy = 500;
int backwheel, turnwheel ;
int angle_0 = 108;

void Adjust_rmt_Data();

void read_remote()
{
    if (Serial1.available()>16 )
    {    //Serial.println(Serial1.read());
        if (Serial1.read()==0x7e)
        {
        for (int i =0 ; i<10; i ++)
        {   
            Serial1.read();
        }
          jx = (Serial1.read()<<8) | Serial1.read();
          jy = (Serial1.read()<<8) | Serial1.read();
          Serial1.read();
          Adjust_rmt_Data();
          //Serial.println("doneeeee");
    }
}
}

void Adjust_rmt_Data()
{

//-------------------------------xaxis------------------------- forward / backward /stop  
if (jx < 470){
  
    backwheel = map(jx, 470, 0, 0, -254);  //backward
        //Serial.println(turnwheel);

}

else if (jx > 550) {
    backwheel = map(jx, 550, 1023, 0, 254); // forward
}

else if (jx <550 && jx >470) {
    backwheel = 0;                          //stop
  }
//---------------------------yaxis----------------------------- turn/ left/right
if (jy < 400) {
    turnwheel = map(jy, 400, 0, angle_0, angle_0+60);;                         //left
  }
  
else if (jy > 550) {
    turnwheel = map(jy, 550, 1023, angle_0, angle_0-65);                         // right
    Serial.println(turnwheel);
}

else if (jy <550 && jy >470) {
    turnwheel = angle_0;                         //straight 
  }

}
