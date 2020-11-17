;; Auto-generated. Do not edit!


(when (boundp 'abb2400_sim::move_req)
  (if (not (find-package "ABB2400_SIM"))
    (make-package "ABB2400_SIM"))
  (shadow 'move_req (find-package "ABB2400_SIM")))
(unless (find-package "ABB2400_SIM::MOVE_REQ")
  (make-package "ABB2400_SIM::MOVE_REQ"))
(unless (find-package "ABB2400_SIM::MOVE_REQREQUEST")
  (make-package "ABB2400_SIM::MOVE_REQREQUEST"))
(unless (find-package "ABB2400_SIM::MOVE_REQRESPONSE")
  (make-package "ABB2400_SIM::MOVE_REQRESPONSE"))

(in-package "ROS")

(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))




(defclass abb2400_sim::move_reqRequest
  :super ros::object
  :slots (_point _orientation ))

(defmethod abb2400_sim::move_reqRequest
  (:init
   (&key
    ((:point __point) (instance geometry_msgs::Point :init))
    ((:orientation __orientation) (instance geometry_msgs::Quaternion :init))
    )
   (send-super :init)
   (setq _point __point)
   (setq _orientation __orientation)
   self)
  (:point
   (&rest __point)
   (if (keywordp (car __point))
       (send* _point __point)
     (progn
       (if __point (setq _point (car __point)))
       _point)))
  (:orientation
   (&rest __orientation)
   (if (keywordp (car __orientation))
       (send* _orientation __orientation)
     (progn
       (if __orientation (setq _orientation (car __orientation)))
       _orientation)))
  (:serialization-length
   ()
   (+
    ;; geometry_msgs/Point _point
    (send _point :serialization-length)
    ;; geometry_msgs/Quaternion _orientation
    (send _orientation :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; geometry_msgs/Point _point
       (send _point :serialize s)
     ;; geometry_msgs/Quaternion _orientation
       (send _orientation :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; geometry_msgs/Point _point
     (send _point :deserialize buf ptr-) (incf ptr- (send _point :serialization-length))
   ;; geometry_msgs/Quaternion _orientation
     (send _orientation :deserialize buf ptr-) (incf ptr- (send _orientation :serialization-length))
   ;;
   self)
  )

(defclass abb2400_sim::move_reqResponse
  :super ros::object
  :slots (_retCode ))

(defmethod abb2400_sim::move_reqResponse
  (:init
   (&key
    ((:retCode __retCode) 0)
    )
   (send-super :init)
   (setq _retCode (round __retCode))
   self)
  (:retCode
   (&optional __retCode)
   (if __retCode (setq _retCode __retCode)) _retCode)
  (:serialization-length
   ()
   (+
    ;; int8 _retCode
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8 _retCode
       (write-byte _retCode s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8 _retCode
     (setq _retCode (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _retCode 127) (setq _retCode (- _retCode 256)))
   ;;
   self)
  )

(defclass abb2400_sim::move_req
  :super ros::object
  :slots ())

(setf (get abb2400_sim::move_req :md5sum-) "41e6e6b8cea8da564de4882c75f56b84")
(setf (get abb2400_sim::move_req :datatype-) "abb2400_sim/move_req")
(setf (get abb2400_sim::move_req :request) abb2400_sim::move_reqRequest)
(setf (get abb2400_sim::move_req :response) abb2400_sim::move_reqResponse)

(defmethod abb2400_sim::move_reqRequest
  (:response () (instance abb2400_sim::move_reqResponse :init)))

(setf (get abb2400_sim::move_reqRequest :md5sum-) "41e6e6b8cea8da564de4882c75f56b84")
(setf (get abb2400_sim::move_reqRequest :datatype-) "abb2400_sim/move_reqRequest")
(setf (get abb2400_sim::move_reqRequest :definition-)
      "geometry_msgs/Point point
geometry_msgs/Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w
---
int8 retCode
")

(setf (get abb2400_sim::move_reqResponse :md5sum-) "41e6e6b8cea8da564de4882c75f56b84")
(setf (get abb2400_sim::move_reqResponse :datatype-) "abb2400_sim/move_reqResponse")
(setf (get abb2400_sim::move_reqResponse :definition-)
      "geometry_msgs/Point point
geometry_msgs/Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w
---
int8 retCode
")



(provide :abb2400_sim/move_req "41e6e6b8cea8da564de4882c75f56b84")


