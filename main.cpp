// Intern Project Code

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "src/MeSingleLineFollower.h"
#include "src/MeCollisionSensor.h"
#include "src/MeBarrierSensor.h"
#include "src/MeNewRGBLed.h"
#include <MeMegaPi.h>

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

bool lineFollowMode = true;
bool obstacleAvoidMode = false;
int impactCounter = 0;
bool handRecMode = false;
int correctHandMotion = 0;
bool passWaveIncorrect = false;
bool passWaveCorrect = false;

void motor_foward_left_run(int16_t speed)
{
   motor_10.run(-speed);
}

void motor_foward_right_run(int16_t speed)
{
  motor_1.run(speed);
}

void motor_back_left_run(int16_t speed)
{
  motor_2.run(-speed);
}

void motor_back_right_run(int16_t speed)
{
  motor_9.run(speed);
}

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

void _delay(float seconds) {
  if(seconds < 0.0){
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void setup() {
  Serial.begin(9600);
  
  rgbled_67.fillPixelsBak(0, 2, 1);
  rgbled_68.fillPixelsBak(0, 2, 1);
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  rgbled_67.setColor(0, 85,32,211);
  rgbled_67.show();

  rgbled_68.setColor(0, 32,211,193);
  rgbled_68.show();
  _delay(2);

}

void _loop() {
}

void loop() {
  while(lineFollowMode == true)
  {
    line_sensor0_value = (linefollower_63.readSensor() == 0);
    line_sensor1_value = (linefollower_64.readSensor() == 0);
    if((line_sensor0_value == 1.000000)  &&  (line_sensor1_value == 1.000000)){

        move_control(0, 40 / 100.0 * 255, 0);

        rgbled_67.setColor(0, 126,211,33);
        rgbled_67.show();

        rgbled_68.setColor(0, 126,211,33);
        rgbled_68.show();

    }
    else if((line_sensor0_value == 0.000000)  &&  (line_sensor1_value == 1.000000)){

        motor_2.run(-50 / 100.0 * 255);
        motor_10.run(-50 / 100.0 * 255);

        motor_1.run(-10 / 100.0 * 255);
        motor_9.run(-10 / 100.0 * 255);

        rgbled_68.setColor(0, 32,166,211);
        rgbled_68.show();

    }
    else if((line_sensor0_value == 1.000000)  &&  (line_sensor1_value == 0.000000)){

        motor_1.run(50 / 100.0 * 255);
        motor_9.run(50 / 100.0 * 255);

        motor_2.run(10 / 100.0 * 255);
        motor_10.run(10 / 100.0 * 255);

        rgbled_67.setColor(0, 32,166,211);
        rgbled_67.show();

    }else{

        motor_1.run(0 / 100.0 * 255);
        motor_9.run(0 / 100.0 * 255);

        motor_2.run(0 / 100.0 * 255);
        motor_10.run(0 / 100.0 * 255);

        rgbled_67.setColor(0, 211,32,32);
        rgbled_67.show();

        rgbled_68.setColor(0, 211,32,32);
        rgbled_68.show();

    }
    if (collision_65.isCollision())
    {
      impactCounter = impactCounter + 1;

      while(collision_65.isCollision())
      {
          ;;
      }

      if(impactCounter >= 3)
      {
          lineFollowMode = false;
          obstacleAvoidMode = true;

          rgbled_67.setColor(0, 85,32,211);
          rgbled_67.show();

          rgbled_68.setColor(0, 32,211,193);
          rgbled_68.show();

          Serial.println("Obstacle Avoidance Initiatied");
      }
      _delay(0.4);
    }
  }

  while(obstacleAvoidMode == true)
  {
    if (!barrier_60.isBarried()  &&  !barrier_61.isBarried()  &&  !barrier_62.isBarried())
    {
        move_control(0, 40 / 100.0 * 255, 0);

        rgbled_67.setColor(0, 126,211,33);
        rgbled_67.show();

        rgbled_68.setColor(0, 126,211,33);
        rgbled_68.show();
    }
    else if(barrier_60.isBarried()  &&  !barrier_62.isBarried()){

        motor_2.run(-30 / 100.0 * 255);
        motor_10.run(-30 / 100.0 * 255);

        motor_1.run(-10 / 100.0 * 255);
        motor_9.run(-10 / 100.0 * 255);

        rgbled_68.setColor(0, 32,166,211);
        rgbled_68.show();

    }
        else if(barrier_60.isBarried()  &&  barrier_61.isBarried() &&  !barrier_62.isBarried()){

        motor_2.run(-50 / 100.0 * 255);
        motor_10.run(-50 / 100.0 * 255);

        motor_1.run(-10 / 100.0 * 255);
        motor_9.run(-10 / 100.0 * 255);

        rgbled_68.setColor(0, 32,166,211);
        rgbled_68.show();

    }
    else if(!barrier_60.isBarried()  &&   barrier_62.isBarried()){

        motor_1.run(30 / 100.0 * 255);
        motor_9.run(30 / 100.0 * 255);

        motor_2.run(10 / 100.0 * 255);
        motor_10.run(10 / 100.0 * 255);

        rgbled_67.setColor(0, 32,166,211);
        rgbled_67.show();

    }
        else if(!barrier_60.isBarried() &&  barrier_61.isBarried()  &&   barrier_62.isBarried()){

        motor_1.run(50 / 100.0 * 255);
        motor_9.run(50 / 100.0 * 255);

        motor_2.run(10 / 100.0 * 255);
        motor_10.run(10 / 100.0 * 255);

        rgbled_67.setColor(0, 32,166,211);
        rgbled_67.show();

    }
        else if(!barrier_60.isBarried() &&  barrier_61.isBarried()  &&  !barrier_62.isBarried()){

        motor_1.run(0 / 100.0 * 255);
        motor_9.run(0 / 100.0 * 255);

        motor_2.run(0 / 100.0 * 255);
        motor_10.run(0 / 100.0 * 255);

        rgbled_67.setColor(0, 211,32,32);
        rgbled_67.show();

        rgbled_68.setColor(0, 211,32,32);
        rgbled_68.show();

        handRecMode = true;
        obstacleAvoidMode = false;
    }
  }
  while(handRecMode == true){
      if(barrier_60.isBarried()){
          correctHandMotion = 1;
      }
      if(barrier_61.isBarried()){
          if(correctHandMotion == false){
              passWaveIncorrect = true;
          }
          if(correctHandMotion == 1){
            correctHandMotion = 2;
          }
      }
      if(barrier_62.isBarried()){
          if(correctHandMotion == 2){
              passWaveCorrect = true;
          }  
      }
      if(passWaveCorrect == true){
        rgbled_67.setColor(0, 126,211,33);
        rgbled_67.show();

        rgbled_68.setColor(0, 126,211,33);
        rgbled_68.show();

        _delay(2);
        handRecMode = false;
        passWaveCorrect = false;
        correctHandMotion = 0;
        lineFollowMode = true;
      }
      if(passWaveIncorrect == true){
        rgbled_67.setColor(0, 211,32,32);
        rgbled_67.show();

        rgbled_68.setColor(0, 211,32,32);
        rgbled_68.show();

        _delay(2);
        passWaveIncorrect = false;
        passWaveCorrect = false;
        correctHandMotion = 0;
      }
  }

  _loop();
}
