#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "process_laserscan_data.h"
#include "control/control.h"
#include <math.h> 
#include <sys/time.h>
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
     cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
     //set up the subscriber for the scan topic
     laser_sub = nh_.subscribe("scan", 1000, &BypassObstacle::laserCallback,&bypass);
    }
  
   geometry_msgs::Vector3 data;
   
   char Collision='x';
    
  //this function process laser data and checks for collisions
  char CheckForCollision(vector<float> data){
  
    int length=data.size();
    int i=0;
    int j=0;
    int flag=0; //indicates collision
    int direction=0; //indicates where the obstacle is located (left/right)
    float d_min=0.25; //minimum distance to an obstacle
    float b=0.5; //width of the car 
    float phi=atan(2.0*d_min/b);
    float p=(phi/PI)*length;
    int p2=ceil(p);
    float data_relevant[2*p2+1];
    
    for(i=0;i<=2*p2;i++){
      data_relevant[i]=data[length/2-p2+i];
    }
    
    int counter=0; //counts how many measurement points imply a critical distance to an obstacle
    int leftside=0;
    int rightside=0;
    
    for (j=0;j<=2*p2;j++){
      
      if (data_relevant[j]<=d_min){
        counter++;
      }
      
      if (data_relevant[j]<=d_min && j<p2){
        leftside++;
      }
            
      if (data_relevant[j]<=d_min && j>p2){
        rightside++;
      }      
      
    }
    
    if(leftside>rightside){
      direction=-1;
    }
    
    if(leftside<rightside){
      direction=1;
    }
    
    //if the minimum distance is undercut a critical amount of times, a near obstacle is assumed
    if (counter>3){
      flag=1; //it is very likely that an obstacle is right in front of the car
    }
    
    if(flag==0){
      return 'x';
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
  
  //this function tries to evade the obstacle
  void Bypass(CharFlag){
  
      base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;  
 
      //standstill
	  base_cmd.linear.x = 1550;
   	  base_cmd.angular.z = 0;
   	  
      //publish the assembled command
      cmd_vel_pub_.publish(base_cmd);
      
      //wait 3 seconds (the obstacle could be removed)
      sleep(3);
      Collision=CheckForCollision(data);
      
      //check whether the road is free again
      if (Collision=='x'){
        return; //the obstacle has moved on, the car continues in its normal automatic mode
      }
      
      //drive backwards
	  base_cmd.linear.x = 1365;
   	  base_cmd.angular.z = 1;
   	  
      //publish the assembled command
      cmd_vel_pub_.publish(base_cmd);
      
      //start to evade obstacle
      if(CharFlag=='r'||CharFlag=='m'){
        
        //get current time
        time_t t;
        time_t t0;
        t0=time(0);
        t=time(0);
        
        while(t-t0<2){
        
          //check whether the road is free, if a new obstacle occurs the function Bypass is called recursively       
          Collision=CheckForCollision(data);
          
          if (Collision!='x'){
            Bypass(Collision);
            return;
          }
          
          //move to the right side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = -20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          
          //update time
          t=time(0);
        
         }
         
        while(t-t0>=2 && t-t0<4){
        
          //check whether the road is free, if a new obstacle occurs the function Bypass is called recursively       
          Collision=CheckForCollision(data);
          
          if (Collision!='x'){
            Bypass(Collision);
            return;
          }
          
          //move to the left side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = 20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          
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
        
        while(t-t0<2){
                
          //check whether the road is free, if a new obstacle occurs the function Bypass is called recursively       
          Collision=CheckForCollision(data);
          
          if (Collision!='x'){
            Bypass(Collision);
            return;
          }
          
          //move to the left side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = 20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          
          //update time
          t=time(0);
        
         }
         
        while(t-t0>=2 && t-t0<4){      
                
          //check whether the road is free, if a new obstacle occurs the function Bypass is called recursively       
          Collision=CheckForCollision(data);
          
          if (Collision!='x'){
            Bypass(Collision);
            return;
          }
          
          //move to the right side
	      base_cmd.linear.x = 1550;
   	      base_cmd.angular.z = -20;
   	  
          //publish the assembled command
          cmd_vel_pub_.publish(base_cmd);
          
          //update time
          t=time(0);
        
         }
        
      }
      
      return;
  }

  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    

    vector<float> data = scan->ranges;
    
    //write laserscan data to file
    //laserscan_data_write(data,length);
    
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "bypass_obstacle");
  ros::NodeHandle nh;
  BypassObstacle mypass(nh);
  //ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &BypassObstacle::laserCallback,&bypass);
  mypass.Collision=mypass.CheckForCollision(mypass.data);//here we get the laserscan data and check for obstacles
  cout<<mypass.Collision<<endl;

  if (mypass.Collision!='x'){
    Bypass(mypass.Collision);
  }
  
  
  ros::spin();
  return 0;

}
