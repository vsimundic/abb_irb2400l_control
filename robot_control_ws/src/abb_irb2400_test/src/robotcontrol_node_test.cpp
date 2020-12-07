#include "abb_irb2400_test/robotcontrol.hpp"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "robot_control_node_test");
    
    ros::NodeHandle nh("");
    ros::NodeHandle nh_private("~");

    ros::AsyncSpinner spinner(4);
    spinner.start();
    

    
    RobotControlNamespace::RobotControl rc(nh, nh_private);

    ROS_INFO("Initialized the robot_control node.");

    ros::waitForShutdown();

}