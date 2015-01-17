//This code allows to rotate the IMU acceleration vector in order to match it with the car's coordinate system

#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "IMU_norm.h"
#include <sensor_msgs/Imu.h>
#include <vector>

class imu_norm
{
private:
  ros::NodeHandle nh_;
 //ros::Publisher 
  ros::Subscriber imu_sub;
  sensor_msgs::Imu imu_normed;
  ros::Publisher imu_pub_;
  geometry_msgs::Vector3 probe;
 public:
  imu_norm(ros::NodeHandle &nh)
  {
  	//publishing on own topic 
    nh_ = nh;
    imu_pub_ = nh_.advertise<sensor_msgs::Imu>("/imu/data_normed", 1);
  };
  void imuCallback(const sensor_msgs::Imu::ConstPtr& imu)
  { 
  //callback function that rotates every IMU data set coming in 
  
  ROS_INFO("X acceleration: %f", imu->linear_acceleration.x);
  ROS_INFO("Y acceleration: %f", imu->linear_acceleration.y);
  ROS_INFO("Z acceleration: %f", imu->linear_acceleration.z);

	
 //init of necessary variables	
    double v[3][1];  //the acceleration vector
    double dummy[3][1];  //dummy
	double R_x[3][3];   //rotation matrix for rotation around x axis
	double R_y[3][3];	//rotation matrix for rotation around y axis
	double R_z[3][3];	//rotation matrix for rotation around z axis
	
    v[1][1]=imu->linear_acceleration.x;
    ROS_INFO("hier");
        v[1][2]=imu->linear_acceleration.y;
            v[1][3]=imu->linear_acceleration.z;
	
	
	
	//rotation around x axis
					    double alpha;
					            alpha=atan(v[1][2]/v[1][3]);  //calculation of rotation angle
					            ROS_INFO("da");
						//rotation matrix set up
					
					    R_x[1][1]=1;
					        R_x[1][2]=0;
					            R_x[1][3]=0;
					                R_x[2][1]=0;
					                    R_x[2][2]=cos(alpha);
					                        R_x[2][3]=-sin(alpha);
												R_x[3][1]=0;
					                                R_x[3][2]=sin(alpha);
					                                    R_x[3][3]=cos(alpha);
														
										//matrix multiplication
										
							for(int x=0;x<3;x++){ 
					    for(int j=0;j<1;j++){  
					        double result = 0; 
					        for(int y=0;y<3;y++){         
                                    result += R_x[x][y] * v[y][j];

                                    ROS_INFO("x %f",x);
                                    ROS_INFO("y %f", y);
                                    ROS_INFO("j %f", j);

                                    ROS_INFO("erster %f", R_x[x][y]);
                                    ROS_INFO("zweiter %f", v[y][j]);

                                     ROS_INFO("result %f", result);
					        } 
                            dummy[x][j] = result;
                            ROS_INFO("REsult %f", result);
					   		 }       
							}					
					        ROS_INFO("dort");
					
					
					        v[1][1]=dummy[1][1];
					        v[2][1]=dummy[2][1];
					        v[3][1]=dummy[3][1];
											
	//rotation around y  axis								
					    double beta;
					            beta=atan(v[1][1]/v[1][3]);  //calculation of rotation angle
						
						//rotation matrix set up
					    R_y[1][1]=cos(beta);
					        R_y[1][2]=0;
					            R_y[1][3]=sin(-beta);
					                R_y[2][1]=0;
					                    R_y[2][2]=1;
					                        R_y[2][3]=0;
												R_y[3][1]=-sin(-beta);
					                                R_y[3][2]=0;
					                                    R_y[3][3]=cos(beta);
									//matrix multiplication
											
						for(int x=0;x<3;x++){ 
					    for(int j=0;j<1;j++){  
					        double result = 0; 
					        for(int y=0;y<3;y++){         
					                result += R_y[x][y] * v[y][j]; 
					        } 
					        dummy[x][j] = result; 
					   		 }       
							}					
					    v[1][1]=dummy[1][1];
					    v[2][1]=dummy[2][1];
					    v[3][1]=dummy[3][1];
					
					    ROS_INFO("lo");
					
//rotation around z axis								
					    double omega;
					    omega=0;    //calculation of rotation angle, here set to zero, as initilisation phase needs to be added (paused due to BUS Error)
					    
					    	//rotation matrix set up
					
					    R_z[1][1]=cos(omega);
					        R_z[1][2]=-sin(omega);
					            R_z[1][3]=0;
					                R_z[2][1]=sin(omega);
					                    R_z[2][2]=cos(omega);
					                        R_z[2][3]=0;
												R_z[3][1]=0;
					                                R_z[3][2]=0;
					                                    R_z[3][3]=1;
						
						//matrix multiplication
							for(int x=0;x<3;x++){ 
					    for(int j=0;j<1;j++){  
					        double result = 0; 
					        for(int y=0;y<3;y++){         
					                result += R_z[x][y] * v[y][j]; 
					        } 
					        dummy[x][j] = result; 
					   		 }       
							}					
					        v[1][1]=dummy[1][1];
					        v[2][1]=dummy[2][1];
					        v[3][1]=dummy[3][1];


        ROS_INFO("do");

	
//saving of rotated acceleration data	
    probe.x=v[1][1];
    probe.y=v[2][1];
    probe.z=v[3][1];
    ROS_INFO("X probe: %f",  probe.x);
    ROS_INFO("Y probe: %f",  probe.y);
    ROS_INFO("Z probe: %f",  probe.z);

    //imu_normed=imu;
  	imu_normed=*imu;
    imu_normed.linear_acceleration =probe ;
    ROS_INFO("do weile");

    imu_pub_.publish(imu_normed);
    ROS_INFO("da");
    ROS_INFO("X acceleration: %f", imu_normed.linear_acceleration.x);
    ROS_INFO("Y acceleration: %f", imu_normed.linear_acceleration.y);
    ROS_INFO("Z acceleration: %f", imu_normed.linear_acceleration.z);


}
};
int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "imu_norm");
  ros::NodeHandle nh;
  imu_norm imu_control(nh);
  ROS_INFO("da der erste");

  ros::Subscriber imu_sub = nh.subscribe("imu/data", 1000, &imu_norm::imuCallback, &imu_control);
 
  ROS_INFO("weile awwa");

  
  ros::spin();
  
  ROS_INFO("da der zwote");


}
