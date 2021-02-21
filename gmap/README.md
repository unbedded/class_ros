# UDACITY ROS Robotics
- URL: https://www.udacity.com/course/robotics-software-engineer--nd209
- ROS Nano-degree class

# BACKGROUND
gmapping provides laser based SLAM. Meaning that you can feed its node with the robot laser measurements and odometry values and expect it to provide you with a 2D occupancy grid map of the environment. The map will be updated as the robot moves and collect sensory information using its laser range finder sensor. 
- Access this link and go over the documentation of the gmapping ROS package http://wiki.ros.org/gmapping


```
 Laser Scan
         +        +----------------+
         +------->+                |
                  |  G-SLAM Map    +---> 2D Occu Grid
        +-------->+                |
Odometry+         +----------------+

```

# SETUP
- Create a catkin_ws in /home/workspace/
  - mkdir -p /home/workspace/catkin_ws/src
  - cd /home/workspace/catkin_ws/src
  - catkin_init_workspace
  - cd ..
  - catkin_make
- Perform a System Update/Upgrade
  - apt-get update
  - apt-get upgrade -y
- Clone turtlebot_gazebo and turtlebot_teleop in src
  - cd src/
  - git clone https://github.com/turtlebot/turtlebot_simulator
  - git clone https://github.com/turtlebot/turtlebot
- Install packages dependencies
The dependencies are already installed in the workspace but it's always good to check!
  - cd ..
  - source devel/setup.bash
  - rosdep -i install turtlebot_gazebo
  - rosdep -i install turtlebot_teleop
- Build the packages
  - catkin_make
  - source devel/setup.bash
- RUN 
  - roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=worlds/willowgarage.world

- Teleop Package
  - [ 36%] Built target turtlebot_teleop_joy
  - roslaunch turtlebot_teleop keyboard_teleop.launch


