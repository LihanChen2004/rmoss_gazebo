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
#ifndef RMOSS_IGN_BASE__IGN_IMU_HPP_
#define RMOSS_IGN_BASE__IGN_IMU_HPP_

#include <memory>
#include <string>
#include <mutex>

#include "ignition/transport/Node.hh"

namespace rmoss_ign_base
{

class IgnImu
{
public:
  IgnImu(
    const std::shared_ptr<ignition::transport::Node> & ign_node,
    const std::string & ign_gimbal_imu_topic);
  ~IgnImu() {}

public:
  double get_pitch();
  double get_yaw(bool is_continuous = true);
  void reset_yaw(double yaw = 0) {continuous_yaw_angle_ = yaw;}

private:
  void ign_imu_cb(const ignition::msgs::IMU & msg);

private:
  std::shared_ptr<ignition::transport::Node> ign_node_;
  // tmp data
  double last_yaw_{0};
  std::mutex msg_mut_;
  // sensor data
  ignition::msgs::IMU imu_msg_;
  double pitch_angle_{0};
  double yaw_angle_{0};
  double last_yaw_angle_{0};
  double continuous_yaw_angle_{0};
};

}  // namespace rmoss_ign_base

#endif  // RMOSS_IGN_BASE__IGN_IMU_HPP_
