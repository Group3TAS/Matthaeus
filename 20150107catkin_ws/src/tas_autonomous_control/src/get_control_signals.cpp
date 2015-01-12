#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "process_laserscan_data.h"

using std::vector;
using std::cout;
using std::endl;

class dynamics_control
{
private:
  ros::NodeHandle nh_;
  ros::Subscriber pwm_signals;
  geometry_msgs::Twist base_cmd;
  
public:

  void PWMCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    
//get current time
  time_t t;
  t=time(0);
    
    pwm_data_write(t,speed,angle); 
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "get_control_signals");
  ros::NodeHandle nh;
  ros::Subscriber pwm_signals = nh.subscribe("scan", 1000, &dynamics_control::PWMCallback);
  ros::spin();

}
