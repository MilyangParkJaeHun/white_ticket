#!/usr/bin/env python
import socket
import rospy
import cv2
import numpy
import pyzbar.pyzbar as pyzbar


def decode():
	rospy.init_node('decode_node', anonymous=True)
	rate = rospy.Rate(10)
	while not rospy.is_shutdown():
		print('hi')
		rate.sleep()

if __name__ == '__main__':
	try:
		decode()
	except rospy.ROSInterruptException:
		pass
