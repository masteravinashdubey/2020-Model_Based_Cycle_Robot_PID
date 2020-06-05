#loading required packages 
pkg load control 
pkg load signal
#-----------------clear command prompt---------------------------------------------------------
clc;
#---------------------------------------------
#-------------------given transfer function is here--------------------------------------------
denom = [1 2 30];
nom = [1 1];
tf_c = tf (nom , denom)# making transfer function ushing 'tf'

#---------------------------------------------
#-------------------Convert transfer function in to discreat domain----------------------------
tf_d1 = c2d (tf_c, 0.5 , 'zoh') #c2d converts continuous trasfer funaction to discreat function 
tf_d2 = c2d (tf_c, 0.1 , 'zoh') #we are using here most comon tachnique called 'zero order hold' or 'zoh'
tf_d3 = c2d (tf_c, 0.01 , 'zoh')#and all 3 diffrent discreat T.F. is derived with 3 diffrent sempling rate

#---------------------------------------------
#-------------------Derive poles and zeros of discreat T.F.------------------------------------
[z1 , p1 , k1 , tsam1] =zpkdata(tf_d1); # deriving poles, zeros, gain, sampling time 
[z2 , p2 , k2 , tsam2] =zpkdata(tf_d2); # the function returns in the data in form of cells 
[z3 , p3 , k3 , tsam3] =zpkdata(tf_d3);
#---------------------------------------------
#-------------------Plot step responce of all transfer function in one plane-------------------
subplot (2,2 ,1) #plotting step responce of continuous time system
step (tf_c,'g')
hold on
step (tf_d1, 'r')#ploting step responce of 0.5 s samopled trnsfer function
hold on
step (tf_d2, 'b')#ploting step responce of 0.1 s samopled trnsfer function
hold on
step (tf_d3, 'y')#ploting step responce of 0.01 s samopled trnsfer function

legend('continuous','t = 0.5','t = 0.1','t = 0.01')
#---------------------------------------------
#-------------------map continuous system poles and zeros in s-plane---------------------------
subplot (2,2 ,2)
pzmap (tf_c)     #ploting poles and zeros of continuous transfer function in s plane
legend ('poles and zeros of continuous system')
#---------------------------------------------
#-------------------map discreat system poles and zeros in Z-plane-----------------------------
subplot (2,2,3)
zplane(1,p2{1,1})#ploting poles and zeros of discreat transfer function in Z plane for sampling time 0.1 sec

subplot (2,2,4)
zplane(1,p3{1,1})#ploting poles and zeros of discreat transfer function in Z plane for sampling time 0.01 sec
#----------------------------------------------------------------------------------------------

