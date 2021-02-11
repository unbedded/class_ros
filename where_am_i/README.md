# ROS AMCL LAB - (**A**daptive MCL)
**mcl** is a probabilistic localization system for a robot moving in 2D. It implements the adaptive (or KLD-sampling) Monte Carlo localization approach (*as described by Dieter Fox - Probabilistic Robotics* ), which uses a particle filter to track the pose of a robot against a known map. ROS AMCL uses:

 - sample_motion_model_odometry, beam_range_finder_model, likelihood_field_range_finder_model, Augmented_MCL, and KLD_Sampling_MCL.

As currently implemented, this node works only with laser scans and laser maps. It could be extended to work with other sensor data. 

# SETUP 
## Native Installation & Virtual Machine

If you are working with a native ROS installation or using a VM, some of the following package might need to be installed. You could install them as shown below:
```
$ sudo apt-get install ros-kinetic-navigation
$ sudo apt-get install ros-kinetic-map-server
$ sudo apt-get install ros-kinetic-move-base
$ sudo apt-get install ros-kinetic-amcl
```

## SETUP - my_robot 
In this project, you should create two ROS packages inside your catkin_ws/src: the drive_bot and the ball_chaser. Here are the steps to design the robot, house it inside your world, and program it to chase white-colored balls:

 1. **drive_bot**
    - Create a my_robot ROS package to hold your robot, the white ball, and the world.
    - Design a differential drive robot with the Unified Robot Description Format. Add two sensors to your robot: a lidar and a camera. Add Gazebo plugins for your robot’s differential drive, lidar, and camera. The robot you design should be significantly different from the one presented in the project lesson. Implement significant changes such as adjusting the color, wheel radius, and chassis dimensions. Or completely redesign the robot model! After all you want to impress your future employers :-D
    - House your robot inside the world you built in the Build My World project.
    - Add a white-colored ball to your Gazebo world and save a new copy of this world.
    - The world.launch file should launch your world with the white-colored ball and your robot.


Your submission should follow the directory structure and contain all the files listed here:

```
    .Project                           # WHERE AM I
    ├── my_robot                       # my_robot package                   
    │   ├── launch                     # launch folder - launch files   
    │   │   ├── robot_description.launch
    │   │   ├── world.launch
    │   ├── meshes                     # meshes folder for sensors
    │   │   ├── hokuyo.dae
    │   ├── urdf                       # urdf folder for xarco files
    │   │   ├── my_robot.gazebo
    │   │   ├── my_robot.xacro
    │   ├── world                      # world folder for world files
    │   │   ├── <yourworld>.world
    │   ├── CMakeLists.txt             # compiler instructions
    │   ├── package.xml                # package info
    |
```

## Verify Your Simulation Setup
- Do a quick catkin_make and source the devel/setup.bash script. Launch the world to verify if the system is good to go!
  - roslaunch my_robot world.launch

## PGM Map File
By default, AMCL package will treat 'darker' pixels as obstacle in the pgm map file, and 'lighter' pixels as free space. The threshold could be set as a parameter which we will cover when we are building the launch file.
- We need libignition-math2-dev and protobuf-compiler to compile the map creator:
  - sudo apt-get install libignition-math2-dev protobuf-compiler
- Clone the Repository: Clone the package pgm_map_creator to your src folder.
  - cd /home/workspace/catkin_ws/src/
  - git clone https://github.com/udacity/pgm_map_creator.git
  - cd ..
  - catkin_make

## Add and Edit the World File
- Copy the Gazebo world you created to the world folder
  - cp <YOUR GAZEBO WORLD FILE> src/pgm_map_creator/world/<YOUR GAZEBO WORLD FILE>
- Insert the map creator plugin to your map file. Open the map file using the editor of your choice. Add the following tag towards the end of the file, but just before \</world> tag:
  - \<plugin filename="libcollision_map_creator.so" name="collision_map_creator"/>


# Create the PGM Map!
Remember, the map is an **(PMG)** image, which means you could edit it using image processing softwares like gimp in Linux. If you have found the map not accurate due to the models, feel free to edit the pgm file directly!

- Open a terminal, run gzerver with the map file:
  - gzserver src/pgm_map_creator/world/willow_garage.world
- Open another terminal, launch the request_publisher node
   - roslaunch pgm_map_creator request_publisher.launch

