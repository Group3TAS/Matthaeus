#include <ros/ros.h>
#include <iostream>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include "matrix_import.h"
#include <math.h>
#include "control/control.h"

using std::vector;
using std::cout;
using std::endl;

vector< vector<float> > policy_mat;
ros::Publisher cmd_vel_pub_;
vector<float> ranges;

char CheckForCollision(vector<float> data){ //this function process laser data and checks for collisions
  
    int length=data.size();
    int i=0;
    int j=0;
    float theta=0;
    int flag=0; //indicates collision
    int direction=0; //indicates where the obstacle is located (left/right)
    float d_min=0.6; //minimum distance to an obstacle
    float b=0.5; //width of the car 
    float phi=atan(b/(2*d_min));
    float BypassAngle=phi=atan(1.5/(2*d_min));
    
    float p=(phi/PI)*length;
    float p0=(BypassAngle/PI)*length;
    int p2=ceil(p);
    int pb=ceil(p0);
    int pw=floor(2.0/9.0*length);
    
    float data_relevant[2*p2+1];
    float left_wall[pw];
    float right_wall[pw];
    float data_bypass[2*pb+1];
    
    int counter=0; //counts how many measurement points imply a critical distance to an obstacle
    int leftside=0;
    int rightside=0;
    int leftwall=0;
    int rightwall=0;
    int WallFlag=0;
    
    for(i=0;i<=2*p2;i++){
    
      data_relevant[i]=data[length/2-p2+i];
            
      if (data_relevant[i]<=d_min){
        counter++;
      }
    }
    
    for(i=0;i<=2*pb;i++){
      data_bypass[i]=data[length/2-p2+i];
      
      if (i<pb){
        rightside=rightside+data_bypass[i];
      } 
      
      if (i>pb){
        leftside=leftside+data_bypass[i];
      }
    }
    
    for(i=0;i<pw;i++){
    
      left_wall[i]=data[length-1-i];
      right_wall[i]=data[i];
      
      theta=atan(float(i)/float(pw-1));
      
      if(right_wall[i]<0.3/cos(theta*PI/180)){
        rightwall++;
      }
      
      if(left_wall[i]<0.3/cos(theta*PI/180)){
        leftwall++;
      }
    }
    
    if (rightwall>2){
      WallFlag=1; //the car is too close to the wall (right hand side)
    }    
    
    if (leftwall>2){
      WallFlag=-1; ////the car is too close to the wall (left hand side)
    }     
          
    if(leftside>rightside){
      direction=-1;
    }
    
    if(leftside==rightside){
      direction=0;
    }
    
    if(leftside<rightside){
      direction=1;
    }
    
    //if the minimum distance is undercut a critical amount of times, a near obstacle is assumed
    if (counter>2){
      flag=1; //it is very likely that an obstacle is right in front of the car
    }
    
    if(flag==0 && WallFlag==0){
      return 'x';
    }
    
    if(flag==0 && WallFlag==1){
      return 'a'; //wall on the right side (no obstacle)
    }
    
    if(flag==0 && WallFlag==-1){
      return 'b'; //wall on the left side (no obstacle)
    }
    
    if(flag==1 && WallFlag==-1){
      return 'r';
    }
        
    if(flag==1 && WallFlag==1){
      return 'l';
    }

    if(flag==1 && direction==-1){
      return 'l';
    }
    
    if(flag==1 && direction==1){
      return 'r';
    }
    
    if(flag==1 && direction==0){
      return 'm';
    }
  } 

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  ranges = scan->ranges;
}

void nextCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
  //ROS_INFO("X Coordinate: %f", msg->pose.pose.position.x);
  //ROS_INFO("Y Coordinate: %f", msg->pose.pose.position.y);

  geometry_msgs::Twist base_cmd;
  int fastspeed=1580;
  int slowspeed=1553;

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

    int maxangle=20;


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
      base_cmd.angular.z = -maxangle;
  }
  else
  {
      if(radius>0.1)
      {
          //links
          cout << "links" << endl;
          base_cmd.angular.z = maxangle;
      }
      else
      {
          //gerade
          cout << "gerade" << endl;
          base_cmd.angular.z = 0;
      }
  }
  
  //check for obstacles and adapt control signals
    char Collision=CheckForCollision(ranges);
    cout<<Collision<<endl;
    
    switch (Collision)
    {
    case 'x':
      //Aktion1
      break;
      
    case 'l':
      //Aktion2
      break;
      
    case 'r':
      //Aktion3
      break;
                  
    case 'm': //treated like 'r'
      //Aktion4
      break;
      
    case 'a':
      //Aktion5
      break;
            
    case 'b':
      //Aktion6
      break;
      
    default:
      cout<<"CheckForCollision failed"<<endl;
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
