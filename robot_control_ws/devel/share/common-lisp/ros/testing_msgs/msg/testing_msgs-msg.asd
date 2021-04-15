
(cl:in-package :asdf)

(defsystem "testing_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "AngleRotation" :depends-on ("_package_AngleRotation"))
    (:file "_package_AngleRotation" :depends-on ("_package"))
    (:file "OperatorDistance" :depends-on ("_package_OperatorDistance"))
    (:file "_package_OperatorDistance" :depends-on ("_package"))
    (:file "OperatorSpeed" :depends-on ("_package_OperatorSpeed"))
    (:file "_package_OperatorSpeed" :depends-on ("_package"))
  ))