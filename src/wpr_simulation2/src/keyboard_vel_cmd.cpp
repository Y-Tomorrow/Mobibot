/*********************************************************************
* Software License Agreement (BSD License)
* 
*  Copyright (c) 2017-2020, Waterplus http://www.6-robot.com
*  All rights reserved.
* 
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
* 
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the WaterPlus nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  FOOTPRINTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/
/*!******************************************************************
 @author     ZhangWanjie
 ********************************************************************/

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <stdio.h>
#include <termios.h>

static float linear_vel = 0.1;
static float angular_vel = 0.1;
static int k_vel = 6;

int GetCh()
{
  static struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  int c = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return c;
}

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  printf("Keyboard Control for WPR Robot: \n");
  printf("w - Accelerate forward \n");
  printf("s - Accelerate backward \n");
  printf("a - Accelerate left \n");
  printf("d - Accelerate right \n");
  printf("q - Accelerate left rotation \n");
  printf("e - Accelerate right rotation \n");
  printf("space - Brake \n");
  printf("x - Exit \n");
  printf("------------- \n");

  auto node = rclcpp::Node::make_shared("keyboard_vel_cmd");

  auto cmd_vel_pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

  auto base_cmd = std::make_shared<geometry_msgs::msg::Twist>();
  base_cmd->linear.x = 0;
  base_cmd->linear.y = 0;
  base_cmd->angular.z = 0;

  while (rclcpp::ok())
  {
    int cKey = GetCh();
    if (cKey == 'w')
    {
      base_cmd->linear.x += linear_vel;
      if (base_cmd->linear.x > linear_vel * k_vel)
        base_cmd->linear.x = linear_vel * k_vel;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
    }
    else if (cKey == 's')
    {
      base_cmd->linear.x += -linear_vel;

      if (base_cmd->linear.x < -linear_vel * k_vel)
        base_cmd->linear.x = -linear_vel * k_vel;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
    }
    else if (cKey == 'a')
    {
      base_cmd->linear.y += linear_vel;
      if (base_cmd->linear.y > linear_vel * k_vel)
        base_cmd->linear.y = linear_vel * k_vel;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
    }
    else if (cKey == 'd')
    {
      base_cmd->linear.y += -linear_vel;
      if (base_cmd->linear.y < -linear_vel * k_vel)
        base_cmd->linear.y = -linear_vel * k_vel;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
    }
    else if (cKey == 'q')
    {
      base_cmd->angular.z += angular_vel;
      if (base_cmd->angular.z > angular_vel * k_vel)
        base_cmd->angular.z = angular_vel * k_vel;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
    }
    else if (cKey == 'e')
    {
      base_cmd->angular.z += -angular_vel;
      if (base_cmd->angular.z < -angular_vel * k_vel)
        base_cmd->angular.z = -angular_vel * k_vel;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
    }
    else if (cKey == ' ')
    {
      base_cmd->linear.x = 0;
      base_cmd->linear.y = 0;
      base_cmd->angular.z = 0;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
    }
    else if (cKey == 'x')
    {
      base_cmd->linear.x = 0;
      base_cmd->linear.y = 0;
      base_cmd->angular.z = 0;
      cmd_vel_pub->publish(*base_cmd);
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n", base_cmd->linear.x, base_cmd->linear.y, base_cmd->angular.z);
      printf("Exited! \n");
      return 0;
    }
    else
    {
      printf(" - Undefined command \n");
    }
  }

  rclcpp::shutdown();
  return 0;
}