## If ROSLAUNCH returns protobuf error
Go to the msgs folder inside pgm_map_creator package and modify the msgs/CMakeLists.txt as following:
```
set (msgs collision_map_request.proto ${PROTOBUF_IMPORT_DIRS}/vector2d.proto ${PROTOBUF_IMPORT_DIRS}/header.proto ${PROTOBUF_IMPORT_DIRS}/time.proto )
```
Change msgs/CMakeLists.txt to:
```
set (msgs collision_map_request.proto
 #${PROTOBUF_IMPORT_DIRS}/vector2d.proto
 #${PROTOBUF_IMPORT_DIRS}/header.proto
 #${PROTOBUF_IMPORT_DIRS}/time.proto )
```

## If PGM MAP IS CROPPED
If the map is cropped, you might want to adjust the parameters in launch/request_publisher.launch, namely the x and y values, which defines the size of the map:
```
  <arg name="xmin" default="-15" />
  <arg name="xmax" default="15" />
  <arg name="ymin" default="-15" />
  <arg name="ymax" default="15" />
  <arg name="scan_height" default="5" />
  <arg name="resolution" default="0.01" />
```

## Add the Map to Your Package

Now we have the map file, let us move it to where it is needed! That is the maps folder you created at the very beginning.
- cp src/pgm_map_creator/maps/map.pgm  src/my_robot/maps/

Create YAML file  
```
 cd src/my_robot/src/maps
 code map.yaml
 add 
  image: map.pgm
  resolution: 0.05
  origin: [-50.0, -50.0, 0.0]
  occupied_thresh: 0.65
  free_thresh: 0.196
  negate: 0
```

# AMCL launch file
Create the file *where_am_i/src/my_robot/launch/amcl.launch*

- **Map Server Node**
  - The first node is the map_server node (http://wiki.ros.org/map_server). The map_server node provides map data as a ROS service to other nodes such as the amcl node. Here, map_server node will locate the map you created in the Map Setup step and send it out as the map data.

- **AMCL Node**
  - The next node to be set up is the amcl node. As it was introduced in the previous concepts, it takes odometry and laser scan data to perform the AMCL localization.
  - From the ROS Wiki (http://wiki.ros.org/amcl), we could find the purpose of the following param - since Remember, AMCL package 'links' the robot (odom frame) with the world (map frame). These parameters are required for amcl package to localize the robot in the world.
    - *odom_frame_id* (string, default: "odom"): Which frame to use for odometry
    - *odom_model_type* (string, default: "diff"): Which model to use, either "diff", "omni", "diff-corrected" or "omni-corrected"
    - *base_frame_id* (string, default: "base_link"): Which frame to use for the robot base
    - *global_frame_id* (string, default: "map"): The name of the coordinate frame published by the localization system

-**Move Base Node**
  - zip the following into my_robot/config/
    - wget https://s3-us-west-1.amazonaws.com/udacity-robotics/Resource/where_am_i/config.zip


Your amcl.launch file should look like this now:
```
<!-- UDACITY ROS class - AMCL lab-->
<launch>
<arg name="map_file" default="$(find my_robot)/maps/map.yaml"/>
<node name="map_server" pkg="map_server" type="map_server" args="$(arg map_file)" />

<node name="amcl" pkg="amcl" type="amcl" output="screen">
  <!-- remap from="scan" to="<my_robot/laser/scan"/ -->
  <param name="odom_frame_id" value="odom"/>
  <param name="odom_model_type" value="diff-corrected"/>
  <param name="base_frame_id" value="robot_footprint"/>
  <param name="global_frame_id" value="map"/>
  <param name="initial_pose_x" value="-50"/>
  <param name="initial_pose_y" value="-50"/>    
</node>

<node name="move_base" pkg="move_base" type="move_base" respawn="false" output="screen">
  <!-- remap from="scan" to="<my_robot/laser/scan"/ -->
  <param name="base_global_planner" value="navfn/NavfnROS" />
  <param name="base_local_planner" value="base_local_planner/TrajectoryPlannerROS"/>
  <rosparam file="$(find my_robot)/config/costmap_common_params.yaml" command="load" ns="global_costmap" />
  <rosparam file="$(find my_robot)/config/costmap_common_params.yaml" command="load" ns="local_costmap" />
  <rosparam file="$(find my_robot)/config/local_costmap_params.yaml" command="load" />
  <rosparam file="$(find my_robot)/config/global_costmap_params.yaml" command="load" />
  <rosparam file="$(find my_robot)/config/base_local_planner_params.yaml" com
</node>

</launch>
```

#Teleop Package
Now you could run the teleop script as is described in the README file:
- cd /home/workspace/catkin_ws/src
- git clone https://github.com/ros-teleop/teleop_twist_keyboard
- cd ..  &  catkin_make & source devel/setup.bash
- rosrun teleop_twist_keyboard teleop_twist_keyboard.py


