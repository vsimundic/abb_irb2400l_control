#! /usr/bin/env python

import rospy
from sensor_msgs.msg import Image 
import csv
import sys

class WriterRotation():
    def __init__(self, filename):
        self.f = open("/home/valentin/FAKS/HDR/dataset_new/results_frames/" + filename, 'a')
        self.sub_ = rospy.Subscriber("/camera/rgb/image_color", Image, self.cb_operator_speed)
        rospy.on_shutdown(self.shutdownHook)
        print("Initialized.")

    def cb_operator_speed(self, msg):
        # print(msg)
        rospy.loginfo("Writing: {0}".format(msg.header.stamp))
        self.f.write("{0}\n".format(msg.header.stamp))

    def shutdownHook(self):
        rospy.loginfo("Shutting down...")
        self.f.close()


if __name__ == '__main__':
    rospy.init_node("listener_frames", anonymous=True)
    
    args = rospy.myargv(argv=sys.argv)
    if len(args) != 2:
        print("ERROR: filename not provided")
        sys.exit(1)

    filename = args[1]
    x = WriterRotation(filename)

    rospy.spin()
