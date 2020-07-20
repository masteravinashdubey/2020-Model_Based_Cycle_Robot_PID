//float Kp = -108, Ki = 0, Kd = -1.9;
float Kp = -150, Ki = -30, Kd = -2.1;
//float Kp = -180, Ki = -100, Kd = -0.5;
//float Kp = -90, Ki = -70, Kd = -1;
//float Kp = -132, Ki = -920, Kd = -4.78; //Z-N method Ku = 220 / tu = 290ms classical
//float Kp = -44, Ki = -303, Kd = -0.8; //Z-N method Ku = 220 / tu = 290ms no overshoot

//############################################################
//function name:      update_gains ()
//passing arguments:  NONE  
//return :            NONE 
//description:        the function changes the gains as per requirment 
//                    we need to pass command in the formate :    <gain_name><sign><value from 1 to 9> 
//                    gain_name: Kp --> 'p', Ki -->'i', Kd --> 'd'
//                    sign:      for increment in net value -->+  ;   for decrement in neet value --> - 
//                    value: the value by which we want to inc/dec our gain (between 1-9 only integer)
//                    (note: for derivative gain the value will be devided by 10)       
//############################################################
void update_gains ()
{
if (Serial.available())
{ int gain = Serial.read();
  int sign = Serial.read();
  int value = Serial.read();
    if ( gain == 'p')                         //for Kp
    {   if (sign == '+') Kp = Kp - value +48; //convertin ASCII value to equivalent integer value 
        else Kp = Kp + value - 48;  }
        
    else if (gain == 'i')                      //for Ki
    {   if (sign == '+') Ki = Ki - value + 48;
        else  Ki = Ki + value - 48;  }
        
    else if (gain == 'd')                     // for Kd
    {   if (sign == '+') Kd = Kd - float(value - 48)/10;
        else Kd = Kd + float(value - 48)/10;    }

     //printing new gains   
     Serial.print("Kp:");
     Serial.print(Kp);
     Serial.print("  Ki:");
     Serial.print(Ki);
     Serial.print("  Kd:");
     Serial.println(Kd);
}
}

//############################################################
//function name:      print_data
//passing arguments:  variables to be printed   
//return :            NONE 
//description:        we can pass up to max 5 arguments 
//                    the argument which is not passed will be initialize as zero            
//############################################################
void print_data(float a = 0, float b = 0, float c = 0, float d = 0, float e = 0)
{
  Serial.print(" ------- ");
  Serial.print(e);
  Serial.print(" ------- ");
  Serial.print(d);
  Serial.print(" ------- ");
  Serial.print(c);
  Serial.print(" ------- ");
  Serial.print(b);
  Serial.print(" ------- ");
  Serial.println(a);
}
