#!/usr/bin/env node

'use strict';
/**
 * This example demonstrates simple sending of messages over the ROS system.
 */

const rosnodejs = require('rosnodejs');
const std_msgs = rosnodejs.require('std_msgs').msg;
const firebase = require('firebase');
const express = require('express');
const app = express();
var rand = 0

const msg = new std_msgs.String();

function getRandomInt(min, max){
	rand = Math.floor(Math.random() * (max - min)) + min;
	console.log(String(rand))
}


function nonce() {
	rosnodejs.log.info('chk1')
  rosnodejs.initNode('nonce_node')
    .then((rosNode) => {
      let pub = rosNode.advertise('seed', std_msgs.String);
      let count = 0;
      setInterval(() => {
          msg.data = String(rand);
          pub.publish(msg);
          rosnodejs.log.info('I said: [' + msg.data + ']');
      },100);
  });
}

if (require.main === module) {
	app.post('/post', (req, res) => {
		res.write(String(rand))
		res.end();
	});
  nonce();
	app.listen(3002, ()=>{
		console.log('port 3002');
		setInterval(() => {
			getRandomInt(0, 10000)
		}, 500);
	});
}

