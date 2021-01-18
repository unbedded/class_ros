# class_ros
udacity - Robotics Software Engineer - nanoDegree

## WORKSPACE Setup
'''
mkdir -p XXX_ws/src
cd XXX_ws/src
catkin_init_workspace
## PACKAGE Setup
'''
$ cd XXX_ws/src/
$ catkin_create_pkg YYY_pkg
mkdir -p YYY_pkg/lanch
mkdir -p YYY_pkg/worlds
'''


## WORLD TEMPLATE 
    '''
    <?xml version="1.0" ?>
    <sdf version="1.4">
    <world name="default">
        <include><uri>model://ground_plane</uri></include>
        <!-- Light source -->
        <include><uri>model://sun</uri></include>

        <!-- World camera -->
        <gui fullscreen='0'>
        <camera name='world_camera'>
            <pose>4.927360 -4.376610 3.740080 0.000000 0.275643 2.356190</pose>
            <view_controller>orbit</view_controller>
        </camera>
        </gui>
    </world>
    </sdf>
    '''

## LAUNCH template 
'''
    <?xml version="1.0" encoding="UTF-8"?>
    <launch>
    <!-- World File -->
    <arg name="world_file" default="$(find my_robot)/worlds/empty.world"/>
    <!-- Launch Gazebo World -->
    <include file="$(find gazebo_ros)/launch/######.launch">
        <arg name="use_sim_time" value="true"/>
        <arg name="debug" value="false"/>
        <arg name="gui" value="true" />
        <arg name="world_name" value="$(arg world_file)"/>
    </include>
    <!-- Find my robot Description-->
    <param name="robot_description" command="$(find xacro)/xacro --inorder '$(find my_robot)/urdf/my_robot.xacro'" />

    <!-- Spawn My Robot -->
    <node name="urdf_spawner" pkg="gazebo_ros" type="spawn_model" respawn="false" output="screen" 
        args="-urdf -param robot_description -model my_robot 
              -x $(arg x) -y $(arg y) -z $(arg z)
              -R $(arg roll) -P $(arg pitch) -Y $(arg yaw)"/>    
    </launch>
'''

## URDF template 
'''
    <?xml version="1.0"?>
    <robot name="two_link_robot">
    <!--Links-->
    <link name="link_1">
        <visual>
        <geometry>
            <cylinder length="0.5" radius="0.2"/>
        </geometry>
        </visual>
    </link>
    <link name="link_2">
        <visual>
        <geometry>
            <box size="0.6 0.1 0.2"/>
        </geometry>
        </visual>
    </link>
    <!--Joints-->
    <joint name="joint_1" type="continuous">
        <parent link="link_1"/>
        <child link="link_2"/>
    </joint>
    </robot>
'''
