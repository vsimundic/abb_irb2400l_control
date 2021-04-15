; Auto-generated. Do not edit!


(cl:in-package testing_msgs-msg)


;//! \htmlinclude OperatorDistance.msg.html

(cl:defclass <OperatorDistance> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (distanceFromEnvelope
    :reader distanceFromEnvelope
    :initarg :distanceFromEnvelope
    :type cl:float
    :initform 0.0)
   (robotVelocity
    :reader robotVelocity
    :initarg :robotVelocity
    :type cl:float
    :initform 0.0))
)

(cl:defclass OperatorDistance (<OperatorDistance>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <OperatorDistance>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'OperatorDistance)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name testing_msgs-msg:<OperatorDistance> is deprecated: use testing_msgs-msg:OperatorDistance instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <OperatorDistance>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader testing_msgs-msg:header-val is deprecated.  Use testing_msgs-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'distanceFromEnvelope-val :lambda-list '(m))
(cl:defmethod distanceFromEnvelope-val ((m <OperatorDistance>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader testing_msgs-msg:distanceFromEnvelope-val is deprecated.  Use testing_msgs-msg:distanceFromEnvelope instead.")
  (distanceFromEnvelope m))

(cl:ensure-generic-function 'robotVelocity-val :lambda-list '(m))
(cl:defmethod robotVelocity-val ((m <OperatorDistance>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader testing_msgs-msg:robotVelocity-val is deprecated.  Use testing_msgs-msg:robotVelocity instead.")
  (robotVelocity m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <OperatorDistance>) ostream)
  "Serializes a message object of type '<OperatorDistance>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'distanceFromEnvelope))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'robotVelocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <OperatorDistance>) istream)
  "Deserializes a message object of type '<OperatorDistance>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'distanceFromEnvelope) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'robotVelocity) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<OperatorDistance>)))
  "Returns string type for a message object of type '<OperatorDistance>"
  "testing_msgs/OperatorDistance")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'OperatorDistance)))
  "Returns string type for a message object of type 'OperatorDistance"
  "testing_msgs/OperatorDistance")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<OperatorDistance>)))
  "Returns md5sum for a message object of type '<OperatorDistance>"
  "7a42ca980d49efae772557b6a6049115")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'OperatorDistance)))
  "Returns md5sum for a message object of type 'OperatorDistance"
  "7a42ca980d49efae772557b6a6049115")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<OperatorDistance>)))
  "Returns full string definition for message of type '<OperatorDistance>"
  (cl:format cl:nil "Header header~%float64 distanceFromEnvelope~%float64 robotVelocity~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'OperatorDistance)))
  "Returns full string definition for message of type 'OperatorDistance"
  (cl:format cl:nil "Header header~%float64 distanceFromEnvelope~%float64 robotVelocity~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <OperatorDistance>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <OperatorDistance>))
  "Converts a ROS message object to a list"
  (cl:list 'OperatorDistance
    (cl:cons ':header (header msg))
    (cl:cons ':distanceFromEnvelope (distanceFromEnvelope msg))
    (cl:cons ':robotVelocity (robotVelocity msg))
))
