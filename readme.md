依赖安装脚本

    cd src/wpr_simulation2/scripts/
    ./install_for_humble.sh

colcon build

source install/setup.bash

地图在src/wpr_simulation2/worlds/factory.world

factory1.world里的路径改一下，可以搜索‘/home/yy/factory_ws/src/models’，修改这些路径

编辑查看地图 

    gazebo factory1.world

在insert中添加模型

1.仿真建图（会加载比较久）

    ros2 launch wpr_simulation2 factory.launch.py 
    ros2 launch slam_pkg slam.launch.py 
    ros2 run wpr_simulation2 keyboard_vel_cmd # 控制机器人移动

地图保存
控制小车终端按X退出，ros2 run nav2_map_server map_saver_cli -f map
ros2 run nav2_map_server map_saver_cli -f ~/factory_ws/src/wpr_simulation2/maps/map
将生成在主目录的两个map文件移动到/src/wpr_simulation2/maps/

2.导航

    ros2 launch wpr_simulation2 factory.launch.py 
    ros2 launch nav_pkg nav.launch.py

先用2D Pose Estimate确定机器人初始位置
用nav2 goal给定目标点

可以add相机获取到的画面（也有深度图）

后面可以根据自己的机器人去修改