#include <iostream>
#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <vector>

/*

Technik Autonomer Systeme

17 January 2015

Group 3

This node performs the definition
of the car's initial position

*/

using std::vector;
using std::endl;
using std::cout;

int main(int argc, char **argv)
{
  // Naming of node
  ros::init(argc, argv, "initial_pose_pos_1");
  ros::NodeHandle nh;

  // Defining the publisher variable
  ros::Publisher initial_pose_pub = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose",1);

  // Defining the parameter variable of equivalent
  // message type of the /initialpose
  geometry_msgs::PoseWithCovarianceStamped pos_1;

  // Declaration of the covariance vector
  vector<float> covariance;
  vector<float>::size_type i;

  // Definition of the covariance vector values
  for(i = 0;i<36;i++)
    {
      covariance.push_back(0);
    }

  covariance[0] = 0.02;
  covariance[7] = 0.02;
  covariance[14] = 0.02;
  covariance[21] = 0.02;
  covariance[28] = 0.02;
  covariance[35] = 0.02;

  // Definition of the initial pose
  pos_1.pose.pose.position.x=11.704;
  pos_1.pose.pose.position.y=18.423;
  pos_1.pose.pose.orientation.z=-0.733;
  pos_1.pose.pose.orientation.w=0.6797;

  // Reading of covariance values into the parameter variable
  for(i = 0;i<36;i++)
    {
      pos_1.pose.covariance[i] = covariance[i];
    }

  // The publishing of the initial pose.  For an effective publish to occur
  // (i.e. reflected in RVIZ), the pose needs to be published many times.
    cout << "publish pose" << endl;
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    sleep(1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    sleep(1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
    initial_pose_pub.publish(pos_1);
}
