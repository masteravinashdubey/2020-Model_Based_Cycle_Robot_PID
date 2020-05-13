# Model-Based-Robot-PID

Project 17 Aniket Nayak (13-05-2020) day 4

After, designing the inverted pendulum with reaction wheel in v-rep.
Now, its time for code and simulate in v-rep, in which we faced alot of problem 
regarding this because we confuse in this is how we would use this mathemattical model
equation in control system which is PID based.

Another problem we faced is this, we have two paramerters for PID input angular displa-
-cement of arm and angular velocity of rection wheel.

And other parameters which would effect the system response how to implement it in the
system using Pid controller.

Mentoring session:-
(Time duration 55 minutes)

In This session we rose this questions, And our mentors answers that, this is the plus 
point about the PID controller is that it doesn't depends upon the physical parameters
to generate the controlled signal. 

Only through P gain, I gain, D gain we can control signal.

then after, our mentor clears all our doubts differnce between cart pendulum, inverted 
pendulum and how inverted pendulum is similar to our cycle bot project.
that just clear our views, regarding concepyual and practical doubts.

Next task is to remove the motor which is acted as a pivot point in the inverted pendulum
and to add v shape like structure at bottom so that it can tilt in only one axis.

------ I tried to control target velocity and target position of the motor using lua 
        language in v-rep.

another problem :- i am facing is how to get the orientation angle using dummy as a child
                   of pole in code to control the signal
