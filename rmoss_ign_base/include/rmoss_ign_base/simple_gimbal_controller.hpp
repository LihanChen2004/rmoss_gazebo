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

#ifndef RMOSS_IGN_BASE__SIMPLE_GIMBAL_CONTROLLER_HPP_
#define RMOSS_IGN_BASE__SIMPLE_GIMBAL_CONTROLLER_HPP_

#include <memory>
#include <string>
#include <mutex>

#include "ignition/transport/Node.hh"
#include "rclcpp/rclcpp.hpp"
#include "rmoss_interfaces/msg/gimbal_cmd.hpp"

namespace rmoss_ign_base
{

class SimpleGimbalController
{
public:
  SimpleGimbalController(
    const rclcpp::Node::SharedPtr & nh,
    const std::string & ros_cmd_topic,
    const std::string & ign_pitch_cmd_topic,
    const std::string & ign_yaw_cmd_topic,
    bool position_control = true);
  ~SimpleGimbalController() {}

private:
  void gimbal_cb(const rmoss_interfaces::msg::GimbalCmd::SharedPtr msg);

private:
  rclcpp::Node::SharedPtr nh_;
  std::shared_ptr<ignition::transport::Node> ign_node_;
  // ros pub and sub
  rclcpp::Subscription<rmoss_interfaces::msg::GimbalCmd>::SharedPtr ros_gimbal_cmd_sub_;
  std::unique_ptr<ignition::transport::Node::Publisher> ign_pitch_pub_;
  std::unique_ptr<ignition::transport::Node::Publisher> ign_yaw_pub_;
  // mode
  bool is_position_controller_;
};
}  // namespace rmoss_ign_base
#endif  // RMOSS_IGN_BASE__SIMPLE_GIMBAL_CONTROLLER_HPP_
