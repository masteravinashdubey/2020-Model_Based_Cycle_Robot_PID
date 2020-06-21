clc;
g = 9.8
Kb = 0.0116   % back emf constant 
Kt = 0.0116   %torque - current constant
R = 4.5         %moter rasistance
mb = 0.772     % mass of only body
mw = 0.176          %mass of reaction wheel
Lw = 0.1        %hight of center of grevity of reaction wheel
Lb = 0.06       %hight of center of grevity of body only
r = 0.05
Mring = 0.049
Ib = (1/12)* mb * (Lb^2)   % inertia of body (assuming it as bar)
Iw = ((1/2)*mw*(r^2))+ (Mring * r *r)%inertia of reaction wheel
It = ((Iw)+(mw*Lw))+((Ib)+(mb*Lb))% total mass moment of inertia

TF_nom = 12/255
TF_denom = [-R*(It -Iw)/Kt,-Kb*Ib/Iw  , R*(mw*Lw + mb*Lb)*g/Kt ]
TF = tf (TF_nom ,TF_denom)
TF_D = c2d (TF,0.01,'zoh')
