#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "laser_processing.h"
#include "amcl_processing.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>

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
  vector<float> ranges_vec;
  vector<float> covariance_vec;
  int flag_laser;
  int flag_pose;
public:
  dynamics_control(ros::NodeHandle &nh)
  {
    nh_ = nh;
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    flag_laser = 0;
    flag_pose = 0;
  }
  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {
    vector<float>::size_type x;
    vector<float> ranges;
    ranges = scan->ranges;
    ranges_vec = ranges;

    if (flag_pose == 1)
    {
        localize();
        flag_laser = 0;
    };
    flag_laser = 1;
  }
  void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
  {
      int x = msg->pose.covariance.size();
      vector<float> cov_vec;
      for (int i = 0; i != x ; i++)
      {
          cov_vec.push_back(msg->pose.covariance[i]);
      }
      covariance_vec = cov_vec;
      cout << "Function 2" << endl;

      if (flag_laser == 1)
      {
          localize();
          flag_pose = 0;
      }
      flag_pose = 1;
  }
  void localize()
  {
      cout << "Localize!" << endl;
      vector<float> ranges = ranges_vec;
      vector<float> cov_vec = covariance_vec;
      float test = process_laser_data(ranges);
      int found = covariance_processing(cov_vec);

      cout << test << endl;

      if(found == 1)
      {
          cout << "Found!" << endl;
          base_cmd.linear.x = 1500;
          base_cmd.angular.z = 0;
      }

      else {if(test == 0)
      {
          base_cmd.linear.x = 1500;
          base_cmd.angular.z = 0;
      }
      else if (test == 1)
      {
          base_cmd.linear.x = 1550;
          base_cmd.angular.z = -30 ;
      }
      else if (test == 2)
      {
          base_cmd.linear.x = 1550;
          base_cmd.angular.z = 30;
      } else if (test == 3)
              base_cmd.linear.x = 1550;
              base_cmd.angular.z = 0;
      }
      cmd_vel_pub_.publish(base_cmd);
  }
};


int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "car_dynamics");
  ros::NodeHandle nh;
  dynamics_control car_control(nh);
  cout << "main" << endl;
  ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &dynamics_control::laserCallback, &car_control);
  cout << "main 2" << endl;
  ros::Subscriber pose_sub = nh.subscribe("amcl_pose", 1000, &dynamics_control::poseCallback, &car_control);
  ros::spin();

}
