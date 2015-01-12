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

  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    
    /*vector<float>::size_type x;
    vector<float> ranges;*/
    
    float data[] = scan->ranges;
    int *datapointer=data;
    int length=sizeof(data)/sizeof(data[0]);
    
    laserscan_data_write(datapointer,length); 
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "get_laserscan_data");
  ros::NodeHandle nh;
  ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &dynamics_control::laserCallback);
  ros::spin();

}
