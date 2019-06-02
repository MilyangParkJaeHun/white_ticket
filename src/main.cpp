#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include "white_ticket/Distance.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
using namespace std;

const int LIMIT = 3;
const int ALL = 5;
const int SAME = 2;

const int DISTANCE_THRESHOLD = 5;

float distance_val = 100;
string ticket_id = "0";
string decode_ticket = "0";
string seed="-1";
string decode="-1";
vector<string> seeds(ALL, "-1");
vector<string> decodes(ALL, "-1");
string seed_now, decode_now;
int seed_cnt, decode_cnt;
int dp[ALL+1][ALL+1];

ros::Publisher pub;
std_msgs::Int16 display_mode;

void distanceCallback(const std_msgs::Float32::ConstPtr& msg)
{
  distance_val = msg->data;
  return;
}

void seedCallback(const std_msgs::String::ConstPtr& msg)
{
  seed = msg->data;
  return;
}

void decodeCallback(const std_msgs::String::ConstPtr& msg)
{
  string content = msg->data;
	string tmp = "";
	int idx = 0;
	while(content[idx]!=','){
		tmp += content[idx];
		idx++;
	}
	decode = tmp;
	decode_ticket = content[idx+1];
  return;
}

void ticketCallback(const std_msgs::String::ConstPtr& msg)
{
	ticket_id = msg->data;
}

void seed_chk(){
	if(seed_now == seed){
		seed_cnt++;
		if(seed_cnt == LIMIT){
			for(int i=ALL-1; i>=1; i--)
				seeds[i] = seeds[i-1];
			seeds[0] = seed_now;
		}
	} else{
		seed_now = seed;
		seed_cnt = 1;
	}
}

void decode_chk(){
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
}

void buf_print(){
	printf("seeds : ");
	for(int i=0;i<ALL;i++)
		printf("%s ", seeds[i].c_str());
	printf("\n");
	printf("decodes : ");
	for(int i=0;i<ALL;i++)
		printf("%s ", decodes[i].c_str());
	printf("\n");
}

void count(){
	for(int i=1;i<=ALL;i++){
		for(int j=1;j<=ALL;j++){
			if(seeds[i-1] != "-1" && decodes[j-1] != "-1" && seeds[i-1]==decodes[j-1])
				dp[i][j] = dp[i-1][j-1]+1;
			else
				dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
		}
	}
}

bool pass_chk(){
	if((decode != "-1") && (decode_ticket != ticket_id))
		return false;
	if(dp[ALL][ALL] >= SAME)
		return true;
	else
		return false;
}

void pass_action(){
	display_mode.data = 0;
	printf("PASS!!!\n");
	pub.publish(display_mode);
}

void not_pass_action(){
	display_mode.data = 1;
	printf("NOT PASS!!!\n");
	pub.publish(display_mode);
}

void take_away_action(){
	//display_mode.data = "take_away";
	printf("TAKE AWAY PHONE!\n");
	//pub.publish(display_mode);
}

void not_in_action(){
	display_mode.data = 4;
	printf("please in your phone\n");
	pub.publish(display_mode);
}

void in_action(){
	display_mode.data = 3;
	printf("IN\n");
	pub.publish(display_mode);
}

void out_of_range_action(){
	display_mode.data = 2;
	printf("your phone is out of range\n");
	printf("please in your phone\n");	
	pub.publish(display_mode);
}

	
int main(int argc, char **argv)
{
  ros::init(argc, argv, "main_node");
	ros::NodeHandle nh;
	pub = nh.advertise<std_msgs::Int16>("display",100);
  ros::Subscriber distance_sub = nh.subscribe("distance",20,distanceCallback);
  ros::Subscriber seed_sub = nh.subscribe("seed",20,seedCallback);
  ros::Subscriber decode_sub = nh.subscribe("decode",20,decodeCallback);
	ros::Subscriber ticket_sub = nh.subscribe("ticket",20,ticketCallback);
  ros::Rate loop_rate(10);

	bool pass = false;
	int in_cnt = 0;
  while(ros::ok())
  {
    //ROS_INFO("distance: %.2fcm", distance_val)lt target tutorial_generate_messages_py
    //ROS_INFO("seed %s  /   decode %s",seed.c_str(), decode.c_str());
		//printf("in_cnt : %d  distance : %f \n", in_cnt, distance_val);
		if(distance_val < DISTANCE_THRESHOLD){
			in_cnt++;
		}
		else{
			not_in_action();
			in_cnt = 0;	
		}
		if(in_cnt >= 5){
			in_action();
			clock_t in_time = clock();
			int out_cnt = 0;
			while(1){
				//printf("in_cnt : %d  distance : %f \n", in_cnt, distance_val);
				seed_chk();
				decode_chk();
				buf_print();
				count();
				if(!pass) pass = pass_chk();
				if(pass){
					pass_action();
					if(out_cnt > 4){
						printf("BYE!\n");
						pass = false;
						in_cnt = 0;
						decodes.assign(ALL,"-1");
						break;
					}else{
						take_away_action();
					}
				}else{
					int time = clock() - in_time;
					if(time > 150000){
						printf("time : %d\n",time);
						while(1){
							if(out_cnt > 4){
								not_pass_action();
								break;
							}else{
								not_pass_action();
								take_away_action();
							}
							if(distance_val >= DISTANCE_THRESHOLD) out_cnt++;
							else out_cnt = 0;
							loop_rate.sleep();
							ros::spinOnce();
						}
						decodes.assign(ALL,"-1");
						break;
					}else{
						if(out_cnt > 4){
							out_of_range_action();
							pass = false;
							in_cnt = 0;
							decodes.assign(ALL,"-1");
							in_time = clock();
							//break;
						}else{
							in_action();
						}
					}
				}
				if(distance_val >= DISTANCE_THRESHOLD) out_cnt++;
				else out_cnt = 0;
				loop_rate.sleep();
				ros::spinOnce();
			}
		}
		loop_rate.sleep();
		ros::spinOnce();
  }
  return 0;
}
