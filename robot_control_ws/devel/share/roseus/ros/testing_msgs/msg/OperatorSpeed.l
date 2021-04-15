;; Auto-generated. Do not edit!


(when (boundp 'testing_msgs::OperatorSpeed)
  (if (not (find-package "TESTING_MSGS"))
    (make-package "TESTING_MSGS"))
  (shadow 'OperatorSpeed (find-package "TESTING_MSGS")))
(unless (find-package "TESTING_MSGS::OPERATORSPEED")
  (make-package "TESTING_MSGS::OPERATORSPEED"))

(in-package "ROS")
;;//! \htmlinclude OperatorSpeed.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass testing_msgs::OperatorSpeed
  :super ros::object
  :slots (_header _humanSpeed _robotVelocity ))

(defmethod testing_msgs::OperatorSpeed
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:humanSpeed __humanSpeed) 0.0)
    ((:robotVelocity __robotVelocity) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _humanSpeed (float __humanSpeed))
   (setq _robotVelocity (float __robotVelocity))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:humanSpeed
   (&optional __humanSpeed)
   (if __humanSpeed (setq _humanSpeed __humanSpeed)) _humanSpeed)
  (:robotVelocity
   (&optional __robotVelocity)
   (if __robotVelocity (setq _robotVelocity __robotVelocity)) _robotVelocity)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64 _humanSpeed
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
     ;; float64 _humanSpeed
       (sys::poke _humanSpeed (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _robotVelocity
       (sys::poke _robotVelocity (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64 _humanSpeed
     (setq _humanSpeed (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _robotVelocity
     (setq _robotVelocity (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get testing_msgs::OperatorSpeed :md5sum-) "e5cb69c02c631c93ca058b4ec5827b97")
(setf (get testing_msgs::OperatorSpeed :datatype-) "testing_msgs/OperatorSpeed")
(setf (get testing_msgs::OperatorSpeed :definition-)
      "Header header
float64 humanSpeed
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



(provide :testing_msgs/OperatorSpeed "e5cb69c02c631c93ca058b4ec5827b97")


