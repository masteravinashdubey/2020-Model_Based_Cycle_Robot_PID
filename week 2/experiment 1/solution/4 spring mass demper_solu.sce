//experiment 1 
//task 3:
//write here your code to plote poles and zeros on s plane 
//and change the values of spring constant,mass and demping constant and obsrve the movement of pole
//after ploting the pole -zero plot also state about stability

// declare your veriables and symbolic veriables here 
mass = 1 ;
c = 1;
k = 1;

//-----------------------------------


//define nominator and denominator here 
nom = 1;
den = mass * s^2 + c*s +k;
//-----------------------------------


//genarate your transfer function here 
g = (num/den);
h = syslin('c', g)
//-----------------------------------


// write code to plot poles and zeros of transfer function
plzr(h)
//-----------------------------------

