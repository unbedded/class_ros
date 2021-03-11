# UDACITY ROS Robotics
- URL: https://www.udacity.com/course/robotics-software-engineer--nd209
- ROS Nano-degree class

# PJT#5 - rTab-MAP - Map My World

## Resources
- RTAB-Map ROS Wiki page http://wiki.ros.org/rtabmap_ros
  - Tutorial -http://wiki.ros.org/rtabmap_ros/Tutorials/SetupOnYourRobot 

    RTAB-Map Parameter Tutorial - http://wiki.ros.org/rtabmap_ros/Tutorials/Advanced%20Parameter%20Tuning
    List of RTAB-Map Parameters - https://github.com/introlab/rtabmap/blob/master/corelib/include/rtabmap/core/Parameters.h 

## Sensor Topics

- A 2D Laser, providing sensor_msgs/LaserScan messages
- Odometry sensors, providing nav_msgs/Odometry messages
- 3D Camera, compatible with openni_launch, openni2_launch or freenect_launch ROS packages


## Testing Procedure
- roslaunch my_robot world.launch
- roslaunch my_robot mapping.launch
- rosrun teleop_twist_keyboard teleop_twist_keyboard.py

### Analysis
- rtabmap-databaseViewer ~/.ros/rtabmap.db
  - Say yes to using the database parameters
    View -> Constraint View
    View -> Graph View

## OPTION
Optional: RTAB-Map Localization

If you desire to perform localization using the map you created, there are only a few changes you need to make. You can start by duplicating your mapping.launch file and renaming the duplicated file tolocalization.launch.

The following changes also need to be made to the localization.launch file:

    Remove the args="--delete_db_on_start" from your node launcher since you will need your database to localize too.

    Remove the Mem/NotLinkedNodesKept parameter

    Add the Mem/IncrementalMemory parameter of type string and set it to false to finalize the changes needed to put the robot into localization mode.

This is another method for localization you can keep in mind when working on your next robotics project!
