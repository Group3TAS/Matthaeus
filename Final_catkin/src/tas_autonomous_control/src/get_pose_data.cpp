//The following node implements a subscriber to the position data. Every time new data are retrieved,
//a function is invoked, which writes the data to a text file. This node is utilized for test purposes
//Author: Group03

#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include "process_pose_data.h"
#include <time.h>

using std::vector;
using std::cout;
using std::endl;

void nextCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){

  //get current time
  time_t t;
  t=time(0);

  //this function writes the retrieved data to a text file (a time stamp is appended)
  position_data_write(msg->pose.pose.position.x,msg->pose.pose.position.y,t); 
}

int main(int argc, char **argv)
{
  //init the ROS node
  ros::init(argc, argv, "pose_data_reader");

  ros::NodeHandle n;

  //subscribe to position data
  ros::Subscriber sub = n.subscribe("amcl_pose", 1000, nextCallback);

  ros::spin();

  return 0;
}
