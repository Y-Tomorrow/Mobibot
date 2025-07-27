#!/usr/bin/env python3
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess, TimerAction, RegisterEventHandler
from launch.event_handlers import OnProcessStart
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    # 1. 基础配置
    pkg_gazebo_ros = get_package_share_directory('gazebo_ros')
    world_path = os.path.join(
        get_package_share_directory('wpr_simulation2'),
        'worlds',
        'factory.world'
    )

    # 2. Gazebo服务器启动配置（关键性能优化）
    gzserver_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gzserver.launch.py')
        ),
        launch_arguments={
            'world': world_path,
            'extra_gazebo_args': '--verbose -s libgazebo_ros_init.so -s libgazebo_ros_factory.so --profiler render --physics ode',
            'pause': 'false'
        }.items()
    )

    # 3. Gazebo客户端配置（优化渲染）
    gzclient_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gzclient.launch.py')
        ),
        launch_arguments={
            'extra_gazebo_args': '--verbose --disable-clouds --disable-fog'
        }.items()
    )

    # 4. 机器人生成（优化参数）
    spawn_robot_cmd = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[
            '-entity', 'wpb_home',
            '-topic', 'robot_description',
            '-x', '-6.0', '-y', '-0.5', '-Y', '0.0',
            '-timeout', '60'
        ],
        output='screen',
        respawn=True,
        respawn_delay=5
    )

    # 5. 障碍物生成（更智能的延迟启动）
    spawn_objects_cmd = RegisterEventHandler(
        event_handler=OnProcessStart(
            target_action=spawn_robot_cmd,
            on_start=[
                TimerAction(
                    period=5.0,  # 机器人生成后5秒再加载障碍物
                    actions=[
                        IncludeLaunchDescription(
                            PythonLaunchDescriptionSource(
                                os.path.join(get_package_share_directory('wpr_simulation2'), 'launch', 'spawn_objects.launch.py')
                            )
                        )
                    ]
                )
            ]
        )
    )

    ld = LaunchDescription()
    
    # 调整启动顺序
    ld.add_action(gzserver_cmd)
    ld.add_action(gzclient_cmd)
    ld.add_action(spawn_robot_cmd)
    ld.add_action(spawn_objects_cmd)
    
    return ld