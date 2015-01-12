#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "process_pwm_signals.h"
#include <geometry_msgs/Vector3.h>

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

  void PWMCallback(const geometry_msgs::Vector3::&servo)
  {    
//get current time
  time_t t;
  t=time(0);
  
  //read control pwm signals
  int speed;
  int angle;
  speed=servo->control_servo.x;
  angle=servo->control_servo.y;
    
    pwm_data_write(t,speed,angle); 
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "get_control_signals");
  ros::NodeHandle nh;
  ros::Subscriber pwm_signals = nh.subscribe("servo", 1000, &dynamics_control::PWMCallback);
  ros::spin();

}
