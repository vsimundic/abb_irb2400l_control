#! /usr/bin/env python

import rospy
from std_msgs.msg import Float64
from roi_msgs.msg import HumanEntries, HumanEntry
from testing_msgs.msg import OperatorSpeed
import csv
import sys

class WriterHumanDetector():
    def __init__(self, filename):
        self.f = open("/home/valentin/FAKS/HDR/dataset_new/results/" + filename, 'a')
        self.sub_ = rospy.Subscriber("/human_tracker_data", HumanEntries, self.cb_entries)
        rospy.on_shutdown(self.shutdownHook)
        print("Initialized.")

    def cb_entries(self, msg):
        # print(len(msg.entries))
        
        rospy.loginfo("Writing: {0},{1}".format(msg.header.stamp, len(msg.entries)))
        self.f.write("{0},{1}\n".format(msg.header.stamp, len(msg.entries)))

    def shutdownHook(self):
        rospy.loginfo("Shutting down...")
        self.f.close()


if __name__ == '__main__':
    rospy.init_node("listener_speed_node", anonymous=True)
    
    args = rospy.myargv(argv=sys.argv)
    if len(args) != 2:
        print("ERROR: filename not provided")
        sys.exit(1)

    filename = args[1]
    x = WriterHumanDetector(filename)

    rospy.spin()
