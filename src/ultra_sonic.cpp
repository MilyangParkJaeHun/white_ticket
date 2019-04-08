#include <ros/ros.h>
#include "wiringPi.h"
#include "white_ticket/Distance.h"

#define TRIG 5
#define ECHO 4

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ultra_sonic");
  ros::NodeHandle nh;

  ros::Publisher pub = nh.advertise<white_ticket::Distance>("distance",100);
  ros::Rate loop_rate(100);

  white_ticket::Distance msg;

  if(wiringPiSetup()==-1)
  return 1;

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  while(ros::ok())
  {
    digitalWrite(TRIG, LOW);
    usleep(2);
    digitalWrite(TRIG, HIGH);
    usleep(20);

    digitalWrite(TRIG, LOW);
    while(digitalRead(ECHO) == LOW);
    long startTime = micros();
    while(digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;

    float distance = (float)travelTime / 58;
    ROS_INFO("Distance: %.2fcm", distance);

    msg.distance = distance;

    ROS_INFO("data = %.2f\n", msg.distance);

    pub.publish(msg);
    loop_rate.sleep();
  }
  return 0;
}

