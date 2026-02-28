```xml
<launch>
  <!-- By default we do not overwrite the URDF. Change the following to true to change the default behavior -->
  <arg name="load_robot_description" default="false"/>

  <!-- The name of the parameter under which the URDF is loaded -->
  <arg name="robot_description" default="robot_description"/>

  <!-- Load universal robot description format (URDF) -->
  <param if="$(arg load_robot_description)" name="$(arg robot_description)" command="xacro  '$(find robot_description)/urdf/aubo_i5.xacro'"/>

  <!-- The semantic description that corresponds to the URDF -->
  <param name="$(arg robot_description)_semantic" textfile="$(find robot_moveit_config)/config/aubo_i5.srdf" />

  <!-- Load updated joint limits (override information from URDF) -->
  <group ns="$(arg robot_description)_planning">
    <rosparam command="load" file="$(find robot_moveit_config)/config/joint_limits.yaml"/>
    <rosparam command="load" file="$(find robot_moveit_config)/config/cartesian_limits.yaml"/>
  </group>

  <!-- Load default settings for kinematics; these settings are overridden by settings in a node's namespace -->
  <group ns="$(arg robot_description)_kinematics">
    <rosparam command="load" file="$(find robot_moveit_config)/config/kinematics.yaml"/>

  </group>

</launch>
```
以上是一个经典的planning_context.launch的文件，可以看到主要有四部分组成，都是加载到参数服务器中：
- URDF文件
- SRDF文件
- 设置约束(关节约束、笛卡尔约束)
- 运动学算法设置