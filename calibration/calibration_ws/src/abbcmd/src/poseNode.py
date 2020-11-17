#!/usr/bin/env python

import rospy as rp
from tf.listener import TransformListener
from geometry_msgs.msg import Pose, Point, Quaternion


rp.init_node("pose_publisher")
posePublish = rp.Publisher("/currentPose", Pose, queue_size=1)
tfListener = TransformListener()


rate=rp.Rate(10)
rp.loginfo("pose_publisher node started")
while not rp.is_shutdown():
    tfListener.waitForTransform('base_link', 'tool0', rp.Time(), rp.Duration(0.5))
    ptData = tfListener.lookupTransform('base_link', 'tool0', rp.Time())
    pt=Point(ptData[0][0],ptData[0][1],ptData[0][2])
    orient=Quaternion(ptData[1][0],ptData[1][1],ptData[1][2],ptData[1][3])
    posePublish.publish(Pose(pt,orient))
    rate.sleep()