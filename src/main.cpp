#include <ros/ros.h>
#include <std_msgs/String.h>
#include "white_ticket/Distance.h"
#include <stdio.h>
#include <string>
using namespace std;

float distance_val = 0;
string seed;
string decode;

void distanceCallback(const white_ticket::Distance::ConstPtr& msg)
{
  distance_val = msg->distance;
  return;
}

void seedCallback(const std_msgs::String::ConstPtr& msg)
{
  seed = msg->data;
  return;
}

void decodeCallback(const std_msgs::String::ConstPtr& msg)
{
  decode = msg->data;
  return;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "main_node");
  ros::NodeHandle nh; 
  ros::Subscriber distance_sub = nh.subscribe("distance",20,distanceCallback);
  ros::Subscriber seed_sub = nh.subscribe("seed",20,seedCallback);
  ros::Subscriber decode_sub = nh.subscribe("decode",20,decodeCallback);
  ros::Rate loop_rate(10);

  while(ros::ok())
  {
    ROS_INFO("distance: %.2fcm", distance_val);
    ROS_INFO("seed %s  /   decode %s",seed.c_str(), decode.c_str());
    loop_rate.sleep();
		ros::spinOnce();
  }
  return 0;
}
