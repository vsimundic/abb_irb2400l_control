
(cl:in-package :asdf)

(defsystem "abb2400_sim-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "move_req" :depends-on ("_package_move_req"))
    (:file "_package_move_req" :depends-on ("_package"))
  ))