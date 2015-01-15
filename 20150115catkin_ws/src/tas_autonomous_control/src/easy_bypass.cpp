#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
//#include "process_laserscan_data.h"
#include "control/control.h"
#include <math.h> 
#include <unistd.h>

using std::vector;
using std::cout;
using std::endl;

class BypassObstacle
{
private:
  ros::NodeHandle nh_;
  ros::Publisher cmd_vel_pub_;
  ros::Subscriber laser_sub;
  ros::Subscriber pose_sub;
  geometry_msgs::Twist base_cmd;
  
public:
//Constructor
  BypassObstacle(ros::NodeHandle &nh)
  {
    nh_ = nh;
    
    //set up the publisher for the cmd_vel topic
       cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 100);
    //set up the subscriber for the scan topic
       //laser_sub = nh_.subscribe("scan", 1000, &BypassObstacle::laserCallback,&this);
  }
    
      //this function tries to evade the obstacle
  /*void BlindBypass(char CharFlag){
  
      base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;  
 
      //standstill
      base_cmd.linear.x = 1500;
   	  base_cmd.angular.z = 0;
   	  
      //publish the assembled command
      cmd_vel_pub_.publish(base_cmd);
      cout<<"standstill"<<endl;
      sleep(2);
      
      //drive backwards
        //get current time
        time_t t;
        time_t t0;
        t0=time(0);
        t=time(0);
        
        while(t-t0<=1){
          
          //move backwards
	      base_cmd.linear.x = 1365;
   	      base_cmd.angular.z = 0;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          cout<<"backwards"<<endl;
          
          //update time
          t=time(0);
        
         }
      sleep(2);
      //start to evade obstacle
      if(CharFlag=='r'||CharFlag=='m'){
        
        //get current time
        time_t t;
        time_t t0;
        t0=time(0);
        t=time(0);
        
        while(t-t0<=1){
          
          //move to the right side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = -20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          cout<<"rightmove"<<endl;
          //update time
          t=time(0);
        
         }
         
        while(t-t0>1 && t-t0<=2){
                  
          //move to the left side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = 20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          cout<<"leftmove"<<endl;
          
          //update time
          t=time(0);
        
         }
        
      }
      
      if(CharFlag=='l'){
        
        //get current time
        time_t t;
        time_t t0;
        t0=time(0);
        t=time(0);
        
        while(t-t0<=1){
                
          //move to the left side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = 20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          cout<<"leftmove"<<endl;
          
          //update time
          t=time(0);
        
         }
         
        while(t-t0>1 && t-t0<=2){

          //move to the right side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = -20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          cout<<"rightmove"<<endl;
          
          //update time
          t=time(0);
        
         }
        
      }

      //stop
      base_cmd.linear.x = 1500;
      base_cmd.angular.z = 0;

      //publish the assembled command
      cmd_vel_pub_.publish(base_cmd);
      cout<<"standstill"<<endl;

      
      return;
  }*/
    
   char CheckForCollision(vector<float> data){ //this function process laser data and checks for collisions
  
    int length=data.size();
    int i=0;
    int j=0;
    float theta=0;
    int flag=0; //indicates collision
    int direction=0; //indicates where the obstacle is located (left/right)
    float d_min=0.5; //minimum distance to an obstacle
    float b=0.5; //width of the car 
    float phi=atan(b/(2*d_min));
    float BypassAngle=phi=atan(1.5/(2*d_min));
    
    float p=(phi/PI)*length;
    float p0=(BypassAngle/PI)*length;
    int p2=ceil(p);
    int pb=ceil(p0);
    int pw=floor(2.0/9.0*length);
    
    float data_relevant[2*p2+1];
    float left_wall[pw];
    float right_wall[pw];
    float data_bypass[2*pb+1];
    
    int counter=0; //counts how many measurement points imply a critical distance to an obstacle
    int leftside=0;
    int rightside=0;
    int leftwall=0;
    int rightwall=0;
    int WallFlag=0;
    
    for(i=0;i<=2*p2;i++){
    
      data_relevant[i]=data[length/2-p2+i];
            
      if (data_relevant[i]<=d_min){
        counter++;
      }
    }
    
    for(i=0;i<=2*pb;i++){
      data_bypass[i]=data[length/2-p2+i];
      
      if (i<pb){
        rightside=rightside+data_bypass[i];
      } 
      
      if (i>pb){
        leftside=leftside+data_bypass[i];
      }
    }
    
    for(i=0;i<pw;i++){
    
      left_wall[i]=data[length-1-i];
      right_wall[i]=data[i];
      
      theta=atan(float(i)/float(pw-1));
      
      if(right_wall[i]<0.3/cos(theta*PI/180)){
        rightwall++;
      }
      
      if(left_wall[i]<0.3/cos(theta*PI/180)){
        leftwall++;
      }
    }
    
    if (rightwall>2){
      WallFlag=1; //the car is too close to the wall (right hand side)
    }    
    
    if (leftwall>2){
      WallFlag=-1; ////the car is too close to the wall (left hand side)
    }     
          
    if(leftside>rightside){
      direction=-1;
    }
    
    if(leftside==rightside){
      direction=0;
    }
    
    if(leftside<rightside){
      direction=1;
    }
    
    //if the minimum distance is undercut a critical amount of times, a near obstacle is assumed
    if (counter>2){
      flag=1; //it is very likely that an obstacle is right in front of the car
    }
    
    if(flag==0 && WallFlag==0){
      return 'x';
    }
    
    if(flag==0 && WallFlag==1){
      return 'a'; //wall on the right side (no obstacle)
    }
    
    if(flag==0 && WallFlag==-1){
      return 'b'; //wall on the left side (no obstacle)
    }
    
    if(flag==1 && WallFlag==-1){
      return 'r';
    }
        
    if(flag==1 && WallFlag==1){
      return 'l';
    }

    if(flag==1 && direction==-1){
      return 'l';
    }
    
    if(flag==1 && direction==1){
      return 'r';
    }
    
    if(flag==1 && direction==0){
      return 'm';
    }
  } 


  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    

    vector<float> data = scan->ranges;

    //move forward
    base_cmd.linear.x = 1550;
    base_cmd.angular.z = 0;

    //publish the assembled command
    cmd_vel_pub_.publish(base_cmd);
    //cout<<"forward"<<endl;
    
    char Collision=CheckForCollision(data);
    cout<<Collision<<endl;
    //write laserscan data to file
    //laserscan_data_write(data,length);
        
    if (Collision!='x'){
        BlindBypass(Collision);
        //sleep(4);
  }
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "easy_bypass");
  ros::NodeHandle nh;
  BypassObstacle pass(nh);
  ros::Subscriber laser_sub = nh.subscribe("scan", 1, &BypassObstacle::laserCallback,&pass);
  ros::spin();

}
