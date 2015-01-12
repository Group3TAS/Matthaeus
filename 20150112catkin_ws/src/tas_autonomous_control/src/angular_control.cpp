#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include "matrix_import.h"
#include <math.h>

using std::vector;
using std::cout;
using std::endl;

vector< vector<float> > policy_mat;
ros::Publisher cmd_vel_pub_;
vector<float> ranges;

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  ranges = scan->ranges;
}

void nextCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
  //ROS_INFO("X Coordinate: %f", msg->pose.pose.position.x);
  //ROS_INFO("Y Coordinate: %f", msg->pose.pose.position.y);

  geometry_msgs::Twist base_cmd;
  int fastspeed=1575;
  int slowspeed=1550;

  float DX=0.05;
  float DY=0.05;
  float DTHETA=3.1415/18;

  float xstart=8;
  float ystart=4;

  int nx=340;
  int ny=360;

  float closedistance=0.4;
  float fardistance=1.5;
  int speed;
  // Laser scanner

  vector<float>::size_type n , i;
  n = ranges.size();
  float mindistance=999999;
  for (i = 0; i!=n; i++)
    {
      if (ranges[i] < mindistance)
    {
        mindistance=ranges[i];
     }
    }

  if(mindistance<closedistance)
  {
      speed=slowspeed;
  }
  else
  {
      if(mindistance>fardistance)
      {
          speed=fastspeed;
      }
      else
      {
          speed=slowspeed+(fastspeed-slowspeed)*(mindistance-closedistance)/(fardistance-closedistance);
      }
  }


  float x=msg->pose.pose.position.x;
  float y=msg->pose.pose.position.y;

  float z=msg->pose.pose.orientation.z;
  float w=msg->pose.pose.orientation.w;

  float theta=atan2(2*w*z,1-2*z*z);

  int xi;
  int yi;
  int thetai;
  int matx;
  int maty;

  float radius;




  //speed =slowspeed;

  //if(((x<21)&&(x>13))||((y<17)&&(y>10)))
  //{
  //    speed=fastspeed;
  //}


  if(x<xstart)
      x=xstart+3*DX;
  if(x>25)
      x=25-3*DX;
  if(y<ystart)
     y=ystart+3*DY;
  if(y>22)
      y=22-3*DY;


  while(theta<0)
      theta=theta+2*3.14159265;

  while(theta>2*3.14159265)
      theta=theta-2*3.14159265;


  if(theta<=DTHETA/2||theta>=2*3.14159265-DTHETA/2)
  {
      thetai=1;

  }
  else
  {
    thetai=round(theta/DTHETA+1);
  }

  xi=round((x-xstart-DX/2)/DX+1);
  yi=round((y-ystart-DY/2)/DY+1);

  maty=yi-1;
  matx=xi-1+(thetai-1)*nx;

  cout << " matx " << matx<< " maty " << maty << " xi " << xi << " yi " << yi << " thetai " << thetai << endl;
  cout << "speed"<< speed << " x " << x << " y " << y << " theta " << theta << endl;

  radius=policy_mat[matx][maty];

  if(radius<-0.1)
  {
      //rechts
      cout << "rechts" << endl;
      base_cmd.angular.z = -30;
  }
  else
  {
      if(radius>0.1)
      {
          //links
          cout << "links" << endl;
          base_cmd.angular.z = 30;
      }
      else
      {
          //gerade
          cout << "gerade" << endl;
          base_cmd.angular.z = 0;
      }
  }
  base_cmd.linear.x = speed;
  cmd_vel_pub_.publish(base_cmd);


}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "angular_control");
  //cout << "testmain";
  ros::NodeHandle n;

  policy_mat = matrix_import("pol.txt");
  cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  ros::Subscriber sub = n.subscribe("amcl_pose", 1000, nextCallback);
  ros::Subscriber sub_2 = n.subscribe("scan", 1000, laserCallback);
  ros::spin();

  return 0;
}
