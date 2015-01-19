//The following node implements a subscriber to the laserscan data. Every time new data are retrieved,
//a function is invoked, which writes the data to a text file. This node is utilized for test purposes
//Author: Group03

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
  ros::Publisher cmd_vel_pub_;
  ros::Subscriber laser_sub;
  ros::Subscriber pose_sub;
  geometry_msgs::Twist base_cmd;
  
public:
//class constructor
  dynamics_control(ros::NodeHandle &nh)
  {
    nh_ = nh;
  }

  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    
    //retrieve new data from the laserscanner
    vector<float> data = scan->ranges;
    
    //write data to file
    laserscan_data_write(data);
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "get_laserscan_data");
  ros::NodeHandle nh;
  
  //generate new object of class dynamics_control
  dynamics_control readlaser(nh);
  
  //subscribe to laserscanner
  ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &dynamics_control::laserCallback,&readlaser);
  ros::spin();

}
