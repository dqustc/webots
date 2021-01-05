/*
 * Copyright 1996-2021 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Description:  This controller shows how to use the Propeller node
 *               The thrust generated by the rotor at the tail creates
 *               a torque opposed to the torque effect due to
 *               then main helix rotation and hence allows yaw stabilization.
 *               The red helicopter in propeller.wbt also manages to stabilize
 *               its altitude by means of a simple velocity control based on GPS measurement
 */

#include <webots/gps.h>
#include <webots/inertial_unit.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAIN_HELIX_VELOCITY 100.0
#define TAIL_HELIX_VELOCITY 100.0
#define WEIGHT_FORCE 16.6555161
#define TARGET_ALTITUDE 2.0
#define LABEL_X 0.75
#define LABEL_Y 0.05
#define RED 0xFF0000

int main() {
  wb_robot_init();
  const int time_step = wb_robot_get_basic_time_step();

  // GPS and inertial unit
  const WbDeviceTag gps = wb_robot_get_device("gps");
  const WbDeviceTag inertial_unit = wb_robot_get_device("inertial unit");
  wb_gps_enable(gps, time_step);
  wb_inertial_unit_enable(inertial_unit, time_step);

  // helixes motion
  const WbDeviceTag main_motor = wb_robot_get_device("main_motor");
  const WbDeviceTag tail_motor = wb_robot_get_device("tail_motor");
  wb_motor_set_position(main_motor, INFINITY);
  wb_motor_set_position(tail_motor, INFINITY);
  wb_motor_set_velocity(main_motor, MAIN_HELIX_VELOCITY + 1);
  wb_motor_set_velocity(tail_motor, TAIL_HELIX_VELOCITY);

  char buffer[50];

  while (wb_robot_step(time_step) != -1) {
    const double altitude = wb_gps_get_values(gps)[1];
    const double yaw = wb_inertial_unit_get_roll_pitch_yaw(inertial_unit)[2];
    sprintf(buffer, "Altitude: %1.1f m", altitude);
    wb_supervisor_set_label(0, buffer, LABEL_X, LABEL_Y, 0.07, RED, 0, "Arial");
    sprintf(buffer, "Yaw: %1.1f rad", yaw);
    wb_supervisor_set_label(1, buffer, LABEL_X, LABEL_Y - 0.03, 0.07, RED, 0, "Arial");
    const double ratio = 1.0 - altitude / TARGET_ALTITUDE;
    wb_motor_set_velocity(main_motor, MAIN_HELIX_VELOCITY + ratio);
  }

  return 0;
}
