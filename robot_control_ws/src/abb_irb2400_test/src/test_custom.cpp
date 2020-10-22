#include <sstream>
#include <ros/ros.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

#include <moveit_visual_tools/moveit_visual_tools.h>

#include <message_filters/subscriber.h>

// Messages comming from the method 2.1 (human detection - 'human_tracker' workspace)
#include <std_msgs/Float64.h>
#include <cv_bridge/cv_bridge.h>

#include <ros/console.h>

#define SPEED_SLOW 0.05
#define SPEED_FAST 0.7

using namespace ros;

// Flags
bool breakTrajectoryExecution = false;
bool timerStopTrigger = false;
bool resetTimer = false;

double current_speed = SPEED_FAST;

// Callback if the data for distance comes 
void distanceCallback(const std_msgs::Float64::ConstPtr& distance_msg)
{
    if(distance_msg->data < 2.0)
    {
        ROS_ERROR("Distance: %.2f", distance_msg->data);
        
        // Slow down since there's a human in there somewhere
        if(current_speed == SPEED_FAST)
        {
            breakTrajectoryExecution = true;
            current_speed = SPEED_SLOW;
        } 
        
        resetTimer = true;
    }
    
}

// Callback for the timer
void timerCallback(const ros::TimerEvent& event)
{
    // Time's up, back to speeeeed
    ROS_ERROR("TIMER CALLBACK!");
    breakTrajectoryExecution = true;
    timerStopTrigger = true;
    current_speed = SPEED_FAST;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "abb_irb2400_test_custom_node");
    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(3);
    spinner.start();

    // Publishers and subscribers
    ros::Subscriber entry_distance_sub = node_handle.subscribe("/distance", 2, distanceCallback);
    ros::Publisher display_publisher = node_handle.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);


    static const std::string PLANNING_GROUP = "manipulator";
    
    // Defining the move group (interface for planning and executing trajectories)
    moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

    // Get the current state of joints
    const robot_state::JointModelGroup* joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
    // moveit_visual_tools::MoveItVisualTools visual_tools("base_link");

    // moveit_msgs::DisplayTrajectory display_trajectory;

    // Replan if environment changes
    move_group.allowReplanning(true);

    //Create a timer for execution resetting
    ros::Timer timer = node_handle.createTimer(ros::Duration(6.0), timerCallback);

    // Setting a custom goal position
    geometry_msgs::Pose target_pose1;
    geometry_msgs::Pose target_pose2;
    geometry_msgs::Pose current_target_pose;
    geometry_msgs::Pose current_pose;
    std::vector<geometry_msgs::Pose> target_poses;
    
    // pose 1
    target_pose1.position.x = 1.0252;
    target_pose1.position.y = -0.680236;
    target_pose1.position.z = 1.54327;

    target_pose1.orientation.w = 0.707101;
    target_pose1.orientation.x = -4.4225e-06;
    target_pose1.orientation.y = 0.707112;
    target_pose1.orientation.z = -192274e-05;

    // pose 2
    target_pose2.position.x = 1.19964;
    target_pose2.position.y = 0.501868;
    target_pose2.position.z = 1.58109;

    target_pose2.orientation.w = -2.55245e-05;
    target_pose2.orientation.x = 0.707112;
    target_pose2.orientation.y = -1.12172e-05;
    target_pose2.orientation.z = 0.707102;

    // Insert poses into the vector
    target_poses.push_back(target_pose1);
    target_poses.push_back(target_pose2);

    // Iterator for poses
    std::vector<geometry_msgs::Pose>::iterator pose_iterator = target_poses.begin();


    bool success;
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    
    // How much is the current pose different from the target????
    double pose_diff_x, pose_diff_y, pose_diff_z;
    
    // Setting the speed/velocity of the robot
    move_group.setMaxVelocityScalingFactor(current_speed);
    move_group.setPlanningTime(10);

    timer.stop();

    while(ros::ok())
    {
        ROS_WARN("I'm on top.");
        std::vector<double> joint_group_positions;
        moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
        joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
        current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);

        move_group.setMaxVelocityScalingFactor(current_speed);

        current_target_pose = *pose_iterator;

        // move_group.setPoseTarget(target_pose);
        move_group.setPoseTarget(current_target_pose);



        // Motion plan from currrent location to custom position
        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

        ROS_INFO_NAMED("testing_custom", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
        
        move_group.asyncExecute(my_plan);
        ROS_WARN("Executing.");
        do
        {
            if (resetTimer)
            {
                ROS_WARN("resetting timer.");
                resetTimer = false;

                if(timer.hasStarted())
                    timer.stop();
                
                timer.start();
            }
            else if(timerStopTrigger)
            {
                ROS_WARN("Stopping timer.");
                timer.stop();
                timerStopTrigger = false;
            }

            if(breakTrajectoryExecution)
            {
                move_group.stop();
                ROS_ERROR("Stopped exec!");
                sleep(1.5);

                break;
            }
            current_pose = move_group.getCurrentPose().pose;

            pose_diff_x = abs(current_target_pose.position.x - current_pose.position.x);
            pose_diff_y = abs(current_target_pose.position.y - current_pose.position.y);
            pose_diff_z = abs(current_target_pose.position.z - current_pose.position.z);

            ROS_WARN("Trajectoryyyyyy");

        } while (pose_diff_x > 0.0005 || pose_diff_y > 0.0005 || pose_diff_z > 0.0005);
        
        ROS_WARN("DONE");

        // If the trajectory ended successfully, set the next one as a target
        if(!breakTrajectoryExecution)
        {
            ROS_WARN("Setting new iterator.");
            ++pose_iterator;

            if(pose_iterator == target_poses.end())
            {
                pose_iterator = target_poses.begin();
            }
        }

        breakTrajectoryExecution = false;

    }


    ros::shutdown();
    
    return 0;
}
