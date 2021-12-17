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
#include "rmoss_ign_base/ign_imu.hpp"

#include <cmath>
#include <memory>
#include <string>

namespace rmoss_ign_base
{


double toPitch(const double & x, const double & y, const double & z, const double & w)
{
  // pitch (y-axis rotation)
  double pitch;
  double sinp = +2.0 * (w * y - z * x);
  if (fabs(sinp) >= 1) {
    pitch = copysign(M_PI / 2, sinp);     // use 90 degrees if out of range
  } else {
    pitch = asin(sinp);
  }
  return pitch;
}

double toYaw(const double & x, const double & y, const double & z, const double & w)
{
  double siny_cosp = +2.0 * (w * z + x * y);
  double cosy_cosp = +1.0 - 2.0 * (y * y + z * z);
  return atan2(siny_cosp, cosy_cosp);
}

IgnImu::IgnImu(
  rclcpp::Node::SharedPtr node,
  const std::shared_ptr<ignition::transport::Node> & ign_node,
  const std::string & ign_gimbal_imu_topic)
{
  ign_node_ = ign_node;
  ign_node_->Subscribe(ign_gimbal_imu_topic, &IgnImu::ign_imu_cb, this);
  pitch_sensor_ = std::make_shared<DataSensor<double>>();
  yaw_sensor_ = std::make_shared<DataSensor<double>>();
}

void IgnImu::ign_imu_cb(const ignition::msgs::IMU & msg)
{
  std::lock_guard<std::mutex> lock(msg_mut_);
  imu_msg_ = msg;
  auto & q = imu_msg_.orientation();
  pitch_angle_ = toPitch(q.x(), q.y(), q.z(), q.w());
  yaw_angle_ = toYaw(q.x(), q.y(), q.z(), q.w());
  // continuous yaw
  double dyaw = yaw_angle_ - last_yaw_angle_;
  if (dyaw > 3) {
    dyaw = dyaw - 3.1415926535 * 2;
  }
  if (dyaw < -3) {
    dyaw = dyaw + 3.1415926535 * 2;
  }
  continuous_yaw_angle_ = continuous_yaw_angle_ + dyaw;
  last_yaw_angle_ = yaw_angle_;
  // update
  pitch_sensor_->update(pitch_angle_, node_->get_clock()->now());
  yaw_sensor_->update(continuous_yaw_angle_, node_->get_clock()->now());
}


IgnGimbalImu::IgnGimbalImu(
  rclcpp::Node::SharedPtr node,
  std::shared_ptr<ignition::transport::Node> ign_node,
  const std::string & ign_gimbal_imu_topic)
{
  ign_node_ = ign_node;
  ign_node_->Subscribe(ign_gimbal_imu_topic, &IgnGimbalImu::ign_imu_cb, this);
  position_sensor_ = std::make_shared<DataSensor<rmoss_interfaces::msg::Gimbal>>();
}

void IgnGimbalImu::ign_imu_cb(const ignition::msgs::IMU & msg)
{
  auto & q = msg.orientation();
  double pitch_angle = toPitch(q.x(), q.y(), q.z(), q.w());
  double yaw_angle = toYaw(q.x(), q.y(), q.z(), q.w());
  // continuous yaw
  double dyaw = yaw_angle - last_yaw_angle_;
  if (dyaw > 3) {
    dyaw = dyaw - 3.1415926535 * 2;
  }
  if (dyaw < -3) {
    dyaw = dyaw + 3.1415926535 * 2;
  }
  continuous_yaw_angle_ = continuous_yaw_angle_ + dyaw;
  last_yaw_angle_ = yaw_angle;
  // update
  cur_position_.yaw = continuous_yaw_angle_;
  cur_position_.pitch = pitch_angle;
  position_sensor_->update(cur_position_, node_->get_clock()->now());
}

}  // namespace rmoss_ign_base
