#!/usr/bin/env python3
#
# Copyright 2023 6-robot.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Authors: Zhang Wanjie

import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    launch_file_dir = os.path.join(get_package_share_directory('wpr_simulation2'), 'launch')
    pkg_gazebo_ros = get_package_share_directory('gazebo_ros')

    use_sim_time = LaunchConfiguration('use_sim_time', default='true')

    world = os.path.join(
        get_package_share_directory('wpr_simulation2'),
        'worlds',
        'table.world'
    )

    gzserver_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gzserver.launch.py')
        ),
        launch_arguments={'world': world}.items()
    )

    gzclient_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gzclient.launch.py')
        )
    )

    # robot_state_publisher_cmd = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(
    #         os.path.join(launch_file_dir, 'robot_state_publisher.launch.py')
    #     ),
    #     launch_arguments={'use_sim_time': use_sim_time}.items()
    # )

    spawn_robot_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(launch_file_dir, 'spawn_wpb_mani.launch.py')
        )
    )

    spawn_table = Node(
            package='gazebo_ros',
            namespace='',
            executable='spawn_entity.py',
            name='spawn_entity',
            arguments=['-file', [os.path.join(get_package_share_directory('wpr_simulation2'), 'models', 'table.model')] , 
            '-entity', 'table',
            '-x', '1.2',
            '-y', '0.0',
            '-Y', '0.0']
        )

    spawn_red_bottle = Node(
            package='gazebo_ros',
            namespace='',
            executable='spawn_entity.py',
            name='spawn_entity',
            arguments=['-file', [os.path.join(get_package_share_directory('wpr_simulation2'), 'models', 'bottles', 'red_bottle.model')] , 
            '-entity', 'red_bottle',
            '-x', '1.1',
            '-y', '0.3',
            '-z', '2.0',
            '-Y', '0.0']
        )

    spawn_green_bottle = Node(
            package='gazebo_ros',
            namespace='',
            executable='spawn_entity.py',
            name='spawn_entity',
            arguments=['-file', [os.path.join(get_package_share_directory('wpr_simulation2'), 'models', 'bottles', 'green_bottle.model')] , 
            '-entity', 'green_bottle',
            '-x', '1.1',
            '-y', '-0.2',
            '-z', '2.0',
            '-Y', '0.0']
        )

    rviz_cmd = Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', [os.path.join(get_package_share_directory('wpr_simulation2'), 'rviz', 'pointcloud.rviz')]]
        )

    ld = LaunchDescription()

    # Add the commands to the launch description
    ld.add_action(gzserver_cmd)
    ld.add_action(gzclient_cmd)
    ld.add_action(spawn_robot_cmd)
    ld.add_action(spawn_table)
    ld.add_action(spawn_red_bottle)
    ld.add_action(spawn_green_bottle)
    # ld.add_action(rviz_cmd)

    return ld
