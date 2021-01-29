# UDACITY TURTLE KALMAN LAB

## LAB SETUP
 - Clone Package:
   - cd /home/workspace/catkin_ws/src
   - git clone https://github.com/turtlebot/turtlebot_simulator
 - Install Dependencies:
   - cd /home/workspace/catkin_ws
   - source devel/setup.bash
   - rosdep -i install turtlebot_gazebo
 - Build Package:
   - catkin_make
   - source devel/setup.bash
 - Launch Nodes:
   - roslaunch turtlebot_gazebo turtlebot_world.launch
 - Topics:
   - rostopic list
        - Or
   - rosrun rqt_graph rqt_graph
