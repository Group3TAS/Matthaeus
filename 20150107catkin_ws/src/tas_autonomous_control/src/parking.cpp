#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "laser_processing.h"

using std::vector;
using std::cout;
using std::endl;

class dynamics_control
{
private:
  ros::NodeHandle nh_;
  ros::Publisher cmd_vel_pub_;
  ros::Subscriber laser_sub;
  geometry_msgs::Twist base_cmd;
  float scan_prev, scan_next;
  int flag_initial_scan;
public:
  dynamics_control(ros::NodeHandle &nh)
  {
    nh_ = nh;
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    flag_initial_scan = 0;
  };
  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {
    vector<float>::size_type x,i;
    vector<float> ranges;
    float left_scan, delta_scan;

    ranges = scan->ranges;
    x = ranges.size();

    left_scan = ranges[x-1];

    if (flag_initial_scan == 0)
      {
	scan_prev = left_scan;
	flag_initial_scan = 1;
      }
    
    scan_next = left_scan;
    

    delta_scan = scan_next - scan_prev;

    if (delta_scan < -1)
    {
        cout<<"Small"<<endl;
    }
    
    scan_prev = scan_next;
    
    cout << scan_prev << endl;
    cout << scan_next << endl;

    //base_cmd.linear.x = test;
    //base_cmd.angular.z = test;
    //cmd_vel_pub_.publish(base_cmd);
  };
};


int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "parking");
  ros::NodeHandle nh;
  dynamics_control car_control(nh);
  ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &dynamics_control::laserCallback, &car_control);
  ros::spin();

}
