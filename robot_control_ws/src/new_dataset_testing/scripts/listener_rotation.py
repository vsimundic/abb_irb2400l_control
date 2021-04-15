#! /usr/bin/env python

import rospy
from std_msgs.msg import Float64
from testing_msgs.msg import AngleRotation
import csv
import sys

class WriterRotation():
    def __init__(self, filename):
        self.f = open("/home/valentin/FAKS/HDR/dataset_new/results/" + filename, 'a')
        self.sub_ = rospy.Subscriber("/dataset_testing/angle_rotation", AngleRotation, self.cb_operator_speed)
        rospy.on_shutdown(self.shutdownHook)
        print("Initialized.")

    def cb_operator_speed(self, msg):
        # print(msg)
        data = 0
        if msg.robotVelocity > 0:
            data = 0
        else:
            data = 1
        
        rospy.loginfo("Writing: {0},{1}".format(msg.header.stamp, data))
        self.f.write("{0},{1}\n".format(msg.header.stamp, data))

    def shutdownHook(self):
        rospy.loginfo("Shutting down...")
        self.f.close()


if __name__ == '__main__':
    rospy.init_node("listener_rotation", anonymous=True)
    
    args = rospy.myargv(argv=sys.argv)
    if len(args) != 2:
        print("ERROR: filename not provided")
        sys.exit(1)

    filename = args[1]
    x = WriterRotation(filename)

    rospy.spin()
