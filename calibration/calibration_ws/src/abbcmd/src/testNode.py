#!/usr/bin/env python

import time as t
from copy import deepcopy
from math import pi

import numpy as np
import rospy as rp
import tf2_ros
from geometry_msgs.msg import Pose, Quaternion
from moveit_msgs.msg import Constraints, OrientationConstraint
from tf.listener import TransformListener
from tf.transformations import euler_from_quaternion, quaternion_from_euler

from abblib.abbCtrl import abbRobot, clear

rp.init_node("testNode")
robot=abbRobot()
(pt,orient)=robot.getEEPoint()
print "Current point [x,y,z]: ", pt
print "Current orientation [x,y,z,w]: ", orient
