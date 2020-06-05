#loading required packages 
pkg load control 
pkg load signal
#-----------------clear command prompt---------------------------------------------------------
clc;
#---------------------------------------------
#-------------------given transfer function is here--------------------------------------------
denom = [1 2 30];
nom = [1 1];
tf_c = tf (nom , denom)

#---------------------------------------------
#-------------------Convert transfer function in to discreat domain----------------------------

##put your code here to transfer contimuous time system to discreat time system
##take care that make at least 3 diffrent discreat transfer function with
##3 diffrent sampling frequency

#---------------------------------------------
#-------------------Derive poles and zeros of discreat T.F.------------------------------------

##put you code here to derive poles and zeros of all discreat transfer function 
##make diffrent veriables for each distreat transferfunction poles and zeros

#---------------------------------------------
#-------------------Plot step responce of all transfer function in one plane-------------------

##plote step responce of all transfer functions in one plane use 'hold on' for that 
##also legend every curve to indetify it 

#---------------------------------------------
#-------------------map continuous system poles and zeros in s-plane---------------------------

##write your code here to plote poles and zeros of continuous time system T.F. in another plane in s-plane

#---------------------------------------------
#-------------------map discreat system poles and zeros in Z-plane-----------------------------

##write your code here to plote poles and zeros of discreat time system T.F. in another plane in Z-plane

#----------------------------------------------------------------------------------------------

