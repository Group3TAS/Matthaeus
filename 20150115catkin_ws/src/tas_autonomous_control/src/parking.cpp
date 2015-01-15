#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "laser_processing.h"
#include <ctime>
#include <sys/time.h>
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
  float scan_next;
  int flag_found, flag_wall_1,flag_wall_2,flag_wall_3;
  int flag_box_1,flag_box_2;
  int flag_phase_1,flag_phase_2,flag_phase_3,flag_phase_4,flag_phase_5;
  int flag_pause;
  int angle_2,angle_3,angle_4;
  int PWM_2,PWM_3,PWM_4;
  float t_2,t_3,t_4;
  float delta_t;
  timeval begin;
    timeval end;
public:
  dynamics_control(ros::NodeHandle &nh)
  {
    nh_ = nh;
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    flag_found = 0;
    flag_wall_1 = 0;
    flag_wall_2 = 0;
    flag_wall_3 = 0;
    flag_box_1 = 0;
    flag_box_2 = 0;
    flag_pause = 0;

    // Parking Parameters

    PWM_2 = 1380;
    PWM_3 = 1380;
    PWM_4 = 1550;

    angle_2 = 30;
    angle_3 = -30;
    angle_4 = 20;

    t_2 = 1.7;
    t_3 = 1.2;
    t_4 = 0.5;

    flag_phase_1 = 0;
    flag_phase_2 = 0;
    flag_phase_3 = 0;
    flag_phase_4 = 0;
    flag_phase_5 = 0;
  };
  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {
    vector<float>::size_type x;
    vector<float> ranges;

    /*
    timeval begin;
    timeval end;
    gettimeofday(&begin, NULL);
    sleep(1.3);
    gettimeofday(&end, NULL);
    cout << "difference: " << (begin.tv_sec + begin.tv_usec/1.0e6) - (end.tv_sec + end.tv_usec/1.0e6) << endl;
    */

    ranges = scan->ranges;
    x = ranges.size();

    scan_next = ranges[x-1];

    if(scan_next > 0.50 && flag_wall_1 == 0 && flag_box_1 == 0 && flag_found == 0)
    {
      cout << "Wall 1!" << endl;
      base_cmd.linear.x = 1550;
      base_cmd.angular.z = 3;
      flag_wall_1 = 1;
    }

    if(scan_next < .40 && flag_box_1 == 0 && flag_wall_1 == 1 && flag_found == 0)
    {
      cout << "Box 1!" << endl;
      flag_box_1 = 1;
    }

    if (scan_next > 0.50 && flag_wall_1 == 1 && flag_box_1 == 1 && flag_found == 0)
    {
      cout << "Wall 2!" << endl;
                 // base_cmd.linear.x = 1380;
      flag_wall_2 = 1;
    }

    if(scan_next < .40 && flag_box_1 == 1 && flag_wall_2 == 1 && flag_found == 0)
    {
      cout << "Box 2!" << endl;
      flag_box_2 = 1;
    }

    if(scan_next > 0.50 && flag_wall_2 == 1 && flag_box_2 == 1 && flag_found == 0)
    {
        cout << "Parking Spot Found!" << endl;
        flag_found = 1;
    }

    if(flag_found == 1 && flag_phase_2 == 0)
    {
        cout << "Parking Spot Found!" << endl;
        if(flag_pause == 0)
        {
            base_cmd.linear.x = 1380;
            base_cmd.angular.z = 4.5;
            cmd_vel_pub_.publish(base_cmd);
            sleep(1);
            base_cmd.linear.x = 1500;
            base_cmd.angular.z = 4.5;
            cmd_vel_pub_.publish(base_cmd);
            sleep(1);
            flag_pause = 1;
        }
        flag_phase_2 = 1;
    }


    if(flag_phase_2 == 1 && flag_phase_3 == 0)
    {
        cout << "Phase 2" <<endl;
        base_cmd.linear.x = PWM_2;
        base_cmd.angular.z = angle_2;
        cmd_vel_pub_.publish(base_cmd);
        delta_t = 0;
        gettimeofday(&begin, NULL);
        while(delta_t < t_2){
            gettimeofday(&end, NULL);
            delta_t = (end.tv_sec + end.tv_usec/1.0e6) - (begin.tv_sec + begin.tv_usec/1.0e6);
        };
        flag_phase_3 = 1;
    }

    if(flag_phase_3 == 1 && flag_phase_4 == 0)
    {
        cout << "Phase 3" <<endl;
        base_cmd.linear.x = PWM_3;
        base_cmd.angular.z = angle_3;
        cmd_vel_pub_.publish(base_cmd);
        delta_t = 0;
        gettimeofday(&begin, NULL);
        while(delta_t < t_3){
            gettimeofday(&end, NULL);
            delta_t = (end.tv_sec + end.tv_usec/1.0e6) - (begin.tv_sec + begin.tv_usec/1.0e6);
        };
        flag_phase_4 = 1;
    }

    if(flag_phase_4 == 1 && flag_phase_5 == 0)
    {
        cout << "Phase 4" <<endl;
        base_cmd.linear.x = PWM_4;
        base_cmd.angular.z = angle_4;
        cmd_vel_pub_.publish(base_cmd);
        delta_t = 0;
        gettimeofday(&begin, NULL);
        while(delta_t < t_4){
            gettimeofday(&end, NULL);
            delta_t = (end.tv_sec + end.tv_usec/1.0e6) - (begin.tv_sec + begin.tv_usec/1.0e6);
        };
        flag_phase_5 = 1;
    }

    if(flag_phase_5 == 1)
    {
        cout << "Phase 5" <<endl;
        base_cmd.linear.x = 1500;
        base_cmd.angular.z = 0;
        cmd_vel_pub_.publish(base_cmd);
    }


    cout << scan_next << endl;
    cout << "Found beginning of box: " << flag_box_1 << endl;
    cout << "Found end of box: " << flag_box_2 << endl;

    cmd_vel_pub_.publish(base_cmd);
    return;
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
