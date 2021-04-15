;; Auto-generated. Do not edit!


(when (boundp 'testing_msgs::OperatorDistance)
  (if (not (find-package "TESTING_MSGS"))
    (make-package "TESTING_MSGS"))
  (shadow 'OperatorDistance (find-package "TESTING_MSGS")))
(unless (find-package "TESTING_MSGS::OPERATORDISTANCE")
  (make-package "TESTING_MSGS::OPERATORDISTANCE"))

(in-package "ROS")
;;//! \htmlinclude OperatorDistance.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass testing_msgs::OperatorDistance
  :super ros::object
  :slots (_header _distanceFromEnvelope _robotVelocity ))

(defmethod testing_msgs::OperatorDistance
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:distanceFromEnvelope __distanceFromEnvelope) 0.0)
    ((:robotVelocity __robotVelocity) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _distanceFromEnvelope (float __distanceFromEnvelope))
   (setq _robotVelocity (float __robotVelocity))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:distanceFromEnvelope
   (&optional __distanceFromEnvelope)
   (if __distanceFromEnvelope (setq _distanceFromEnvelope __distanceFromEnvelope)) _distanceFromEnvelope)
  (:robotVelocity
   (&optional __robotVelocity)
   (if __robotVelocity (setq _robotVelocity __robotVelocity)) _robotVelocity)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64 _distanceFromEnvelope
    8
    ;; float64 _robotVelocity
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float64 _distanceFromEnvelope
       (sys::poke _distanceFromEnvelope (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _robotVelocity
       (sys::poke _robotVelocity (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64 _distanceFromEnvelope
     (setq _distanceFromEnvelope (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _robotVelocity
     (setq _robotVelocity (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get testing_msgs::OperatorDistance :md5sum-) "7a42ca980d49efae772557b6a6049115")
(setf (get testing_msgs::OperatorDistance :datatype-) "testing_msgs/OperatorDistance")
(setf (get testing_msgs::OperatorDistance :definition-)
      "Header header
float64 distanceFromEnvelope
float64 robotVelocity
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

")



(provide :testing_msgs/OperatorDistance "7a42ca980d49efae772557b6a6049115")


