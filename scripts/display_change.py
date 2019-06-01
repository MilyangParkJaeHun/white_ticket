#!/usr/bin/env python
import rospy
import cv2
import sys
from std_msgs.msg import Int16
from std_msgs.msg import String
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw

display_mode = 0
picture_name = "00.jpg"
img_path = '/home/park/catkin_ws/src/white_ticket/scripts/images/'
qr_cnt = 0
ticket_id = 0
before_mode = -1
ticket_row = 0
ticket_number = 0
phone_number = ""

def make_img():
	img = Imgae.open(img_path+"TL.jpeg")
	draw = ImageDraw.Draw(img)
	font = ImageFont.truetype("FreeSansBold.ttf",25)
	draw.text((349, 413),phone_number,(255,255,255), font=font)
	draw.text((603, 413),seat_row,(255,255,25), font=font)
	draw.text((650, 413),seat_num,(255,255,25), font=font)
	img.save(img_path+"accept.jpg")
	

def ticket_callback(data):
	ticket_id = data.data

def display_callback(data):
	display_mode = data.data
	if(display_mode != 3):
		qr_cnt = 0
	if(display_mode == 0):
		if(before_mode != 0):
			make_img()
		picture_name = img_path+"accept.jpg"
	elif(display_mode == 1):
		picture_name = img_path+"fail.jpg"
	elif(display_mode == 2):
		picture_name = img_path+"disconnect.jpg"
	elif(display_mode == 3):
		picture_name = img_path+"qr/"+str(qr_cnt)+".jpg"
		qr_cnt = (qr_cnt + 1) % 22
	elif(display_mode == 4):
		picture_name = img_path+'wait'+str(ticket_id)+'.jpg'
	before_mode = display_mode

def decode_callback(data):
	decode = data.data
	decode = decode.split(',')
	ticket_row = decode[2]
	ticket_number = decode[3]
	phone_number = decode[4]

def main():
	global display_mode
	rospy.init_node('display_node', anonymous=True)
	r = rospy.Rate(10)
	display_sub = rospy.Subscriber('display', Int16, display_callback)
	ticket_sub = rospy.Subscriber('ticket', String, ticket_callback)
	decode_sub = rospy.Subscriber('decode', String, decode_callback)
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
