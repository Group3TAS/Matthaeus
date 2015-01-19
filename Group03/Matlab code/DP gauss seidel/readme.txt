Readme:


1. To compute a policy:
-run init.m
-open valueiterationforward.m
-set the number of iterations (at least 10 iterations recommended, expect 30 minutes per iteration)
-run valueiterationforward.m
-wait a long time
-run simulate.m to see the simulation
-run testvalue.m to see the value function


2. To visualize the existing stuff:
-load rueckwaerts3-6.mat or good.mat (the latter one is used on the car)
-run simulate.m to see the simulation
-run testvalue.m to see the value function

3. To get a policy on a car:
-perform either step 1 or step 2
-run newshape.m
-take the created pol.csv and move it to your catkin workspace folder
-rename it to pol.txt
-you are good to go
-remember: if you want to start dynamic_angular_control you need to be in your catkin workspace folder otherwise you will receive a core dumped error.

