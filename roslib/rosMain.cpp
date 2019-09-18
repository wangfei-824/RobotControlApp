//
//  rosMain.cpp
//  testSocket
//
//  Created by Lucky on 19/8/7.
//  Copyright © 2019年 Felix. All rights reserved.
//

#include "rosMain.h"
#include "ros.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Int8.h"

#include <iostream>
#include <pthread.h>
#include <unistd.h>


const double max_linear_speed = 0.8;
const double max_angular_speed = 1.57;

ros::NodeHandle nh;
geometry_msgs::Pose2D cmd_velocity;
std_msgs::Int8 cmd_action;

void (*poseCallBackImpl)(double,double,double,double,double,double);
//void poseCallback(const geometry_msgs::PoseStamped& pose);
//
//ros::Subscriber<geometry_msgs::PoseStamped> sub_pose("wheel_odom", poseCallback);
void poseCallback(const geometry_msgs::PoseStamped& pose);

ros::Subscriber<geometry_msgs::PoseStamped> sub_pose("wheel_odom", poseCallback);
ros::Publisher pub_cmd_velocity("target_velocity", &cmd_velocity);
ros::Publisher pub_cmd_action("cmd_action", &cmd_action);


void rosMainInit(char *server_hostname) {
    nh.initNode(server_hostname);
//    nh.subscribe(sub_pose);
    nh.advertise(pub_cmd_velocity);
//    nh.advertise(pub_cmd_action);
//    std::cout << server_hostname << std::endl;
}

void rosMainClose() {
    nh.getHardware()->close();
}

bool rosMainIsConnected() {
    return nh.getHardware()->isConnected();
}

void rosMainLoop() {
    nh.spinOnce();
}

void rosMainSendCmd(int cmd) {
    cmd_action.data = cmd;
    pub_cmd_action.publish(&cmd_action);
}

void rosMainSendCmdVelocity(double linear, double angular) {
    cmd_velocity.x = linear * max_linear_speed;
    cmd_velocity.y = angular * max_angular_speed;
    cmd_velocity.theta = 0;
    pub_cmd_velocity.publish(&cmd_velocity);
}

void rosMainSetPoseCallback(void (*PoseCallback)(double, double, double, double, double, double)) {
    poseCallBackImpl = PoseCallback;
}

//void poseCallback(const geometry_msgs::PoseStamped& pose) {
//    poseCallBackImpl(pose.pose.position.x, pose.pose.position.y, 0, 0, 0, pose.pose.position.z);
//}

void poseCallback(const geometry_msgs::PoseStamped& pose) {
    poseCallBackImpl(pose.pose.position.x, pose.pose.position.y, 0, 0, 0, pose.pose.position.z);
}
