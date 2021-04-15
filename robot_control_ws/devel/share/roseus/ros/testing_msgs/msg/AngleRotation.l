;; Auto-generated. Do not edit!


(when (boundp 'testing_msgs::AngleRotation)
  (if (not (find-package "TESTING_MSGS"))
    (make-package "TESTING_MSGS"))
  (shadow 'AngleRotation (find-package "TESTING_MSGS")))
(unless (find-package "TESTING_MSGS::ANGLEROTATION")
  (make-package "TESTING_MSGS::ANGLEROTATION"))

(in-package "ROS")
;;//! \htmlinclude AngleRotation.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass testing_msgs::AngleRotation
  :super ros::object
  :slots (_header _radialVelocity _robotVelocity ))

(defmethod testing_msgs::AngleRotation
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:radialVelocity __radialVelocity) 0.0)
    ((:robotVelocity __robotVelocity) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _radialVelocity (float __radialVelocity))
   (setq _robotVelocity (float __robotVelocity))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:radialVelocity
   (&optional __radialVelocity)
   (if __radialVelocity (setq _radialVelocity __radialVelocity)) _radialVelocity)
  (:robotVelocity
   (&optional __robotVelocity)
   (if __robotVelocity (setq _robotVelocity __robotVelocity)) _robotVelocity)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64 _radialVelocity
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
     ;; float64 _radialVelocity
       (sys::poke _radialVelocity (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _robotVelocity
       (sys::poke _robotVelocity (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64 _radialVelocity
     (setq _radialVelocity (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _robotVelocity
     (setq _robotVelocity (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get testing_msgs::AngleRotation :md5sum-) "de165192dcde44004c7a45b00482db8e")
(setf (get testing_msgs::AngleRotation :datatype-) "testing_msgs/AngleRotation")
(setf (get testing_msgs::AngleRotation :definition-)
      "Header header
float64 radialVelocity
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



(provide :testing_msgs/AngleRotation "de165192dcde44004c7a45b00482db8e")


