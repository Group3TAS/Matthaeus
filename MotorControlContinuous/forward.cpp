#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "forward.h"


class RobotDriver
{
private:
  //! The node handle we'll be using
  ros::NodeHandle nh_;
  //! We will be publishing to the "/base_controller/command" topic to issue commands
  ros::Publisher cmd_vel_pub_;

public:
  //! ROS node initialization
  RobotDriver(ros::NodeHandle &nh)
  {
    nh_ = nh;
    //set up the publisher for the cmd_vel topic
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  }

  //! Loop forever while sending drive commands based on keyboard input
  bool driveKeyboard()
  {
	//get desired speed an steering angle from user
	int PWMFreq;
	int SteeringAngle;
	
	while (1) //read and verify user inputs
	{
		std::cout << "Please enter the desired PWM frequency.\n Minimum: 1200, Maximum: 1700, Standstill: 1500\n";
		std::cin >> PWMFreq;
		
		if (!cin) {
		    // input was not an integer
			std::cout << "Input must be an integer. Try again.";
			continue;
		}
		
		if (PWMFreq>1700 || PWMFreq<1200)
		{
			std::cout << "Input exceeds expected range. Try again.";
			continue;
		}
		
		std::cout << "Please enter the desired steering angle (degree).\n Allowed Range: [-30,30]\n";
		std::cin >> SteeringAngle;
		
		if (!cin) {
		    // input was not an integer
			std::cout << "Input must be an integer. Try again.";
			continue;
		}
		
		if (SteeringAngle>30 || SteeringAngle<-30)
		{
			std::cout << "Input exceeds expected range. Try again.";
			continue;
		}
		
		break;
		
	}
	
	std::cout << "Type a command and then press enter.  "
		"Use 'x' to exit.\n";

    //we will be sending commands of type "twist"
    geometry_msgs::Twist base_cmd;

    char cmd[50];
    while(nh_.ok()){

      std::cin.getline(cmd, 50);

      /*if(cmd[0]!='x'||cmd[0]!='')
      {
        std::cout << "unknown command:" << cmd << "\n";
        continue;
      }*/

      base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;  
 
      //move commands
	  base_cmd.linear.x = PWMFreq;
   	  base_cmd.angular.z = SteeringAngle;

      //quit
       if(cmd[0]=='x'){
        break;
      }

      //publish the assembled command
      cmd_vel_pub_.publish(base_cmd);
    }
    return true;
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;

  RobotDriver driver(nh);
  driver.driveKeyboard();
}
