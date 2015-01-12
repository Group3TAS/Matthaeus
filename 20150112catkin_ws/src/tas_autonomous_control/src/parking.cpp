#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "laser_processing.h"
#include <ctime>
#include <sstream>
#include <fstream>
#include <ios>
#include <cstdio>
#include <string>

using std::vector;
using std::cout;
using std::endl;
using std::string;

class dynamics_control
{
private:
  ros::NodeHandle nh_;
  ros::Publisher cmd_vel_pub_;
  ros::Subscriber laser_sub;
  geometry_msgs::Twist base_cmd;
  float scan_prev, scan_next;
  int flag_initial_scan, flag_box_begin, flag_box_end, flag_found, flag_wall_1,flag_wall_2,flag_wall_3;
  int flag_box_1,flag_box_2;
  int vec_idx;
  vector<float> measurements;
public:
  dynamics_control(ros::NodeHandle &nh)
  {
    nh_ = nh;
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    flag_initial_scan = 0;
    flag_box_begin = 0;
    flag_box_end = 0;
    vec_idx = 0;
    flag_found = 0;
    flag_wall_1 = 0;
    flag_wall_2 = 0;
    flag_wall_3 = 0;
    flag_box_1 = 0;
    flag_box_2 = 0;
  };
  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {
    vector<float>::size_type x,i;
    vector<float> ranges;
    float left_scan, delta_scan;
    string output;
    ranges = scan->ranges;
    x = ranges.size();
    int gap = 1;

    left_scan = ranges[x-1];
    measurements.push_back(left_scan);

    cout << vec_idx << endl;
    if (flag_found == 0)
      {
	std::ofstream log("distance_measurements.txt", std::ios_base::app | std::ios_base::out);
	std::ostringstream left_scan_string;
	left_scan_string << left_scan;
	output = left_scan_string.str();
	log << output << "\n";
      }
    

    if (vec_idx > gap)
      {
	/*
	if (flag_initial_scan == 0)
	  {
	    scan_prev = left_scan;
	    flag_initial_scan = 1;
	  }
    
	scan_next = left_scan;
	*/
	
	scan_next = measurements[vec_idx];
    scan_prev = measurements[vec_idx - gap];
	delta_scan = scan_next - scan_prev;
/*
	if (delta_scan < -0.25)
	  {
	    cout << "Begin of Box!" << endl;
	    flag_box_begin = flag_box_begin + 1;
	  }

	if (delta_scan > 0.25)
	  {
	    cout << "End of Box!" << endl;
	    flag_box_end = flag_box_end + 1;
	  }
      }
*/
    while(flag_found== 1)
    {
        base_cmd.linear.x = 1350;
        base_cmd.angular.z = 30;
        cmd_vel_pub_.publish(base_cmd);
    }
    if(scan_next > 0.50 && flag_wall_1 == 0 && flag_box_1 == 0 && flag_found == 0)
    {
      cout << "Wall 1!" << endl;
      base_cmd.linear.x = 1550;
      base_cmd.angular.z = 4.5;
      flag_wall_1 = 1;
    }

    if(scan_next < .40 && flag_box_1 == 0 && flag_wall_1 == 1 && flag_found == 0)
    {
      cout << "Box 1!" << endl;
      base_cmd.linear.x = 1550;
      base_cmd.angular.z = 4.5;
      flag_box_1 = 1;
    }

    if (scan_next > 0.50 && flag_wall_1 == 1 && flag_box_1 == 1 && flag_found == 0)
    {
      cout << "Wall 2!" << endl;
      base_cmd.linear.x = 1550;
      base_cmd.angular.z = 4.5;
      flag_wall_2 = 1;
    }

    if(scan_next < .40 && flag_box_1 == 1 && flag_wall_2 == 1 && flag_found == 0)
    {
      cout << "Box 2!" << endl;
      base_cmd.linear.x = 1550;
      base_cmd.angular.z = 4.5;
      flag_box_2 = 1;
    }

    if(scan_next > 0.50 && flag_wall_2 == 1 && flag_box_2 == 1)
    {
        cout << "Parking Spot Found!" << endl;
        flag_found = 1;
        base_cmd.linear.x = 1500;
        base_cmd.angular.z = 4.5;
        //cmd_vel_pub_.publish(base_cmd);
        //return;
    }


    }



    cout << scan_prev << endl;
    cout << scan_next << endl;
    cout << "Found beginning of box: " << flag_box_1 << endl;
    cout << "Found end of box: " << flag_box_2 << endl;

    /*
    if (flag_box_begin == 2 && flag_box_end == 2)
      {
	cout << "Parking Spot Found!" << endl;
	flag_found = 1;
	base_cmd.linear.x = 1500;
	base_cmd.angular.z = 3.6;
    cmd_vel_pub_.publish(base_cmd);
	return;
      }else
      {
	cout << "Searching for Parking Spot..." << endl;
    base_cmd.linear.x = 1550;
	base_cmd.angular.z = 3.6;
      };
    */

    cmd_vel_pub_.publish(base_cmd);
    ++vec_idx;
  };
};


int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "parking");
  ros::NodeHandle nh;
  dynamics_control car_control(nh);
  ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &dynamics_control::laserCallback, &car_control);
  //ros::Rate loop_rate(.1);
  ros::spin();

}
