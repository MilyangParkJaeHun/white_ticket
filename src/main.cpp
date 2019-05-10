#include <ros/ros.h>
#include <std_msgs/String.h>
#include "white_ticket/Distance.h"
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

const int LIMIT = 5;
const int ALL = 3;
const int SAME = 2;

float distance_val = 0;
string seed;
string decode;
vector<string> seeds(ALL, "-1");
vector<string> decodes(ALL, "-1");
string seed_now, decode_now;
int speed_cnt, decode_cnt;
int dp[ALL+1][ALL+1];

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

		if(seed_now == seed){
			seed_cnt++;
			if(seed_cnt == LIMIT){
				for(int i=ALL-1; i>=1; i--)
					seeds[i] = seeds[i-1];
				seeds[0] = seed;
			}
		} else{
			seed_now = seed;
			seed_cnt = 1;
		}
		if(decode_now == decode){
			decode_cnt++;
			if(decode_cnt == LIMIT){
				for(int i=ALL-1;i>=1;i--)
					decodes[i] = decodes[i-1];
				decodes[0] = decode;
			}
		} else{
			decode_now = decode;
			decode_cnt = 1;
		}
		printf("seeds : ");
		for(int i=0;i<ALL;i++)
			printf("%s ", seeds[i].c_str());
		printf("\n");
		printf("decodes : ");
		for(int i=0;i<ALL;i++)
			printf("%s ", decodes[i].c_str());
		printf("\n");

		for(int i=1;i<=ALL;i++){
			for(int j=1;j<=ALL;j++){
				if(seeds[i-1] != "-1" && decodes[j-1] != "-1" && seeds[i-1]==decodes[j-1])
					dp[i][j] = dp[i-1][j-1]+1
				else
					dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
			}
		}
		if(dp[ALL][ALL] >= SAME)
			ROS_INFO("PASS!");
    loop_rate.sleep();
		ros::spinOnce();
  }
  return 0;
}
