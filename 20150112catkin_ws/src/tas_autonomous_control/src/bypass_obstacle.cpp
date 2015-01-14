#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "process_laserscan_data.h"
#include "control.h"
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
  ros::Publisher cmd_shutdown_;
  ros::Subscriber laser_sub;
  ros::Subscriber pose_sub;
  geometry_msgs::Twist base_cmd;
  std_msgs::Bool shutdown;
  int iter=0;

  
public:
//Constructor
  BypassObstacle(ros::NodeHandle &nh)
  {
    nh_ = nh;
    
    //set up the publisher for the cmd_vel topic
     cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
     cmd_shutdown_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    }
    
   
  char CheckForCollision(vector<float> data){ //this function process laser data and checks for collisions
  
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

  void Bypass(){
  
     
    
  }

  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    

    vector<float> data = scan->ranges;
    int length=data.size();
    
    char Collision=CheckForCollision(data);
    cout<<Collision<<endl;
    
    //write laserscan data to file
    laserscan_data_write(data,length);
    
    
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "get_laserscan_data");
  ros::NodeHandle nh;
  BypassObstacle bypass(nh);
  ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &BypassObstacle::laserCallback,&bypass);
  ros::spin();

}
