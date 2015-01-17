#include <iostream>
#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <vector>

using std::vector;
using std::endl;
using std::cout;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "initial_pose_pos_1");
  ros::NodeHandle nh;
  ros::Publisher initial_pose_pub = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose",1);

  geometry_msgs::PoseWithCovarianceStamped pos_1;
  
  vector<float> covariance;
  vector<float>::size_type i;

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

  pos_1.pose.pose.position.x=11.704;
  pos_1.pose.pose.position.y=18.423;
  pos_1.pose.pose.orientation.z=-0.733;
  pos_1.pose.pose.orientation.w=0.6797;

  for(i = 0;i<36;i++)
    {
      pos_1.pose.covariance[i] = covariance[i];
    }

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
