//experiment 2 
//task 3:
//write here your code to plote poles and zeros on s plane 
//and change the values of inductunce, capacitance and resistance and obsrve the movement of pole
//after ploting the pole -zero plot also state about stability

// declare your veriables and symbolic veriables here 
L = 1 ;
R = 1;
C = 1;

//-----------------------------------


//define nominator and denominator here 
nom = 1;
den = L * s^2 + R*s +1/c;
//-----------------------------------


//genarate your transfer function here 
g = (num/den);
h = syslin('c', g)
//-----------------------------------


// write code to plot poles and zeros of transfer function
plzr(h)
//-----------------------------------
