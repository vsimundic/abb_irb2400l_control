#ifndef CONTROLROBOT_HPP
#define CONTROLROBOT_HPP

#include <sstream>
#include <ros/ros.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

// // OpenCV
#include "opencv2/opencv.hpp"
#include <opencv2/core/cvdef.h> // for CV_PI

#include <message_filters/subscriber.h>
#include <geometry_msgs/Point.h>
#include <visualization_msgs/Marker.h>

// Messages comming from the method 2.1 (human detection - 'human_tracker' workspace)
#include <std_msgs/Float64.h>
#include <cv_bridge/cv_bridge.h>

#include "roi_msgs/HumanEntries.h"
#include "roi_msgs/HumanEntry.h"
#include "testing_msgs/OperatorSpeed.h"
#include "testing_msgs/OperatorDistance.h"
#include "testing_msgs/AngleRotation.h"

#include <ros/console.h>

using namespace ros;

namespace RobotControlNamespace
{
    class RobotControl
    {
        public:
            RobotControl(const ros::NodeHandle &node_handle, 
                const ros::NodeHandle &private_node_handle);
            
            ~RobotControl();

            /**
             * Initialize the publishers, subscribers
             * and parameters for the MoveGroupInterface
             */
            void init();

            /**
             * Human data callback
             * @param human_entries
             */
            void humanEntriesCallback(const roi_msgs::HumanEntriesConstPtr& entries_msg);
            

            /**
             * Loops while ROS is running and controls smth
             */
            void loopEverything();

        private:
            // Public ROS node handle
            ros::NodeHandle nh_;

            // Private ROS node handle
            ros::NodeHandle pnh_;

            // Subscribers and publishers
            ros::Subscriber entry_distance_sub;
            ros::Publisher display_pub;
            ros::Publisher radialVel_pub;
            ros::Publisher operatorSpeed_pub;
            ros::Publisher angleRotation_pub;
            ros::Publisher distanceFromEnvelope_pub;

            // Fixed points
            geometry_msgs::Point robotFixedPoint;
            geometry_msgs::Point entryPoint;

            // Distance from the robot
            double entryDistance;

            // MoveIt! stuff
            const std::string PLANNING_GROUP;
            moveit::planning_interface::MoveGroupInterface move_group;
            moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
            
            bool planning_success;
            // moveit::planning_interface::MoveGroupInterface::Plan my_plan;

            std::vector<double> joint_group_positions;
            const robot_state::JointModelGroup* joint_model_group;
            moveit::core::RobotStatePtr current_state;

            // Robot positions
            geometry_msgs::Pose current_pose;
            std::vector<geometry_msgs::Pose> target_poses;
            std::vector<geometry_msgs::Pose>::iterator current_target_pose_iterator;

            // How much is the current pose different from the target????
            double pose_diff_x, pose_diff_y, pose_diff_z;

            //SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
            const double SPEED_FAST;
            const double SPEED_SLOW;
            double CURRENT_SPEED;

            // Flags for control
            bool breakTrajectoryExecution;


            
            /****** HUMAN DETECTION STUFF ******/
            const double BASE_REACH;
            const double mediumReach;
            const double highReach;

            
            // used in the function for determining the reach
            double tempCalculatedReach;
            
            // the value tempCalculatedReach outside of the function
            double calculatedReach;

            double operatorSpeed;


            // transformation matrices
            cv::Mat TCR;
            cv::Mat TRC;

            cv::Mat robotPosition;

            void loadMatrices();


            // Implementing conditions (1)
            double calculateReach(roi_msgs::HumanEntries entries);
            double calculateOperatorSpeed(cv::Mat vOperator);
            double calculateRadialOperatorSpeed(cv::Mat vOperator, cv::Mat positionOperator);
            double calculateTangentialOperatorSpeed(double vOperator, double vRadial);
            double calculateRadialAngle(cv::Mat vOperator, cv::Mat vRadial);
            double calculateEuclideanDistance(cv::Mat position1, cv::Mat position2);

            // speeeeeeeeeeed stuff
            double reach;
            double vTangentialRef = 0.1f;
            double vOperator;
            double vRadial;
            double vTangential;
            double distanceFromRobotEnvelope;
            
            double vRobot;
            double temp_vRobot;
            double radialAngle;


            // Implementing conditions (2)
            double calculateFinalRobotSpeed(roi_msgs::HumanEntries operatorEntries);

    }; // class RobotControl

} //namespace RobotControlNamespace



#endif // ROBOT_CONTROL_HPP