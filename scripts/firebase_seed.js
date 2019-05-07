#!/usr/bin/env node

'use strict';
/**
 * This example demonstrates simple sending of messages over the ROS system.
 */

const rosnodejs = require('rosnodejs');
const std_msgs = rosnodejs.require('std_msgs').msg;
const firebase = require('firebase');

var config = {
    apiKey: "AIzaSyBNujE4Z4PQOXKorfo_ebcNkSbJ6TdPmEQ",
    authDomain: "white-ticket.firebaseapp.com",
    databaseURL: "https://white-ticket.firebaseio.com",
    storageBucket: "gs://white-ticket.appspot.com"
};

firebase.initializeApp(config);
console.log(firebase.name);
var database = firebase.database();

function talker() {
  rosnodejs.initNode('firebase_seed')
    .then((rosNode) => {
      let pub = rosNode.advertise('seed', std_msgs.String);
      let count = 0;
      const msg = new std_msgs.String();
      setInterval(() => {
        database.ref('/random').once('value').then((snapshot)=>{
          msg.data = snapshot.val().seed.toString();
          pub.publish(msg);
          rosnodejs.log.info('I said: [' + msg.data + ']');
          ++count;
        });
      }, 100);
    });
}

if (require.main === module) {
  talker();
}

