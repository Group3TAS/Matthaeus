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
  dynamics_control(ros::NodeHandle &nh)
  {
    nh_ = nh;
    }

  void PWMCallback(const geometry_msgs::Vector3::ConstPtr&servo)
  {    
//get current time
  time_t t;
  t=time(0);
  
  //read control pwm signals
  int speed;
  int angle;
  speed=servo->x;
  angle=servo->y;
    
    pwm_data_write(t,speed,angle); 
  }

};

int main(int argc, char** argv)
{

  //init the ROS node
  ros::init(argc, argv, "get_control_signals");
  ros::NodeHandle nh;
  dynamics_control readpwm(nh);
  ros::Subscriber pwm_signals = nh.subscribe("servo", 1000, &dynamics_control::PWMCallback,&readpwm);
  ros::spin();

}
