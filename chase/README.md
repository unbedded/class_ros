**Go Chase It!**
  [ Download Project](https://review-api.udacity.com/api/v1/submissions/2704136/archive)

What we covered in this project
- how to create publishers and subscribers
- how to create services
- how to create and handle callback functions
- how to handle image sensor data.

In this project, you should create two ROS packages inside your catkin_ws/src: the drive_bot and the ball_chaser. Here are the steps to design the robot, house it inside your world, and program it to chase white-colored balls:

 1. **drive_bot**
    - Create a my_robot ROS package to hold your robot, the white ball, and the world.
    - Design a differential drive robot with the Unified Robot Description Format. Add two sensors to your robot: a lidar and a camera. Add Gazebo plugins for your robot’s differential drive, lidar, and camera. The robot you design should be significantly different from the one presented in the project lesson. Implement significant changes such as adjusting the color, wheel radius, and chassis dimensions. Or completely redesign the robot model! After all you want to impress your future employers :-D
    - House your robot inside the world you built in the Build My World project.
    - Add a white-colored ball to your Gazebo world and save a new copy of this world.
    - The world.launch file should launch your world with the white-colored ball and your robot.

 2.  **ball_chaser:**
    - Create a ball_chaser ROS package to hold your C++ nodes.
    - Write a drive_botC++ node that will provide a ball_chaser/command_robot service to drive the robot by controlling its linear x and angular z velocities. The service should publish to the wheel joints and return back the requested velocities.
    - Write a process_image C++ node that reads your robot’s camera image, analyzes it to determine the presence and position of a white ball. If a white ball exists in the image, your node should request a service via a client to drive the robot towards it.
    - The ball_chaser.launch should run both the drive_bot and the process_image nodes.

Your submission should follow the directory structure and contain all the files listed here:

```
    .Project2                          # Go Chase It Project
    ├── my_robot                       # my_robot package                   
    │   ├── launch                     # launch folder for launch files   
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
    ├── ball_chaser                    # ball_chaser package                   
    │   ├── launch                     # launch folder for launch files   
    │   │   ├── ball_chaser.launch
    │   ├── src                        # source folder for C++ scripts
    │   │   ├── drive_bot.cpp
    │   │   ├── process_images.cpp
    │   ├── srv                        # service folder for ROS services
    │   │   ├── DriveToTarget.srv
    │   ├── CMakeLists.txt             # compiler instructions
    │   ├── package.xml                # package info                  
    └──                              
```

**Note**
Create a folder (rviz) and save inside the rviz configuration you created in
rviz.  Then use \<node type="rviz" name="rviz" pkg="rviz" args="-d \$(find
package_name)/rviz/config_file.rviz" /\> to load a saved configuration to Rviz

**Robot Design**
-   Lidar and camera sensors.
-   Gazebo plugins for the robot’s differential drive, lidar, and camera.
-   Housed inside the world
-   Significant changes from the sample taught in the project lesson.
-   Robot is stable when moving

**External Material**
You can try exploring actionlib in ROS by following [thistutorial](http://wiki.ros.org/actionlib/Tutorials) in ROS/wiki

Finally, you can try using OpenCV to tackle this problem. I would like to [share](https://youtu.be/0H-OVbAhwqA) with you a video I made as a mentor to my students on how to approach the problem using OpenCV. You might find it interesting.

**Steering Methods**
[Video: Common Steering Methods](https://www.youtube.com/watch?v=F3G0sUz3_Jw&feature=emb_title)  
[Video: How to configure the differential drive ROS controller](https://www.youtube.com/watch?v=9OWxX7PA4SU)  
[Video: Omnidirectional Wheeled Mobile Robots (Part 1 of 2)](https://www.youtube.com/watch?v=NcOT9hOsceE)  
[Video: Omnidirectional Wheeled Mobile Robots (Part 2 of 2)](https://www.youtube.com/watch?v=B1K-ti5Lqjc)

**ROS Launch**
-   Library of roslaunch [command-line syntax](http://wiki.ros.org/roslaunch/Commandline%20Tools).
-   Roslaunch organization for large robotics [projects](https://review.udacity.com/wiki.ros.org/roslaunch/Tutorials/Roslaunch%20tips%20for%20larger%20projects).
-   How to use roslaunch with nodes [testing and debugging](http://wiki.ros.org/roslaunch/Tutorials/Roslaunch%20Nodes%20in%20Valgrind%20or%20GDB).

**ROS Configuration**
[Differential Drive Controller vs Gazebo ros controller](https://answers.ros.org/question/209389/what-is-the-difference-between-the-gazebo-differential-drive-plugin-and-a-differential-drive-controller-spawned-by-controller_spawner/)

**More on sensors models available in ROS and their plugins**
-   [List of ROS sensor plugins](http://gazebosim.org/tutorials?tut=ros_gzplugins)
-   [Creating our own sensor plugin tutorial](https://review.udacity.com/The%20next%20step%20is%20to%20make%20sure%20we%20are%20confident%20writing%20our%20own%20plugins,%20and%20this%20is%20explained%20here%20with%20a%20great%20level%20of%20details.)

**Misc**
 - [Video: Import a model from SolidWorks into Gazebo.](https://www.youtube.com/watch?v=T7X_p_KMwus)  
 - [Video: Create a two store appartment](https://www.youtube.com/watch?v=gSurY5XlsIs)

**Gazebo World Requirements**
-   Same as the world designed in the **Build My World** project or a new world
    that you design on the building editor for this project.
-   Includes a white-colored ball
-   Same as the world designed in the Build My World project or a new world that
    you design on the building editor for this project.
-   Includes a white-colored ball

**Ball Chasing Requirements**
-   A ball_chaser/command_robot service.
-   Service accepts linear x and angular z velocities.
-   Service publishes to the the wheel joints.
-   Service returns the requested velocities.

**process_image requirements:**
-   Subscribes to the robot’s camera image.
-   A function to analyze the image and determine the presence and position of a
white ball.
-   Requests a service to drive the robot towards a white ball (when present).

**drive_bot requirements:**
-   A ball_chaser/command_robot service.
-   Service accepts linear x and angular z velocities.
-   Service publishes to the the wheel joints.
-   Service returns the requested velocities.

**Launch Files Requirements**
-   Launch the world (which includes a white ball).
-   Launch the robot.

**World.launch requirements**
-   Launch the world (which includes a white ball).
-   Launch the robot.

**ball_chaser.launch requirements**
-   Run the drive_bot C++ node.
-   Run the process_image C++ node.

