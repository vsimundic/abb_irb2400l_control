; Auto-generated. Do not edit!


(cl:in-package abb2400_sim-srv)


;//! \htmlinclude move_req-request.msg.html

(cl:defclass <move_req-request> (roslisp-msg-protocol:ros-message)
  ((point
    :reader point
    :initarg :point
    :type geometry_msgs-msg:Point
    :initform (cl:make-instance 'geometry_msgs-msg:Point))
   (orientation
    :reader orientation
    :initarg :orientation
    :type geometry_msgs-msg:Quaternion
    :initform (cl:make-instance 'geometry_msgs-msg:Quaternion)))
)

(cl:defclass move_req-request (<move_req-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <move_req-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'move_req-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name abb2400_sim-srv:<move_req-request> is deprecated: use abb2400_sim-srv:move_req-request instead.")))

(cl:ensure-generic-function 'point-val :lambda-list '(m))
(cl:defmethod point-val ((m <move_req-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader abb2400_sim-srv:point-val is deprecated.  Use abb2400_sim-srv:point instead.")
  (point m))

(cl:ensure-generic-function 'orientation-val :lambda-list '(m))
(cl:defmethod orientation-val ((m <move_req-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader abb2400_sim-srv:orientation-val is deprecated.  Use abb2400_sim-srv:orientation instead.")
  (orientation m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <move_req-request>) ostream)
  "Serializes a message object of type '<move_req-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'point) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'orientation) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <move_req-request>) istream)
  "Deserializes a message object of type '<move_req-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'point) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'orientation) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<move_req-request>)))
  "Returns string type for a service object of type '<move_req-request>"
  "abb2400_sim/move_reqRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'move_req-request)))
  "Returns string type for a service object of type 'move_req-request"
  "abb2400_sim/move_reqRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<move_req-request>)))
  "Returns md5sum for a message object of type '<move_req-request>"
  "41e6e6b8cea8da564de4882c75f56b84")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'move_req-request)))
  "Returns md5sum for a message object of type 'move_req-request"
  "41e6e6b8cea8da564de4882c75f56b84")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<move_req-request>)))
  "Returns full string definition for message of type '<move_req-request>"
  (cl:format cl:nil "geometry_msgs/Point point~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'move_req-request)))
  "Returns full string definition for message of type 'move_req-request"
  (cl:format cl:nil "geometry_msgs/Point point~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <move_req-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'point))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'orientation))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <move_req-request>))
  "Converts a ROS message object to a list"
  (cl:list 'move_req-request
    (cl:cons ':point (point msg))
    (cl:cons ':orientation (orientation msg))
))
;//! \htmlinclude move_req-response.msg.html

(cl:defclass <move_req-response> (roslisp-msg-protocol:ros-message)
  ((retCode
    :reader retCode
    :initarg :retCode
    :type cl:fixnum
    :initform 0))
)

(cl:defclass move_req-response (<move_req-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <move_req-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'move_req-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name abb2400_sim-srv:<move_req-response> is deprecated: use abb2400_sim-srv:move_req-response instead.")))

(cl:ensure-generic-function 'retCode-val :lambda-list '(m))
(cl:defmethod retCode-val ((m <move_req-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader abb2400_sim-srv:retCode-val is deprecated.  Use abb2400_sim-srv:retCode instead.")
  (retCode m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <move_req-response>) ostream)
  "Serializes a message object of type '<move_req-response>"
  (cl:let* ((signed (cl:slot-value msg 'retCode)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <move_req-response>) istream)
  "Deserializes a message object of type '<move_req-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'retCode) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<move_req-response>)))
  "Returns string type for a service object of type '<move_req-response>"
  "abb2400_sim/move_reqResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'move_req-response)))
  "Returns string type for a service object of type 'move_req-response"
  "abb2400_sim/move_reqResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<move_req-response>)))
  "Returns md5sum for a message object of type '<move_req-response>"
  "41e6e6b8cea8da564de4882c75f56b84")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'move_req-response)))
  "Returns md5sum for a message object of type 'move_req-response"
  "41e6e6b8cea8da564de4882c75f56b84")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<move_req-response>)))
  "Returns full string definition for message of type '<move_req-response>"
  (cl:format cl:nil "int8 retCode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'move_req-response)))
  "Returns full string definition for message of type 'move_req-response"
  (cl:format cl:nil "int8 retCode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <move_req-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <move_req-response>))
  "Converts a ROS message object to a list"
  (cl:list 'move_req-response
    (cl:cons ':retCode (retCode msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'move_req)))
  'move_req-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'move_req)))
  'move_req-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'move_req)))
  "Returns string type for a service object of type '<move_req>"
  "abb2400_sim/move_req")