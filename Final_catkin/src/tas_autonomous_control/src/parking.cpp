#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include <ctime>
#include <sys/time.h>

/*

Technik Autonomer Systeme

17 January 2015

Group 3

This node carries out the "Parking Task".

*/

using std::vector;
using std::cout;
using std::endl;

class dynamics_control
{
private:
  // Declaration of variables to be used within the task.
  ros::NodeHandle nh_;
  ros::Publisher cmd_vel_pub_;
  ros::Subscriber laser_sub;
  geometry_msgs::Twist base_cmd;
  float scan_next;
  int flag_wall_1,flag_wall_2,flag_wall_3;
  int flag_box_1,flag_box_2;
  int flag_phase_1,flag_phase_2,flag_phase_3,flag_phase_4,flag_phase_5;
  int angle_1,angle_2,angle_3,angle_4,angle_5;
  int PWM_1,PWM_2,PWM_3,PWM_4,PWM_5;
  float t_2,t_3,t_4;
  float delta_t;
  timeval begin;
  timeval end;
  float dist_min, dist_max;
public:
  dynamics_control(ros::NodeHandle &nh)
  {
    // Constructor

    // Passes the node handle initiated in the main function
    nh_ = nh;

    // Defines the publisher variable to one of /cmd_vel
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    // Distance decision parameters from car to object
    dist_min = 0.45; // If smaller, box found
    dist_max = 0.60; // If larger, wall

    // Discovery Flags
    flag_wall_1 = 0; // Wall from start position to box 1
    flag_wall_2 = 0; // Wall between box 1 and box 2, a.k.a. parking spot
    flag_wall_3 = 0; // Wall after box 2
    flag_box_1 = 0; // First box
    flag_box_2 = 0; // Second box

    // Parking Flags
    flag_phase_1 = 0; // Car has found both boxes
    flag_phase_2 = 0; // Car reverse with wheels pointed toward wall
    flag_phase_3 = 0; // Car reverse with wheels pointed toward hall
    flag_phase_4 = 0; // Car's final foward adjustment
    flag_phase_5 = 0; // Car stops

    // Velocity (in PWM) Parameters per phase
    PWM_1 = 1550; // Forward
    PWM_2 = 1380; // Reverse
    PWM_3 = 1380; // Reverse
    PWM_4 = 1550; // Forward
    PWM_5 = 1500; // Stop

    // Angle parameters per phase
    angle_1 = 3; // Straight ahead with a compensation value
    angle_2 = 30; // While in reverse, wheels point toward wall 
    angle_3 = -30; // While in reverse, wheels point toward hall
    angle_4 = 20; // Final forward adjust, wheels point toward wall
    angle_5 = 0; // Wheels straight ahead

    // Lengths of time for actions carried out in phases 2-4
    t_2 = 1.7; // Reverse time while wheels pointed toward wall
    t_3 = 1.2; // Reverse time while wheels pointed toward hall
    t_4 = 0.5; // Forward time for final forward adjustment

  };
  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {
    // Retrieval of laser scanner vector information
    vector<float>::size_type x;
    vector<float> ranges;
    ranges = scan->ranges;
    x = ranges.size();

    // Obtainment of laser readings for distance on left of car
    scan_next = ranges[x-1];

    // Discovery of initial wall
    if(scan_next > dist_max && flag_wall_1 == 0 && flag_box_1 == 0 && flag_phase_1 == 0)
    {
      cout << "Wall 1!" << endl;
      base_cmd.linear.x = PWM_1;
      base_cmd.angular.z = angle_1;
      flag_wall_1 = 1;
    }

    // If a distance is recored smaller than minimum
    // distance after discovery of initial wall, box 1 is found.
    if(scan_next < dist_min && flag_box_1 == 0 && flag_wall_1 == 1 && flag_phase_1 == 0)
    {
      cout << "Box 1!" << endl;
      flag_box_1 = 1;
    }

    // If box 1 has been found and a reading is greater
    // than the maximum defined distance, the wall between 
    // box 1 and box 2 has been found, a.k.a. the parking spot.
    if(scan_next > dist_max && flag_wall_1 == 1 && flag_box_1 == 1 && flag_phase_1 == 0)
    {
      cout << "Wall 2!" << endl;
      flag_wall_2 = 1;
    }

    // If the parking spot has been found and a reading is smaller
    // than the minimum defined distance, box 2 has been found.
    if(scan_next < dist_min && flag_box_1 == 1 && flag_wall_2 == 1 && flag_phase_1 == 0)
    {
      cout << "Box 2!" << endl;
      flag_box_2 = 1;
    }

    // If box 2 has been found and a reading is greater than the maximum
    // distance, the wall following box 2 has been discovered.
    if(scan_next > dist_max && flag_wall_2 == 1 && flag_box_2 == 1 && flag_phase_1 == 0)
    {
        cout << "Parking Spot Found!" << endl;
	// Allows to enter the final stage of phase 1
        flag_phase_1 = 1;
    }

    // If the car has just passed the second box,
    // it is set to prepare itself for the initial reverse.
    if(flag_phase_1 == 1 && flag_phase_2 == 0)
    {
        cout << "Phase 1" << endl;
	// The following commands are required in order
	// for the car to be able to go into reverse
        base_cmd.linear.x = 1380; // Reverse
        base_cmd.angular.z = 4.5;
        cmd_vel_pub_.publish(base_cmd);
        sleep(1);
        base_cmd.linear.x = 1500; // Stop
        base_cmd.angular.z = 4.5;
        cmd_vel_pub_.publish(base_cmd);
        sleep(1);

	// Flag to allow beginning of phase 2
        flag_phase_2 = 1;
    }

    // If the car is stopped following finding the second box,
    // it begins to reverse with wheels pointing toward wall
    if(flag_phase_2 == 1 && flag_phase_3 == 0)
    {
        cout << "Phase 2" <<endl;
	// Commands to have car go into reverse
	// at a velocity and angle defined in constructor.
        base_cmd.linear.x = PWM_2;
        base_cmd.angular.z = angle_2;
        cmd_vel_pub_.publish(base_cmd);

	// The following lines allow for a defined amount of time
	// to transpire with the above published commands. 
        delta_t = 0;
        gettimeofday(&begin, NULL);
        while(delta_t < t_2){
            gettimeofday(&end, NULL);
            delta_t = (end.tv_sec + end.tv_usec/1.0e6) - (begin.tv_sec + begin.tv_usec/1.0e6);
        };
  
	// Flag to allow the beginning of phase 3
        flag_phase_3 = 1;
    }

    // The car continues in reverse, yet with
    // the wheels pointing toward the hall
    if(flag_phase_3 == 1 && flag_phase_4 == 0)
    {
        cout << "Phase 3" <<endl;
	// The reverse velocity and angle of phase
	// 3 are defined above in the constructor.
        base_cmd.linear.x = PWM_3;
        base_cmd.angular.z = angle_3;
        cmd_vel_pub_.publish(base_cmd);

	// The following lines allow for a defined amount of time
	// to transpire with the above published commands. 
        delta_t = 0;
        gettimeofday(&begin, NULL);
        while(delta_t < t_3){
            gettimeofday(&end, NULL);
            delta_t = (end.tv_sec + end.tv_usec/1.0e6) - (begin.tv_sec + begin.tv_usec/1.0e6);
        };

	// Flag to allow the beginning of phase 4
        flag_phase_4 = 1;
    }

    // If the car has performed its two reverse actions,
    // it needs to then perform its final forward adjustment.
    if(flag_phase_4 == 1 && flag_phase_5 == 0)
    {
        cout << "Phase 4" <<endl;

	// The forward velocity and angle for the final
	// forward adjustment are defined in the constructor.
        base_cmd.linear.x = PWM_4;
        base_cmd.angular.z = angle_4;
        cmd_vel_pub_.publish(base_cmd);

	// The following lines allow for a defined amount of time
	// to transpire with the above published commands. 
        delta_t = 0;
        gettimeofday(&begin, NULL);
        while(delta_t < t_4){
            gettimeofday(&end, NULL);
            delta_t = (end.tv_sec + end.tv_usec/1.0e6) - (begin.tv_sec + begin.tv_usec/1.0e6);
        };

	// Flag to allow the beginning of phase 5
        flag_phase_5 = 1;
    }

    // Once the car has performed its final foward adjustment,
    // it is then parked and needs to stop.
    if(flag_phase_5 == 1)
    {
        cout << "Phase 5" <<endl;
	// Commands to tell the car to stop with straight wheels.
        base_cmd.linear.x = PWM_5;
        base_cmd.angular.z = angle_5;
        cmd_vel_pub_.publish(base_cmd);
    }

    // Metrics continually published to inform user of the status
    // of the car's discovery of the parking spot.
    cout << "Distance: " << scan_next << endl;
    cout << "Found wall 1: " << flag_wall_1 << endl;
    cout << "Found wall 2: " << flag_wall_2 << endl;
    cout << "Found beginning of box 1: " << flag_box_1 << endl;
    cout << "Found beginning of box 2: " << flag_box_2 << endl;

    // Publishes defined angle and velocity
    cmd_vel_pub_.publish(base_cmd);
    return;
  };
};

int main(int argc, char** argv)
{
  //initializes the ROS node
  ros::init(argc, argv, "parking");
  ros::NodeHandle nh;

  // Defines a variable of the class dynamics_control
  dynamics_control car_control(nh);

  // Each time laser data is published, the subscriber calls the function
  // laserCallback within the dynamics_control class.  This function
  // performs the parking task.
  ros::Subscriber laser_sub = nh.subscribe("scan", 1000, &dynamics_control::laserCallback, &car_control);
  ros::spin();

}
