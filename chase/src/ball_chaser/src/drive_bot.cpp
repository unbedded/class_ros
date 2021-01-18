#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include <std_msgs/Float64.h>
#include <boost/format.hpp>
#include <sstream> // std::ostringstream

ros::Publisher motor_command_pub;

bool handle_drive_request(ball_chaser::DriveToTarget::Request &req, ball_chaser::DriveToTarget::Response &res)
{
    // TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
    // This function should publish the requested linear x and angular velocities to the robot wheel joints
    // After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities
    std::stringstream os;
    os << "handle_drive_request (vel/angle)  "
       << boost::format("%11.6f") % (float)req.linear_x << "/"
       << boost::format("%11.6f") % (float)req.linear_x; //<< std::endl;

    ROS_INFO("DRIVE REQ");

    // Publish angles to drive the robot
    //std::vector<float> mtr_cmd = clamp_at_boundaries(req.joint_1, req.joint_2);
    geometry_msgs::Twist motor_command;
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;
    motor_command_pub.publish(motor_command);

    ROS_INFO_STREAM(os.str());

    res.msg_feedback = "TESTING 123"; //os.str();
    ROS_INFO_STREAM(res.msg_feedback);
    return true;
}

int main(int argc, char **argv)
{
    // Initialize the arm_mover node and create a handle to it
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;
    motor_command_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    ROS_INFO("Ready to send robot vel cmds");

    // Handle ROS communication events
    ros::spin();

    return 0;
}