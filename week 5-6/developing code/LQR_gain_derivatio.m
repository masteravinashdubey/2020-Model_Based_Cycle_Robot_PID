pkg load signal
%robot parameters
ke = 0.473;
kt = 0.347;
R = 4.5; %motor resistance
bb = 0.002; %Viscous friction constant
g = 9.81; %gravity
Ts = 0.004;
bR = 0.002;
MR =0.333
Mm = 0.158
Mb = 0.544
lm = 0.095
lR = 0.085
lb = 0.045
Iro = MR*(lR^2)
Imo = Mm*(lm^2)
Ibo = 1.194*(10^(-3))           %pendulum inertia with respect to pivot
Iso = Ibo+Iro+Imo    
IR  = 0.333*0.045*0.045            %inertia of reaction wheel
kmgl= (Mb*lb + Mm*lm + MR*lR)*g



A = [0                       1                    0;
    kmgl/Iso                -bb/Iso       (kt*ke/R*Iso)+(bR/Iso);
    -kmgl/Iso                bb/Iso       (-(Iso+IR)/Iso*IR)*((bR+ke*kt/R))]
B = [0; -kt/(R*Iso); ((Iso+IR)*kt)/(Iso*IR*R)]

C = [1 0 0;
     0 1 0;
     0 0 1;]
D = [0;0;0];
Q= [ 5000  0   0; 
      0    1   0; 
      0    0   75000]
R = 10;

G = ss(A,B,C,D);
##step(G)
PP = c2d(G,Ts);

%%K  = lqr (A,B,Q,R)#continuous time k
Kd = dlqr(PP,Q,R)# discreat time system

#Lam = Ad - Bd*Kd;
#zplane(1,eigs(Lam));


