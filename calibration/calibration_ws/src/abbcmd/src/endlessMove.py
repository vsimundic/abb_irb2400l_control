#!/usr/bin/env python

import rospy as rp
import numpy as np
from abblib.abbCtrl import abbRobot

rp.init_node("abb_endlessMove")
robot=abbRobot()

while(not rp.is_shutdown()):
    jointList=[]
    for i in xrange(10):
        x=1
        y=np.random.uniform(-1,1,1)
        z=np.random.uniform(1,1.5,1)
        jointList.append([x,y[0],z[0]])
    robot.move2Point(jointList)
    robot.jointAction([[0,0,0,0,0,0]])
    rp.sleep(1.5)
