#!/usr/bin/env python
import rospy
import cv2
import sys
from std_msgs.msg import Int16

display_mode = 0

def display_callback(data):
	global display_mode
	display_mode = data.data

def main():
	global display_mode
	rospy.init_node('display_node', anonymous=True)
	r = rospy.Rate(10)
	sub = rospy.Subscriber('display', Int16, display_callback)
	w,h = 640, 480
	while True:
		filename = '/home/park/catkin_ws/src/white_ticket/scripts/images/'+str(display_mode)+'.jpg'
		img = cv2.imread(filename,cv2.IMREAD_COLOR)
		img = cv2.resize(img, (w,h), interpolation=cv2.INTER_CUBIC)
		cv2.imshow('img',img)
		key = cv2.waitKey(1)
		if key == 27:
			break
	rospy.spin()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	main()
