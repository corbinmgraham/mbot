#ifndef Controller_H
#define Controller_H

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

#include "MeFirmware/MeSingleLineFollower.h"
#include "MeFirmware/MeCollisionSensor.h"
#include "MeFirmware/MeBarrierSensor.h"
#include "MeFirmware/MeNewRGBLed.h"

#include "Data.h"

// Define controls?
// Define modes?

// Move these to their respective classes

bool lineFollowMode = true;
bool obstacleAvoidMode = false;
bool handRecMode = false;

MeNewRGBLed rgbled_67(67,4);
MeNewRGBLed rgbled_68(68,4);
MeSingleLineFollower linefollower_63(63);
MeSingleLineFollower linefollower_64(64);
MeCollisionSensor collision_65(65);
MeBarrierSensor barrier_60(60);
MeBarrierSensor barrier_61(61);
MeBarrierSensor barrier_62(62);
MeMegaPiDCMotor motor_1(1);
MeMegaPiDCMotor motor_9(9);
MeMegaPiDCMotor motor_2(2);
MeMegaPiDCMotor motor_10(10);
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;

float both_over_line = 0;
float left_over_right_off_line = 0;
float right_over_left_off_line = 0;
float both_off_line = 0;
float line_sensor_value = 0;
float line_sensor1_value = 0;
float line_sensor0_value = 0;

int impactCounter = 0;
int correctHandMotion = 0;
bool passWaveIncorrect = false;
bool passWaveCorrect = false;

enum MODE {
    LINE_FOLLOW,
    OBSTACLE_AVOID,
    HAND_REC
};

class Controller {
    private:
        MODE mode;
    public:
        Controller(MODE mode) {
            this->mode = mode;
        }
        ~Controller() {}

        void start() {}
};

/**
 * @brief This should be removed
 * @author ???
 * 
 */
void _loop() {}

/**
 * @brief Write a description
 * 
 * @param speed ???
 */
void motor_foward_left_run(int16_t speed) {
   motor_10.run(-speed);
};

void motor_foward_right_run(int16_t speed) {
  motor_1.run(speed);
};

void motor_back_left_run(int16_t speed) {
  motor_2.run(-speed);
};

void motor_back_right_run(int16_t speed) {
  motor_9.run(speed);
};

void move_control(int16_t vx, int16_t vy, int16_t vw)
{
  int16_t foward_left_speed;
  int16_t foward_right_speed;
  int16_t back_left_speed;
  int16_t back_right_speed;

  foward_left_speed = vy + vx + vw;
  foward_right_speed = vy - vx - vw;
  back_left_speed = vy - vx + vw;
  back_right_speed = vy + vx - vw;

  motor_foward_left_run(foward_left_speed);
  motor_foward_right_run(foward_right_speed);
  motor_back_left_run(back_left_speed);
  motor_back_right_run(back_right_speed);
}

void delay_seconds(float seconds) {
  if(seconds < 0.0){
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

#endif