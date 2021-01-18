#include "ros/ros.h"
//#include "simple_arm/GoToPosition.h"
//#include <sensor_msgs/JointState.h>
//#include "geometry_msgs/Twist.h"
// #include "ball_chaser/DriveToTarget.h"  SERVICE
#include <std_msgs/Float64.h>
#include <boost/format.hpp>
#include <sstream>

////////////////////////
#define NODE_NAME "look_away"

class SubscribeAndPublish
{
private:
    const char *joint_state_topic = "/simple_arm/joint_states";
    const char *service_topic = "/arm_mover/safe_move";
    const char *camera_topic = "rgb_camera/image_raw";
    const char *publish_topic = "rgb_camera/image_raw";

public:
    ros::NodeHandle n;
    ros::ServiceClient _client;
    ros::ServiceServer _service;
    ros::Subscriber _sub_ZZZ;
    ros::Publisher _pub_YYY;

    SubscribeAndPublish()
    {
        ROS_INFO("Arm  init");
        _sub_ZZZ = n.subscribe(camera_topic, 10, &SubscribeAndPublish::XXX_callback, this);
        _pub_YYY = n.advertise<std_msgs::Float64>(publish_topic, 10);
        _client = n.serviceClient<simple_arm::GoToPosition>(service_topic);
        _service = n.advertiseService(service_topic, service_request);
    }

    void XXX_callback(const sensor_msgs::Image img)
    {
        std::stringstream os;
        os << "SUBSCRIPTION:" << camera_topic << "(vel/angle) "
           << boost::format("%1.2f") % (float)req.linear_x << "/"
           << boost::format("%12.2f") % (float)req.linear_x; //<< std::endl;
        ROS_INFO_STREAM(os.str());

        bool uniform_image = true;
        for (int i = 0; i < img.height * img.step; i++)
        {
            if (img.data[i] - img.data[0] != 0)
            {
                uniform_image = false;
                break;
            }
        }
        if (uniform_image)
        {
            simple_arm::GoToPosition srv;
            srv.request.joint_1 = 1.57;
            srv.request.joint_2 = 1.57;
            std::stringstream os;
            os << "REQUEST:" << publish_topic << "(vel/angle) "
               << boost::format("%12.2f") % (float)j_angle;
            ROS_INFO_STREAM(os.str());
            if (!_client.call(srv))
        }
        // PUBLISH
        {
            std_msgs::Float64 j_angle = 42.0;
            std::stringstream os;
            os << "PUBLISH:" << publish_topic << "(vel/angle) "
               << boost::format("%12.2f") % (float)j_angle;
            ROS_INFO_STREAM(os.str());
            _pub_YYY.publish(j_angle);
        }
    }
};

bool service_request(simple_arm::GoToPosition::Request &req, simple_arm::GoToPosition::Response &res)
{
    std::stringstream os;
    os << "REQUEST:" << NODE_NAME << "/" << service_topic << "(vel/angle) "
       << boost::format("%1.2f") % (float)req.linear_x << "/"
       << boost::format("%12.2f") % (float)req.linear_x; //<< std::endl;
    ROS_INFO_STREAM(os.str());

    joint1_angle.data = 1.0;
    _sub_ZZZ.publish(joint1_angle);
    ros::Duration(3).sleep();

    res.msg_feedback = "Joint angles set - j1: " + std::to_string(joint_angles[0]) + " , j2: " + std::to_string(joint_angles[1]);
    os << "RESPONSE:" << NODE_NAME << "/" << service_topic << " - "
       << res.msg_feedback;
    ROS_INFO_STREAM(os.str());
    return true;
}

int main(int argc, char **argv)
{
    SubscribeAndPublish snp;
    std::stringstream os;
    ros::init(argc, argv, NODE_NAME);
    os << "NODE STARTUP:" << NODE_NAME << "  File:" << __FILE__;
    ROS_INFO_STREAM(os.str());
    ros::spin();
    os << "NODE SHUTDOWN:" << NODE_NAME << "  File:" << __FILE__;
    ROS_INFO_STREAM(os.str());
    return 0;
}