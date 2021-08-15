// Copyright 2021 RoboMaster-OSS
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RMOSS_IGN_BASE__ADVANCED_CHASSIS_CONTROLLER_HPP_
#define RMOSS_IGN_BASE__ADVANCED_CHASSIS_CONTROLLER_HPP_

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rmoss_ign_base/ign_chassis_cmd.hpp"
#include "rmoss_ign_base/ign_joint_encoder.hpp"
#include "rmoss_ign_base/ign_imu.hpp"
#include "rmoss_ign_base/pid.hpp"
#include "rmoss_interfaces/msg/chassis_cmd.hpp"
#include "geometry_msgs/msg/twist.hpp"

namespace rmoss_ign_base
{

class AdvancedChassisController
{
public:
  AdvancedChassisController(
    const rclcpp::Node::SharedPtr & node,
    const std::string & ros_chassis_cmd_topic,
    std::shared_ptr<IgnChassisCmd> & ign_chassis_cmd,
    std::shared_ptr<IgnJointEncoder> & ign_gimbal_encoder);
  ~AdvancedChassisController() {}

public:
  void set_chassis_pid(struct PidParam pid_param);
  void set_control_mode(bool follow_mode_flag) {follow_mode_flag_ = follow_mode_flag;}

private:
  void chassis_cb(const rmoss_interfaces::msg::ChassisCmd::SharedPtr msg);
  void cmd_vel_cb(const geometry_msgs::msg::Twist::SharedPtr msg);
  void update();

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<rmoss_interfaces::msg::ChassisCmd>::SharedPtr ros_chassis_cmd_sub_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr ros_cmd_vel_sub_;
  rclcpp::CallbackGroup::SharedPtr callback_group_;
  rclcpp::TimerBase::SharedPtr controller_timer_;
  // ignition tool
  std::shared_ptr<IgnChassisCmd> ign_chassis_cmd_;
  std::shared_ptr<IgnJointEncoder> ign_gimbal_encoder_;
  // target data
  double target_vx_{0};
  double target_vy_{0};
  double target_w_{0};
  std::mutex msg_mut_;
  // pid and pid parameter
  ignition::math::PID chassis_pid_;
  PidParam chassis_pid_param_;
  // flag
  bool update_pid_flag_{false};
  bool follow_mode_flag_{true};
};


}  // namespace rmoss_ign_base

#endif  // RMOSS_IGN_BASE__ADVANCED_CHASSIS_CONTROLLER_HPP_
