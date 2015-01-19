Racing:
//To start the racing nodes, the following steps are necessary:

// run rviz and launch hardware
- new terminal
- roslaunch tas run_rviz.launch

// initialize amcl for starting position one
- new terminal
- rosrun tas_autonomous_control initial_pose

// start obstacle avoidance node
- new terminal
- rosrun tas_autonomous_control easy_bypass

// start racing lookup and velocity regulation node
// important: policy must be stored as pol.txt in your catkin workspace folder.
// see readme in matlab/DP gauss seidel folder on how policies are created (only if new policy is required)
- new terminal
- cd (your catkin workspace) 
- rosrun tas_autonomous_control dynamic_angular_control



Parking:

// run rviz and launch hardware
- new terminal
- roslaunch tas run_rviz.launch

// start parking node
- new terminal
- rosrun tas_autonomous_control parking