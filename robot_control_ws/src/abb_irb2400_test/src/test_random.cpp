//MoveIt! header file
#include <moveit/move_group_interface/move_group_interface.h>
// #include <moveit/move_group_interface/move_group.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "abb_irb2400_test_random_node", ros::init_options::AnonymousName);
    
    // start a ROS spinning thread
    ros::AsyncSpinner spinner(1);
    spinner.start();
    
    // this connects to a running instance of the move_group node
    // Here the Planning group is "arm"
    // move_group_interface::MoveGroup group("arm");
    static const std::string PLANNING_GROUP = "manipulator";

    moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
    
    // specify that our target will be a random one
    move_group.setRandomTarget();
    
    // plan the motion and then move the group to the sampled target
    move_group.move();
    ros::waitForShutdown();
}