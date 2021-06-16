#include "irb2400_control/robotcontrol.hpp"

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
        // Define the fixed point of the robot relative to the camera
        robotFixedPoint.x = 1.25;
        robotFixedPoint.y = -3.05;
        robotFixedPoint.z = -0.27;

        // Defining subscribers and publishers
        entry_distance_sub = pnh_.subscribe("/human_tracker_data", 10, &RobotControl::humanEntriesCallback, this);
        display_pub = pnh_.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);
        operatorSpeed_pub = pnh_.advertise<testing_msgs::OperatorSpeed>("/dataset_testing/operator_speed", 10, true);
        angleRotation_pub = pnh_.advertise<testing_msgs::AngleRotation>("/dataset_testing/angle_rotation", 10, true);
        distanceFromEnvelope_pub = pnh_.advertise<testing_msgs::OperatorDistance>("/dataset_testing/distance_from_envelope", 10, true);

        joint_model_group = move_group.getCurrentState()->getJointModelGroup("manipulator");
        move_group.allowReplanning(true);

        this->timer = this->nh_.createTimer(ros::Duration(3.0), &RobotControl::timerCallback, this);


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
        TRC = cv::Mat(4, 4, CV_64F);
        TCR = cv::Mat(4, 4, CV_64F);

        RobotControl::loadMatrices();

        robotPosition = cv::Mat(1, 2, CV_64F);
        // robotPosition.at<double>(0,0) = TRC.at<double>(0, 3);
        // robotPosition.at<double>(0,1) = TRC.at<double>(1, 3);

        robotPosition.at<double>(0,0) = -2.22f;
        robotPosition.at<double>(0,1) = 4.06f;


        ROS_ERROR("After loading matrices");

        breakTrajectoryExecution = false;

        CURRENT_SPEED = SPEED_FAST;

        calculatedReach = BASE_REACH;

        ROS_ERROR("Robot control initialized.");
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
                matrix_file >> TRC.at<double>(i, j);
            }
        }

        matrix_file.close();

        matrix_file.open(TCR_filename_path);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix_file >> TCR.at<double>(i, j);
            }
        }

        matrix_file.close();

    }


    double RobotControl::calculateRadialOperatorSpeed(cv::Mat vOperator, cv::Mat positionOperator)
    {        
        // Assuming that the velocity of the base of the robot is 0
        /**
         * all variables listed below are vectors except vR (which is a single number)
         * ##########################################################
         * ### vR = (vOperator - vRobot) * (pOperator - pRobot)   ###
         * ###      --------------------------------------------  ###
         * ###                 |pOperator - pRobot|               ###
         * ##########################################################
         */
        // norm (with type NORM_L2) calculates the eucliedan distance between two points
        // ROS_WARN("Dot prod: %f", vOperator.dot(positionOperator - robotPosition));
        // ROS_WARN("eucl dist: %f", cv::norm(positionOperator, robotPosition, cv::NORM_L2));
        return vOperator.dot(robotPosition - positionOperator) / RobotControl::calculateEuclideanDistance(robotPosition, positionOperator);
    }

    double RobotControl::calculateRadialAngle(cv::Mat vOperator, cv::Mat vRadial)
    {
        // ROS_ERROR("[DEBUG] In acos: %f", vRadial.dot(vOperator) / RobotControl::calculateEuclideanDistance(vRadial, vOperator));
        // ROS_ERROR("[DEBUG] In acos2: %f", vOperator.dot(vRadial) / RobotControl::calculateEuclideanDistance(vOperator, vRadial));
        double magnitudeOfVectors = sqrt( (pow(vOperator.at<double>(0,0), 2) + pow(vOperator.at<double>(0,1), 2) ) * ( pow(vRadial.at<double>(0,0), 2) + pow(vRadial.at<double>(0,1), 2) ) );
        return acos(vOperator.dot(vRadial) / magnitudeOfVectors) * (180.0f/CV_PI);
    }

    double RobotControl::calculateTangentialOperatorSpeed(double vOperator, double vRadial)
    {
        return vOperator*vOperator - vRadial*vRadial;
    }

    double RobotControl::calculateOperatorSpeed(cv::Mat vOperator)
    {
        return sqrt(vOperator.at<double>(0,0)*vOperator.at<double>(0,0) + vOperator.at<double>(0,1)*vOperator.at<double>(0,1));
    }

    double RobotControl::calculateEuclideanDistance(cv::Mat position1, cv::Mat position2)
    {
        return sqrt( pow(position1.at<double>(0,0) - position2.at<double>(0,0), 2) + pow(position1.at<double>(0,1) - position2.at<double>(0,1), 2));
    }


    /**
     * 
     * Conditions (1)
     */
    double RobotControl::calculateReach(roi_msgs::HumanEntries entries)
    {
        double calculatedReach = BASE_REACH;
        double referenceReach = BASE_REACH;

        cv::Mat vOperatorMat = cv::Mat::zeros(1, 2, CV_64F);

        for(int i = 0; i < entries.entries.size(); i++)
        {
            vOperatorMat.at<double>(0,0) = entries.entries[i].Xvelocity;
            vOperatorMat.at<double>(0,1) = entries.entries[i].Zvelocity;

            operatorSpeed = calculateOperatorSpeed(vOperatorMat);

            // ROS_ERROR("[DEBUG] Operator speed = %f", operatorSpeed);

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
    double RobotControl::calculateFinalRobotSpeed(roi_msgs::HumanEntries entries)
    {
        vRobot = 0.5f; // max velocity
        reach = RobotControl::calculateReach(entries);
        
        cv::Mat vOperatorMat = cv::Mat::zeros(1, 2, CV_64F);
        cv::Mat positionOperatorMat = cv::Mat::zeros(1, 2, CV_64F);
        cv::Mat vRadialMat = cv::Mat::zeros(1, 2, CV_64F);

        for(int i = 0; i < entries.entries.size(); i++)
        {
            vOperatorMat.at<double>(0,0) = entries.entries[i].Xvelocity;
            vOperatorMat.at<double>(0,1) = entries.entries[i].Zvelocity;
            positionOperatorMat.at<double>(0, 0) = entries.entries[i].personCentroidX;
            positionOperatorMat.at<double>(0, 1) = entries.entries[i].personCentroidZ;

            distanceFromRobotEnvelope = RobotControl::calculateEuclideanDistance(positionOperatorMat, robotPosition) - reach;

            vOperator = calculateOperatorSpeed(vOperatorMat);
            vRadial = calculateRadialOperatorSpeed(vOperatorMat, positionOperatorMat);
            vTangential = calculateTangentialOperatorSpeed(vOperator, vRadial);

            vRadialMat = vRadial * (robotPosition - positionOperatorMat) / RobotControl::calculateEuclideanDistance(positionOperatorMat, robotPosition);            
            radialAngle = calculateRadialAngle(vOperatorMat, vRadialMat);

            vTangentialRef = abs(vTangentialRef - vTangential) > 0.3f ? vTangential : vTangentialRef;
            vTangentialRef = abs(vTangentialRef) < 0.1f ? 0.1f : vTangentialRef;

            /* CONDITIONS (2) */
            temp_vRobot = 0.5f;

            if (vOperator >= 0.1f && vOperator < 2.0f) 
                temp_vRobot = 0.5f/(10.0f*vTangentialRef);
            else if (vOperator < 0.1f)
                temp_vRobot = 0.5f;
            else if (vOperator >= 2.0f || distanceFromRobotEnvelope < 0.0f)
            {
                temp_vRobot = 0.0f;
                resetTimer = true;
            }

            if ((distanceFromRobotEnvelope <= 0.15f && vRadial > 0.0f) || 
                (distanceFromRobotEnvelope > 0.15f && distanceFromRobotEnvelope <= 0.3f && radialAngle < 45.0f) ||
                vOperator >= 2.0f || 
                distanceFromRobotEnvelope < 0.0f)
            {
                temp_vRobot = 0.0f;
                resetTimer = true;
            }

            if (temp_vRobot < vRobot) 
                vRobot = temp_vRobot;
            
            /*****************/
            // Publish stuff for dataset testing 
            testing_msgs::OperatorSpeed opSpeed_msg; // publish operator speed
            opSpeed_msg.header.stamp = entries.header.stamp;
            opSpeed_msg.humanSpeed = vOperator;
            opSpeed_msg.robotVelocity = vRobot;
            operatorSpeed_pub.publish(opSpeed_msg);
            
            testing_msgs::OperatorDistance opDistance_msg; // publish operator distance from envelope
            opDistance_msg.header.stamp = entries.header.stamp;
            opDistance_msg.distanceFromEnvelope = distanceFromRobotEnvelope;
            opDistance_msg.robotVelocity = vRobot;
            distanceFromEnvelope_pub.publish(opDistance_msg);

        }
        
        testing_msgs::AngleRotation angleRotation_msg; // publish operator angle in relation to the robot
        angleRotation_msg.header.stamp = entries.header.stamp;
        angleRotation_msg.robotVelocity = vRobot;
        angleRotation_pub.publish(angleRotation_msg);

        return vRobot;
    }

    void RobotControl::humanEntriesCallback(const roi_msgs::HumanEntriesConstPtr& entries_msg)
    {
        
        double vRobot = RobotControl::calculateFinalRobotSpeed(*entries_msg);

        if (vRobot != CURRENT_SPEED)
        {
            CURRENT_SPEED = vRobot;
            breakTrajectoryExecution = true;
        }
    }

    void RobotControl::timerCallback(const ros::TimerEvent& event)
    {
        breakTrajectoryExecution = true;
        timerStopTrigger = true;
    }

    void RobotControl::loopEverything()
    {
        // Setting velocity max planning time
        move_group.setPlanningTime(10.0);
        
        moveit::planning_interface::MoveGroupInterface::Plan my_plan;
        ROS_ERROR("Initialized plan");
        
        while(ros::ok())
        {
            move_group.setMaxVelocityScalingFactor(CURRENT_SPEED);

            geometry_msgs::Pose pose_now = *current_target_pose_iterator;

            move_group.setPoseTarget(pose_now);

            planning_success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            ROS_INFO_NAMED("Plan succession", "Visualizing plan (pose goal) %s", planning_success ? "" : "FAILED");
            assert(planning_success);

            move_group.asyncExecute(my_plan);

            do
            {
                if (resetTimer)
                {
                    resetTimer = false;

                    if(timer.hasStarted())
                        timer.stop();
                    
                    timer.start();
                }
                else if(timerStopTrigger)
                {
                    timer.stop();
                    timerStopTrigger = false;
                }

                if(breakTrajectoryExecution)
                {
                    move_group.stop();
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