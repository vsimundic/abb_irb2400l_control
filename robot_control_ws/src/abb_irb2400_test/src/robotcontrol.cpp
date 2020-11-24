#include "abb_irb2400_test/robotcontrol.hpp"

namespace RobotControlNamespace
{
    RobotControl::RobotControl(const ros::NodeHandle &node_handle, const ros::NodeHandle &private_node_handle): 
    nh_(node_handle), 
    pnh_(private_node_handle), 
    SPEED_FAST(0.7), 
    SPEED_SLOW(0.05),
    BASE_REACH(1.81),
    mediumReach(BASE_REACH*1.1),
    highReach(BASE_REACH*1.5),
    PLANNING_GROUP("manipulator"),
    move_group(moveit::planning_interface::MoveGroupInterface(PLANNING_GROUP))
    {
        this->init();
    }

    RobotControl::~RobotControl()
    {
        nh_.shutdown();
        pnh_.shutdown();
    }

    void RobotControl::init()
    {
        ROS_ERROR("In init!");
        // Define the fixed point of the robot relative to the camera
        robotFixedPoint.x = 2.0;
        robotFixedPoint.y = -2.0;
        robotFixedPoint.z = -0.15;

        // Defining subscribers and publishers
        entry_distance_sub = pnh_.subscribe("/human_tracker_data", 10, &RobotControl::humanEntriesCallback, this);
        display_pub = pnh_.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);


        joint_model_group = move_group.getCurrentState()->getJointModelGroup("manipulator");
        ROS_ERROR("Got joint model group.");
        // Defining the move group (interface for planning and executing trajectories)
        // move_group = ;
        move_group.allowReplanning(true);

        //Defining timer
        timer = pnh_.createTimer(ros::Duration(3.0), &RobotControl::timerCallback, this);

        //Robot poses
        geometry_msgs::Pose target_pose1;
        geometry_msgs::Pose target_pose2;
        
        // pose 1
        target_pose1.position.x = 0.735395; target_pose1.position.y = -0.427505; target_pose1.position.z = 1.82907;
        target_pose1.orientation.w = 0.707101; target_pose1.orientation.x = 4.4225e-05; target_pose1.orientation.y = 0.707112; target_pose1.orientation.z = -3.34027e-05;
        // pose 2
        target_pose2.position.x = 0.740174; target_pose2.position.y = 0.628105; target_pose2.position.z = 1.60914;
        target_pose2.orientation.w = 0.707077; target_pose2.orientation.x = 3.86214e-05; target_pose2.orientation.y = 0.707135; target_pose2.orientation.z = 4.3254e-05;
    
        target_poses.push_back(target_pose1);
        target_poses.push_back(target_pose2);

        current_target_pose_iterator = target_poses.begin();

        // // Setting velocity scaling factor and max planning time
        // move_group.setMaxVelocityScalingFactor(CURRENT_SPEED);
        // move_group.setPlanningTime(10.0);

        // Timer is started only when a human enters the area
        timer.stop();

        /** 
         * Trajectory execution will break if the speed is fast and
         * the human comes into the robot's area or if a timer runs
         * out to speed the high speeeeeeed.
         */   
        breakTrajectoryExecution = false;

        CURRENT_SPEED = SPEED_FAST;

        calculatedReach = BASE_REACH;

