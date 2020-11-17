#!/usr/bin/env python

import rospy as rp
from geometry_msgs.msg import Point, Quaternion
from abblib.abbCtrl import abbRobot
from abb2400_sim.srv import move_req
from tf.transformations import euler_from_quaternion as Eulerq

def serviceHandler(req):
    pt=[req.point.x,req.point.y,req.point.z]
    euler=Eulerq([req.orientation.x,req.orientation.y,req.orientation.z,req.orientation.w])
    #rp.loginfo("Got points:%f %f %f",pt[0],pt[1],pt[2])
    robot.move2Point([pt],[euler],end_effector='tool0')
    rp.sleep(1.5)
    return 0

rp.init_node("moveit_service")
moveService=rp.Service("move_req",move_req, serviceHandler)
robot=abbRobot()
rp.loginfo("moveit_service started")
rp.spin()