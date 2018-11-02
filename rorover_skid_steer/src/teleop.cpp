/*-----------------------------------------------------------\\
||                                                           ||
||                 LIDAR fusion GNC project                  ||
||               ----------------------------                ||
||                                                           ||
||    Surrey Space Centre - STAR lab                         ||
||    Space Engineering MSc disseration                      ||
||    (c) Pete Blacker 2016                                  ||
||    Pete dot Blacker at Gmail dot com                      ||
||                                                           ||
\\-----------------------------------------------------------//

locomotion.cpp

Entry point of the lidar scan ros node. 
-----------------------------------------

This ROS node takes care of performing 3D lidar scans with
camera data. The node published various topics containing the
mesh data, and can also be set to record each mesh to a file
for later analysis.

A single scan can be started from the command line or the node
will setup a scan service avaiable to the ROS network.

Topics avilable are:
--------------------

sme_awareness/bare-mesh
sme_awareness/bare-simplified-mesh
sme_awareness/color-mesh
sme_awareness/color-simplified-mesh
sme_awareness/scan-progress-monitor

Services available are:
-----------------------
sme_awareness/scan

-------------------------------------------------------------*/
#include <stdio.h>
#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/Float64.h"

#define SERVO_MID			307//384
#define SERVO_RANGE			10

#define N_SERVO_MID			322
#define N_SERVO_RANGE			102

#define SERVO_MIN			SERVO_MID - SERVO_RANGE
#define SERVO_MAX			SERVO_MID + SERVO_RANGE

//#define SERVO_MID			(SERVO_MIN + SERVO_MAX) / 2
//#define SERVO_HALF_RANGE	(SERVO_MAX - SERVO_MIN) / 2

ros::Publisher servoPublisher;
ros::Publisher cmdVelPublisher;
ros::Publisher hookPublisher;

std::vector<int> prevButtons;

/*
 * locomotion.cpp
 *
 *  Created on: 30 Oct 2016
 *      Author: pete
 */

void joystickMsgHandler(sensor_msgs::Joy stickInput)
{
  float axisX = stickInput.axes[3];
  float axisY = stickInput.axes[4];

  // create Twist message and publish on /cmd_vel topic
  geometry_msgs::Twist cmdVel;
  cmdVel.linear.x = axisY;
  cmdVel.angular.z = 0.0 - axisX;
  cmdVelPublisher.publish(cmdVel);

  // check for button events
  std::vector<int> events;
  for (int b=0; b<prevButtons.size() && b<stickInput.buttons.size(); ++b)
  {
    events.push_back(stickInput.buttons[b] - prevButtons[b]);
  }
  prevButtons = stickInput.buttons;

  if (events.size() >= 4)
  {
	std_msgs::Float64 hookMsg;

	  if (events[0] == 1)
          {
	    hookMsg.data = 2.8;
	    hookPublisher.publish(hookMsg);
	    printf("button 0 down event\n");
          }

	  if (events[1] == 1)
          {
	    hookMsg.data = 0.0;
	    hookPublisher.publish(hookMsg);
	    printf("button 1 down event\n");
          }

	  if (events[2] == 1)
          {
	    hookMsg.data = 2.3;
	    hookPublisher.publish(hookMsg);
	    printf("button 2 down event\n");
          }
  }
}

int main(int argc, char **argv)
{
	printf("--[Starting Locomotion Node]--\n");

	// setup ros for this node and get handle to ros system
	ros::init(argc, argv, "locomotion");
	ros::start();

	// set 100Hz loop rate for processing ros messages
	ros::Rate loop_rate(100);

	// get parameters
	ros::NodeHandle pn("~");

	std::string hookTopic, cmdVelTopic;
	pn.param<std::string>("hook_topic", hookTopic, "/default_topic");
	pn.param<std::string>("cmd_vel_topic", cmdVelTopic, "/cmd_vel");

	//prevButtons = std::vector<int>();

	// get handle for this node
	ros::NodeHandle n;

	// setup servo publisher object to send messages to the servo pwm node
	cmdVelPublisher = n.advertise<geometry_msgs::Twist>(cmdVelTopic, 10);

        hookPublisher = n.advertise<std_msgs::Float64>(hookTopic, 10);

	// subscribe to the joy topic for joystick events.
	ros::Subscriber joySub = n.subscribe("joy", 100, joystickMsgHandler);


	// create a Int16MultiArray message
	std_msgs::Int16MultiArray positions;

	float Angle = 0;

	// loop continuously writing servo positions
	while (ros::ok())
	{
		/*Angle += 3.141592654 / 10;

		float Pos = (sin(Angle)+1) / 2;

		int value = 150 + (Pos * 450.0);
		int invValue = 600 - (Pos * 450.0);


		positions.data.clear();

		positions.data.push_back(value);
		positions.data.push_back(value);
		positions.data.push_back(invValue);
		positions.data.push_back(invValue);

		servoPublisher.publish(positions);*/

		loop_rate.sleep();

		ros::spinOnce();
	}

	return 0;
}

