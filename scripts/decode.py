#!/usr/bin/env python
import socket
import rospy
import cv2
import numpy
import pyzbar.pyzbar as pyzbar
from std_msgs.msg import String

TCP_IP = '192.168.123.13'
TCP_PORT = 5005

def recvall(sock, count):
  buf = b''
  while count:
    newbuf = sock.recv(count)
    if not newbuf:
      return None
    buf += newbuf
    count -= len(newbuf)
  return buf

def decode():
  rospy.init_node('decode_node', anonymous=True)
  pub = rospy.Publisher('decode', String, queue_size=10)

  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.bind((TCP_IP, TCP_PORT))
  s.listen(True)

  conn, addr = s.accept()
  arr = []
  while True:
    len_length = int.from_bytes(conn.recv(1),byteorder='big')
    print("len_length : ", len_length)
    length = int.from_bytes(conn.recv(len_length), byteorder='big')
    stringData = recvall(conn, length)
    data = numpy.fromstring(stringData, dtype='uint8')
    gray=cv2.imdecode(data,0)
    print(type(gray))
    if(str(type(gray))!="<class 'NoneType'>"):
      decoded = pyzbar.decode(gray)
      print(len(decoded))
      if(decoded!=None and len(decoded)!=0):
        for d in decoded:
          decode_data = d.data.decode("utf-8")
          decode_data = decode_data.split('}')[0]
          barcode_data = decode_data.split('=')[1]
          #if(len(barcode_data)<2):
          # pub.publish('-1')
          # break
          print("data :",barcode_data)
          pub.publish(barcode_data)
          arr.append(barcode_data)
          break
      else:
          pub.publish('-1')
      cv2.imshow('SERVER',gray)
      if(cv2.waitKey(1) & 0xFF == ord('q')):
        print(arr)
        break
    else:
      print(gray)
      count += 1

if __name__ == '__main__':
  try:
    decode()
  except rospy.ROSInterruptException:
    pass

