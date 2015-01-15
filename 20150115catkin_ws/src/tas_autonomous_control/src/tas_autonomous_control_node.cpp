#include "control/control.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "autonomous_control");
    control autonomous_control;

    ros::Rate loop_rate(50);
	
    while(ros::ok())
    {
        if(autonomous_control.control_Mode.data==0)
        {
            ROS_INFO("Manually Control!");
        }
        else
        {
            if(autonomous_control.control_Brake.data==1)
            {
                autonomous_control.control_servo.x=1500;
                autonomous_control.control_servo.y=1500;
            }
            else
            {
                ROS_INFO("Automatic Control!");
                //The car accepts a pwm-signal in the range [1200-1700]. 1500 means standstill. The following if-structures check 
                //whether the pwm-signal is within the allowed range.

                if(autonomous_control.cmd_linearVelocity>1700)
         
                {
					autonomous_control.cmd_linearVelocity=1700;
                }

                if(autonomous_control.cmd_linearVelocity<1200)
                {
					autonomous_control.cmd_linearVelocity=1200;
                }

				autonomous_control.control_servo.x = autonomous_control.cmd_linearVelocity;

                autonomous_control.control_servo.y = autonomous_control.cmd_steeringAngle;
            }

            autonomous_control.control_servo_pub_.publish(autonomous_control.control_servo);

        }

        ros::spinOnce();
        loop_rate.sleep();

    }

    return 0;

}
