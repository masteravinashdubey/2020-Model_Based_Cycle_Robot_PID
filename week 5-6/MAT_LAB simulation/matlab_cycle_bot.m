

%% veriables defination
%syms Q Qd Q1 Q1d F
syms X V th W t Tr a alf Va
syms Xt(t) Vt(t) tht(t) Wt(t) alfat(t) at(t) 
%syms Qt(t) Qdt(t) Q1t(t) t(t) alfat(t) at(t) 
m1 = 0.725
m2 = 0.305
L1 = 0.05
L2 = 0.1
r  = 0.05
mw = 0.176     %mass of reaction wheel disk
r = 0.05        %redius of the reaction wheel metel disk
Mring = 0.049 +0.084   %mass of the matal chain clipped at periferal of reaction wheel

I1 = (1/12)* m1 * (L1^2)   % inertia of body (assuming it as bar)
I2 = ((1/2)*m2*(r^2))+ (Mring * r *r)%inertia of reaction wheel


%motor parameters
 stalltorque_gear = 0.8335
 stalltorque_motor = 0.0278
 stallcurrent = 2.4
Kg= 9000/300 %gear box ratio
Kt = stalltorque_motor/stallcurrent%torque constant
Kb =Kt;  %back emf constant
Ra = 4.5; 
R = 0.15%radius of ball

%---------------------------------------       %motor resistance
g = 9.81;       %gravity


%finding components of euler lagrange 
 clc;
 Tr = Kt *(Va -Kb*W)/Ra;
 
%  K = 0.5*(m1*L1^2 + m2*L2^2 +I1+ I2 )*V^2 + I2 *V *W+0.5*I2*W^2
%  P = (m1*L1 +m2*L2)*g*cos(X)
%  
%  F = 
%  L = K - P ;
% 
%  del_L_V = diff (L , V) ;      %keep here non "t" dependent veriable to difrenciate L with V without error
%  del_L_V = subs (del_L_V , [X V th W] ,[Xt(t) Vt(t) tht(t) Wt(t)]);%to diff. further by t replace "t" independent veriable with "t" dependent(the variable that are function of t)
%  d_L_V_dt=  diff (del_L_V , t); %diff. with respect to "t" to find d(del L/del v)/dt
%  d_L_V_dt=  subs (d_L_V_dt,[diff(Xt(t),t), diff(Vt(t),t) ,diff(tht(t),t),diff(Wt(t),t),Xt(t) Vt(t) tht(t) Wt(t)] , [V a W alf X V th W]); %replace diff(A,t) turms with apprepreate name(ex. => diff(v,t )shows v_dot
%   
%  del_L_X = diff (L , X);
% 
%  del_L_W = diff (L , W);
%  del_L_W =  subs (del_L_W , [X V th W] ,[Xt(t) Vt(t) tht(t) Wt(t)]);
%  d_L_W_dt=  diff (del_L_W , t);
%  d_L_W_dt=  subs (d_L_W_dt,[diff(Xt(t),t), diff(Vt(t),t) ,diff(tht(t),t),diff(Wt(t),t),Xt(t) Vt(t) tht(t) Wt(t)] , [V a W alf X V th W]); %replace diff(A,t) turms with apprepreate name(ex. => diff(v,t )shows v_dot
%  
%  del_L_th = diff (L , th);
% 
%  ELequn1 = (d_L_V_dt - del_L_X) == F ;
%  ELequn2 = (d_L_W_dt - del_L_th) == 0;
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 ELequn1 =  (m1*L^2 + m2*L^2+ I1+I2)*V + I2*V*W -(m1*L1 +m2*L2)*g*sin(X) == 0
 ELequn2 = I2*(a + alf) == Tr
 % equation checked ...all ok till now
 
 x4_dot = solve (ELequn2 , alf)
 ELequn1 =subs(ELequn1,alf,x4_dot)
 x2_dot = solve (ELequn1 , a) 
 x4_dot = vpa (subs(x4_dot,a,x2_dot),5)
  
jacbA = vpa(jacobian ([V ,x2_dot, W, x4_dot] ,[X V th W]),5);
jacbB = vpa(jacobian ([V ,x2_dot, W, x4_dot] ,Va),5);

A = vpa(subs(jacbA , [X,V,th,W], [0 , 0 , 0 , 0]),8)
B = vpa(subs(jacbB , [X,V,th,W], [0 , 0 , 0 , 0]),8)


    

 
