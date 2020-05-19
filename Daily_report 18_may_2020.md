#internship day week2-day 18-may 2020

#internship day week2-day 18-may 2020

Today I have tried to optimezed my code and model in v-rep, while doing this i have found some bugs in my code.

In the sensor fusion section i have derived logic for gyro angle derivation was
Gyro_angle(n) = gyro_angle(n-1) + gyro_anguler_velocity*dt

But as we already had a more accurate data of complementary filter I have replaced it with that data 
Gyro_angle(n) = complement_filter_angle(n-1) + gyro_anguler_velocity*dt

The second mistake i have found was the direction of angles derived from accelerometer and gyro sensor was opposite so i have also corrected it 

The 3rd mistake i have found was the timings for loop or say 'dt' I got to know that in each simulation loop the function jointcall back gets executed 10 times so i have corrected my 'dt' using
sim.getSimulationTime()
And also analysed lot more kp-ki-kd by plotting graph for inclination angle and found accurate values and uploaded final file on git hub (here)

