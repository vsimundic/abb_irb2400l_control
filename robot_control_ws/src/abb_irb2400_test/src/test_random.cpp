//MoveIt! header file
#include <moveit/move_group_interface/move_group_interface.h>
// #include <moveit/move_group_interface/move_group.h>

#include <visualization_msgs/Marker.h>


int main(int argc, char **argv)
{
    ros::init(argc, argv, "abb_irb2400_test_random_node", ros::init_options::AnonymousName);
    ros::NodeHandle node_handle;

    ros::Publisher vis_pub = node_handle.advertise<visualization_msgs::Marker>("visual_marker", 0);

    visualization_msgs::Marker marker;
    marker.header.frame_id = "camera_rgb_optical_frame";
    marker.header.stamp = ros::Time();
    marker.ns = "my_namespace";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = -2.22;
    marker.pose.position.y = 0.80;
    marker.pose.position.z = 4.06;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 1;
    marker.scale.y = 0.1;
    marker.scale.z = 0.1;
    marker.color.a = 1.0; // Don't forget to set the alpha!
    marker.color.r = 0.0;
    marker.color.g = 1.0;
    marker.color.b = 0.0;

    while(ros::ok())
    {
        vis_pub.publish(marker);

        ros::spinOnce();
    }

}