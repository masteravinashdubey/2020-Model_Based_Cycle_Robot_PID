clc;
g = 9.8        %gravity constant
Kb = 0.0116   % back emf constant 
Kt = 0.0116   %torque - current constant
R = 4.5         %moter rasistance
mb = 0.772     % mass of only body
Lw = 0.1        %hight of center of grevity of reaction wheel
Lb = 0.06       %hight of center of grevity of body only

%reaction wheel is made up of metele disk and a chain clipped at its
%periferal to increase inertia
mw = 0.176     %mass of reaction wheel disk
r = 0.05        %redius of the reaction wheel metel disk
Mring = 0.049   %mass of the matal chain clipped at periferal of reaction wheel

Ib = (1/12)* mb * (Lb^2)   % inertia of body (assuming it as bar)
Iw = ((1/2)*mw*(r^2))+ (Mring * r *r)%inertia of reaction wheel
It = ((Iw)+(mw*Lw))+((Ib)+(mb*Lb))% total mass moment of inertia

TF_nom = 12/255         %nominator for continuous time transfer function
TF_denom = [-R*(It -Iw)/Kt,-Kb*Ib/Iw  , R*(mw*Lw + mb*Lb)*g/Kt ] %denominator for continuous time transfer function
TF = tf (TF_nom ,TF_denom) %transfer function for continuous time system 
TF_D = c2d (TF,0.01,'zoh') %deriving deascreat time function for 0.01 sec sampling rate 


%once deriving the descreate time transfer function we need to update it in
%PID_tuning.slx (simulink file) and then we can tune controller there 