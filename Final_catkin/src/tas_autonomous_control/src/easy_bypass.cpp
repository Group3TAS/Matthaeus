//This node implements a collision detection and avoidance algorithm. It subscribes to the steering commands 
//provided by the dynamic programming algorithm. If an obstacle or a wall is detected, the steering commands 
//are adapted
//Author: Group03


#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "control/control.h"
#include <math.h> 
#include <unistd.h>

using std::vector;
using std::cout;
using std::endl;

class BypassObstacle
{
private:

//declare subscribers and publishers
  ros::NodeHandle nh_;
  ros::Publisher cmd_vel_pub_;
  ros::Subscriber laser_sub;
  ros::Subscriber cmd_sub;
  geometry_msgs::Twist base_cmd;
  
public:
//Constructor
  BypassObstacle(ros::NodeHandle &nh)
  {
    nh_ = nh;
    
    //set up the publisher for the cmd_vel topic
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 100);
  }
  
   double cmd_linearVelocity;
   double cmd_angularVelocity;
    
   char CheckForCollision(vector<float> data){ //this function process laser data and checks for collisions
  
    int length=data.size(); //get length of the laserscan data array
    int i=0;
    int j=0;
    float theta=0;
    int flag=0; //indicates collision
    int direction=0; //indicates where the obstacle is located (left/right)
    float d_min=0.7; //minimum distance to an obstacle
    float w_min=0.5; //minimum distance to the wall
    float b=1.4; //width of the observation window
    float phi=atan(b/(2*d_min)); //observation angle
    
    float p=(phi/PI)*length;
    int p2=ceil(p); //number of data points in the observation window in front of the car
    int pw=floor(2.0/9.0*length); 
    
    float data_relevant[2*p2+1]; //data points in the observation window in front of the car
    float left_wall[pw]; //data points to the left of the car
    float right_wall[pw];//data points to the right of the car
    
    int counter=0; //counts how many measurement points imply a critical distance to an obstacle
    int leftside=0; //counts critical data points in the left part of the front window
    int rightside=0; //counts critical data points in the right part of the front window
    int leftwall=0; //counts critical data points to the left of the car
    int rightwall=0; //counts critical data points to the right of the car
    int WallFlag=0; //indicates collision with a wall
    
    //checks for collisions with obstacles and checks whether the obstacle is more on the 
    //right or on the left side of the front data window
    for(i=0;i<=2*p2;i++){
    
      data_relevant[i]=data[length/2-p2+i];
            
      if (data_relevant[i]<=d_min){ 
        counter++;
      }
      
      if (data_relevant[i]<=d_min && i<p2){ 
        rightside++;
      }
                 
      if (data_relevant[i]<=d_min && i>p2){ 
        leftside++;
      }
    }
    
    //checks whether the car is too close to one of the walls
    for(i=0;i<pw;i++){
    
      left_wall[i]=data[length-1-i];
      right_wall[i]=data[i];
      
      theta=atan(float(i)/float(pw-1));
      
      if(right_wall[i]<w_min/cos(theta*PI/180)){
        rightwall++;
      }
      
      if(left_wall[i]<w_min/cos(theta*PI/180)){
        leftwall++;
      }
    }
    
    //assigns results to flag variables
    if (rightwall>2){
      WallFlag=1; //the car is too close to the wall (right hand side)
    }    
    
    if (leftwall>2){
      WallFlag=-1; //the car is too close to the wall (left hand side)
    }     
          
    if(leftside<rightside){
      direction=-1;
    }
    
    if(leftside==rightside){
      direction=0;
    }
    
    if(leftside>rightside){
      direction=1;
    }
    
    //if the minimum distance is undercut a critical amount of times, a near obstacle is assumed
    if (counter>2){
      flag=1; //it is very likely that an obstacle is right in front of the car
    }
    
    //assign flag variables to output
    if(flag==0 && WallFlag==0){
      return 'x'; //neither a wall nor an obstacle endangers the car
    }
    
    if(flag==0 && WallFlag==1){
      return 'a'; //wall to the right side (no obstacle)
    }
    
    if(flag==0 && WallFlag==-1){
      return 'b'; //wall to the left side (no obstacle)
    }
    
    if(flag==1 && WallFlag==-1){
      return 'r'; //wall on the left side and obstacle in front of the car
    }
        
    if(flag==1 && WallFlag==1){
      return 'l'; //wall on the right side and obstacle in front of the car
    }

    if(flag==1 && direction==-1){
      return 'l'; //obstacle in front of the car (evade to the left side)
    }
    
    if(flag==1 && direction==1){
      return 'r'; //obstacle in front of the car (evade to the right side)
    }
    
    if(flag==1 && direction==0){
      return 'm'; //obstacle in the middle of the laserscan (evade either to the right or to the left)
    }
  } 

  void cmdsCallback(const geometry_msgs::Twist::ConstPtr& msg)
  {    

    //assign control commands to new variables
    cmd_linearVelocity = msg->linear.x;
    cmd_angularVelocity = msg->angular.z;
    
  }

  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    

    vector<float> data = scan->ranges;

    //move forward
    base_cmd.linear.x = cmd_linearVelocity;
    base_cmd.angular.z = cmd_angularVelocity;
    
    int evadeangle=30; //angle offset if an obstacle is detected
    int wallangle=20; //angle offset in case of a close wall
    
    //check for obstacles and walls
    char Collision=CheckForCollision(data);
    cout<<Collision<<endl;
    
    //adapt control signals
    switch (Collision)
    {
    case 'x':
      //nothing to do here
      break;
      
    case 'l':
      base_cmd.angular.z=base_cmd.angular.z+evadeangle;
      break;
      
    case 'r':
      base_cmd.angular.z=base_cmd.angular.z-evadeangle;
      break;
                  
    case 'm': //treated like 'r'
      base_cmd.angular.z=base_cmd.angular.z-evadeangle;
      break;
      
    case 'a':
      base_cmd.angular.z=base_cmd.angular.z+wallangle;
      break;
            
    case 'b':
      base_cmd.angular.z=base_cmd.angular.z-wallangle;
      break;
      
    default:
      cout<<"CheckForCollision failed"<<endl;
    }
    
    //publish the assembled command
    cmd_vel_pub_.publish(base_cmd);

  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "easy_bypass");
  ros::NodeHandle nh;
  BypassObstacle pass(nh); //create a new object of class BypassObstacle
  
  //subscribe to control commands (published by the dynamic programming algorithm)
  ros::Subscriber cmd_sub = nh.subscribe("control_cmds", 1, &BypassObstacle::cmdsCallback,&pass);
  //subscribe to laser data
  ros::Subscriber laser_sub = nh.subscribe("scan", 1, &BypassObstacle::laserCallback,&pass);
  ros::spin();

}
