#include "irb2400_control/robotcontrol.hpp"
#include <string>

int main(int argc, char** argv)
{
    std::string node_name = "robot_control_node";
    ros::init(argc, argv, node_name);

    ros::NodeHandle nh("");
    ros::NodeHandle nh_private("~");

    ros::AsyncSpinner spinner(4);
    spinner.start();
    
    RobotControlNamespace::RobotControl robot_control_node(nh, nh_private);

    robot_control_node.loopEverything();

    ros::waitForShutdown();
}