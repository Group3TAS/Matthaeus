%%Imu rotation programm 
%%first rotations with still standing car,  afterwards the car is
%%accelerated in only one (x or y ) direction and the z axis rotation is
%%based upon this

x=[2.2; 1.7; 9.4]  %%Measured IMU vector with still standing car
Acceleration=[3;0.5;0];  %%Measured Acceleration when the car accelerates in X direction only

%%rotation around x axis
alpha_x=atan(x(2)/x(3));  %%rotation degree
R_x=[1,0,0;0,cos(alpha_x),-sin(alpha_x);0,sin(alpha_x),cos(alpha_x)];  %%rotation matrix
x=R_x*x;  %%rotation
x_after_x=x;

%%rotation around y axis
alpha_y=atan(x(1)/x(3));  %%rotation degree
R_y=[cos(alpha_y),0,sin(-alpha_y);0,1,0;-sin(-alpha_y),0,cos(alpha_y)]; %%rotation matrix
x=R_y*x;           %%rotation
x_after_y=x;

%%Acceleration of car in x direction
 x=x+Acceleration;
 x_after_acc=x;
 %%Rototion around z axis
 alpha_z=-atan(x(2)/x(1));
 R_z=[cos(alpha_z),-sin(alpha_z),0;sin(alpha_z),cos(alpha_z),0;0,0,1];
 x=R_z*x