        ROS_ERROR("End of init!");
    }

    void RobotControl::timerCallback(const ros::TimerEvent& event)
    {
        breakTrajectoryExecution = true;
        
        CURRENT_SPEED = SPEED_FAST;
        calculatedReach = BASE_REACH;

        timer.stop();
    }

    
    double RobotControl::calculateReach(roi_msgs::HumanEntries entries)
    {
        double currentTempCalculatedReach = BASE_REACH;

        for(int i = 0; i < entries.entries.size(); i++)
        {
            humanVelocity = sqrt(pow(entries.entries[i].Xvelocity, 2) + pow(entries.entries[i].Yvelocity, 2));

            if(humanVelocity < 0.1)
                tempCalculatedReach = BASE_REACH;
            
            else if(humanVelocity >= 0.1 && humanVelocity < 1.0)
                tempCalculatedReach = mediumReach;
            
            else if(humanVelocity >= 1.0)
                tempCalculatedReach = highReach;
            
            if(tempCalculatedReach > currentTempCalculatedReach)
                currentTempCalculatedReach = tempCalculatedReach;
        }

        return currentTempCalculatedReach;
        
    }

    void RobotControl::humanEntriesCallback(const roi_msgs::HumanEntriesConstPtr& entries_msg)
    {
        calculatedReach = RobotControl::calculateReach(*entries_msg);


        for(int i = 0; i < entries_msg->entries.size(); i++)
        {

            entryPoint.x = entries_msg->entries[i].personCentroidY;
            entryPoint.y = -entries_msg->entries[i].personCentroidX;
            entryPoint.z = -entries_msg->entries[i].personCentroidZ;

            entryDistance = sqrt(pow(robotFixedPoint.x - entryPoint.x, 2) + pow(robotFixedPoint.y - entryPoint.y, 2));

            if(entryDistance < calculatedReach)
            {
                ROS_ERROR("Distance: %.2f", entryDistance);
                ROS_ERROR("Human velocity: %.2f", humanVelocity);
                ROS_ERROR("REACH: %f", calculatedReach);
                ROS_WARN("\n########################################################\n");


                if(CURRENT_SPEED == SPEED_FAST)
                {
                    breakTrajectoryExecution = true;
                    CURRENT_SPEED = SPEED_SLOW;
                }

                RobotControl::resetTimer();
            }
        }
    }

    void RobotControl::resetTimer()
    {
        if(timer.hasStarted())
            timer.stop();
        timer.start();
    }

    void RobotControl::loopEverything()
    {
        // sleep(2.0);
        ROS_ERROR("In loop everything!");
        
        // Setting velocity scaling factor and max planning time
        move_group.setMaxVelocityScalingFactor(CURRENT_SPEED);
        move_group.setPlanningTime(10.0);
        
        ROS_ERROR("Set planning time finished");
        
        moveit::planning_interface::MoveGroupInterface::Plan my_plan;
        ROS_ERROR("Initialized plan");
        
        while(ros::ok())
        {
            // ROS_ERROR("Speeeeeeeeed: %f", CURRENT_SPEED);
            move_group.setMaxVelocityScalingFactor(CURRENT_SPEED);
            

            geometry_msgs::Pose pose_now = *current_target_pose_iterator;

            move_group.setPoseTarget(pose_now);


            planning_success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            // ROS_INFO_NAMED("Plan succession", "Visualizing plan 1 (pose goal) %s", planning_success ? "" : "FAILED");
            assert(planning_success);

            move_group.asyncExecute(my_plan);
            // ROS_WARN("Executing");

            do
            {
                if(breakTrajectoryExecution)
                {
                    move_group.stop();
                    ROS_ERROR("Stopped exec!");
                    sleep(1.0);

                    break;
                }

                current_pose = move_group.getCurrentPose().pose;

                pose_diff_x = abs((*current_target_pose_iterator).position.x - current_pose.position.x);
                pose_diff_y = abs((*current_target_pose_iterator).position.y - current_pose.position.y);
                pose_diff_z = abs((*current_target_pose_iterator).position.z - current_pose.position.z);

            } while (pose_diff_x > 0.0005 || pose_diff_y > 0.0005 || pose_diff_z > 0.0005);
            
            if(!breakTrajectoryExecution)
            {
                ++current_target_pose_iterator;

                if(current_target_pose_iterator == target_poses.end())
                {
                    current_target_pose_iterator = target_poses.begin();
                }
            }

            breakTrajectoryExecution = false;
        }
    }
}