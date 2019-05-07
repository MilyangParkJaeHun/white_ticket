import socket
import cv2
import numpy
import time

def int_to_bytes(x):
	return x.to_bytes((x.bit_length() + 7) // 8, 'big')

with open('/home/park/catkin_ws/src/white_ticket/scripts/tcp_setting.txt') as f:
	TCP_IP = f.readline()[:-1]
	TCP_PORT = int(f.readline()[:-1]
#TCP_IP = '192.168.43.192'
#TCP_PORT = 5004
BUFSIZE = 1024
WIDTH = 300
HEIGHT = 300

sock = socket.socket()
sock.connect((TCP_IP, TCP_PORT))

capture = cv2.VideoCapture(0)
capture.set(cv2.CAP_PROP_FRAME_WIDTH, WIDTH)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, HEIGHT)
while True:
	ret, frame = capture.read()
	encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
	result, imgencode = cv2.imencode('.jpg', frame, encode_param)
	data = numpy.array(imgencode)
	stringData = data.tostring()
	#send
	send_time = time.time()
	length = int_to_bytes(len(stringData))
	len_length = int_to_bytes(len(length))
	sock.send(len_length)
	sock.send(length)
	sock.send(stringData)
	decimg=cv2.imdecode(data,1)
	#cv2.imshow('CLIENT',decimg)
	if cv2.waitKey(1)>0:
		sock.close()
		break
capture.release()
cv2.destroyAllWindows() 
