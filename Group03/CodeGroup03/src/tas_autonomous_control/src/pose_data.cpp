#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include "wlan_data.h"

/*

Technik Autonomer Systeme

17 January 2015

Group 3

WLAN and Pose data reading and recording

*/

using std::vector;
using std::cout;
using std::endl;

void nextCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){

  // Retrieves size of /amcl_pose's covariance matrix
  // and saves it into a vector
  int x = msg->pose.covariance.size();
  vector<float> cov_vec, orient_vec;

  for (int i = 0; i != x ; i++)
  {
      cov_vec.push_back(msg->pose.covariance[i]);
      cout << cov_vec[i] << endl;
  }

  // Saves the quarternion data into a vector
  orient_vec.push_back(msg->pose.pose.orientation.z);
  orient_vec.push_back(msg->pose.pose.orientation.w);

  // Calls the following function to retrieve a strength reading
  // on all available WLAN signals and combines the readings to
  // the pose data into a simicolon separated line which is saved
  // into a text file
  wlan_data_write(msg->pose.pose.position.x,msg->pose.pose.position.y,cov_vec,orient_vec);

}

int main(int argc, char **argv)
{
  // Create Node
  ros::init(argc, argv, "pose_data_reader");

  ros::NodeHandle n;

  // Create a subscriber callback function each time pose data
  // is received
  ros::Subscriber sub = n.subscribe("amcl_pose", 1000, nextCallback);

  ros::spin();

  return 0;
}
