#include "ros/ros.h"
#include <vector>
//#include "simple_arm/GoToPosition.h"
//#include <sensor_msgs/JointState.h>
//#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <std_msgs/Float64.h>
#include <boost/format.hpp>
#include <sstream>
#include <algorithm>

////////////////////////
#define NODE_NAME "process_image"

class SubscribeAndPublish
{
private:
    const bool _debugEnabled = true;
    const char *sub_camera_topic = "/camera/rgb/image_raw";
    const char *srv_robot = "/ball_chaser/command_robot";
    const int ball_threshold = 15;
    typedef enum
    {
        BALL_LEFT,
        BALL_CENTER,
        BALL_RIGHT,
        BALL_MISSING,
        BALL_REACHED
    } ball_yaw_t;

public:
    ros::NodeHandle n;
    ros::Subscriber _sub_camera;
    ros::ServiceClient _client;

    SubscribeAndPublish()
    {
        ROS_INFO("SubscribeAndPublish  init");
        _sub_camera = n.subscribe(sub_camera_topic, 10, &SubscribeAndPublish::camera_callback, this);
        _client = n.serviceClient<ball_chaser::DriveToTarget>(srv_robot);
        //_client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    }

    void ballCtlr(ball_yaw_t ball_yaw, float error)
    {
        const float gain = 1;
        ball_chaser::DriveToTarget drive;
        const float fwdVel = 1;
        const float turnVel = gain * error;
        float lin_x = 0.0;
        float ang_z = 0.0;
        switch (ball_yaw)
        {
        case BALL_LEFT:
            ang_z = turnVel;
            break;
        case BALL_CENTER:
            lin_x = fwdVel;
            break;
        case BALL_RIGHT:
            ang_z = -turnVel;
            break;
        case BALL_MISSING:
            ang_z = -0.2;
        case BALL_REACHED:
            break;
        }
        drive.request.linear_x = lin_x;
        drive.request.angular_z = ang_z;
        auto msg_feedback = _client.call(drive);
    }
    //The standard has a specific feature to improve the efficiency of returning by
    // value. It's called "copy elision", and more specifically in this case the
    // "named return value optimization (NRVO)".
    // When NRVO is applied,there will be no copying in the following code
    std::vector<int> colsum(const sensor_msgs::Image &img)
    {
        //ss int idx = 0;
        //ROS_INFO_STREAM("STEP:" << img.step);
        const int redOffset = 0;
        const int greenOffset = 1;
        const int blueOffset = 2;
        const int white_pixel = 250;
        const int step = img.step;
        std::vector<int> cSum(img.width, 0);
        if (step == img.width * 3)
        {
            for (int row = 0; row < img.height; row++)
            {
                int idx = row * step;
                for (int col = 0; col < img.width; col++, idx += 3)
                {
                    if (
                        img.data[idx + redOffset] >= white_pixel &&
                        img.data[idx + greenOffset] >= white_pixel &&
                        img.data[idx + blueOffset] >= white_pixel)
                    {
                        cSum[col]++;
                    }
                }
            }
        }
        else
        {
            ROS_ERROR("RGB IMAGE ERROR");
        }
        return cSum;
    }

    float ballDistance(const std::vector<int> &cSum)
    {
        const float percentGoal = 0.5;
        const int totalCol = cSum.size();
        int nEmptyCol = 0;
        for (auto col : cSum)
        {
            nEmptyCol += (0 == col) ? 1 : 0;
        }
        float percentBall = (totalCol - nEmptyCol) / (float)totalCol;
        float percent = std::max(0.0F, 1 - (percentBall / percentGoal));
        return percent;
    }

    void camera_callback(const sensor_msgs::Image img)
    {
        std::stringstream os;
        int ballColOffset;
        float bl;
        float error = 0.0F;
        ball_yaw_t ball_yaw = BALL_MISSING;
        std::vector<int> cSum = colsum(img); // NRVO return optimization
        std::vector<int>::iterator max_it = std::max_element(cSum.begin(), cSum.end());
        const float distancePercent = ballDistance(cSum);
        if (0.0F == distancePercent)
        {
            ball_yaw = BALL_REACHED;
        }
        else if (*max_it >= ball_threshold)
        { // WARNING - MAGIC NUMBERS BELOW
            ballColOffset = std::distance(cSum.begin(), max_it);
            bl = (float)ballColOffset / img.width;
            ball_yaw = bl < 0.4F ? BALL_LEFT : (bl > 0.6F ? BALL_RIGHT : BALL_CENTER);
        }
        if (BALL_LEFT == ball_yaw || BALL_RIGHT == ball_yaw)
        {
            int middleCol = (img.width / 2);
            error = abs(middleCol - ballColOffset) / (float)middleCol;
        }
        ballCtlr(ball_yaw, error);
        if (_debugEnabled)
        {
            os << "M:" << *max_it << " yaw:" << ball_yaw
               << " Err:" << error << " Dist:" << distancePercent << " ";
            for (int col = 0; col < img.width; col += 10)
            {
                os << (10 * cSum[col]) / (1 + (*max_it));
            }
            ROS_INFO_STREAM(os.str());
        }
        // os.clear();
        // os << "SUBSCRIPTION:" << sub_camera_topic << "(height/step) "
        //    << boost::format("%d") % img.height << "/"
        //    << boost::format("%d") % img.step; //<< std::endl;
        // ROS_INFO_STREAM(os.str());
    }
}; //SubscribeAndPublish

int main(int argc, char **argv)
{
    ros::init(argc, argv, NODE_NAME);
    SubscribeAndPublish snp;
    std::stringstream os;
    os << "NODE STARTUP:" << NODE_NAME << "  File:" << __FILE__;
    ROS_INFO_STREAM(os.str());
    ros::spin();
    os << "NODE SHUTDOWN:" << NODE_NAME << "  File:" << __FILE__;
    ROS_INFO_STREAM(os.str());
    return 0;
}