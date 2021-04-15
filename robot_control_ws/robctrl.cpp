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
        robotFixedPoint.x = 1.25;
        robotFixedPoint.y = -3.05;
        robotFixedPoint.z = -0.27;

        // Defining subscribers and publishers
        entry_distance_sub = pnh_.subscribe("/human_tracker_data", 10, &RobotControl::humanEntriesCallback, this);
        display_pub = pnh_.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);


        joint_model_group = move_group.getCurrentState()->getJointModelGroup("manipulator");
        ROS_ERROR("Got joint model group.");
        // Defining the move group (interface for planning and executing trajectories)
        // move_group = ;
        move_group.allowReplanning(true);


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

        ROS_ERROR("Before loading matrices");
        // // Load transformation matrices
        TRC = cv::Mat(4, 4, CV_32F);
        TCR = cv::Mat(4, 4, CV_32F);

        RobotControl::loadMatrices();

        robotPosition = cv::Mat(1, 2, CV_32F);
        robotPosition.at<float>(0,0) = TRC.at<float>(0, 3);
        robotPosition.at<float>(0,1) = TRC.at<float>(1, 3);

        ROS_ERROR("After loading matrices");

        breakTrajectoryExecution = false;

        CURRENT_SPEED = SPEED_FAST;

        calculatedReach = BASE_REACH;

        ROS_ERROR("End of init!");
    }

    void RobotControl::loadMatrices()
    {

        std::string matrices_path = ros::package::getPath("abb2400_sim") ;
        std::string TRC_filename_path = matrices_path + "/TRC.txt";
        std::string TCR_filename_path = matrices_path + "/TCR.txt";
        
        std::ifstream matrix_file;
        matrix_file.open(TRC_filename_path);

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix_file >> TRC.at<float>(i, j);
            }
        }

        matrix_file.close();

        matrix_file.open(TCR_filename_path);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix_file >> TCR.at<float>(i, j);
            }
        }

        matrix_file.close();

    }


    float RobotControl::calculateRadialOperatorSpeed(cv::Mat vOperator, cv::Mat positionOperator)
    {        
        // Assuming that the velocity of the base of the robot is 0
        /**
         * all variables listed below are vectors except vR (which is a single number)
         * ##########################################################
         * ### vR = (vOperator - vRobot) * (pOperator - pRobot)   ###
         * ### -------------------------------------------------- ###
         * ###                 |pOperator - pRobot|               ###
         * ##########################################################
         */
        // norm (with type NORM_L2) calculates the eucliedan distance between two points
        // ROS_WARN("Dot prod: %f", vOperator.dot(positionOperator - robotPosition));
        // ROS_WARN("eucl dist: %f", cv::norm(positionOperator, robotPosition, cv::NORM_L2));
        return vOperator.dot(positionOperator - robotPosition) / cv::norm(positionOperator, robotPosition, cv::NORM_L2);
    }

    float RobotControl::calculateRadialAngle(cv::Mat vOperator, cv::Mat vRadial)
    {
        return abs(vOperator.dot(vRadial)/cv::norm(vOperator, vRadial, cv::NORM_L2)) * 180.0f/CV_PI;
    }

    float RobotControl::calculateTangentialOperatorSpeed(float vOperator, float vRadial)
    {
        return vOperator*vOperator - vRadial*vRadial;
    }

    float RobotControl::calculateOperatorSpeed(cv::Mat vOperator)
    {
        return sqrt(vOperator.at<float>(0,0)*vOperator.at<float>(0,0) + vOperator.at<float>(0,1)*vOperator.at<float>(0,1));
    }

    /**
     * 
     * Conditions (1)
     */
    float RobotControl::calculateReach(roi_msgs::HumanEntries entries)
    {
        float calculatedReach = BASE_REACH;
        float referenceReach = BASE_REACH;

        cv::Mat vOperatorMat = cv::Mat::zeros(1, 2, CV_32F);

        for(int i = 0; i < entries.entries.size(); i++)
        {
            vOperatorMat.at<float>(0,0) = entries.entries[i].Xvelocity;
            vOperatorMat.at<float>(0,1) = entries.entries[i].Yvelocity;

            operatorSpeed = calculateOperatorSpeed(vOperatorMat);

            if(operatorSpeed < 0.1f)
                calculatedReach = BASE_REACH;
            
            else if(operatorSpeed >= 0.1f && operatorSpeed < 1.0f)
                calculatedReach = mediumReach;
            
            else if(operatorSpeed >= 1.0f)
                return highReach;
            
            if(calculatedReach > referenceReach)
                referenceReach = calculatedReach;
        }

        return referenceReach;
    }


    /**
     * 
     * Conditions (2)
     */
    float RobotControl::calculateFinalRobotSpeed(roi_msgs::HumanEntries entries)
    {
        vRobot = 0.5f; // max velocity
        reach = RobotControl::calculateReach(entries);
        
        cv::Mat vOperatorMat = cv::Mat::zeros(1, 2, CV_32F);
        cv::Mat positionOperatorMat = cv::Mat::zeros(1, 2, CV_32F);
        cv::Mat vRadialMat = cv::Mat::zeros(1, 2, CV_32F);

        for(int i = 0; i < entries.entries.size(); i++)
        {
            vOperatorMat.at<float>(0,0) = entries.entries[i].Xvelocity;
            vOperatorMat.at<float>(0,1) = entries.entries[i].Yvelocity;
            positionOperatorMat.at<float>(0, 0) = entries.entries[i].personCentroidX;
            positionOperatorMat.at<float>(0, 1) = entries.entries[i].personCentroidY;

            distanceFromRobotEnvelope = cv::norm(positionOperatorMat, robotPosition, cv::NORM_L2) - reach;

            vOperator = calculateOperatorSpeed(vOperatorMat);
            vRadial = calculateRadialOperatorSpeed(vOperatorMat, positionOperatorMat);
            vTangential = calculateTangentialOperatorSpeed(vOperator, vRadial);
            
            vRadialMat = vRadial * (positionOperatorMat - robotPosition) / cv::norm(positionOperatorMat, robotPosition, cv::NORM_L2);            
            radialAngle = calculateRadialAngle(vOperatorMat, vRadialMat);

            vTangentialRef = abs(vTangentialRef - vTangential) > 0.3f ? vTangential : vTangentialRef;
            vTangentialRef = abs(vTangentialRef) < 0.1f ? 0.1f : vTangentialRef;

            // ROS_WARN("Vo = %f", vOperator);
            ROS_WARN("Vr = %f", vRadial);
            // ROS_WARN("Vt = %f", vTangential);

            if (vOperator >= 2.0f) 
                return 0.0f;    
            
            else if(distanceFromRobotEnvelope > 0.15f && distanceFromRobotEnvelope <= 0.3f)
            {
                if (radialAngle < 45.0f) 
                    return 0.0f;
            }
            else if (distanceFromRobotEnvelope <= 0.15f)
            {
                if (vRadial > 0.0f) 
                    return 0.0f;
            }
            temp_vRobot = 0.5f;

            if (vOperator >= 0.1f && vOperator < 2.0f) {

                temp_vRobot = 0.5f/(10.0f*vTangentialRef);
            }
            if (vOperator < 0.1f) 
                temp_vRobot = 0.5f;


            if (temp_vRobot < vRobot) vRobot = temp_vRobot;

        }

        return vRobot;
    }

    void RobotControl::humanEntriesCallback(const roi_msgs::HumanEntriesConstPtr& entries_msg)
    {
        
        float vRobot = RobotControl::calculateFinalRobotSpeed(*entries_msg);
        /*
        // conditions (1) implemented
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

            }
        }
        */

       if (vRobot != CURRENT_SPEED)
       {
           CURRENT_SPEED = vRobot;
           breakTrajectoryExecution = true;
       }

    }

    void RobotControl::loopEverything()
    {
        // sleep(2.0);
        ROS_ERROR("In: loop everything!");
        
        // Setting velocity max planning time
        move_group.setPlanningTime(10.0);
        
        moveit::planning_interface::MoveGroupInterface::Plan my_plan;
        ROS_ERROR("Initialized plan");
        
        while(ros::ok())
        {
            // ROS_ERROR("Speeeeeeeeed: %f", CURRENT_SPEED);
            move_group.setMaxVelocityScalingFactor(CURRENT_SPEED);

            geometry_msgs::Pose pose_now = *current_target_pose_iterator;

            move_group.setPoseTarget(pose_now);


            planning_success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            ROS_INFO_NAMED("Plan succession", "Visualizing plan 1 (pose goal) %s", planning_success ? "" : "FAILED");
            assert(planning_success);

            move_group.asyncExecute(my_plan);
            // ROS_WARN("Executing");

            do
            {
                if(breakTrajectoryExecution)
                {
                    move_group.stop();
                    ROS_ERROR("Stopped exec!");
                    sleep(0.5);

                    break;
                }

                current_pose = move_group.getCurrentPose().pose;

                pose_diff_x = abs((*current_target_pose_iterator).position.x - current_pose.position.x);
                pose_diff_y = abs((*current_target_pose_iterator).position.y - current_pose.position.y);
                pose_diff_z = abs((*current_target_pose_iterator).position.z - current_pose.position.z);

            } while (pose_diff_x > 0.0005 || pose_diff_y > 0.0005 || pose_diff_z > 0.0005);
            
            if(!breakTrajectoryExecution) // if robot finished the trajectory without breaking it 
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
